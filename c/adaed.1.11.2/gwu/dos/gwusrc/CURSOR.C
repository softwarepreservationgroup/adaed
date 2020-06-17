/*
    GWAda Development Environment for 386/486 PCs   
    Copyright (C) 1993, Arthur Vargas Lopes  & Michael Bliss Feldman
                        vlopes@vortex.ufrgs.br mfeldman@seas.gwu.edu
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.    

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* cursor.c */

#include "externs.h"




void AVL_UPDATE_CURSOR()
{
	struct rccoord old;
	AVL_EDIT_WINDOW_PTR w;
	if (avl_hscroll_on)
		return;
	w = &avl_windows[avl_window];
	w -> scr_col = AVL_COL() - w -> offset;
	_settextposition(w -> scr_row, w -> scr_col);
}


void AVL_CURSOR_HOME()
{
	avl_windows[avl_window].txt_col = 0;
	if (avl_windows[avl_window].offset > 0)  {
		avl_windows[avl_window].offset = 0;
		avl_windows[avl_window].txt_col= 0;
		AVL_SCROLL();
		}
	else
		AVL_UPDATE_LINE();
}



void AVL_CURSOR_END()
{
	AVL_EDIT_WINDOW_PTR w;
	short n, i, oldoffset;
	char *d;
	w = &avl_windows[avl_window];
	oldoffset = w -> offset;
	w -> txt_col = strlen (w -> current_line -> line);
	if (w -> txt_col < 0)
		w -> txt_col = 0;
	w -> offset = AVL_OFFSET();
	if (w -> offset != oldoffset)
		AVL_SCROLL();
	else
		AVL_UPDATE_LINE();
	AVL_UPDATE_CURSOR();
}

void AVL_CURSOR_PGUP()
{
	int i;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	for (i = 1; i < (w -> r2 - w -> r1 + 2); ++i)
		AVL_CURSOR_UP(1);
	AVL_UPDATE_CURSOR();
}

void AVL_CURSOR_PGDN()
{
	int i;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	for (i = 1; i < (w -> r2 - w -> r1 + 2); ++i)
		AVL_CURSOR_DOWN(1);
	AVL_UPDATE_CURSOR();
}

void AVL_CURSOR_LEFT(int no)
{
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while ( no-- )  {
		if (w -> txt_col > 0)  {
			w -> txt_col -= 1;
			if (w -> scr_col == 1)  {
				w -> offset = AVL_OFFSET();
				AVL_SCROLL();
				}
			AVL_UPDATE_CURSOR();
			}
		}
}

void AVL_CURSOR_RIGHT(int no)
{
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while ( no-- )  {
		if ((w -> txt_col) < strlen(w -> current_line -> line))  {
			w -> txt_col += 1;
			if (w -> scr_col >= w -> c2)  {
				w -> offset = AVL_OFFSET();
				AVL_SCROLL();
				}
			AVL_UPDATE_CURSOR();
			}
		}
}



void AVL_CURSOR_UP(int no)
{
	AVL_EDIT_WINDOW_PTR w;
	short att, k;
	att = _settextcursor(0x2000);
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while (no --)  {
		if (w -> head != w -> current_line -> previous)  {
			w -> current_line = w -> current_line -> previous;
			if (w -> scr_row == 1) {
				_scrolltextwindow( -1 );
				AVL_UPDATE_LINE();
				}
			else {
				w -> scr_row -= 1;
				}
			}
		}
	att = _settextcursor(att);
	AVL_UPDATE_CURSOR();
}

void AVL_CURSOR_DOWN(int no)
{
	AVL_EDIT_WINDOW_PTR w;
	short att, k;
	att = _settextcursor(0x2000);
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while ( no-- )  {
		if (w -> head != w -> current_line -> next)  {
			w -> current_line = w -> current_line -> next;
			if (w -> scr_row == (w -> r2 - w -> r1 + 1)) {
				_scrolltextwindow( 1 );
				AVL_UPDATE_LINE();
				}
			else {
				w -> scr_row += 1;
				}
			}
		}
	att = _settextcursor(att);
	AVL_UPDATE_CURSOR();
}


