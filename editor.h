#ifndef _EDITOR_H_
#define _EDITOR_H_

#include <vector>
#include <string>

#include "container.h"

class editor_t {
	protected:
		/* the character container of a line */
		typedef editor_list<char> _char_t;
		
		/* the line container of the whole file */
		typedef editor_list<_char_t> _line_t;

		/* the whole file */
		_line_t a;
		
		/* the pointer of the current line */
		_line_t::iterator Xit; 

		/* the pointer of the current character */
		_char_t::iterator Yit;

		/* the current coodination */
		int Xpos;
		
		/* the number of rows */
		int nRow;
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
		void save_to_file(FILE *fp);
		void find(const char* str, int &resx, int &resy);
		void find_rev(const char* str, int &resx, int &resy);
		void replace_all(const char *str, const char *dst);
};
#endif

