#include <ncurses.h>
#include <signal.h>
#include "ui.h"

/* width and height of screen */
int w, h;
/* fix xterm kbs=^H issue */
int ctrlh2kbs;
/* current mode */
int mode;
/* frontend class */
ui_t *ui;

/* window resize signal(SIGWINCH) handler */
void handle_winch(int sig) {
	endwin();
	refresh();
	clear();
	getmaxyx(stdscr, h, w);
	ui->resizescr(h, w);
}

/* fix xterm kbs=^H and return real ^H/KEY_BACKSPACE */
int fixed_getch(void) {
	int ch = getch();
	if (ctrlh2kbs && ch == KEY_BACKSPACE)
		ch = 8;
	if (ch == 127)
		ch = KEY_BACKSPACE;
	return ch;
}

int main(int argc, char **argv) {
	/* initialization */
	initscr();
	raw();
	nonl();
	noecho();
	keypad(stdscr, TRUE);
	/* if kbs=^H, then there is an issue */
	const char *kbs_seq = tparm(tigetstr("kbs"));
	if (kbs_seq[0] == 8 && kbs_seq[1] == 0) ctrlh2kbs = 1;
	set_escdelay(25);
	signal(SIGWINCH, handle_winch);
	ui = new ui_t;
	if (argc < 2)
		ui->initialize("");
	else
		ui->initialize(argv[1]);
	/* tell frontend screen size */
	getmaxyx(stdscr, h, w);
	ui->resizescr(h, w);

	/* key event loop */
	while (1) {
		int ch = fixed_getch();
		int endflag = 0;

		/* key handling in normal mode */
		if (mode == 0) {
			switch(ch) {
				/* Ctrl-Q: quit */
				case 17:
					endflag = 1;
					break;
				/* Ctrl-S: save */
				case 19:
					mode = MODE_COMM;
					ui->comm_start("save");
					break;
				/* I: insert mode */
				case 'i':
					mode = MODE_INS;
					ui->set_mode(mode);
					break;
				/* H,Left/J,Up/K,Down/L,Right: move left/down/up/right */
				case 'k': case KEY_UP:
					ui->key_up();
					break;
				case 'j': case KEY_DOWN:
					ui->key_down();
					break;
				case 'h': case KEY_LEFT:
					ui->key_left();
					break;
				case 'l': case KEY_RIGHT:
					ui->key_right();
					break;
				/* Ctrl-H/Ctrl-J/Ctrl-K/Ctrl-L: move left/down/up/right
				 * half screen */
				case 11:
					ui->key_pageup();
					break;
				case 10:
					ui->key_pagedown();
					break;
				case 8:
					ui->key_pageleft();
					break;
				case 12:
					ui->key_pageright();
					break;
				/* G/Shift-G: move to top/bottom */
				case 'g':
					ui->key_top();
					break;
				case 'G':
					ui->key_bottom();
					break;
				/* Shift-6/Shift-4: move to head/tail of current line */
				case '^':
					ui->key_head();
					break;
				case '$':
					ui->key_tail();
					break;
				/* D/Shift-D: delete line and move down/up */
				case 'd':
					ui->key_deletedown();
					break;
				case 'D':
					ui->key_deleteup();
					break;
				/* X/Shift-X: delete next/previous character */
				case 'x':
					ui->key_deleteright();
					break;
				case 'X':
					ui->key_deleteleft();
					break;
				/* O/Shift-O: insert a new line after/before */
				case 'o':
					ui->key_insertdown();
					break;
				case 'O':
					ui->key_insertup();
					break;
				/* //Shift-//Ctrl-/: find/find backwards/replace */
				case '/':
					mode = MODE_COMM;
					ui->comm_start("find");
					break;
				case '?':
					mode = MODE_COMM;
					ui->comm_start("find'");
					break;
				case 31:
					mode = MODE_COMM;
					ui->comm_start("replace");
					break;
			}
			if (endflag) break;
		/* key handling in command mode */
		} else if (mode == MODE_COMM) {
			/* insert printable character */
			if (ch > 31 && ch < 127)
				ui->comm_key_insert(ch);
			else
				switch(ch) {
					/* Esc,Ctrl-C: exit comand mode */
					case 27: case 3:
						ui->set_mode(0);
						mode = 0;
						break;
					/* Enter: go to next argument or execute */
					case 13: case 10:
						mode = ui->comm_key_nextarg();
						break;
					/* Delete/Backspace: delete next/previous character */
					case KEY_DC:
						ui->comm_key_deleteright();
						break;
					case KEY_BACKSPACE:
						ui->comm_key_deleteleft();
						break;
					/* Up/Down: go to previous/next argument */
					case KEY_UP:
						ui->comm_key_up();
						break;
					case KEY_DOWN:
						ui->comm_key_down();
						break;
					/* Left/Right: move left/right a character */
					case KEY_LEFT:
						ui->comm_key_left();
						break;
					case KEY_RIGHT:
						ui->comm_key_right();
						break;
				}
		/* key handling in insert mode */
		} else {
			/* insert printable character */
			if (ch == '\t' || ch > 31 && ch < 127)
				ui->key_insert(ch);
			else
				switch(ch) {
					/* Esc,Ctrl-C: exit insert mode */
					case 27: case 3:
						mode = 0;
						ui->set_mode(0);
						break;
					/* Enter: insert a new line */
					case 13: case 10:
						ui->key_insert('\n');
						break;
					/* Delete/Backspace: delete next/previous character */
					case KEY_DC:
						ui->key_deleteright();
						break;
					case KEY_BACKSPACE:
						ui->key_deleteleft();
						break;
					/* Up/Down/Left/Right: move up/down/left/right */
					case KEY_UP:
						ui->key_up();
						break;
					case KEY_DOWN:
						ui->key_down();
						break;
					case KEY_LEFT:
						ui->key_left();
						break;
					case KEY_RIGHT:
						ui->key_right();
						break;
				}
		}
	}

	/* finalization */
	endwin();
	return 0;
}
