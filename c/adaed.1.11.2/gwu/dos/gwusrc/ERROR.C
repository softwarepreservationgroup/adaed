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

/* error.c */

#include "externs.h"


void AVL_ERROR(char *s)
{
	AVL_WIN_PTR win;
	short cor;
	int ch, n1, n2;
	struct rccoord old;
	char msgs[3][77];
	int i, j = 0, k = 0;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	if (w -> line_no != 0)  {
		for(i = 0; i < strlen(s); ++i) {
			if (*(s+i) == '\n' ||  j > 75) {
				msgs[k][j++] = '\0';
				j = 0;
				k++;
				}
			msgs[k][j++] = *(s+i);
			}
		msgs[k][j] = '\0';
		}
	else 
		strcpy(msgs[k],s);
	n1 = strlen(s) + 10;
	if (n1 > 77) n1 = 80;
	n2 = (80 - n1) / 2;
	if (n2 <= 0) n2 = 1;
	if ((n1 + n2) >= 80) n2 = 0;
	win = AVL_MAKE_WINDOW(" Error Report ",7,n2,7+k+2,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	cor = _settextcolor(avl_err_color);
	for(i = 0; i <= k; ++i)  {
		_settextposition(2+i,2);
		_outtext(msgs[i]);
		}
	AVL_PAUSE(20);
	AVL_DEL_WINDOW(win);
}

void AVL_PAUSE(short l)
{
	static char *cont = "Press any key to continue ...";
	int ch, n1, n2;
	AVL_WIN_PTR w;
	n1 = strlen(cont);
	n1 += 2;
	n2 = (80 - n1) - 5;
	w = AVL_MAKE_WINDOW("",l,n2,l+2,n1+n2+1,avl_wnd_bk_color,avl_wnd_color);
	_setbkcolor(avl_msg_bk_color);
	_settextcolor(avl_msg_color);
	_settextposition(1,2);
	_outtext(cont);
	ch = getch();
	if (ch == 0) ch = getch();
	AVL_DEL_WINDOW(w);
}
