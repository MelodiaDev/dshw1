#ifndef _EDITOR_H_
#define _EDITOR_H_
#define TAB_WIDTH 4
typedef struct editor_t {
	char *data, **line;
	int posx, posy, row, noeol, sum;
} editor_t;
void editor_initialize(editor_t **editor, const char *file);
void editor_retrieve(editor_t *editor, int x, int y, int h, int w, char **res);
void editor_go_y(editor_t *editor, int y, int dy, int *resdy);
void editor_go_x(editor_t *editor, int dx, int y, int *resdx, int *resy);
#endif
