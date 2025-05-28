#include "common.h"
#include <ncurses.h>

typedef struct {
  int total_chars;
  int valid_chars;
  double elapsed_time;
  int *mistakes;
} TypingStats;

void init_game(void);
void cleanup_game(void);
void init_terminal(void);
void setup_colors(void);
void handle_resize(int sig);
void draw_wrapped_text(const char *text, int start_row, int console_width);

int show_menu_and_get_choice(void);
TypingStats run_typing_test(int test_type, int *global_mistakes);
void display_quote(const char *text);
void process_typing_input(const char *text, TypingStats *stats);
int is_backspace(int ch);
void handle_backspace(int *col_iter, int *row_index, const char *text);
void display_correct_char(char ch, int row, int col);
void display_wrong_char(char ch, int row, int col);

void display_statistics(const TypingStats *stats, int text_len);
void display_hardest_keys(const int *mistakes);
int find_max_mistake(int *mistakes, int *max_value);
int wait_for_continue(void);

void handle_resize(int sig) {
  getmaxyx(stdscr, console_height, console_width);
  resizeterm(console_height, console_width);
  refresh();
}
int main(void) {
  init_game();

  int global_mistakes[MAX_KEYTYPES] = {0};
  int is_active = 1;

  while (is_active) {
    getmaxyx(stdscr, console_height, console_width);

    int menu_choice = show_menu_and_get_choice();

    switch (menu_choice) {
    case '1': {
      TypingStats stats = run_typing_test(1, global_mistakes);

      is_active = wait_for_continue();

      free(stats.mistakes);
      break;
    }
    case '2': {
      TypingStats stats = run_typing_test(2, global_mistakes);

      is_active = wait_for_continue();

      free(stats.mistakes);
      break;
    }
    case ESCAPE_CODE:
      is_active = 0;
      break;
    default:
      break;
    }
  }

  // Cleanup
  cleanup_game();
  return 0;
}
void init_game(void) {
  init_quote_array();
  load_quotes();
  init_joke_array();

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  console_width = w.ws_col;
  console_height = w.ws_row;

  init_terminal();
  setup_colors();

  srand(time(NULL));
}

void init_terminal(void) {
  initscr();
  cbreak();
  noecho();
  use_default_colors();
  signal(SIGWINCH, handle_resize);
}

void setup_colors(void) {
  start_color();
  init_pair(1, COLOR_GREEN, -1);        // Correct character
  init_pair(2, COLOR_BLACK, COLOR_RED); // Wrong character
  init_pair(3, COLOR_WHITE, -1);        // Default text
  init_pair(4, COLOR_BLUE, -1);         // Statistics
}

void draw_wrapped_text(const char *text, int start_row, int console_width) {
  int i = 0;
  while (text[i] != '\0') {
    int row = i / console_width + start_row;
    int col = i % console_width;
    mvaddch(row, col, text[i]);
    i++;
  }
}

int show_menu_and_get_choice(void) {
  clear();
  show_menu();
  refresh();

  int choice;
  while ((choice = getch())) {
    switch (choice) {
    case '1':
    case '2':
      return choice;
    case ESCAPE_CODE:
      return ESCAPE_CODE;
    default:
      break;
    }
  }
  return 0;
}

