#include <cstdlib>
#include "editor.h"
void editor_t::initialize(const char *file) {
	FILE *fp = fopen(file, "r");
	row = 0;
	int ch, prech = -1;
	sum = 0;
	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n') row++;
		prech = ch;
		sum++;
	}
	if (prech != -1 && prech != '\n') {
		noeol = 1;
		sum++;
		row++;
	} else noeol = 0;
	fclose(fp);
	data = new char[sum];
	line = new char*[row+1];
	fp = fopen(file, "r");
	int pos = 0, linepos = 0, newline = 1;
	while ((ch = fgetc(fp)) != EOF) {
		data[pos++] = ch;
		if (newline) {
			line[linepos++] = data + pos-1;
			newline = 0;
		}
		if (ch == '\n') newline = 1;
	}
	fclose(fp);
	if (noeol) data[pos++] = '\n';
	line[linepos] = data + pos;
	posx = posy = 0;
}
void editor_t::retrieve(int x, int y, int h, int w, char **res) {
	int i, j;
	for (i = 0; i < h; i++)
		if (x+i < row) {
			int nowy = 0, newy, k, flag = 0;
			for (j = 0; j < line[x+i+1] - line[x+i]; j++) {
				if (line[x+i][j] == '\t')
					newy = (nowy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
				else
					newy = nowy + 1;
				if (newy > y) {
					flag = 1;
					for (k = (nowy < y ? y : nowy); k < newy && k < y+w; k++)
						if (line[x+i][j] == '\t')
							res[i][k-y] = ' ';
						else if (line[x+i][j] == '\n')
							res[i][k-y] = 0;
						else if (line[x+i][j] > 31 && line[x+i][j] < 127)
							res[i][k-y] = line[x+i][j];
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
void editor_t::go_y(int y, int dy, int &resdy) {
	if (!sum) {
		resdy = 0;
		return;
	}
	int newy = y, des = y+dy;
	if (dy >= 0)
		for (; newy < des && posy < line[posx+1] - line[posx] - 1; posy++)
			if (line[posx][posy] == '\t')
				newy = (newy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
			else
				newy++;
	else {
		int nowy = 0;
		posy = 0;
		for (; posy < line[posx+1] - line[posx] - 1; posy++) { 
			if (line[posx][posy] == '\t')
				newy = (nowy + TAB_WIDTH) / TAB_WIDTH * TAB_WIDTH;
			else
				newy = nowy + 1;
			if (newy > des) break;
			nowy = newy;
		}
		newy = nowy;
	}
	resdy = newy - y;
}
void editor_t::go_x(int dx, int y, int &resdx, int &resy) {
	if (!sum) {
		resdx = resy = 0;
		return;
	}
	int newx = posx + dx;
	if (newx >= row)
		newx = row-1;
	if (newx < 0)
		newx = 0;
	resdx = newx - posx;
	posx = newx;
	if (resdx == 0)
		resy = y;
	else {
		posy = 0;
		go_y(0, y, resy);
	}
}
