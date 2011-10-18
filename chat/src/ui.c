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

void *ui_thread(void *argument) {
    return NULL;
}

int main()
{
	char user[20];
	char str[80];
	int row,col;				/* to store the number of rows and *
					 * the number of colums of the screen */
	int y, x, chat_line, user_line;

	initscr();				/* start the curses mode */
	start_color();
	keypad(stdscr, TRUE);
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	getmaxyx(stdscr,row,col);		/* get the number of rows and columns */

	attron(COLOR_PAIR(1));
	printw("Please enter your username:\t\tPress Ctrl+C to exit");
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

	box(w_chat, ACS_VLINE, ACS_HLINE);
	box(w_input, ACS_VLINE, ACS_HLINE);
	box(w_user, ACS_VLINE, ACS_HLINE);
	box(w_time, ACS_VLINE, ACS_HLINE);

	wmove(w_input, 1, 1);
	wgetstr(w_input, user);
	wclear(w_input);
	box(w_input, ACS_VLINE, ACS_HLINE);

	mvwprintw(w_user, user_line, 1, "%s", user);
	user_line++;

	while(true){
		refresh_all();

		wmove(w_input, 1, 1);

		wgetstr(w_input, str);

		if(chat_line > 28){
			chat_line = 1;
			wclear(w_chat);
			box(w_chat, ACS_VLINE, ACS_HLINE);

			refresh_all();
		}

		mvwprintw(w_chat, chat_line, 1, "<%s> %s", user, str);
		chat_line++;
		wclear(w_input);
		box(w_input, ACS_VLINE, ACS_HLINE);
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
