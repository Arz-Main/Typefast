#include "common.h"

int console_width = 0, console_height = 0;
FILE * SEQUENCE_FILE;
int TOTAL_SEQUENCES_IN_FILE = 0;
char const * CURRENT_SEQUENCE = NULL;
int CURRENT_SEQUENCE_LENGTH = 0;

int get_text_type(){
  int option;
  while ((option = getch())){
    switch (option){
      case '1': return QUOTES_TEXT_TYPE; break;
      case '2': return JOKES_TEXT_TYPE; break;
    }
  }
  return 0;
}

int get_num_of_lines(FILE * text_file){
    if(!text_file){
        printf("The text file isn't available.");
        return -1;
    } 
    int number_of_lines = 0;
    // buffer to store the first line from file
    char * buffer = malloc(MAX_LINE_SIZE * sizeof(char));
    if(!buffer){
      printf("Failed to allocate memory in get_num_of_lines function.");
      return -1;
    }
    // get the number of text entries (placed in the first line of text file)
    fgets(buffer, MAX_LINE_SIZE, text_file);
    // check to see if it read the number correctly
    if(sscanf(buffer, "%d", &number_of_lines) != 1){
      free(buffer);
      return -1;
    }
    free(buffer);
    return number_of_lines;
}

void show_menu() {
  clear();
  refresh();
  attron(COLOR_PAIR(5));
  printw("_____                     _______________________________\n");
  printw("__  /_____  _________________  ____/__    |_  ___/__  __/\n");
  printw("_  __/_  / / /__  __ \\  _ \\_  /_   __  /| |____ \\__  /   \n");
  printw("/ /_ _  /_/ /__  /_/ /  __/  __/   _  ___ |___/ /_  /    \n");
  printw("\\__/ _\\__, / _  .___/\\___//_/      /_/  |_/____/ /_/     \n");
  printw("     /____/  /_/                                         \n\n");

  attroff(COLOR_PAIR(4));
  attron(COLOR_PAIR(4) | A_BLINK);
  printw("+======================================+\n");
  printw("| Made by Lisnic Dan & Pisarenco Denis |\n");
  printw("+======================================+\n\n");
  attrset(A_NORMAL);
  printw("Select what you want to type: \n");
  printw("  1: Quotes\n");
  printw("  2: Jokes\n");
  printw("  ESC: Exit\n");
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
  init_pair(5, COLOR_RED, -1);         // Logo
}

void cleanup_typefast() {
  free((void*)CURRENT_SEQUENCE);
  endwin();
}
