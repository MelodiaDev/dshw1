#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "editor.h"
#include "ui.h"
void ui_initialize(ui_t **ui, const char *file) {
	*ui = (ui_t *) malloc(sizeof(ui_t));
	(*ui)->posx = (*ui)->posy = (*ui)->scrx = (*ui)->scry = 0;
	(*ui)->realposy = 0;
	editor_initialize(&(*ui)->editor, file);
	(*ui)->file = (char *) malloc(strlen(file)+1);
	strcpy((*ui)->file, file);
}
void ui_refresh(ui_t *ui, int flag) {
	if (flag) {
		char *buf = (char *) malloc(ui->w * ui->h);
		char **data = (char **) malloc(ui->h * sizeof(char *));
		int i, j;
		for (i = 0; i < ui->h; i++)
			data[i] = buf + i*ui->w;
		editor_retrieve(ui->editor, ui->scrx, ui->scry, ui->h, ui->w, data);
		for (i = 0; i < ui->h; i++) {
			move(i, 0);
			clrtoeol();
			for (j = 0; j < ui->w; j++)
				if (data[i][j] != 0 && data[i][j] != '\n')
					addch(data[i][j] == '\t' ? ' ' : data[i][j]);
				else
					break;
		}
		free(data);
		free(buf);
	}
	move(ui->posx - ui->scrx, ui->posy - ui->scry);
}
void ui_keydown(ui_t *ui) {
	int newposx, newposy;
	editor_gooffset(ui->editor, ui->posx+1, ui->realposy, &newposx, &newposy);
	if (newposx == ui->posx) return;
	ui->posx = newposx, ui->posy = newposy;
	int refresh = 0;
	if (ui->posx >= ui->scrx + ui->h) {
		ui->scrx++;
		refresh = 1;
	}
	if (ui->posy < ui->scry) {
		ui->scry -= ((ui->scry - ui->posy - 1) / (ui->w/2) + 1) * (ui->w/2);
		refresh = 1;
	} else if (ui->posy >= ui->scry + ui->w) {
		ui->scry += ((ui->posy - ui->scry - ui->w) / (ui->w/2) + 1) * (ui->w/2);
		refresh = 1;
	}
	ui_refresh(ui, refresh);
}
void ui_keyup(ui_t *ui) {
	int newposx, newposy;
	editor_gooffset(ui->editor, ui->posx-1, ui->realposy, &newposx, &newposy);
	if (newposx == ui->posx) return;
	ui->posx = newposx, ui->posy = newposy;
	int refresh = 0;
	if (ui->posx < ui->scrx) {
		ui->scrx--;
		refresh = 1;
	}
	if (ui->posy < ui->scry) {
		ui->scry -= ((ui->scry - ui->posy - 1) / (ui->w/2) + 1) * (ui->w/2);
		refresh = 1;
	} else if (ui->posy >= ui->scry + ui->w) {
		ui->scry += ((ui->posy - ui->scry - ui->w) / (ui->w/2) + 1) * (ui->w/2);
		refresh = 1;
	}
	ui_refresh(ui, refresh);
}
void ui_keyleft(ui_t *ui) {
	int newposx, newposy;
	editor_gooffset(ui->editor, ui->posx, ui->posy-1, &newposx, &newposy);
	if (newposy == ui->posy) return;
	ui->realposy = ui->posy = newposy;
	int refresh = 0;
	if (ui->posy < ui->scry) {
		ui->scry -= ui->w/2;
		refresh = 1;
	}
	ui_refresh(ui, refresh);
}
void ui_keyright(ui_t *ui) {
	if (ui->realposy > ui->posy) {
		ui->realposy = ui->posy;
		return;
	}
	int newposx, newposy;
	editor_gooffset(ui->editor, ui->posx, ui->posy+1, &newposx, &newposy);
	if (newposy == ui->posy) return;
	ui->realposy = ui->posy = newposy;
	int refresh = 0;
	if (ui->posy >= ui->scry + ui->w) {
		ui->scry += ui->w/2;
		refresh = 1;
	}
	ui_refresh(ui, refresh);
}
