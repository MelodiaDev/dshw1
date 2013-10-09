#include <cstdio>
#include <cstdlib>

#include "editor.h"
#include "container.h"

void editor_t::initialize(const char *file) {
	FILE *fp = fopen(file, "r");
	int c;
	Xpos = Ypos = nRow = nCol = nChar = 0;
	if (fp == NULL) return;
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
	a.insert(a.end(), *tmp);
	nRow++;
	Xit = a.begin();
	Yit = Xit->value.begin();
}

void editor_t::info(int &num_row, int &num_char) {
	num_row = nRow, num_char = nChar;
}

void editor_t::retrieve(int x, int y, int h, int w, std::vector<std::string>& ret) {
	_line_t::iterator line_it = a.getPos(x);
	ret.clear();
	for (int i = 0; i < h && line_it != a.end(); i++) {
		std::string st;
		_char_t::iterator char_it = line_it->value.begin();
		int nowy = 0, newy;
		for (; char_it != line_it->value.end();) {
			if (char_it->value == '\t')
				newy = (nowy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
			else
				newy = nowy + 1;
			for (int k = (nowy < y ? y : nowy); k < newy && k < y+w; k++)
				if (char_it->value == '\t')
					st += ' ';
				else if (char_it->value > 31 && char_it->value < 127)
					st += char_it->value;
				else
					st += '.';
			nowy = newy;
			if (nowy >= y+w) break;
			char_it = line_it->value.Next(char_it);
		}
		line_it = a.Next(line_it);
		ret.push_back(st);
	}
}

void editor_t::go_y(int dy, int &resdy) {
	int dir = 1;
	if (dy < 0) dy = -dy, dir = 0;
	resdy = 0;
	int back_y = Ypos;
	_char_t::iterator it = Yit;
	int tmp = 0;
	if (dir == 1) {
		while (tmp < dy && it != Xit->value.end()) {
			tmp += it->sum - it->ch[0]->sum;
			it = it->ch[1];
			Ypos++;
		}
	} else {
		while (tmp < dy && it->ch[0] != Xit->value.end()) {
			tmp += it->ch[0]->sum - it->ch[0]->ch[0]->sum;
			it = it->ch[0];
			Ypos--;
		}
		tmp = -tmp;
	}
	resdy = tmp;
	Yit = it;
}

void editor_t::go_x(int dx, int y, int &resdx, int &resdy) {
	int tmp = Xpos; 
	Xit = a.getPosAt(Xit, dx);
	Xpos += dx;
	if (Xpos < 0) Xpos = 0;
	if (Xpos >= nRow) Xpos = nRow - 1;
	resdx = Xpos - tmp;
	Ypos = 0; Yit = Xit->value.begin();
	go_y(y, resdy);
}

int editor_t::aim_to_line(int lineno) {
	int ret;
	if (lineno >= nRow) ret = nRow - 1;
	else ret = lineno;
	Xpos = ret; Ypos = 0;
	Xit = a.getPos(Xpos);
	Yit = Xit->value.begin();
	return ret;
}

void editor_t::erase(int bias) {
	int sig = 1; if (bias < 0) sig = -1, bias = -bias;
	_line_t::iterator it = Xit;
	_line_t::iterator ot = a.getPosAt(it, sig * (bias - 1));
	if (sig < 0) {
		Xit = a.erase(ot, it->ch[1])->ch[0], Yit = Xit->value.begin();
		Xpos -= bias;
	}
	else {
		Xit = a.erase(it, ot->ch[1]), Yit = Xit->value.begin();
	}
	Ypos = 0;
	nRow -= bias;
}

void editor_t::insert(int dir) {
	_char_t *tmp = new _char_t();
	if (dir == 0) {
		a.insert(Xit, *tmp);
	} else {
		a.insert(Xit->ch[1], *tmp);
		Xpos++;
	}
	Xit = Xit->ch[dir];
	Yit = Xit->value.begin();
	Ypos = 0;
	nRow++;
}

void editor_t::insert_c(int c) {
	if (c != '\n') {
		Yit = Xit->value.insert(Yit, c);
	} else {
		_line_t::iterator tmp = Xit;
		Xit = a.insert(Xit->ch[1], editor_list<char>(Yit, Xit->value.end()));
		tmp->value.erase(Yit, Xit->value.end());
		Yit = Xit->value.begin();
		Ypos = 0;
		nRow++;
	}
}

int editor_t::delete_c(int dir) {
	if (dir == 0) {
		if (Yit == Xit->value.begin()) return '\0';
		int ret = Yit->ch[0]->value;
		Xit->value.erase(Yit->ch[0]);
		Ypos--;
		return ret;
	} else
	if (Yit != Xit->value.end()) {
		int ret = Yit->value;
		Yit = Xit->value.erase(Yit);
		return ret;
	} else {
		_char_t::iterator tmp = Xit->value.Prev(Xit->value.end());
		_line_t::iterator Next = Xit->ch[1];
		Next->value.begin()->ch[0] = tmp;
		tmp->ch[1] = Next->value.begin();
		_char_t::iterator tt = Next->value.Prev(Next->value.end());
		tt->ch[1] = Xit->value.end();
		Xit->value.Update(tmp);
		Xit->ch[1] = Next->ch[1];
		Next->ch[1]->ch[0] = Xit->ch[0];
		delete Next;
		return '\n';
	}
}

void editor_t::aim_to_end(void) {
	Ypos = Xit->value.size();
	Yit = Xit->value.end();
}

void editor_t::aim_to_begin(void) {
	Ypos = 0; Yit = Xit->value.end();
}

