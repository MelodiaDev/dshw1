#include <ncurses.h>
#include <signal.h>
#include "ui.h"
int w, h, ctrlh2kbs;
ui_t *ui;
void handle_winch(int sig) {
	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, h, w);
	ui->resize(h, w);
}
int fixed_getch(void) {
	int ch = getch();
	if (ctrlh2kbs && ch == KEY_BACKSPACE)
		ch = 8;
	if (ch == 127)
		ch = KEY_BACKSPACE;
	return ch;
}
int main(int argc, char **argv) {
	initscr();
	raw();
	nonl();
	noecho();
	keypad(stdscr, TRUE);
	const char *kbs_seq = tparm(tigetstr("kbs"));
	if (kbs_seq[0] == 8 && kbs_seq[1] == 0) ctrlh2kbs = 1;
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
		int ch = fixed_getch();
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
			case 8:
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
			case 'o':
				ui->keyinsertdown();
				break;
			case 'O':
				ui->keyinsertup();
				break;
			default:
				break;
		}
		if (endflag) break;
	}
	endwin();
	return 0;
}
