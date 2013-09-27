#ifndef _UI_H_
#define _UI_H_
#include "editor.h"
typedef struct ui_t {
	int w, h, posx, posy, scrx, scry, realposy;
	editor_t *editor;
	char *file;
} ui_t;
void ui_initialize(ui_t **ui, const char *file);
void ui_keydown(ui_t *ui);
void ui_keyup(ui_t *ui);
void ui_keyleft(ui_t *ui);
void ui_keyright(ui_t *ui);
#endif
