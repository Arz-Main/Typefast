#include "common.h"

int console_width=0, console_height=0;
FILE * QUOTES_FILE = NULL;
int TOTAL_QUOTES = 0;
char const * CURRENT_QUOTE = NULL;
int CURRENT_QUOTE_LENGTH = 0;

FILE * JOKES_FILE = NULL;
int TOTAL_JOKES = 0;
char const * CURRENT_JOKE = NULL;

int get_text_type(){
  int option;
  while ((option = getch())){
    switch (option){
      case '1': return QUOTES_TEXT_TYPE; break;
      // case '2': return JOKES_TEXT_TYPE; break;
    }
  }
  return 0;
}

int get_num_of_lines(FILE * text_file){
    if(!text_file){
        printf("The text file can't be opened.");
        return -1;
    } 
    int number_of_lines = 0;
    // buffer to store all lines
    char * buffer = malloc(MAX_LINE_SIZE * sizeof(char));
    // get the number of text entries (placed in the text file)
    fgets(buffer, MAX_LINE_SIZE, text_file);
    // check to see if it read the number correctly
    if(sscanf(buffer, "%d", &number_of_lines) != 1){
      return -1;
    }
    return number_of_lines;
}

void show_menu() {
  clear();
  refresh();
  attron(COLOR_PAIR(4));
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
  // printw("  2: Jokes\n");
  printw("  ESC: Exit\n");
}





// A HUGE WASTE OF TIME
// void free_text_array(char *** text_array_ptr, int text_array_size){
//   if(!text_array_ptr || !(*text_array_ptr)){
//     printf("Nothing to be freed.");
//     return;
//   }
//   char ** text_array = *text_array_ptr;
//   for(int i=0; i< text_array_size; i++){
//     if(text_array[i]){
//       free(text_array[i]);
//     }
//   }
//   free(text_array);
//   *text_array_ptr = NULL;
// }

// // all this should do is: ONLY make space in the selected array of text, NOT place actual text in the array
// int init_text_array(int text_type) {
//   switch (text_type){
//     case QUOTES_TEXT_TYPE:
//       QUOTES_FILE = fopen("quotes.txt", "r");
//       if(!QUOTES_FILE){
//         printf("No such file quotes.txt.");
//         fclose(QUOTES_FILE);
//         return 1;
//       }
//       QUOTES_COUNTER = get_num_of_lines(QUOTES_FILE); 
//       // check if number is correct
//       if(QUOTES_COUNTER < 1) {
//         printf("Invalid number of quotes in \"quotes.txt\" file.");
//         fclose(QUOTES_FILE);
//         return 1;
//       }
//       // allocate space for the quotes in the QUOTES_ARRAY
//       QUOTES_ARRAY = calloc(QUOTES_COUNTER, sizeof(char*));
//       if(!QUOTES_ARRAY) {
//         printf("Allocation for quotes array failed.");
//         fclose(QUOTES_FILE);
//         return 1;
//       }
//       for(int i=0; i < QUOTES_COUNTER; i++){
//         QUOTES_ARRAY[i] = calloc(MAX_LINE_SIZE, sizeof(char));
//         if(!QUOTES_ARRAY[i]) {
//           printf("Allocation for quotes array failed.");
//           free_text_array(&QUOTES_ARRAY, QUOTES_COUNTER);
//           fclose(QUOTES_FILE);
//           return 1;
//         }
//       }
//       fclose(QUOTES_FILE);
//       break;
//     case JOKES_TEXT_TYPE:
//       JOKES_FILE = fopen("jokes.txt", "r");
//       if(!JOKES_FILE){
//         fclose(JOKES_FILE);
//         printf("No such file jokes.txt.");
//         return 1;
//       }
//       JOKES_COUNTER = get_num_of_lines(JOKES_FILE); 
//       // check if number is correct
//       if(JOKES_COUNTER < 1) {
//         printf("Invalid number of jokes \"jokes.txt\" file");
//         fclose(JOKES_FILE);
//         return 1;
//       }
//       // allocate space for the jokes in the JOKES_ARRAY
//       JOKES_ARRAY = calloc(JOKES_COUNTER, sizeof(char*));
//       if(!QUOTES_ARRAY) {
//         printf("Allocation for quotes array failed.");
//         fclose(JOKES_FILE);
//         return 1;
//       }
//       for(int i=0; i < JOKES_COUNTER; i++){
//         JOKES_ARRAY[i] = calloc(MAX_LINE_SIZE, sizeof(char));
//         if(!JOKES_ARRAY[i]) {
//           printf("Allocation for jokes array failed.");
//           free_text_array(&JOKES_ARRAY, JOKES_COUNTER);
//           fclose(JOKES_FILE);
//           return 1;
//         }
//       }
//       fclose(JOKES_FILE);
//       break;
//     // add more cases here if needed

//     default:
//       printf("Not a valid type of text file.");
//       return 1;
//       break;
//   }
//   return 0;
// }




