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
#define DELIMITER '+'
#define ASCII_OFFSET '!'


// add text types here and continue as follows:
#define QUOTES_TEXT_TYPE 1
#define JOKES_TEXT_TYPE 2

extern int console_width, console_height;

extern FILE * SEQUENCE_FILE;
extern int TOTAL_SEQUENCES_IN_FILE;
extern char const * CURRENT_SEQUENCE;
extern int CURRENT_SEQUENCE_LENGTH;

void init_typefast();
void setup_display_colors();
void cleanup_typefast();
void show_menu();

// helper functions for going through the text files
int get_text_type();
int get_num_of_lines(FILE * text_file);

#endif
