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

/* delete.c */

#include "externs.h"


void AVL_EDIT_DEL_LEFT(int no)
{
	int i, n;
	char *t;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while (no-- )  {
		n = strlen(t = w -> current_line -> line);
		if (w -> txt_col > 0)  {
			for (i = w -> txt_col - 1; i < n; ++i) 
				*(t + i) = *(t + i + 1);
			*(t + n - 1) = '\0';
			AVL_CLEAR_LINE();
			if (w -> scr_col == 1)  {
				w -> offset -= 20;
				AVL_SCROLL();
				}
			w -> txt_col -= 1;
			w -> buffer_size -= 1;
			w -> changed = 1;
			AVL_UPDATE_LINE();
			}
		else {  /* Join with the previous line */
			w -> changed = 1;
			w -> offset = 0;
			AVL_JOIN_LEFT();
			}
		}
}

void AVL_EDIT_DEL_RIGHT(int no)
{
	int i, n;
	char *t;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while ( no-- )  {
		n = strlen(t = w -> current_line -> line);
		if ((w -> txt_col) < n)  {
			for (i = w -> txt_col; i < n; ++i)
				*(t + i) = *(t + i + 1);
			*(t + n - 1) = '\0';
			AVL_CLEAR_LINE();
			w -> buffer_size -= 1;
			AVL_UPDATE_LINE();
			w -> changed = 1;
			}
		else   /* Join with the next line */
			AVL_JOIN_RIGHT();
		}
}

void AVL_DELETE_LINE()
{
   short left, top, right, bottom;
   struct rccoord rc;
   _gettextwindow( &top, &left, &bottom, &right );
   rc = _gettextposition();
   _settextwindow( rc.row, left, bottom, right );
   _scrolltextwindow( _GSCROLLUP );
   _settextwindow( top, left, bottom, right );
   _settextposition( rc.row, rc.col );
}



void AVL_DO_DEL_LINE()
{
	AVL_EDIT_DEL_LINE(1);
}

	
void AVL_REMOVE_CUR_LINE(AVL_LINE_PTR t1)
{
	t1 -> previous -> next = t1 -> next;
	t1 -> next -> previous = t1 -> previous;
	free(t1);
}

void AVL_EDIT_DEL_LINE(int no)
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR t1, temp, temp2;
	struct rccoord old;
	short pos, pos2;
	w = &avl_windows[avl_window];
	if (no <= 0) no = 1;
	while(no--) {
		if (w -> current_line == w -> head)
			return;
		w -> changed = 1;
		t1 = w -> current_line;
		/* Test if last text's line */
/*printf("\nlno = %d current_line = %x next = %x previous = %x head = %x\n",
  w -> current_line -> line_no,	t1, t1 -> next, t1 -> previous, w -> head);
*/
		if (t1 -> next == w -> head) {
			AVL_CLEAR_LINE(); 
			if (t1 -> previous != w -> head)
				AVL_CURSOR_UP(1);
			else {
				AVL_REMOVE_CUR_LINE(t1);
				w -> current_line = AVL_MAKE_LINE("",2);
	        	AVL_LINK(w -> current_line,w -> head);
				return;
				}
			AVL_REMOVE_CUR_LINE(t1);
			continue;
			}
		old = _gettextposition();
		if (w -> scr_row == 1) {
			old = _settextposition(w -> scr_row - 1,1);
			AVL_DELETE_LINE();
			_settextposition(old.row,old.col);
			}
		else if (w -> scr_row < (w -> r2 - w -> r1 + 1)) {
			old = _settextposition(w -> scr_row + 1,1);
			AVL_DELETE_LINE();
			_settextposition(old.row,old.col);
			}
		w -> current_line = t1 -> next;
		AVL_REMOVE_CUR_LINE(t1);
		if (w -> txt_col > strlen(w -> current_line -> line))
			AVL_CURSOR_END();
		pos2 = w -> txt_col;
		pos = w -> scr_row;
		temp = temp2 = w -> current_line;
		/* Go to last screen's line */
		while ((pos < (w -> r2 - w -> r1 + 1)) && (temp -> next != w -> head))	{
			temp = temp -> next;
			++pos;
			}
		/* Is there text to be displayed in the last screen's line */
		if (pos == (w -> r2 - w -> r1 + 1) && temp != w -> head)  {
			w -> current_line = temp;
			w -> scr_row = w -> r2 - w -> r1 + 1;
			_settextposition( w -> scr_row, 1);
			AVL_UPDATE_LINE();
			}
		w -> current_line = temp2;
		old = _settextposition(w -> scr_row = old.row,w -> scr_col = old.col);
		w -> txt_col = pos2;
		}
	if (w -> current_line != w -> head)
		AVL_MAKE_NUMBER();
}
				
