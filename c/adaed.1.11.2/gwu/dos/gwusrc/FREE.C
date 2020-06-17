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

/* free.c */

#include "externs.h"



void AVL_FREE_ALL()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR t, t2;
	w = &avl_windows[avl_window];
	if (w -> changed && avl_window == 0) 
		if (AVL_QUESTION("Save current text before exiting (Y/N) ? ") == 'Y')
			AVL_SAVE();
	for (t = w -> head -> next; t != w -> head; t = t2) {
		t2 = t -> next;
		free(t);
		}
	free(w -> head);
	w -> head = w -> current_line = NULL;
}
