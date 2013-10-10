#include <ncurses.h>
#include <signal.h>
#include "ui.h"
int w, h, ctrlh2kbs;
int mode;
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
		if (mode == 0) {
			switch(ch) {
				case 17:
					endflag = 1;
					break;
				case 19:
					mode = MODE_COMM;
					ui->start_comm("save");
					break;
				case 'i':
					mode = MODE_INS;
					ui->set_mode(mode);
					break;
				case 'k': case KEY_UP:
					ui->keyup();
					break;
				case 'j': case KEY_DOWN:
					ui->keydown();
					break;
				case 'h': case KEY_LEFT:
					ui->keyleft();
					break;
				case 'l': case KEY_RIGHT:
					ui->keyright();
					break;
				case 11:
					ui->keypageup();
					break;
				case 10:
					ui->keypagedown();
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
				case '^':
					ui->keyhead();
					break;
				case '$':
					ui->keytail();
					break;
				case 'd':
					ui->keydeletedown();
					break;
				case 'D':
					ui->keydeleteup();
					break;
				case 'x':
					ui->keydeleteright();
					break;
				case 'X':
					ui->keydeleteleft();
					break;
				case 'o':
					ui->keyinsertdown();
					break;
				case 'O':
					ui->keyinsertup();
					break;
				case '/':
					mode = MODE_COMM;
					ui->start_comm("find");
					break;
				case '?':
					mode = MODE_COMM;
					ui->start_comm("find'");
					break;
				case 31:
					mode = MODE_COMM;
					ui->start_comm("replace");
					break;
			}
			if (endflag) break;
		} else if (mode == MODE_COMM) {
			if (ch > 31 && ch < 127)
				ui->keycomminsert(ch);
			else
				switch(ch) {
					case 27: case 3:
						ui->set_mode(0);
						mode = 0;
						break;
					case 13: case 10:
						mode = ui->keycommnextarg();
						break;
					case KEY_DC:
						ui->keycommdeleteright();
						break;
					case KEY_BACKSPACE:
						ui->keycommdeleteleft();
						break;
					case KEY_UP:
						ui->keycommup();
						break;
					case KEY_DOWN:
						ui->keycommdown();
						break;
					case KEY_LEFT:
						ui->keycommleft();
						break;
					case KEY_RIGHT:
						ui->keycommright();
						break;
				}
		} else {
			if (ch == '\t' || ch > 31 && ch < 127)
				ui->keyinsert(ch);
			else
				switch(ch) {
					case 27: case 3:
						mode = 0;
						ui->set_mode(0);
						break;
					case 13: case 10:
						ui->keyinsert('\n');
						break;
					case KEY_DC:
						ui->keydeleteright();
						break;
					case KEY_BACKSPACE:
						ui->keydeleteleft();
						break;
					case KEY_UP:
						ui->keyup();
						break;
					case KEY_DOWN:
						ui->keydown();
						break;
					case KEY_LEFT:
						ui->keyleft();
						break;
					case KEY_RIGHT:
						ui->keyright();
						break;
				}
		}
	}
	endwin();
	return 0;
}
