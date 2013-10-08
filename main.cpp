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
	if (argc < 2)
		ui->initialize("");
	else
		ui->initialize(argv[1]);
	ui->resize(h, w);
	while (1) {
		int ch = getch();
		int endflag = 0;
		switch(ch) {
			case 'q':
				endflag = 1;
				break;
			case 'j': case KEY_DOWN:
				ui->keydown();
				break;
			case 'k': case KEY_UP:
				ui->keyup();
				break;
			case 'h': case KEY_LEFT:
				ui->keyleft();
				break;
			case 'l': case KEY_RIGHT:
				ui->keyright();
				break;
			case 10:
				ui->keypagedown();
				break;
			case 11:
				ui->keypageup();
				break;
			case KEY_BACKSPACE: case 8:
				ui->keypageleft();
				break;
			case 12:
				ui->keypageright();
				break;
			case 'g':
				ui->keytop();
				break;
			case 'G':
				ui->keybottom();
				break;
			case 'd':
				ui->keydeletedown();
				break;
			case 'D':
				ui->keydeleteup();
				break;
			default:
				break;
		}
		if (endflag) break;
	}
	endwin();
	return 0;
}
