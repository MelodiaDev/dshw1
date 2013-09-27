#include <stdio.h>
#include <stdlib.h>
#include "editor.h"
void editor_initialize(editor_t **editor, const char *file) {
	*editor = (editor_t *) malloc(sizeof(editor_t));
	FILE *fp = fopen(file, "r");
	(*editor)->row = 0;
	int ch, prech = -1;
	(*editor)->sum = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n') (*editor)->row++;
		prech = ch;
		(*editor)->sum++;
	}
	if (prech != -1 && prech != '\n') {
		(*editor)->noeol = 1;
		(*editor)->sum++;
		(*editor)->row++;
	}
	fclose(fp);
	(*editor)->data = (char *) malloc((*editor)->sum);
	(*editor)->line = (char **) malloc(((*editor)->row+1) * sizeof(char *));
	fp = fopen(file, "r");
	int pos = 0, linepos = 0, newline = 1;
	while ((ch = fgetc(fp)) != EOF) {
		(*editor)->data[pos++] = ch;
		if (newline) {
			(*editor)->line[linepos++] = (*editor)->data + pos-1;
			newline = 0;
		}
		if (ch == '\n') newline = 1;
	}
	fclose(fp);
	if ((*editor)->noeol) (*editor)->data[pos++] = '\n';
	(*editor)->line[linepos] = (*editor)->data + pos;
	(*editor)->posx = (*editor)->posy = 0;
}
void editor_retrieve(editor_t *editor, int x, int y, int h, int w, char **res) {
	int i, j;
	for (i = 0; i < h; i++)
		if (x+i < editor->row) {
			if (y >= editor->line[x+i+1] - editor->line[x+i]) {
				res[i][0] = '\n';
				continue;
			}
			for (j = 0; j < w; j++)
				if (y+j < editor->line[x+i+1] - editor->line[x+i])
					res[i][j] = editor->line[x+i][y+j];
				else break;
		} else
			res[i][0] = 0;
}
void editor_gooffset(editor_t *editor, int x, int y, int *resx, int *resy) {
	*resx = x, *resy = y;
	if (x >= editor->row)
		*resx = editor->row-1;
	if (x < 0)
		*resx = 0;
	if (y >= editor->line[x+1] - editor->line[x] - 1)
		*resy = editor->line[x+1] - editor->line[x] - 2;
	if (*resy < 0)
		*resy = 0;
}
