#ifndef _EDITOR_H_
#define _EDITOR_H_
typedef struct editor_t {
	char *data, **line;
	int posx, posy, row, noeol, sum;
} editor_t;
void editor_initialize(editor_t **editor, const char *file);
void editor_retrieve(editor_t *editor, int x, int y, int h, int w, char **res);
void editor_gooffset(editor_t *editor, int x, int y, int *resx, int *resy);
#endif
