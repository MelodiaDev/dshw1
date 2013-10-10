#include <ncurses.h>
#include <cstring>
#include <string>
#include <vector>
#include "editor.h"
#include "ui.h"
using std::vector;
using std::string;
const int comms = 4;
const char *commnames[comms] = {"save", "find", "find'", "replace"};
const int commargs[comms] = {1, 1, 1, 2};
const char *commprefs[comms][10] = {{"Save: "}, {"Find: "}, {"Find backwards: "}, {"Find        : ", "Replace With: "}};
int ui_t::window_adjust(void) {
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
void ui_t::initialize(const char *_file) {
	posx = posy = realposy = scrx = scry = 0;
	editor = new editor_t;
	editor->initialize(_file);
	int chars;
	editor->info(rows, chars);
	file = new char[strlen(_file)+1];
	strcpy(file, _file);
}
void ui_t::refreshscr(int flag) {
	int realh = h;
	if (mode == MODE_COMM)
		for (int i = 0; i < commarg; i++)
			realh -= (strlen(commpref[i]) + strlen(commbuf[i])) / w + 1;
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
	prerealh = realh;
	move(realh, 0);
	clrtoeol();
	if (w-25-10-2 >= 0) {
		move(realh, 2);
		char fmt[10];
		sprintf(fmt, "%%.%ds", w-25-2-2);
		printw(fmt, file[0] ? file : "[NEW]");
		refresh();
	}
	move(realh, w-25);
	printw(mode == MODE_INS ? "|INS|" : "|   |");
	printw(" %d,%d", posx+1, posy);
	refresh();
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
void ui_t::keyup(void) {
	int newdx, newy;
	editor->go_x(-1, realposy, newdx, newy);
	if (newdx == 0) return;
	posx += newdx, posy = newy;
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keydown(void) {
	int newdx, newy;
	editor->go_x(1, realposy, newdx, newy);
	if (newdx == 0) return;
	posx += newdx, posy = newy;
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keyleft(void) {
	int newdx, newdy;
	editor->go_y(-1, newdy);
	if (newdy == 0) {
		if (posx == 0) return;
		posx--;
		editor->go_x(-1, 0, newdx, newdy);
		posy = realposy = editor->aim_to_end();
	} else {
		posy += newdy;
		realposy = posy;
	}
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keyright(void) {
	int newdx, newdy;
	editor->go_y(1, newdy);
	if (newdy == 0) {
		if (posx == rows-1) return;
		posx++;
		editor->go_x(1, 0, newdx, newdy);
		posy = realposy = 0;
	} else {
		posy += newdy;
		realposy = posy;
	}
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keypageup(void) {
	int newdx, newy;
	scrx -= h/2;
	editor->go_x(-h/2, realposy, newdx, newy);
	posx += newdx, posy = newy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keypagedown(void) {
	int newdx, newy;
	scrx += h/2;
	editor->go_x(h/2, realposy, newdx, newy);
	posx += newdx, posy = newy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keypageleft(void) {
	int newdy;
	scry -= w/2;
	if (scry < 0) scry = 0;
	editor->go_y(-w/2, newdy);
	posy += newdy;
	realposy = posy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keypageright(void) {
	int newdy;
	scry += w/2;
	editor->go_y(w/2, newdy);
	posy += newdy;
	realposy = posy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keytop(void) {
	posx = editor->aim_to_line(0);
	posy = 0;
	window_adjust();
	refreshscr(1);
}
void ui_t::keybottom(void) {
	posx = editor->aim_to_line(rows-1);
	posy = 0;
	window_adjust();
	refreshscr(1);
}
void ui_t::keyhead(void) {
	posy = realposy = 0;
	editor->aim_to_begin();
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keytail(void) {
	posy = realposy = editor->aim_to_end();
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keydeletedown(void) {
	if (posx == rows-1) return;
	editor->erase(1);
	posy = realposy = 0;
	rows--;
	window_adjust();
	refreshscr(1);
}
void ui_t::keydeleteup(void) {
	if (posx == 0) return;
	editor->erase(-1);
	posy = realposy = 0;
	rows--; posx--, scrx--;
	window_adjust();
	refreshscr(1);
}
void ui_t::keydeleteleft(void) {
	if (posx == 0 && posy == 0) return;
	keyleft();
	keydeleteright();
	int chars;
	editor->info(rows, chars);
	window_adjust();
	refreshscr(1);
}
void ui_t::keydeleteright(void) {
	editor->delete_c();
	int chars;
	editor->info(rows, chars);
	window_adjust();
	refreshscr(1);
}
void ui_t::keyinsertdown(void) {
	editor->insert(1);
	posy = realposy = 0;
	rows++; posx++;
	window_adjust();
	refreshscr(1);
}
void ui_t::keyinsertup(void) {
	editor->insert(0);
	posy = realposy = 0;
	rows++; scrx++;
	window_adjust();
	refreshscr(1);
}
void ui_t::keyinsert(int c) {
	editor->insert_c(c);
	int chars;
	editor->info(rows, chars);
	window_adjust();
	refreshscr(1);
	keyright();
}
void ui_t::keycommup(void) {
	if (commposx == 0) return;
	commposx--;
	commposy = strlen(commbuf[commposx]);
	refreshscr(0);
}
void ui_t::keycommdown(void) {
	if (commposx == commarg-1) return;
	commposx++;
	commposy = strlen(commbuf[commposx]);
	refreshscr(0);
}
int ui_t::keycommnextarg(void) {
	if (commposx == commarg-1) end_comm();
	else keycommdown();
	return mode;
}
void ui_t::keycommleft(void) {
	if (commposy == 0) return;
	commposy--;
	refreshscr(0);
}
void ui_t::keycommright(void) {
	if (commposy == strlen(commbuf[commposx])) return;
	commposy++;
	refreshscr(0);
}
void ui_t::keycommdeleteleft(void) {
	if (commposy == 0) return;
	for (int i = commposy, l = strlen(commbuf[commposx]); i <= l; i++)
		commbuf[commposx][i-1] = commbuf[commposx][i];
	commposy--;
	refreshscr(0);
}
void ui_t::keycommdeleteright(void) {
	int l = strlen(commbuf[commposx]);
	if (commposy == l) return;
	for (int i = commposy; i < l; i++)
		commbuf[commposx][i] = commbuf[commposx][i+1];
	refreshscr(0);
}
void ui_t::keycomminsert(int c) {
	for (int i = strlen(commbuf[commposx]); i >= commposy; i--)
		commbuf[commposx][i+1] = commbuf[commposx][i];
	commbuf[commposx][commposy] = c;
	commposy++;
	refreshscr(0);
}
void ui_t::resize(int _h, int _w) {
	h = _h-1, w = _w;
	window_adjust();
	refreshscr(1);
}
void ui_t::set_mode(int _mode) {
	mode = _mode;
	refreshscr(1);
}
void ui_t::start_comm(const char *name) {
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
void ui_t::end_comm(void) {
	set_mode(0);
}
