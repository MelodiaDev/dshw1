#ifndef _EDITOR_H_
#define _EDITOR_H_
#define TAB_WIDTH 4
class editor_t {
	protected:
		char *data, **line;
		int posx, posy, row, noeol, sum;
	public:
		editor_t() { }
		void initialize(const char *file);
		void retrieve(int x, int y, int h, int w, char **res);
		void go_y(int y, int dy, int &resdy);
		void go_x(int dx, int y, int &resdx, int &resy);
};
#endif
