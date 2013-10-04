#include <cstdio>

#include "editor.h"
#include "container.h"

void editor_t::initialize(const char *file) {
	FILE *fp = fopen(file, "r");
	int c;
	Xpos = Ypos = nRow = nCol = nChar = 0;
	_char_t* tmp = new _char_t;
	while ((c = fgetc(fp)) != EOF) {
		if (c != '\n') {
			tmp->insert(tmp->end(), c);
		} else {
			a.insert(a.end(), *tmp);
			tmp = new _char_t;
			nRow++;
		}
		nChar++;
	}
}

void editor_t::retrieve(int x, int y, int h, int w, std::vector<std::string>& ret) {
	_line_t::iterator line_it = a.getPos(x);
	ret.clear();
	for (int i = 0; i < h && line_it != a.end(); i++) {
		std::string st;
		_char_t::iterator char_it = line_it->value.getPos(y);
		for (int j = 0; j < w && char_it != line_it->value.end();) {
			if (char_it->value != '\t') {
				st += char_it->value;
				j++;
			} else {
				for (int k = 0; k < TAB_WIDTH && k < w - j; k++) st += ' ';
				j += TAB_WIDTH;
			}
			char_it = line_it->value.Next(char_it);
		}
		line_it = a.Next(line_it);
		ret.push_back(st);
	}
}

void editor_t::go_y(int y, int dy, int &resdy) {
	int dir = 1;
	if (dy < 0) dy = -dy, dir = 0;
	resdy = 0;
	_char_t now = a.getPos(Xpos)->value;
	_char_t::iterator it = now.begin();
	for (int i = 0; i < y; it = it->ch[1])
		if (it->value == '\t') i += 4;
		else i++;
	if (!dir) it = it->ch[dir];
	for (; resdy < dy && it != now.end(); it = it->ch[dir]) {
		if (it->value == '\t') resdy += 4;
		else resdy++;
	}
	if (!dir) {
		resdy = -resdy;
		if (it == now.end()) it = now.begin();
		else it = it->ch[!dir];
	}
}

void editor_t::go_x(int dx, int y, int &resdx, int &resdy) {
	int tmp = Xpos; Xpos += dx;
	if (Xpos < 0) Xpos = 0;
	if (Xpos >= nRow) Xpos = nRow - 1;
	resdx = Xpos - tmp;
	Ypos = 0;
	go_y(0, y, resdy);
}

