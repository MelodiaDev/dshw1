#include <ncurses.h>
#include <cstring>
#include <string>
#include <vector>
#include "editor.h"
#include "ui.h"
using std::vector;
using std::string;

/* predefined commands */
const int comms = 4;
const char *commnames[comms] = {"save", "find", "find'", "replace"};
const int commargs[comms] = {1, 1, 1, 2};
const char *commprefs[comms][10] = {{"Save: "}, {"Find: "}, {"Find backwards: "}, {"Find        : ", "Replace With: "}};

int ui_t::adjustscr(void) {
	int flag = 0;
	if (posx >= scrx + h) {
		scrx = posx - (h-1);
		flag = 1;
	} else if (posx < scrx) {
		scrx = posx;
		flag = 1;
	}
	if (posy < scry) {
		scry -= ((scry - posy - 1) / (w/2) + 1) * (w/2);
		if (scry < 0) scry = 0;
		flag = 1;
	} else if (posy >= scry + w) {
		scry += ((posy - scry - w) / (w/2) + 1) * (w/2);
		flag = 1;
	}
	if (scrx + h > rows) scrx = rows - h;
	if (scrx < 0) scrx = 0;
	return flag;
}
void ui_t::refreshscr(int flag) {
	/* draw text area */
	/* calculate height of the text area */
	int realh = h;
	if (mode == MODE_COMM)
		for (int i = 0; i < commarg; i++)
			realh -= (strlen(commpref[i]) + strlen(commbuf[i])) / w + 1;
	/* retrieve text from backend and refresh text area */
	if (flag || realh > prerealh) {
		vector<string> data;
		editor->retrieve(scrx, scry, realh, w, data);
		for (int i = 0; i < realh; i++) {
			move(i, 0);
			clrtoeol();
			if (i >= data.size()) continue;
			for (int j = 0; j < w && j < data[i].size(); j++)
				addch(data[i][j]);
		}
	}

	/* draw status line */
	prerealh = realh;
	move(realh, 0);
	clrtoeol();
	/* file name */
	if (w-25-10-2 >= 0) {
		move(realh, 2);
		char fmt[10];
		sprintf(fmt, "%%.%ds", w-25-2-2);
		printw(fmt, file[0] ? file : "[NEW]");
		refresh();
	}
	/* mode */
	move(realh, w-25);
	printw(mode == MODE_INS ? "|INS|" : "|   |");
	/* displayed position */
	printw(" %d,%d", posx+1, posy);
	refresh();

	/* draw command area and move cursor */
	if (mode == MODE_COMM) {
		int nowh = realh+1, posh;
		for (int i = 0; i < commarg; i++) {
			if (i == commposx) posh = nowh;
			attron(A_BOLD);
			mvprintw(nowh, 0, "%s", commpref[i]);
			attroff(A_BOLD);
			printw("%s\n", commbuf[i]);
			nowh += (strlen(commpref[i]) + strlen(commbuf[i])) / w + 1;
		}
		int pos = strlen(commpref[commposx]) + commposy;
		move(posh + pos/w, pos%w);
	} else {
		move(posx - scrx, posy - scry);
	}
	refresh();
}

void ui_t::initialize(const char *_file) {
	posx = posy = realposy = scrx = scry = 0;
	/* initialize backend */
	editor = new editor_t;
	editor->initialize(_file);
	/* get number of rows from backend */
	int chars;
	editor->info(rows, chars);
	file = new char[strlen(_file)+1];
	strcpy(file, _file);
}

