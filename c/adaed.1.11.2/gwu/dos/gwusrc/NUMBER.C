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

/* number.c */

#include "externs.h"


void AVL_FIX_NUMBER(AVL_LINE_PTR from)
{
	AVL_LINE_PTR temp;
	temp = from;
	while (temp -> line_no != 1)  {
		temp -> line_no -= 1;
		temp = temp -> next;
		}
}



void AVL_MAKE_NUMBER()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR l;
	int no = 1;
	w = &avl_windows[avl_window];
	for(l = w -> head -> next; l != w -> head; l = l -> next)
		l -> line_no = ++no;
}
