#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "editor.h"
#include "container.h"

/*
 * read the content from the file and initialize the editor.
 */

void editor_t::initialize(FILE *fp) {
	int c;
	Xpos = Ypos = nRow = nCol = nChar = 0;
	_char_t* tmp = new _char_t;
	/* read the content from a file */
	if (fp != NULL) {
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
	a.insert(a.end(), *tmp);
	/* initialize the information of the editor */
	nRow++;
	Xit = a.begin();
	Yit = Xit->value.begin();
}

/*
 * get the infomation of current state
 */
void editor_t::info(int &num_row, int &num_char) {
	num_row = nRow, num_char = nChar;
}

/*
 * get the content in a rectangle, and print it on screen
 */

void editor_t::retrieve(int x, int y, int h, int w, std::vector<std::string>& ret) {
	_line_t::iterator line_it = a.getPos(x);
	ret.clear();
	for (int i = 0; i < h && line_it != a.end(); i++) {
		std::string st;
		_char_t::iterator char_it = line_it->value.begin();
		int nowy = 0, newy;
		for (; char_it != line_it->value.end();) {
			/* resolve the TAB issue */
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

/*
 * move the current position horizontally
 */

void editor_t::go_y(int dy, int &resdy) {
	int dir = 1;
	if (dy < 0) dy = -dy, dir = 0;
	resdy = 0;
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

/*
 * move the current position vertically
 */

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

/*
 * jump the a special line
 */

int editor_t::aim_to_line(int lineno) {
	int ret;
	if (lineno >= nRow) ret = nRow - 1;
	else ret = lineno;
	Xpos = ret; Ypos = 0;
	Xit = a.getPos(Xpos);
	Yit = Xit->value.begin();
	return ret;
}

/*
 * delete a interval of lines
 */
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

/*
 * insert a line, and the dir argument denote that the relative position of the current line.
 */

void editor_t::insert(int dir) {
	if (dir == 0) {
		Xit = a.insert(Xit, *new _char_t);
	} else {
		Xit = a.insert(Xit->ch[1], *new _char_t());
		Xpos++;
	}
	Yit = Xit->value.begin();
	Ypos = 0;
	nRow++;
}

/*
 * insert a single character
 */

void editor_t::insert_c(int c) {
	if (c != '\n') {
		Yit = Xit->value.insert(Yit, c);
	} else if (Ypos == 0) {
		Xit = a.insert(Xit, *new _char_t());
		Yit = Xit->value.begin();
		nRow++;
	} else {
		_char_t::iterator tmp = Xit->value.Prev(Yit);
		_line_t::iterator Next = Xit;
		Xit = a.insert(Xit, *new _char_t());
		Xit->value.begin() = Next->value.begin();
		Xit->value.begin()->ch[0] = Xit->value.end();
		Xit->value.end()->ch[1] = Xit->value.begin();
		tmp->ch[1] = Xit->value.end();
		Xit->value.end()->ch[0] = tmp;
		Next->value.begin() = Yit;
		Yit->ch[0] = Next->value.end();
		Next->value.end()->ch[1] = Yit;
		Next->value.Update(Yit);
		Yit = Xit->value.end();
		nRow++;
	}
}

/*
 * delete a single character.
 */

int editor_t::delete_c(void) {
	if (Yit != Xit->value.end()) {
		int ret = Yit->value;
		Yit = Xit->value.erase(Yit);
		return ret;
	} else if (Xit->ch[1] == a.end()) {
		return '\0';
	} else if (Xit->value.begin() == Xit->value.end()) {
		Xit = a.erase(Xit);
		Yit = Xit->value.begin();
		nRow--;
		return '\n';
	} else {
		_char_t::iterator tmp = Xit->value.Prev(Yit);
		_line_t::iterator Next = Xit->ch[1];
		Yit = Next->value.begin();
		Yit->ch[0] = tmp;
		tmp->ch[1] = Yit;
		Next->value.begin() = Xit->value.begin();
		Next->value.begin()->ch[0] = Next->value.end();
		Next->value.end()->ch[1] = Next->value.begin();
		Next->value.Update(Yit);
		a.erase(Xit);
		Xit = Next;
		nRow--;
		return '\n';
	}
}

/*
 * jump to the end of a line
 */
int editor_t::aim_to_end(void) {
	Ypos = Xit->value.size();
	Yit = Xit->value.end();
	return Yit->ch[0]->sum;
}

/*
 * jump to the begin of a line
 */

void editor_t::aim_to_begin(void) {
	Ypos = 0; Yit = Xit->value.begin();
}

/*
 * save current content into a special file
 */
void editor_t::save_to_file(FILE *fp) {
	for (_line_t::iterator it = a.begin(); it != a.end(); it = it->ch[1]) {
		for (_char_t::iterator ot = it->value.begin(); ot != it->value.end(); ot = ot->ch[1]) {
			fputc(ot->value, fp);
		}
		fputc('\n', fp);
	}
	fclose(fp);
}

/*
 * find the position where the special string ocurred in the file
 */
void editor_t::find(const char* str, int &resx, int &resy) {
	resx = Xpos; resy = Yit->ch[0]->sum;
	std::string ss(str);
	_line_t::iterator it = Xit; _char_t::iterator res;
	res = it->value.match(ss, Yit);
	if (res != it->value.end()) {
		resx = Xpos;
		resy = res->ch[0]->sum;
		Ypos = it->value.getWhere(res);
		Yit = res;
		return;
	}
	int tmp = Xpos + 1;
	it = it->ch[1];
	for (;it != a.end(); it = it->ch[1], tmp++) {
		res = it->value.match(ss, it->value.begin());
		if (res != it->value.end()) {
			resx = Xpos = tmp;
			resy = res->ch[0]->sum;
			Ypos = it->value.getWhere(res);
			Yit = res;
			return;
		}
	}
}

/*
 * find the position where the special string ocurred in the file in a reverse direction.
 */
void editor_t::find_rev(const char *str, int &resx, int &resy) {
	resx = Xpos; resy = Yit->ch[0]->sum;
	std::string ss(str);
	_line_t::iterator it = Xit; _char_t::iterator res;
	res = it->value.match(ss, Yit, 0);
	if (res != it->value.end()) {
		resx = Xpos;
		resy = res->ch[0]->sum;
		Ypos = it->value.getWhere(res);
		Yit = res;
		return;
	}
	int tmp = Xpos - 1; it = it->ch[0];
	for (; it != a.end(); it = it->ch[0], tmp--) {
		res = it->value.match(ss, it->value.end()->ch[0]);
		if (res != it->value.end()) {
			resx = Xpos = tmp;
			resy = res->ch[0]->sum;
			Ypos = it->value.getWhere(res);
			Yit = res;
			return;
		}
	}
}

/*
 * replace a string to another one.
 */

void editor_t::replace_all(const char *str, const char* dst) {
	std::string ss(str); int Len = strlen(dst);
	for (_line_t::iterator it = a.begin(); it != a.end(); it = it->ch[1]) {
		_char_t::iterator ot = it->value.begin();
		while (true) {
			ot = it->value.match(ss, ot);
			if (ot != it->value.end()) {
				_char_t::iterator other = ot;
				for (int i = 0; i < (int)ss.length(); i++) other = other->ch[1];
				ot = it->value.erase(ot, other);
				assert(other == ot);
				for (int i = 0; i < Len; i++) {
					other = it->value.insert(other, dst[i]);
				}
			} else break;
		}
	}
	Xpos = Ypos = 0;
	Xit = a.begin(); Yit = Xit->value.begin();
}

