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

/* tell.c */

#include "externs.h"


void AVL_TELL(char *s)
{
	AVL_WIN_PTR win;
	short cor, n1, n2;
	n1 = strlen(s) + 10;
	if (n1 > 76) n1 = 76;
	*(s+n1) = '\0';
	n2 = (80 - n1) / 2;
	if (n2 <= 0) n2 = 1;
	if ((n1 + n2) >= 80) n2 = 1;
	win = AVL_MAKE_WINDOW(" Action Report ",7,n2,9,n1+n2+2,avl_wnd_bk_color,avl_wnd_color);
	cor = _settextcolor(avl_err_color);
	_outtext(" ");
	_outtext(s);
	AVL_PAUSE(20);
	AVL_DEL_WINDOW(win);
}