TypingStats run_typing_test(int test_type, int *global_mistakes) {
  TypingStats stats = {0};
  stats.mistakes = calloc(MAX_KEYTYPES, sizeof(int));

  const char *text_to_type = NULL;
  int text_len = 0;
  int joke_index = 0;
  if (test_type == 1) {
    if (QUOTE_COUNTER > 0) {
      int quote_index = rand() % QUOTE_COUNTER;
      text_to_type = QUOTE_ARRAY[quote_index];
    }
  } else if (test_type == 2) {
    if (jokeCount > 0 && JOKE_ARRAY != NULL) {
      joke_index = (rand() % (jokeCount / 2)) * 2;
      text_to_type = JOKE_ARRAY[joke_index];
    }
  }

  if (!text_to_type || strlen(text_to_type) == 0) {
    text_to_type = "No text available for typing test.";
  }

  text_len = strlen(text_to_type);

  display_quote(text_to_type);

  struct timeval start, end;
  gettimeofday(&start, NULL);

  process_typing_input(text_to_type, &stats);

  gettimeofday(&end, NULL);
  stats.elapsed_time =
      (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

  for (int i = 0; i < MAX_KEYTYPES; i++) {
    global_mistakes[i] += stats.mistakes[i];
  }
  printw("\n%s\n", JOKE_ARRAY[joke_index + 1]);
  display_statistics(&stats, text_len);
  display_hardest_keys(global_mistakes);

  return stats;
}

void display_quote(const char *text) {
  clear();
  printw("Type fast:\n");
  draw_wrapped_text(text, 1, console_width);
  move(1, 0);
}

void process_typing_input(const char *text, TypingStats *stats) {
  int text_len = strlen(text);
  int col_iter = 0, row_index = 1;
  int choice;

  while ((choice = getch()) != ESCAPE_CODE && col_iter < text_len) {
    stats->total_chars++;

    if (is_backspace(choice)) {
      handle_backspace(&col_iter, &row_index, text);
      continue;
    }

    row_index = col_iter / console_width + 1;

    if (text[col_iter] == choice) {
      stats->valid_chars++;
      display_correct_char(choice, row_index, col_iter % console_width);
    } else {
      if (choice >= ' ' && choice < ' ' + MAX_KEYTYPES) {
        stats->mistakes[choice - ' ']++;
      }
      display_wrong_char(text[col_iter], row_index, col_iter % console_width);
    }

    col_iter++;
  }
}

int is_backspace(int ch) {
  return (ch == 127 || ch == 8 || ch == KEY_BACKSPACE);
}

void handle_backspace(int *col_iter, int *row_index, const char *text) {
  if (*col_iter > 0) {
    (*col_iter)--;

    if ((*col_iter + 1) % console_width == 0) {
      (*row_index)--;
    }

    *row_index = *col_iter / console_width + 1;

    attron(COLOR_PAIR(3));
    mvprintw(*row_index, *col_iter % console_width, "%c", text[*col_iter]);
    move(*row_index, *col_iter % console_width);
  }
}

void display_correct_char(char ch, int row, int col) {
  attron(COLOR_PAIR(1));
  mvprintw(row, col, "%c", ch);
}

void display_wrong_char(char ch, int row, int col) {
  attron(COLOR_PAIR(2));
  mvprintw(row, col, "%c", ch);
}

void display_statistics(const TypingStats *stats, int text_len) {
  attron(COLOR_PAIR(4));
  printw("\n\nThe statistics:\n");
  printw("1) Time taken to type: %.2lf seconds\n", stats->elapsed_time);

  double words_per_minute = (text_len / 5.0) / (stats->elapsed_time / 60.0);
  printw("2) Words per minute: %.2lf WPM\n", words_per_minute);

  if (stats->total_chars > 0) {
    double accuracy = (stats->valid_chars / (double)stats->total_chars) * 100.0;
    printw("3) Accuracy: %.2lf%%\n", accuracy);
  } else {
    printw("3) Accuracy: 0.00%%\n");
  }
}

void display_hardest_keys(const int *mistakes) {
  printw("4) Top 5 hardest keys to type: (RESULTS FOR ALL SESSIONS)\n");

  int copy_mistakes[MAX_KEYTYPES];
  memcpy(copy_mistakes, mistakes, MAX_KEYTYPES * sizeof(int));

  for (int j = 0; j < 5; j++) {
    int max_value = 0;
    int max_pos = find_max_mistake(copy_mistakes, &max_value);

    if (max_value == 0)
      break;

    copy_mistakes[max_pos] = 0;
    printw("   Key: \"%c\", it was typed wrong: %d time(s).\n", max_pos + ' ',
           max_value);
  }
}

int find_max_mistake(int *mistakes, int *max_value) {
  int max_pos = 0;
  *max_value = 0;

  for (int i = 0; i < MAX_KEYTYPES; i++) {
    if (mistakes[i] > *max_value) {
      *max_value = mistakes[i];
      max_pos = i;
    }
  }

  return max_pos;
}

int wait_for_continue(void) {
  attron(COLOR_PAIR(3));
  printw("\nPress \"F\" to try again.\n");
  printw("Press \"ESC\" to exit the program.\n");

  int choice;
  while ((choice = getch())) {
    if (choice == 'F' || choice == 'f') {
      return 1;
    } else if (choice == ESCAPE_CODE) {
      return 0;
    }
  }
  return 0;
}

void cleanup_game(void) {
  deallocate_quote_array();
  deallocate_joke_array();
  endwin();
}
