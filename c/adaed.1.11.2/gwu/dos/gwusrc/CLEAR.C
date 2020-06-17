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

/* clear.c */

#include "externs.h"



void AVL_CLEAR_LINE()
{
	AVL_EDIT_WINDOW_PTR w;
	int i, n;
	short att, bk, co;
	att = *AVL_MAP(25,1);
	att = att >> 8;
	bk  = att >> 4;
	co  = (att << 4) >> 4;
	w   = &avl_windows[avl_window];
	n   = w -> scr_row + w -> r1 - 1;
	for(i = w -> c1; i <= (w -> c2 - w -> c1 + 1); ++i)
/*  AVL_PUT(' ',n,i,bk,co);
*/
		AVL_WVIDEO(' ',(unsigned char) (avl_txt_bk_color << 4 | avl_txt_color)
			, AVL_MAP(n,i));
}
