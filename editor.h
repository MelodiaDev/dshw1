#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <vector>
#include <string>

#include "container.h"

class editor_t {
	protected:
		typedef editor_list<char> _char_t;
		typedef editor_list<_char_t> _line_t;
		_line_t a; _line_t::iterator Xit; _char_t::iterator Yit;
		int Xpos, nRow;
	public:
		editor_t() {Xit = a.end(); }
		void initialize(FILE *fp);
		void info(int &num_row);
		void retrieve(int x, int y, int h, int w, std::vector<std::string>&);
		void go_y(int dy, int &resdy);
		void go_x(int dx, int y, int &resdx, int &resy);
		int aim_to_line(int lineno);
		void erase(int bias);
		void insert(int dir = 0);
		void insert_c(int);
		int delete_c(void);
		int aim_to_end(void);
		void aim_to_begin(void);
		void saveToFile(FILE *fp);
		void Find(const char* str, int &resx, int &resy);
		void Find_rev(const char* str, int &resx, int &resy);
};
#endif

