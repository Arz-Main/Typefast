#include "common.h"

void handle_resize(int sig) {
  getmaxyx(stdscr, console_height, console_width);
  resizeterm(console_height, console_width);
  refresh();
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

int main() {
  // load quotes
  init_quote_array();
  load_quotes();

  // char array of tipical typing mistakes
  // will store mistakes on all sessions
  int tipical_mistakes[MAX_KEYTYPES] = {0};

  // get terminal size
  int isActive = 1;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  console_width = w.ws_col;
  console_height = w.ws_row;

  // setting things up
  initscr();
  cbreak();
  noecho();
  use_default_colors();
  start_color();
  init_pair(1, COLOR_GREEN, -1);
  init_pair(2, COLOR_BLACK, COLOR_RED);
  init_pair(3, COLOR_WHITE, -1);
  init_pair(4, COLOR_BLUE, -1);
  srand(time(NULL));

  // monitor time
  struct timeval start, end;
  double elapsed;

  while (isActive) {
    signal(SIGWINCH, handle_resize);
    getmaxyx(stdscr, console_height, console_width);

    int choice, col_iter = 0, row_index = 0;
    int total_typed_characters = 0, valid_typed_characters = 0;
    clear();

    show_menu();

    printw("When ready, start typing by pressing \"F\" on your keyboard.\n");
    refresh();

    while ((choice = getch())) {
      if (choice == '1') {
        break;
      }
    }
    clear();

    printw("Type fast:\n");
    int chosen_quote_index = rand() % QUOTE_COUNTER;
    char const *chosen_quote = QUOTE_ARRAY[chosen_quote_index];
    int chosen_quote_len = strlen(chosen_quote);
    draw_wrapped_text(chosen_quote, row_index + 1, console_width);

    // move cursor to start
    move(1, 0);

    // start timer
    gettimeofday(&start, NULL);

    while ((choice = getch()) != ESCAPE_CODE) {
      total_typed_characters++;

      // if backspace, remove char
      if (choice == 127 || choice == 8 ||
          choice == KEY_BACKSPACE) { // for different terminals
        if (col_iter) {
          col_iter--;
        }

        // update row in case of going back a line
        if ((col_iter + 1) % console_width == 0) {
          row_index -= 1;
        }

        attron(COLOR_PAIR(3));
        mvprintw(row_index, col_iter % console_width, "%c",
                 chosen_quote[col_iter]);
        move(row_index, col_iter % console_width);
        continue;
      }

      // update row
      row_index = col_iter / console_width + 1;

      // see if char typed was correctly
      if (chosen_quote[col_iter] == choice) {
        valid_typed_characters++;
        attron(COLOR_PAIR(1));
        mvprintw(row_index, col_iter % console_width, "%c", choice);
      } else {
        // a check for what keys are hard to type
        tipical_mistakes[choice - ' '] += 1;

        attron(COLOR_PAIR(2));
        mvprintw(row_index, col_iter % console_width, "%c",
                 chosen_quote[col_iter]);
      }

      // stop when the text is fully typed by user
      if (col_iter == chosen_quote_len - 1) {
        break;
      }

      col_iter++;
    }

    // end timer
    gettimeofday(&end, NULL);
    elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;

    // display some info about the user's session
    attron(COLOR_PAIR(4));
    printw("\nThe statistics:\n");
    printw("1) Time taken to type: %.2lf seconds\n", elapsed);

    double words_per_minute = (chosen_quote_len / 5.0) / (elapsed / 60.0);
    printw("2) Words per minute: %.2lf WPM\n", words_per_minute);

    double accuracy =
        (valid_typed_characters / (double)total_typed_characters) * 100.0;
    printw("3) Accuracy: %.2lf %%\n", accuracy);

    // top 5 hardest keys to type
    printw("4) Top 5 hardest keys to type: (RESULTS FOR ALL SESSIONS)\n");

    // copy mistakes array, because mistakes will be acumulated from all sessions
    int copy_tipical_mistakes[MAX_KEYTYPES];
    memcpy(copy_tipical_mistakes, tipical_mistakes, MAX_KEYTYPES * sizeof(int));

    for (int j = 0; j < 5; j++) {
      int max = 0, max_pos = 0, i;
      for (i = 0; i < MAX_KEYTYPES; i++) {
        if (copy_tipical_mistakes[i] && max < copy_tipical_mistakes[i]) {
          max = copy_tipical_mistakes[i];
          max_pos = i;
        }
      }
      copy_tipical_mistakes[max_pos] = 0;
      printw("   Key: \"%c\", it was typed wrong: %d time(s).\n", max_pos + ' ',
             max);
    }

    // wait for user to end
    attron(COLOR_PAIR(3));
    printw("\rPress \"F\" to try again.\n");
    printw("\rPress \"ESC\" to exit the program.\n");

    while ((choice = getch())) {
      if (choice == 'F') {
        clear();
        break;
      } else if (choice == ESCAPE_CODE) {
        isActive = 0;
        break;
      }
    }
  }
  deallocate_quote_array();
  endwin();
  return 0;
}
