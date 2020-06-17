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

/*  Dos.c  */

#include <externs.h>



void AVL_DOS_CMD() 
{ 
	AVL_WIN_PTR w;
	short old_c;
	w = AVL_MAKE_WINDOW("",1,1,25,80,avl_wnd_bk_color,avl_wnd_color);
	_settextrows( 25 );
	_clearscreen( _GCLEARSCREEN );
	old_c = _settextcursor(0x0707);
	printf("\nType EXIT to return to GWAda!\n");
	system("command");
	_settextcursor(old_c);
	AVL_DEL_WINDOW(w);
}
