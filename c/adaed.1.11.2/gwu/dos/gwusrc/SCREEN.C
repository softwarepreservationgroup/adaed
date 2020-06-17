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

/* screen.c */

#include "externs.h"



void AVL_FIX_SCREEN()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR l, l1;
	struct rccoord old;
	int i, j, r, c, c2;
	short att;
	old = _gettextposition();
	_settextrows( 25 );
	_clearscreen( _GCLEARSCREEN );
	w = &avl_windows[avl_window];
	l = l1 = w -> current_line;
	j = old.row;
	r = w -> scr_row;
	c = w -> txt_col;
	c2 = w -> scr_col;
	while (j > 1 && l -> line_no > 0)  {
		l = l -> previous;
		--j;
		}
	w -> txt_col = 0;
	for (i = 1; i <= (w -> r2 - w -> r1 + 1) && l != w -> head; ++i)  {
		_settextposition(i,1);
        w -> scr_row = i;
        w -> current_line = l;
        AVL_UPDATE_LINE();
        l = l -> next;
        }
	w -> scr_row = r;
	w -> txt_col = c;
	w -> scr_col = c2;
	_settextposition(old.row,old.col);
    w -> current_line = l1;
}		
				


void AVL_UPDATE_SCREEN()
{
	short att;
	att = _settextcursor(0x2000);
/*   _settextwindow(w -> r1, w -> c1, w -> r2, w -> c2);
*/
	AVL_FIX_SCREEN();
	AVL_UPDATE_CURSOR();
	att = _settextcursor(att);
}

