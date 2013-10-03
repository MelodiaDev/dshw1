#include <ncurses.h>
#include <cstring>
#include "editor.h"
#include "ui.h"
void ui_t::initialize(const char *_file) {
	posx = posy = realposy = scrx = scry = 0;
	editor = new editor_t;
	editor->initialize(_file);
	file = new char[strlen(_file)+1];
	strcpy(file, _file);
}
void ui_t::refresh(int flag) {
	if (flag) {
		char *buf = new char[w * h];
		char **data = new char*[h];
		int i, j;
		for (i = 0; i < h; i++)
			data[i] = buf + i*w;
		editor->retrieve(scrx, scry, h, w, data);
		for (i = 0; i < h; i++) {
			move(i, 0);
			clrtoeol();
			for (j = 0; j < w; j++)
				if (data[i][j] != 0 && data[i][j] != '\n')
					addch(data[i][j] == '\t' ? ' ' : data[i][j]);
				else
					break;
		}
		delete [] data;
		delete [] buf;
	}
	move(posx - scrx, posy - scry);
	//refresh();
}
void ui_t::keydown(void) {
	int newdx, newy;
	editor->go_x(1, realposy, newdx, newy);
	if (newdx == 0) return;
	posx += newdx, posy = newy;
	int refresh_flag = 0;
	if (posx >= scrx + h) {
		scrx++;
		refresh_flag = 1;
	}
	if (posy < scry) {
		scry -= ((scry - posy - 1) / (w/2) + 1) * (w/2);
		if (scry < 0) scry = 0;
		refresh_flag = 1;
	} else if (posy >= scry + w) {
		scry += ((posy - scry - w) / (w/2) + 1) * (w/2);
		refresh_flag = 1;
	}
	refresh(refresh_flag);
}
void ui_t::keyup(void) {
	int newdx, newy;
	editor->go_x(-1, realposy, newdx, newy);
	if (newdx == 0) return;
	posx += newdx, posy = newy;
	int refresh_flag = 0;
	if (posx < scrx) {
		scrx--;
		refresh_flag = 1;
	}
	if (posy < scry) {
		scry -= ((scry - posy - 1) / (w/2) + 1) * (w/2);
		if (scry < 0) scry = 0;
		refresh_flag = 1;
	} else if (posy >= scry + w) {
		scry += ((posy - scry - w) / (w/2) + 1) * (w/2);
		refresh_flag = 1;
	}
	refresh(refresh_flag);
}
void ui_t::keyleft(void) {
	int newdy;
	editor->go_y(posy, -1, newdy);
	posy += newdy;
	realposy = posy;
	if (newdy == 0) return;
	int refresh_flag = 0;
	if (posy < scry) {
		scry -= w/2;
		if (scry < 0) scry = 0;
		refresh_flag = 1;
	}
	refresh(refresh_flag);
}
void ui_t::keyright(void) {
	int newdy;
	editor->go_y(posy, 1, newdy);
	posy += newdy;
	realposy = posy;
	if (newdy == 0) return;
	int refresh_flag = 0;
	if (posy >= scry + w) {
		scry += w/2;
		refresh_flag = 1;
	}
	refresh(refresh_flag);
}
void ui_t::resize(int _h, int _w) {
	h = _h, w = _w;
	if (posx >= scrx + h)
		scrx = posx - (h-1);
	if (posy >= scry + w)
		scry = posy - w/2;
	refresh(1);
}