void ui_t::key_up(void) {
	int newdx, newy;
	/* get displayed position from desired position */
	editor->go_x(-1, realposy, newdx, newy);
	if (newdx == 0) return;
	posx += newdx, posy = newy;
	int refresh_flag = adjustscr();
	refreshscr(refresh_flag);
}
void ui_t::key_down(void) {
	int newdx, newy;
	editor->go_x(1, realposy, newdx, newy);
	if (newdx == 0) return;
	posx += newdx, posy = newy;
	int refresh_flag = adjustscr();
	refreshscr(refresh_flag);
}
void ui_t::key_left(void) {
	int newdx, newdy;
	editor->go_y(-1, newdy);
	if (newdy == 0) {
		/* if at the first character of current line, go up to the last
		 * character of previous line */
		if (posx == 0) return;
		posx--;
		editor->go_x(-1, 0, newdx, newdy);
		posy = realposy = editor->aim_to_end();
	} else {
		posy += newdy;
		realposy = posy;
	}
	int refresh_flag = adjustscr();
	refreshscr(refresh_flag);
}
void ui_t::key_right(void) {
	int newdx, newdy;
	editor->go_y(1, newdy);
	if (newdy == 0) {
		/* if at the last character of current line, go down to the first
		 * character of next line */
		if (posx == rows-1) return;
		posx++;
		editor->go_x(1, 0, newdx, newdy);
		posy = realposy = 0;
	} else {
		posy += newdy;
		realposy = posy;
	}
	int refresh_flag = adjustscr();
	refreshscr(refresh_flag);
}
void ui_t::key_pageup(void) {
	int newdx, newy;
	scrx -= h/2;
	editor->go_x(-h/2, realposy, newdx, newy);
	posx += newdx, posy = newy;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_pagedown(void) {
	int newdx, newy;
	scrx += h/2;
	editor->go_x(h/2, realposy, newdx, newy);
	posx += newdx, posy = newy;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_pageleft(void) {
	int newdy;
	scry -= w/2;
	if (scry < 0) scry = 0;
	editor->go_y(-w/2, newdy);
	posy += newdy;
	realposy = posy;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_pageright(void) {
	int newdy;
	scry += w/2;
	editor->go_y(w/2, newdy);
	posy += newdy;
	realposy = posy;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_top(void) {
	posx = editor->aim_to_line(0);
	posy = 0;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_bottom(void) {
	posx = editor->aim_to_line(rows-1);
	posy = 0;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_head(void) {
	posy = realposy = 0;
	editor->aim_to_begin();
	int refresh_flag = adjustscr();
	refreshscr(refresh_flag);
}
void ui_t::key_tail(void) {
	posy = realposy = editor->aim_to_end();
	int refresh_flag = adjustscr();
	refreshscr(refresh_flag);
}
void ui_t::key_deletedown(void) {
	if (posx == rows-1) return;
	editor->erase(1);
	posy = realposy = 0;
	rows--;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_deleteup(void) {
	if (posx == 0) return;
	editor->erase(-1);
	posy = realposy = 0;
	rows--; posx--, scrx--;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_deleteleft(void) {
	if (posx == 0 && posy == 0) return;
	key_left();
	key_deleteright();
}
void ui_t::key_deleteright(void) {
	editor->delete_c();
	int chars;
	editor->info(rows, chars);
	adjustscr();
	refreshscr(1);
}
void ui_t::key_insertdown(void) {
	editor->insert(1);
	posy = realposy = 0;
	rows++; posx++;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_insertup(void) {
	editor->insert(0);
	posy = realposy = 0;
	rows++; scrx++;
	adjustscr();
	refreshscr(1);
}
void ui_t::key_insert(int c) {
	editor->insert_c(c);
	int chars;
	editor->info(rows, chars);
	adjustscr();
	refreshscr(1);
	key_right();
}

void ui_t::comm_key_up(void) {
	if (commposx == 0) return;
	commposx--;
	commposy = strlen(commbuf[commposx]);
	refreshscr(0);
}
void ui_t::comm_key_down(void) {
	if (commposx == commarg-1) return;
	commposx++;
	commposy = strlen(commbuf[commposx]);
	refreshscr(0);
}
int ui_t::comm_key_nextarg(void) {
	if (commposx == commarg-1) comm_end();
	else comm_key_down();
	return mode;
}
void ui_t::comm_key_left(void) {
	if (commposy == 0) return;
	commposy--;
	refreshscr(0);
}
void ui_t::comm_key_right(void) {
	if (commposy == strlen(commbuf[commposx])) return;
	commposy++;
	refreshscr(0);
}
void ui_t::comm_key_deleteleft(void) {
	if (commposy == 0) return;
	for (int i = commposy, l = strlen(commbuf[commposx]); i <= l; i++)
		commbuf[commposx][i-1] = commbuf[commposx][i];
	commposy--;
	refreshscr(0);
}
void ui_t::comm_key_deleteright(void) {
	int l = strlen(commbuf[commposx]);
	if (commposy == l) return;
	for (int i = commposy; i < l; i++)
		commbuf[commposx][i] = commbuf[commposx][i+1];
	refreshscr(0);
}
void ui_t::comm_key_insert(int c) {
	for (int i = strlen(commbuf[commposx]); i >= commposy; i--)
		commbuf[commposx][i+1] = commbuf[commposx][i];
	commbuf[commposx][commposy] = c;
	commposy++;
	refreshscr(0);
}

void ui_t::resizescr(int _h, int _w) {
	h = _h-1, w = _w;
	adjustscr();
	refreshscr(1);
}
void ui_t::set_mode(int _mode) {
	mode = _mode;
	refreshscr(1);
}
void ui_t::comm_start(const char *name) {
	int i = 0;
	for (; i < comms; i++)
		if (strcmp(commnames[i], name) == 0) break;
	commarg = commargs[i];
	commname = commnames[i];
	commpref = commprefs[i];
	commposx = commposy = 0;
	for (i = 0; i < commarg; i++) commbuf[i][0] = 0;
	if (strcmp(name, "save") == 0) {
		strcpy(commbuf[0], file);
		commposy = strlen(commbuf[0]);
	}
	prerealh = -1;
	set_mode(MODE_COMM);
}
void ui_t::comm_end(void) {
	set_mode(0);
}
