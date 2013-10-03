#include <ncurses.h>
#include <signal.h>
#include "ui.h"
int w, h;
ui_t *ui;
void handle_winch(int sig) {
	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, h, w);
	ui->resize(h, w);
	refresh();
}
int main(int argc, char **argv) {
	initscr();
	raw();
	nonl();
	noecho();
	keypad(stdscr, TRUE);
	set_escdelay(25);
	getmaxyx(stdscr, h, w);
	signal(SIGWINCH, handle_winch);
	ui = new ui_t;
	ui->initialize(argv[1]);
	ui->w = w, ui->h = h;
	ui->refresh(1);
	while (1) {
		int ch = getch();
		int endflag = 0;
		switch(ch) {
			case 3:
				endflag = 1;
				break;
			case 'j':
				ui->keydown();
				break;
			case 'k':
				ui->keyup();
				break;
			case 'h':
				ui->keyleft();
				break;
			case 'l':
				ui->keyright();
				break;
			default:
				break;
		}
		if (endflag) break;
	}
	endwin();
	return 0;
}
