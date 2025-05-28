#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ESCAPE_CODE 27
#define MAX_QUOTE_ARRAY_SIZE 100
#define MAX_QUOTE_LINE_SIZE 1000

extern int console_width, console_height;

extern char **QUOTE_ARRAY;
extern int QUOTE_COUNTER;

void ShowMenu();
void init_quote_array();
void add_quote(char *text);
void deallocate_quote_array();
void load_quotes();

#endif
