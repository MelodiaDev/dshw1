#ifndef _UI_H_
#define _UI_H_
#include "editor.h"
class ui_t {
	protected:
		int posx, posy, scrx, scry, realposy;
		int rows;
		editor_t *editor;
		char *file;
		int window_adjust(void);
	public:
		int w, h;
		ui_t() { }
		void initialize(const char *file);
		void keyup(void);
		void keydown(void);
		void keyleft(void);
		void keyright(void);
		void keypageup(void);
		void keypagedown(void);
		void keypageleft(void);
		void keypageright(void);
		void keytop(void);
		void keybottom(void);
		void keydeletedown(void);
		void keydeleteup(void);
		void keyinsertdown(void);
		void keyinsertup(void);
		void refreshscr(int flag);
		void resize(int _h, int _w);
};
#endif
