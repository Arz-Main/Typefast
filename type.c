#include <ncurses.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ESCAPE_CODE 27
#define MAX_QUOTE_ARRAY_SIZE 100
#define MAX_QUOTE_LINE_SIZE 1000

char ** QUOTE_ARRAY = NULL;
int QUOTE_COUNTER=0;

void init_quote_array(){
    if(QUOTE_ARRAY) return;
    QUOTE_ARRAY = malloc(MAX_QUOTE_ARRAY_SIZE * sizeof(char*));
}

void add_quote(char *text){
    if(QUOTE_COUNTER>MAX_QUOTE_ARRAY_SIZE-1){
        printf("Increase the max quotes limit.");
        return;
    }
    QUOTE_ARRAY[QUOTE_COUNTER++] = strdup(text);
}

void deallocate_quote_array(){
    if(!QUOTE_ARRAY){
        return;
    }
    for(int i=0; i< QUOTE_COUNTER; i++){
        free(QUOTE_ARRAY[i]);
    }
    free(QUOTE_ARRAY);
    QUOTE_ARRAY = NULL;
}

void load_quotes(){
    add_quote("Software is like sex: it's better when it's free. - Linus Torvalds");
    add_quote("If we want users to like our software, we should design it to behave like a likable person. - Alan Cooper");
    add_quote("Quality is a product of a conflict between programmers and testers. - Yegor Bugayenk");
    add_quote("Everybody should learn to program a computer because it teaches you how to think. - Steve Jobs");
    add_quote("I taught myself how to program computers when I was a kid, bought my first computer when I was 10, and sold my first commercial program when I was 12. - Elon Musk");
    add_quote("Software and cathedrals are much the same — first we build them, then we pray.");
    add_quote("Web development is difficult, only then it is fun to do. You just have to set your standards. If it were to be easy, would anyone do it? - Olawale Daniel");
    add_quote("Programmers seem to be changing the world. It would be a relief, for them and for all of us, if they knew something about it. - Ellen Ullman");
    add_quote("Most good programmers do programming not because they expect to get paid or get adulation by the public, but because it is fun to program. - Linus Torvalds");
    add_quote("When I wrote this code, only God and I understood what I did. Now only God knows. - Anonymous");
    add_quote("I'm not a great programmer; I'm just a good programmer with great habits. - Kent Beck");
    add_quote("You might not think that programmers are artists, but programming is an extremely creative profession. It's logic-based creativity. - John Romero");
    add_quote("Programming is learned by writing programs. - Brian Kernighan");
    add_quote("Software comes from heaven when you have good hardware. - Ken Olsen");
    add_quote("There is always one more bug to fix. - Ellen Ullman");
    add_quote("If debugging is the process of removing bugs, then programming must be the process of putting them in. - Sam Redwine");
    add_quote("Talk is cheap. Show me the code. - Linus Torvalds");
    add_quote("Sometimes it pays to stay in bed on Monday, rather than spending the rest of the week debugging Monday's code. - Dan Salomon");
    add_quote("If, at first, you do not succeed, call it version 1.0. - Khayri R.R. Woulfe");
    add_quote("Computers are fast; developers keep them slow. - Anonymous");
}

int main() {
    // load quotes
    init_quote_array();
    load_quotes();

    // get terminal size
    int isActive = 1;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int console_width = w.ws_col;

    // setting things up
    initscr();
    cbreak();
    noecho();
    use_default_colors(); 
    start_color();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_BLACK,COLOR_RED);
    init_pair(3, COLOR_WHITE,-1);
    init_pair(4, COLOR_BLUE,-1);
    srand(time(NULL));

    // monitor time 
    struct timeval start, end;
    double elapsed;

    while(isActive){
        int c, col_index = 0, row_index = 0;
        int total_typed_characters = 0, valid_typed_characters = 0;
        printw("When ready, start typing by pressing \"F\" on your keyboard.\n");
        while((c = getch())){
            if(c == 'F'){
                break;
            }
        }
        clear();

        printw("Type the following sentence as fast as you can:\n");
        int chosen_quote_index = rand() % QUOTE_COUNTER;
        char * chosen_quote = QUOTE_ARRAY[chosen_quote_index];
        int chosen_quote_len = strlen(chosen_quote);
        printw("%s", chosen_quote);

        // move cursor to start
        move(1, 0);

        // start timer
        gettimeofday(&start, NULL); 

        while((c = getch()) != ESCAPE_CODE){
            total_typed_characters++;
            
            // if backspace, remove char
            if (c == 127 || c == 8 || c == KEY_BACKSPACE) { // for different terminals
                if (col_index) col_index--;
                // update row in case of going back a line
                if ((col_index + 1) % console_width == 0) {
                    row_index -= 1;
                }
                attron(COLOR_PAIR(3));
                mvprintw(row_index,col_index % console_width, "%c", chosen_quote[col_index]);
                move(row_index, col_index % console_width);
                continue;
            }
            
            // update row
            row_index =  col_index / console_width + 1;
            
            // see if char typed was correctly
            if(chosen_quote[col_index] == c){
                valid_typed_characters++;
                attron(COLOR_PAIR(1));
                mvprintw(row_index,col_index % console_width, "%c", c);
            }else{ 
                attron(COLOR_PAIR(2));
                mvprintw(row_index,col_index % console_width,"%c", chosen_quote[col_index]);
            }

            // stop when the text is fully typed by user
            if(col_index == chosen_quote_len-1){
                break;
            }
            
            col_index++;
        }

        // end timer
        gettimeofday(&end, NULL);
        elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
        
        // display some info about the user's session
        attron(COLOR_PAIR(4));
        printw("\nThe statistics:\n");
        printw("Time taken to type: %.2lf seconds\n", elapsed);

        double words_per_minute = (chosen_quote_len / 5.0) / (elapsed / 60.0);
        printw("Words per minute: %.2lf WPM\n", words_per_minute);

        double accuracy = (valid_typed_characters / (double)total_typed_characters) * 100.0;
        printw("Accuracy: %.2lf %%\n", accuracy);

        // wait for user to end
        attron(COLOR_PAIR(3));
        printw("\rPress \"F\" to try again.\n");
        printw("\rPress \"ESC\" to exit the program.\n");
        while((c = getch())){
            if(c == 'F'){
                clear();
                break;
            }else if(c == ESCAPE_CODE){
                isActive = 0;
                break;
            }
        } 
    }
    deallocate_quote_array();
    endwin();
    return 0;
}