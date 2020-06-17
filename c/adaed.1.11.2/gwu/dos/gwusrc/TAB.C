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

/*  Tab.c  */

#include <externs.h>



void AVL_SET_TAB()
{
	char f[AVL_MAX_LINEL+10];
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR m1;
	short co;
	int n1, n2;
	static char *msg = " GWAda - Set Identation Length ";
	static char *msg2 = "Current identation length: ";
	w = &avl_windows[avl_window];
	n1 = strlen(msg) + 10;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msg,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 ) {
		_settextposition(1,1);
		sprintf(f,"%d", avl_tab_size);
		_outtext(msg2);
		if (AVL_PROMPT(1,strlen(msg2)+1,f,2)) {
			AVL_DEL_WINDOW(m1);
			return;
			}
		sscanf(f,"%d",&n2);
		if (n2 >= 1 && n2 <= 8) {
			w -> tabsize = avl_tab_size = n2;
			AVL_DEL_WINDOW(m1);
			return;
			}
		else 
			AVL_ERROR("Invalid identation length. Must be in the 1..8 range!");
		}
}
