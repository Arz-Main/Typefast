#ifndef COMMON_H
#define COMMON_H

#define ESCAPE_CODE 27
#define MAX_QUOTE_ARRAY_SIZE 100
#define MAX_QUOTE_LINE_SIZE 1000
#define MAX_KEYTYPES 256

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

extern FILE *jokes;
extern int console_width, console_height;
extern int jokeCount;

extern char **QUOTE_ARRAY;
extern char **JOKE_ARRAY;

extern int QUOTE_COUNTER;
extern int JOKE_COUNTER;

void show_menu();
void init_quote_array();
void add_quote(char *text);
void deallocate_quote_array();
void init_joke_array();
void add_joke(char *text);
void deallocate_joke_array();

void load_quotes();

#endif
