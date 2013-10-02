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
	} else (*editor)->noeol = 0;
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
			int nowy = 0, newy, k, flag = 0;
			for (j = 0; j < editor->line[x+i+1] - editor->line[x+i]; j++) {
				if (editor->line[x+i][j] == '\t')
					newy = (nowy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
				else
					newy = nowy + 1;
				if (newy > y) {
					flag = 1;
					for (k = (nowy < y ? y : nowy); k < newy && k < y+w; k++)
						if (editor->line[x+i][j] == '\t')
							res[i][k-y] = ' ';
						else if (editor->line[x+i][j] == '\n')
							res[i][k-y] = 0;
						else if (editor->line[x+i][j] > 31 && editor->line[x+i][j] < 127)
							res[i][k-y] = editor->line[x+i][j];
						else
							res[i][k-y] = '.';
				}
				nowy = newy;
				if (nowy >= y+w) break;
			}
			if (!flag) res[i][0] = 0;
		} else
			res[i][0] = 0;
}
void editor_go_y(editor_t *editor, int y, int dy, int *resdy) {
	if (!editor->sum) {
		*resdy = 0;
		return;
	}
	int newy = y, des = y+dy;
	if (dy >= 0)
		for (; newy < des && editor->posy < editor->line[editor->posx+1] - editor->line[editor->posx] - 1; editor->posy++)
			if (editor->line[editor->posx][editor->posy] == '\t')
				newy = (newy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
			else
				newy++;
	else {
		int nowy = 0;
		editor->posy = 0;
		for (; editor->posy < editor->line[editor->posx+1] - editor->line[editor->posx] - 1; editor->posy++) { 
			if (editor->line[editor->posx][editor->posy] == '\t')
				newy = (nowy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
			else
				newy = nowy + 1;
			if (newy > des) break;
			nowy = newy;
		}
		newy = nowy;
	}
	*resdy = newy - y;
}
void editor_go_x(editor_t *editor, int dx, int y, int *resdx, int *resy) {
	if (!editor->sum) {
		*resdx = *resy = 0;
		return;
	}
	int newx = editor->posx + dx;
	if (newx >= editor->row)
		newx = editor->row-1;
	if (newx < 0)
		newx = 0;
	*resdx = newx - editor->posx;
	editor->posx = newx;
	if (*resdx == 0)
		*resy = y;
	else {
		editor->posy = 0;
		editor_go_y(editor, 0, y, resy);
	}
}
