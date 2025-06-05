#include "common.h"
#include <ncurses.h>

typedef struct {
  int total_chars_typed;
  int valid_chars;
  int invalid_char_array[MAX_KEYTYPES];
  double elapsed_time;
}TypingStats;

TypingStats statistics;

void get_text_sequence_from_text_type(int text_type);
char typing_process(int * character_counter_adr, int * col_adr, int * row_adr, int * flag_for_delimiter);
void start_sequence_typing(int text_type);
int  wait_for_user_option();

int  is_backspace(int ch);
void handle_backspace(int *col_iter, int *row_index, const char *text);
void display_correct_char(char ch, int row, int col);
void display_wrong_char(char ch, int row, int col);

void display_statistics(const TypingStats *stats, int text_len);
void display_hardest_keys(const int * mistakes);
int  find_max_mistake(int *mistakes, int *max_value);

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
    start_sequence_typing(text_type);
    // give stats
    display_statistics(&statistics, CURRENT_SEQUENCE_LENGTH);
    // show mistakes 
    display_hardest_keys(statistics.invalid_char_array);
    // after the typing session give options
    typefast_is_active = wait_for_user_option();
  }
  cleanup_typefast();
  return 0;
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
  // for hiding "+" delimiter when backspacing 
  if(current_text_sequence[*col] == '+') mvprintw(*row, *col % console_width, " ");
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

  if (stats->total_chars_typed > 0) {
    double accuracy = (stats->valid_chars / (double)stats->total_chars_typed) * 100.0;
    printw("3) Accuracy: %.2lf%%\n", accuracy);
  } else {
    printw("3) Accuracy: 0.00%%\n");
  }
}

