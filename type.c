#include "common.h"
#include <ncurses.h>

typedef struct {
  int total_chars;
  int valid_chars;
  double elapsed_time;
  int *mistakes;
}TypingStats;

void init_typefast();
void cleanup_typefast();
void setup_display_colors();

void get_text_sequence_from_text_type(int text_type);
void start_typing_process(int text_type);

int  is_backspace(int ch);
void handle_backspace(int *col_iter, int *row_index, const char *text);
void display_correct_char(char ch, int row, int col);
void display_wrong_char(char ch, int row, int col);

void display_statistics(const TypingStats *stats, int text_len);
void display_hardest_keys(const int *mistakes);
int  find_max_mistake(int *mistakes, int *max_value);
int  wait_for_user_option();

int main(void) {
  init_typefast();
  int typefast_is_active = 1;
  while(typefast_is_active){
    show_menu();
    // get user's text type option
    int text_type = get_text_type();
    clear(); refresh();
    get_text_sequence_from_text_type(text_type);
    // all logic for the typing process
    start_typing_process(text_type);
    // after the typing session give options
    typefast_is_active = wait_for_user_option();
  }
  return 0;
}

void init_typefast() {
  srand(time(NULL));

  // get terminal size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  console_width = w.ws_col;
  console_height = w.ws_row;

  // initialize ncurses lib utilities
  initscr();
  cbreak();
  noecho();
  use_default_colors();
  setup_display_colors();
}

void setup_display_colors() {
  start_color();
  init_pair(1, COLOR_GREEN, -1);        // Correct character
  init_pair(2, COLOR_BLACK, COLOR_RED); // Wrong character
  init_pair(3, COLOR_WHITE, -1);        // Default text
  init_pair(4, COLOR_BLUE, -1);         // Statistics
}

void start_typing_process(int text_type){
  int c, character_counter = 0;
  int col, row;
  // the typing begins here
  printw("Typefast:\n");

  switch(text_type){
    case QUOTES_TEXT_TYPE:
      // display the quote to user
      printw("%s", CURRENT_QUOTE);
      // move cursor to start of typing sequence
      move(1,0);
      // get current char from user
      while((c = getch())){
        if(!console_width){
          printw("The console width is zero.");
          return;
        }
        col = character_counter % console_width;
        row = character_counter / console_width + 1;
        // in case of backspace
        if(is_backspace(c)){
          handle_backspace(&character_counter, &row, CURRENT_QUOTE);
          continue;
        }
        if(c == CURRENT_QUOTE[character_counter]){
          display_correct_char(c, row, col);
        }else{
          display_wrong_char(CURRENT_QUOTE[character_counter], row, col);
        }
        if(character_counter == CURRENT_QUOTE_LENGTH - 1) return;
        character_counter++;
      }
      break;
    case JOKES_TEXT_TYPE:
    // just copy paste for any other types u know the drill
      ;
      break;
    default:
      printf("Not a valid text type.");
      return;
    }
  return;
}

int is_backspace(int ch) {
  return (ch == 127 || ch == 8 || ch == KEY_BACKSPACE);
}

void handle_backspace(int *col, int *row, const char *current_text_sequence) {
  if (*col > 0) {
    (*col)--;
  }
  if ((*col + 1) % console_width == 0) {
    (*row)--;
  }
  *row = *col / console_width + 1;
  attron(COLOR_PAIR(3));
  mvprintw(*row, *col % console_width, "%c", current_text_sequence[*col]);
  move(*row, *col % console_width);
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

int wait_for_user_option() {
  attron(COLOR_PAIR(3));
  printw("\nPress \"F\" to try again.\n");
  printw("Press \"ESC\" to exit the program.\n");

  int c;
  while ((c = getch())) {
    if (c == 'F') {
      return 1;
    } else if (c == ESCAPE_CODE) {
      return 0;
    }
  }
  return 0;
}

// void cleanup_typefast() {
//   endwin();
// }

void get_text_sequence_from_text_type(int text_type){
  switch(text_type){
    case QUOTES_TEXT_TYPE:
    QUOTES_FILE = fopen("quotes.txt", "r");
      if(!QUOTES_FILE){
        printf("No such file quotes.txt.");
        fclose(QUOTES_FILE);
        return;
      }
      TOTAL_QUOTES = get_num_of_lines(QUOTES_FILE); 
      // check if number is correct
      if(TOTAL_QUOTES < 1) {
        printf("Invalid number of quotes in \"quotes.txt\" file.");
        fclose(QUOTES_FILE);
        return;
      }
      int pick_random_line = rand() % TOTAL_QUOTES;
      // buffer to store the line from quotes file
      char * buffer = malloc(MAX_LINE_SIZE * sizeof(char));
      // obtain the random text line from the quotes file and place it in buffer
      while(fgets(buffer, MAX_LINE_SIZE, QUOTES_FILE) && pick_random_line--);
      // terminate the buffer
      int buffer_size = strcspn(buffer, "\n");
      buffer[buffer_size] = '\0';
      // save the quote
      CURRENT_QUOTE = buffer;
      CURRENT_QUOTE_LENGTH = buffer_size;
      fclose(QUOTES_FILE);
      break;
    case JOKES_TEXT_TYPE:
      ;
      break;
    default:
      printf("Not a valid text type.");
      return;
    }
  return;
}
