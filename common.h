#ifndef COMMON_H
#define COMMON_H
#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#define ESCAPE_CODE 27
#define MAX_ARRAY_SIZE 100
#define MAX_LINE_SIZE 1000
#define MAX_KEYTYPES 256

// add text types here and continue as follows:
#define QUOTES_TEXT_TYPE 1
#define JOKES_TEXT_TYPE 2

extern FILE * JOKES_FILE;
extern FILE * QUOTES_FILE;
extern int console_width, console_height;

extern int TOTAL_QUOTES;
extern char const * CURRENT_QUOTE;
extern int CURRENT_QUOTE_LENGTH;

extern int TOTAL_JOKES;
extern char const * CURRENT_JOKE;
extern int CURRENT_JOKE_LENGTH;

void show_menu();

// helper functions for going through the text files
int get_text_type();
int get_num_of_lines(FILE * text_file);

#endif