void display_hardest_keys(const int * mistakes) {
  printw("4) Top 5 hardest keys to type: (RESULTS FOR ALL SESSIONS)\n");

  int copy_mistakes[MAX_KEYTYPES];
  memcpy(copy_mistakes, mistakes, MAX_KEYTYPES * sizeof(int));

  for (int j = 0; j < 5; j++) {
    int max_value = 0;
    int max_pos = find_max_mistake(copy_mistakes, &max_value);

    if (max_value == 0)
      break;

    copy_mistakes[max_pos] = 0;
    printw("   Key: \"%c\", it was typed wrong: %d time(s).\n", max_pos + ASCII_OFFSET, max_value);
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

void get_text_sequence_from_text_type(int text_type){
  int pick_random_line;
  char * buffer;
  int buffer_size;
  switch(text_type){
    case QUOTES_TEXT_TYPE:
    SEQUENCE_FILE = fopen("/home/arz/Documents/4_Git_Projects/TypefastRepo/quotes.txt", "r");
      if(!SEQUENCE_FILE){
        printf("No such file quotes.txt.");
        exit(1);
      }
      TOTAL_SEQUENCES_IN_FILE = get_num_of_lines(SEQUENCE_FILE); 
      // check if number is correct
      if(TOTAL_SEQUENCES_IN_FILE < 1) {
        printf("Invalid number of quotes in \"quotes.txt\" file.");
        fclose(SEQUENCE_FILE);
        exit(1);
      }
      pick_random_line = rand() % TOTAL_SEQUENCES_IN_FILE;
      // buffer to store the line from quotes file
      buffer = malloc(MAX_LINE_SIZE * sizeof(char));
      if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(SEQUENCE_FILE);
        exit(1);
      }
      // obtain the random text line from the quotes file and place it in buffer
      while(fgets(buffer, MAX_LINE_SIZE, SEQUENCE_FILE) && pick_random_line--);
      // terminate the buffer
      buffer_size = strcspn(buffer, "\n");
      buffer[buffer_size] = '\0';
      // save the quote
      CURRENT_SEQUENCE = buffer;
      CURRENT_SEQUENCE_LENGTH = buffer_size;
      fclose(SEQUENCE_FILE);
      break;
    case JOKES_TEXT_TYPE:
      SEQUENCE_FILE = fopen("/home/arz/Documents/4_Git_Projects/TypefastRepo/jokes.txt", "r");
      if(!SEQUENCE_FILE){
        printf("No such file jokes.txt.");
        exit(1);
      }
      TOTAL_SEQUENCES_IN_FILE = get_num_of_lines(SEQUENCE_FILE); 
      // check if number is correct
      if(TOTAL_SEQUENCES_IN_FILE < 1) {
        printf("Invalid number of jokes in \"jokes.txt\" file.");
        fclose(SEQUENCE_FILE);
        exit(1);
      }
      pick_random_line = rand() % TOTAL_SEQUENCES_IN_FILE;
      // buffer to store the line from quotes file
      buffer = malloc(MAX_LINE_SIZE * sizeof(char));
      // obtain the random text line from the quotes file and place it in buffer
      while(fgets(buffer, MAX_LINE_SIZE, SEQUENCE_FILE) && pick_random_line--);
      // terminate the buffer
      buffer_size = strcspn(buffer, "\n");
      buffer[buffer_size] = '\0';
      // save the quote
      CURRENT_SEQUENCE = buffer;
      CURRENT_SEQUENCE_LENGTH = buffer_size;
      fclose(SEQUENCE_FILE);
      break;
    default:
      printf("Not a valid text type.");
      exit(1);
    }
  return;
}

char typing_process(int * character_counter_adr, int * col_adr, int * row_adr, int * flag_for_delimiter){
  int c;
  while((c = getch())){
    if(!console_width){
      printw("The console width is zero.");
      return 1;
    }

    statistics.total_chars_typed++;

    *col_adr = *character_counter_adr % console_width;
    *row_adr = *character_counter_adr / console_width + 1;

    // in case of backspace
    if(is_backspace(c)){
      handle_backspace(&(*character_counter_adr), &(*row_adr), CURRENT_SEQUENCE);
      continue;
    }
    if(c == CURRENT_SEQUENCE[*character_counter_adr]){
      statistics.valid_chars++;
      display_correct_char(c, *row_adr, *col_adr);
    }else{
      statistics.invalid_char_array[CURRENT_SEQUENCE[*character_counter_adr] - ASCII_OFFSET]++;
      display_wrong_char(CURRENT_SEQUENCE[*character_counter_adr], *row_adr, *col_adr);
    }
    if(*character_counter_adr == CURRENT_SEQUENCE_LENGTH - 1) return 0;
    (*character_counter_adr)++;
    // all this bullshit for the "+" delimiter, could be done better? yes, do i want to? no
    // it's absolute garbage man...
    if(CURRENT_SEQUENCE[*(character_counter_adr)] == DELIMITER && *flag_for_delimiter){
      *flag_for_delimiter=0;
      return c;
    }else if (CURRENT_SEQUENCE[*(character_counter_adr)] == DELIMITER){
      attron(COLOR_PAIR(3));
      printw(" ");
      (*character_counter_adr)++;
      (*col_adr)++;
    }
  }
  return 0;
}

void start_sequence_typing(int text_type){
  int row = 1;
  int col = 0;
  int character_counter = 0;
  int flag_for_delimiter = 1;
  struct timeval start;
  struct timeval end;
  double total_time = 0;
  // the typing begins here
  printw("Typefast:\n");
  switch(text_type){
    case QUOTES_TEXT_TYPE: 
      // display the sequence to user
      printw("%s", CURRENT_SEQUENCE);
      // move cursor to start of typing sequence
      move(row,0);
      // get current char from user
      gettimeofday(&start, 0);
      typing_process(&character_counter, &col, &row, &flag_for_delimiter);
      gettimeofday(&end, 0);
      break;
    case JOKES_TEXT_TYPE: 
      // display the setup of joke sequence to user
      for(int i=0; CURRENT_SEQUENCE[i] != '+'; i++){
        printw("%c", CURRENT_SEQUENCE[i]);
      }
      // move cursor to start of typing sequence
      move(row,0);
      gettimeofday(&start, 0);
      // this will stop after encountering the + delimiter, specific for jokes
      typing_process(&character_counter, &col, &row, &flag_for_delimiter);
      // skip delimiter
      character_counter++;
      col++;
      attron(COLOR_PAIR(3));
      printw(" ");
      // print the punchline
      for(int i=character_counter; CURRENT_SEQUENCE[i] != '\0'; i++){
        printw("%c", CURRENT_SEQUENCE[i]);
      }
      move(row,col+1);
      typing_process(&character_counter, &col, &row, &flag_for_delimiter);
      gettimeofday(&end, 0);
      break;
    default: 
      printw("Invalid option for text type.");
      return;
  }
  total_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / (double)1000000; 
  statistics.elapsed_time = total_time;
  return;
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