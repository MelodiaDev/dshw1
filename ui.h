#ifndef _UI_H_
#define _UI_H_
#include "editor.h"
#define COMM_BUFSIZE 1024
#define MODE_COMM 1
#define MODE_INS 2
class ui_t {
	protected:
		/* file name */
		char *file;
		/* width and height of screen without status line */
		int w, h;
		/* displayed position and desired position of current cursor */
		int posx, posy, realposy;
		/* position of upper-left corner of the screen */
		int scrx, scry;
		/* number of rows of file */
		int rows;
		/* mode number */
		int mode;
		/* number of arguments in command mode */
		int commarg;
		/* position of current cursor in command mode */
		int commposx, commposy;
		/* previous height of the text area, used in command mode to judge
		 * whether to refresh the text area */
		int prerealh;
		/* command name */
		const char* commname;
		/* prefixes of arguments */
		const char** commpref;
		/* user typed arguments */
		char commbuf[10][COMM_BUFSIZE];
		/* last searched word */
		char findword[COMM_BUFSIZE];
		/* property flags of opened file */
		int file_new, file_error, file_edited;
		/* error flag */
		int errtype;
		/* editor backend */
		editor_t *editor;
		/* adjust screen position by current position of cursor */
		int adjustscr(void);
		/* redraw screen
		 * flag: 0 - only refresh status line and command area
		 *       1 - thorough refresh */
		void refreshscr(int flag);
	public:
		ui_t() { }
		/* initialization function */
		void initialize(const char *_file);

		/* key actions in normal/insert mode */
		/* move up/down a line */
		void key_up(void);
		void key_down(void);
		/* move left/right a character */
		void key_left(void);
		void key_right(void);
		/* move up/down half a screen */
		void key_pageup(void);
		void key_pagedown(void);
		/* move left/right half a screen */
		void key_pageleft(void);
		void key_pageright(void);
		/* move to the first/last line of file */
		void key_top(void);
		void key_bottom(void);
		/* move to the first/last character of current line */
		void key_head(void);
		void key_tail(void);
		/* delete current line and move up/down */
		void key_deleteup(void);
		void key_deletedown(void);
		/* delete previous/next character */
		void key_deleteleft(void);
		void key_deleteright(void);
		/* insert a blank line before/after the current line and move to it */
		void key_insertup(void);
		void key_insertdown(void);
		/* insert a character after the cursor and move right */
		void key_insert(int c);
		/* find previous/next match */
		void key_matchbackward(void);
		void key_matchforward(void);

		/* key actions in command mode */
		/* move to previous/next argument */
		void comm_key_up(void);
		void comm_key_down(void);
		/* move left/right a character */
		void comm_key_left(void);
		void comm_key_right(void);
		/* delete previous/next character */
		void comm_key_deleteleft(void);
		void comm_key_deleteright(void);
		/* insert a character after the cursor and move right */
		void comm_key_insert(int c);

		/* screen resize */
		void resizescr(int _h, int _w);
		/* set new mode */
		void set_mode(int _mode);
		/* start command */
		void comm_start(const char *name);
		/* end command */
		void comm_end(void);
};
#endif
