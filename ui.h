#ifndef _UI_H_
#define _UI_H_
#include "editor.h"
class ui_t {
	protected:
		int posx, posy, scrx, scry, realposy;
		editor_t *editor;
		char *file;
	public:
		int w, h;
		ui_t() { }
		void initialize(const char *file);
		void keydown(void);
		void keyup(void);
		void keyleft(void);
		void keyright(void);
		void refresh(int flag);
		void resize(int _h, int _w);
};
#endif
