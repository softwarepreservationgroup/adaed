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

/* readin.c */

#include "externs.h"


void AVL_READ_IN()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, head = NULL;
	AVL_WIN_PTR m1;
	short n1, n2, x, i, n, rp;
	AVL_SOURCE_SIZE t;
	char s[121];
	int first = 0;
	static char fname[121];
	static char lastfn[121];
	static char *msg = "GWAda - Get from which file? ";
	char msg2[80];
	w = &avl_windows[avl_window];
	n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msg,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	if (first == 0)
		sprintf(lastfn,"%s%c*.ada",avl_dir_sources,92);
	strcpy(fname,lastfn);
	first = 1;
	rp = AVL_PROMPT(1,1,fname,60);
	if (fname[0] == '\0' || rp)   {
			AVL_DEL_WINDOW(m1);
			return;
			}
	strcpy(lastfn,fname);
	temp = AVL_MAKE_LINE("### DUMMY ###",0);
    AVL_LINE_INSERT(temp,&head);
	AVL_GET_FILE_NAMES(&t,fname);
	s[0] = '\0';
	if (t.no_files == 0)  {
		sprintf(s,"Can't find any file under \'%s\'",fname);
		AVL_ERROR(s);
		}
	else  {		
		n = AVL_SOURCES(&t,t.no_files,fname);
		for(i = strlen(fname) - 1; t.no_files > 1 && i > 0; --i)  {
			if (fname[i] == '\\' && i > 0) {
				fname[i+1] = '\0';
				i  = -10;
				break;
				}
			}
		if (i < -1)
			sprintf(s,"%s%s",fname,t.name[n]);
		else
			sprintf(s, "%s",t.name[n]);
		}
	if (s[0] != '\0')  {
	    x = AVL_LOAD(s, &head);
		if (!x) {
			sprintf(msg2,"Can't open \'%s\'", s);
			AVL_ERROR(msg2);
			AVL_DEL_WINDOW(m1);
			}
		else {
			AVL_DEL_WINDOW(m1);
			AVL_CLEAR_BLOCK();
			avl_block_first_line = head -> next;
			avl_block_last_line = head -> previous;
			avl_block_first_col = 0;
			avl_block_last_col = strlen(head -> previous -> line) - 1;
			if (avl_block_last_col < 0) avl_block_last_col = 0;
			AVL_EDIT_COPY(1);
			AVL_CLEAR_BLOCK();
			}
		}
	else
		AVL_DEL_WINDOW(m1);
}
