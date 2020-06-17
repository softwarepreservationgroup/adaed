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

/*  question.c  */

#include <externs.h>


int AVL_QUESTION(char *s)
{
	AVL_WIN_PTR m;
	int n, j, ch;
	short att; 
	att = _settextcursor(0x0007);
	n = (80 - (j = strlen(s))) / 2;
	m = AVL_MAKE_WINDOW("",20,n,22,n+j+2,avl_que_bk_color,avl_que_color);
	_settextposition(1,1);
	_outtext(s);
	_settextposition(1,j+1);
	ch = toupper(getch());
	if (ch == 0) ch = getch();
	while (!(ch == 'Y' || ch == 'N')) {
		AVL_ERROR("You must enter either Y or N");
		_settextposition(1,j+1);
		ch = toupper(getch());
		if (ch == 0) ch = getch();
		}
	AVL_DEL_WINDOW(m);
	_settextcursor(att);
	return ch;
}
