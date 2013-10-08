#include <ncurses.h>
#include <cstring>
#include <string>
#include <vector>
#include "editor.h"
#include "ui.h"
using std::vector;
using std::string;
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
	if (flag) {
		vector<string> data;
		editor->retrieve(scrx, scry, h, w, data);
		for (int i = 0; i < h; i++) {
			move(i, 0);
			clrtoeol();
			if (i >= data.size()) continue;
			for (int j = 0; j < w && j < data[i].size(); j++)
				addch(data[i][j]);
		}
	}
	move(h, 0);
	clrtoeol();
	if (w-20-10-2 >= 0) {
		move(h, 10);
		char fmt[10];
		sprintf(fmt, "%%.%ds", w-20-10-2);
		printw(fmt, file[0] ? file : "[NEW]");
		refresh();
	}
	move(h, w-20);
	printw(" %d,%d", posx+1, posy+1);
	refresh();
	move(posx - scrx, posy - scry);
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
	int newdy;
	editor->go_y(posy, -1, newdy);
	posy += newdy;
	realposy = posy;
	if (newdy == 0) return;
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keyright(void) {
	int newdy;
	editor->go_y(posy, 1, newdy);
	posy += newdy;
	realposy = posy;
	if (newdy == 0) return;
	int refresh_flag = window_adjust();
	refreshscr(refresh_flag);
}
void ui_t::keypageup(void) {
	int newdx, newy;
	scrx -= h/2;
	if (scrx < 0) scrx = 0;
	editor->go_x(-h/2, realposy, newdx, newy);
	posx += newdx, posy = newy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keypagedown(void) {
	int newdx, newy;
	scrx += h/2;
	if (scrx + h > rows) scrx = rows - h;
	if (scrx < 0) scrx = 0;
	editor->go_x(h/2, realposy, newdx, newy);
	posx += newdx, posy = newy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keypageleft(void) {
	int newdy;
	scry -= w/2;
	if (scry < 0) scry = 0;
	editor->go_y(posy, -w/2, newdy);
	posy += newdy;
	realposy = posy;
	window_adjust();
	refreshscr(1);
}
void ui_t::keypageright(void) {
	int newdy;
	scry += w/2;
	editor->go_y(posy, w/2, newdy);
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
void ui_t::keydelete(void) {
	editor->erase(posx);
	rows--;
	int dx = 0, newdx;
	if (posx >= rows && posx > 0) {
		dx = -1;
		posx = rows-1;
	}
	editor->go_x(dx, realposy, newdx, posy);
	refreshscr(1);
}
void ui_t::resize(int _h, int _w) {
	h = _h-1, w = _w;
	if (posx >= scrx + h)
		scrx = posx - (h-1);
	if (posy >= scry + w)
		scry = posy - w/2;
	if (scrx + h > rows) scrx = rows - h;
	if (scrx < 0) scrx = 0;
	refreshscr(1);
}
