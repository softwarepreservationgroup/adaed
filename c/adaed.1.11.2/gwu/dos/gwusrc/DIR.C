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

/*  Dir.c  */

#include <externs.h>



void AVL_SET_DIRECTORIES()
{
	char f[2][AVL_MAX_LINEL+10];
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR m;
	int n1, n2, ch, i;
	static char *msg = " GWAda - Directories ";
	static char *msgs[4] = {"Ada sources: "
	                       ,"Library    : "
	                       ,"Ok" 
	                       ,"Cancel"};
	int current = 1;
	w = &avl_windows[avl_window];
	n1 = 66;
	n2 = (80 - n1) / 2;
	strcpy(f[0],avl_dir_sources);
	strcpy(f[1],avl_dir_library);
	m = AVL_MAKE_WINDOW(msg,7,n2,12,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 )  {
		for(i = 1; i < 5; ++i)  {
			_settextposition(i,1);
			_settextcolor(avl_men_letter);
			_outmem(msgs[i-1],1);
			_settextcolor(avl_men_word);
			_outtext(msgs[i-1] + 1);
			if (i < 3)
				_outtext(f[i-1]);
			}
		ch = getch();
		switch( toupper(ch) )  {
			case 'A' :
				current = 1;
				AVL_PROMPT(current,strlen(msgs[current-1]) + 1,f[current-1],40);
				break;
			case 'L' :
				current = 2;
				AVL_PROMPT(current,strlen(msgs[current-1]) + 1,f[current-1],40);
				break;
			case 'O' :
					strcpy(avl_dir_sources,f[0]);
					strcpy(avl_dir_library,f[1]);
					AVL_DEL_WINDOW(m);
					return;
			case 27  :
			case 'C' :
					AVL_DEL_WINDOW(m);
					return;
			case 0   : 
			case 0XE0: {
				ch = getch();
				switch(ch)  {
					case 72 : /* Up  */ 
						if (--current < 1) current = 2;
						break;
					case 80 : /* Down */ 
						if (++current > 2) current = 1;
						break;
					default : putch(7); break;
					}
				break;
				}
			default : putch(7); break;
			}
		}
}
