#ifndef _UI_H_
#define _UI_H_
#include "editor.h"
#define COMM_BUFSIZE 1024
#define MODE_COMM 1
#define MODE_INS 2
class ui_t {
	protected:
		int posx, posy, scrx, scry, realposy;
		int rows;
		int mode;
		int commarg, commposx, commposy;
		int prerealh;
		const char* commname;
		const char** commpref;
		char commbuf[10][COMM_BUFSIZE];
		editor_t *editor;
		char *file;
		int window_adjust(void);
	public:
		int w, h;
		ui_t() { }
		void initialize(const char *_file);
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
		void keyhead(void);
		void keytail(void);
		void keydeletedown(void);
		void keydeleteup(void);
		void keydeleteleft(void);
		void keydeleteright(void);
		void keyinsertdown(void);
		void keyinsertup(void);
		void keyinsert(int c);
		void keycommup(void);
		void keycommdown(void);
		int keycommnextarg(void);
		void keycommleft(void);
		void keycommright(void);
		void keycommdeleteleft(void);
		void keycommdeleteright(void);
		void keycomminsert(int c);
		void refreshscr(int flag);
		void resize(int _h, int _w);
		void set_mode(int _mode);
		void start_comm(const char *name);
		void end_comm(void);
};
#endif
