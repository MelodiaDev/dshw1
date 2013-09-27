#include <ncurses.h>
#include <signal.h>
#include "ui.h"
int w, h;
ui_t *ui;
/*
void handle_winch(int sig) {
	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, h, w);
	mvprintw(h-1, 0, "window:%d %d", h, w);
	refresh();
}
*/
int main(int argc, char **argv) {
	initscr();
	raw();
	nonl();
	noecho();
	keypad(stdscr, TRUE);
	set_escdelay(25);
	getmaxyx(stdscr, h, w);
	ui_initialize(&ui, argv[1]);
	ui->w = w, ui->h = h;
	ui_refresh(ui);
	while (1) {
		int ch = getch();
		int endflag = 0;
		switch(ch) {
			case 3:
				endflag = 1;
				break;
			case 'j':
				ui_keydown(ui);
				break;
			case 'k':
				ui_keyup(ui);
				break;
			case 'h':
				ui_keyleft(ui);
				break;
			case 'l':
				ui_keyright(ui);
				break;
			default:
				break;
		}
		if (endflag) break;
	}
	endwin();
	return 0;
}
