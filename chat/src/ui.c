/*
 * Jeff Stubler & Matt Edwards
 * CS 3841
 * Lab 5 -- UNIX chat program
 * 18 October 2011
 *
 * User interface thread
 */

#include "ui.h"

WINDOW * w_chat;
WINDOW * w_input;
WINDOW * w_user;
WINDOW * w_time;

extern login_info login;

void *ui_thread(void *argument) {
    char str[80];
    char tick[10];
    char new_user[20];
    char[20] time;
    char[20] user;
    char[80] message;
    int row, col, y, x, chat_line, user_line, buffer;

    initscr();				/* start the curses mode */
    start_color();
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    getmaxyx(stdscr,row,col);		/* get the number of rows and columns */

    attron(COLOR_PAIR(1));
    printw("\t\tType 'EXIT' to quit the program.");
    refresh();
    attroff(COLOR_PAIR(1));

    x=col/1.38;
    y=row-6;
    user_line = 1;
    chat_line = 1;

    
    w_chat = newwin(y, x, 1, 1);
    w_input = newwin(3, x, y+1, 1);
    w_user = newwin(y, col/6, 1, x+2);
    w_time = newwin(3, col/6, y+1, x+2);

    /*Create panels for each window*/
    box(w_chat, ACS_VLINE, ACS_HLINE);
    box(w_input, ACS_VLINE, ACS_HLINE);
    box(w_user, ACS_VLINE, ACS_HLINE);
    box(w_time, ACS_VLINE, ACS_HLINE);

    /*TODO: Display ticking time in the bottom right panel
    * TODO: Displaying all users in chat room
    * TODO: Displaying received input
    * TODO: Displaying sent input
    */
    mvwprintw(w_user, user_line, 1, "%s", login.username);
    user_line++;

    while(true){
        refresh_all();
        
        if(has_keys() == true){
            buffer = 0;
	    //placeholder until keyboard.c code is implemented
            while(buffer < 80 && kb_buffer[buffer] != "\r\n") {
		mvwaddch(w_input,1, buffer+1, kb_buffer[buffer]);
		strcat(str,kb_buffer[buffer]);
                buffer++;
		refresh_all();
            }
        }

        if(strcmp(str, "EXIT") == 0){
            endwin();
            return 0;
        }

        if(chat_line > 28){
            chat_line = 1;
            wclear(w_chat);
            box(w_chat, ACS_VLINE, ACS_HLINE);

            refresh_all();
        }

	//placeholder
	if(has_new_user() == true){
	    strcpy(new_user, get_user());
	    mvwprintw(w_user, user_line, 1, "%s", new_user);
	    user_line++;
	    refresh_all();
	}

	//placeholder until has_message is implemented in some form
	if(has_message() == true){
	    //placeholders
	    strcpy(time, get_time());
	    if(get_user() == NULL){
		strcpy(user, "SYSTEM");
	    } else{
		strcpy(user, get_user());
	    }
	    strcpy(message, get_message());
	
	    mvwprintw(w_chat, chat_line, 1, "[%s]<%s> %s", time, user, message);  
	    chat_line++;  
	}
	
	//placeholder
	strcpy(tick, get_tick());
	mvwprintw(w_time, 1, 1, "%i", tick);
    }

    endwin();

    return 0;
}

void refresh_all(){
    wrefresh(w_chat);
    wrefresh(w_input);
    wrefresh(w_user);
    wrefresh(w_time);
    refresh();
}
