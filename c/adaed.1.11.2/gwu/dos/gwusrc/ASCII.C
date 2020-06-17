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

/* ASCII.c */

#include <externs.h>


void AVL_SHOW_ASCII()
{
	AVL_WIN_PTR hw;
	int i, j = 2, k = 2, s = 0;
	char l[10];
	short co;

	hw = AVL_MAKE_WINDOW(" ASCII Table ",3,1,24,80,avl_wnd_bk_color,avl_wnd_color);
	co = _settextcolor(avl_men_letter);
	_outtext(" Ch Dec Hex ");
	_outtext("Ch Dec Hex ");
	_outtext("Ch Dec Hex ");
	_outtext("Ch Dec Hex ");
	_outtext("Ch Dec Hex ");
	_outtext("Ch Dec Hex ");
	_outtext("Ch Dec Hex ");
	co = _settextcolor(co);
	for(i = 0; i < 128; ++i)	{
		if (j > 20)  {
			s += 11;
			j = 2;
			}
		_settextposition(j,k+s);
		sprintf(l,"%c  ", i); 
		_outtext(l);
		_settextposition(j,k + 3 + s);
		sprintf(l,"%3d ", i); 
		_outtext(l);
		sprintf(l,"%3x ", i);
		_outtext(l);
		++j;
		}
	i = getch(); if (i == 0) i = getch();
	AVL_DEL_WINDOW(hw);
}