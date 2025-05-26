#include <ncurses.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define ESCAPE_CODE 27
#define MAX_TEXT_ARRAY_SIZE 100
#define MAX_CHAR_LINE_SIZE 1000

char ** TEXT_ARRAY;
int TEXT_INDEX = 0;

void init_text_array(){
    TEXT_ARRAY =  malloc(sizeof(char*) * MAX_TEXT_ARRAY_SIZE);
    if(!TEXT_ARRAY){
        printf("Allocation failed.");
        return;
    }
    for(int i=0; i< MAX_TEXT_ARRAY_SIZE; i++){
        TEXT_ARRAY[i] = (char*)malloc(MAX_CHAR_LINE_SIZE);
    }
}

char * get_random_text(){
    if(!TEXT_ARRAY){
        init_text_array();
    }
    
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Software is like sex: it's better when it's free. - Linus Torvalds");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"If we want users to like our software, we should design it to behave like a likable person. - Alan Cooper");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Quality is a product of a conflict between programmers and testers. - Yegor Bugayenk");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Everybody should learn to program a computer because it teaches you how to think. - Steve Jobs");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"I taught myself how to program computers when I was a kid, bought my first computer when I was 10, and sold my first commercial program when I was 12. - Elon Musk");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Software and cathedrals are much the same — first we build them, then we pray.");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Web development is difficult, only then it is fun to do. You just have to set your standards. If it were to be easy, would anyone do it? - Olawale Daniel");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Programmers seem to be changing the world. It would be a relief, for them and for all of us, if they knew something about it. - Ellen Ullman");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Most good programmers do programming not because they expect to get paid or get adulation by the public, but because it is fun to program. - Linus Torvalds");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"When I wrote this code, only God and I understood what I did. Now only God knows. - Anonymous");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"I'm not a great programmer; I'm just a good programmer with great habits. - Kent Beck");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"You might not think that programmers are artists, but programming is an extremely creative profession. It's logic-based creativity. - John Romero");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Programming is learned by writing programs. - Brian Kernighan");    
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Software comes from heaven when you have good hardware. - Ken Olsen");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"There is always one more bug to fix. - Ellen Ullman");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"If debugging is the process of removing bugs, then programming must be the process of putting them in. - Sam Redwine");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Talk is cheap. Show me the code. - Linus Torvalds");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Sometimes it pays to stay in bed on Monday, rather than spending the rest of the week debugging Monday's code. - Dan Salomon");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"If, at first, you do not succeed, call it version 1.0. - Khayri R.R. Woulfe");
    strcpy(TEXT_ARRAY[TEXT_INDEX++],"Computers are fast; developers keep them slow. - Anonymous");

    return TEXT_ARRAY[rand() % TEXT_INDEX];
}

int main() {
    // to start again
    again:

    // get terminal size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int console_width = w.ws_col;

    // begin
    int c, col_index = 0, row_index = 0;
    int total_typed_characters = 0, valid_typed_characters = 0;
    initscr();
    cbreak();
    noecho();
    use_default_colors(); 
    start_color();
    init_pair(1, COLOR_GREEN, -1);
    init_pair(2, COLOR_BLACK,COLOR_RED);
    init_pair(3, COLOR_WHITE,-1);
    init_pair(4, COLOR_BLUE,-1);

    // monitor time 
    struct timeval start, end;
    double elapsed;

    printw("When ready, start typing by pressing \"F\" on your keyboard.\n");
    while(c = getch()){
        if(c == 'F'){
            break;
        }
    }
    clear();

    srand(time(NULL));
    printw("Type the following sentence as fast as you can:\n");
    char * text_to_type = get_random_text();
    int text_len = strlen(text_to_type);
    printw("%s", text_to_type);

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
            mvprintw(row_index,col_index % console_width, "%c", text_to_type[col_index]);
            move(row_index, col_index % console_width);
            continue;
        }
        
        // update row
        row_index =  col_index / console_width + 1;
        
        // see if char typed was correctly
        if(text_to_type[col_index] == c){
            valid_typed_characters++;
            attron(COLOR_PAIR(1));
            mvprintw(row_index,col_index % console_width, "%c", c);
        }else{ 
            attron(COLOR_PAIR(2));
            mvprintw(row_index,col_index % console_width,"%c", text_to_type[col_index]);
        }

        // stop when the text is fully typed by user
        if(col_index == text_len-1){
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

    double words_per_minute = (text_len / 5.0) / (elapsed / 60.0);
    printw("Words per minute: %.2lf WPM\n", words_per_minute);

    double accuracy = (valid_typed_characters / (double)total_typed_characters) * 100.0;
    printw("Accuracy: %.2lf %%\n", accuracy);

    // wait for user to end
    attron(COLOR_PAIR(3));
    printw("\rPress \"F\" to try again.\n");
    printw("\rPress \"ESC\" to exit the program.\n");
    while((c = getch()) != ESCAPE_CODE){
        if(c == 'F'){
            clear();
            refresh();
            goto again;
        } 
    } 
    endwin();
    return 0;
}