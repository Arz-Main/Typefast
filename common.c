#include "common.h"

char **QUOTE_ARRAY = NULL;
int QUOTE_COUNTER = 0; // face ceva ODR violation da poh
int console_width, console_height;

void init_quote_array() {
  if (QUOTE_ARRAY)
    return;
  QUOTE_ARRAY = malloc(MAX_QUOTE_ARRAY_SIZE * sizeof(char *));
}

void add_quote(char *text) {
  if (QUOTE_COUNTER > MAX_QUOTE_ARRAY_SIZE - 1) {
    printf("Increase the max quotes limit.");
    return;
  }
  QUOTE_ARRAY[QUOTE_COUNTER++] = strdup(text);
}

void deallocate_quote_array() {
  if (!QUOTE_ARRAY) {
    return;
  }
  for (int i = 0; i < QUOTE_COUNTER; i++) {
    free(QUOTE_ARRAY[i]);
  }
  free(QUOTE_ARRAY);
  QUOTE_ARRAY = NULL;
}

void load_quotes() {
  add_quote(
      "Software is like sex: it's better when it's free. - Linus Torvalds");
  add_quote("If we want users to like our software, we should design it to "
            "behave like a likable person. - Alan Cooper");
  add_quote("Quality is a product of a conflict between programmers and "
            "testers. - Yegor Bugayenk");
  add_quote("Everybody should learn to program a computer because it teaches "
            "you how to think. - Steve Jobs");
  add_quote("I taught myself how to program computers when I was a kid, bought "
            "my first computer when I was 10, and sold my first commercial "
            "program when I was 12. - Elon Musk");
  add_quote("Software and cathedrals are much the same - first we build them, "
            "then we pray.");
  add_quote("Web development is difficult, only then it is fun to do. You just "
            "have to set your standards. If it were to be easy, would anyone "
            "do it? - Olawale Daniel");
  add_quote("Programmers seem to be changing the world. It would be a relief, "
            "for them and for all of us, if they knew something about it. - "
            "Ellen Ullman");
  add_quote("Most good programmers do programming not because they expect to "
            "get paid or get adulation by the public, but because it is fun to "
            "program. - Linus Torvalds");
  add_quote("When I wrote this code, only God and I understood what I did. Now "
            "only God knows. - Anonymous");
  add_quote("I'm not a great programmer; I'm just a good programmer with great "
            "habits. - Kent Beck");
  add_quote("You might not think that programmers are artists, but programming "
            "is an extremely creative profession. It's logic-based creativity. "
            "- John Romero");
  add_quote("Programming is learned by writing programs. - Brian Kernighan");
  add_quote(
      "Software comes from heaven when you have good hardware. - Ken Olsen");
  add_quote("There is always one more bug to fix. - Ellen Ullman");
  add_quote("If debugging is the process of removing bugs, then programming "
            "must be the process of putting them in. - Sam Redwine");
  add_quote("Talk is cheap. Show me the code. - Linus Torvalds");
  add_quote("Sometimes it pays to stay in bed on Monday, rather than spending "
            "the rest of the week debugging Monday's code. - Dan Salomon");
  add_quote("If, at first, you do not succeed, call it version 1.0. - Khayri "
            "R.R. Woulfe");
  add_quote("Computers are fast; developers keep them slow. - Anonymous");
}

void show_menu() {
  attron(COLOR_PAIR(4));
  printw(" ______        _     _______                    \n");
  printw("|  ____|      | |   |__   __|                   \n");
  printw("| |__ __ _ ___| |_     | |_   _ _ __   ___ _ __ \n");
  printw("|  __/ _` / __| __|    | | | | | '_ \\ / _ \\ '__|\n");
  printw("| | | (_| \\__ \\ |_     | | |_| | |_) |  __/ |   \n");
  printw("|_|  \\__,_|___/\\__|    |_|\\__, | .__/ \\___|_|   \n");
  printw("                           __/ | |              \n");
  printw("                          |___/|_|              \n\n");
  attroff(COLOR_PAIR(4));
  attron(COLOR_PAIR(4) | A_BLINK);
  printw(" +===========================================+\n");
  printw(" |    Made by Linic Dan & Pisarenco Denis    |\n");
  printw(" +===========================================+\n\n");
  attrset(A_NORMAL);
}
