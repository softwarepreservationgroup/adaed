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

/* write.c */

#include "externs.h"



void AVL_WRITE_OUT()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR m1, m2;
	int n1, n2, x, from_here = 0, till_here = 0;
	char fname[80];
	static char *msg = "GWAda - Put to which file? ";
	char msg2[80];
	AVL_LINE_PTR temp, temp2 = NULL;
	int fp, i;
	int line_no = 0;
	long fsize;
	char *buf;
	char line[1024];
	unsigned int size, n, j;
	if (avl_block_first_line == NULL ||
	    avl_block_last_line == NULL) {
	    AVL_ERROR("Mark a block first.");
	    return;
	    }
	w = &avl_windows[avl_window];
	n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msg,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	sprintf(fname,"%s%cblock",avl_dir_sources,'\\');
	AVL_PROMPT(1,1,fname,60);
	sprintf(msg2,"Writing \'%s\'", fname);
	n1 = strlen(msg2);
	if (n1 < 17) n1 = 17;
	n1 += 5;
	n2 = (80 - n1) / 2;
	m2 = AVL_MAKE_WINDOW("",23,1,25,strlen(msg2)+3,avl_wnd_bk_color,avl_wnd_color);
	_setbkcolor(avl_msg_bk_color);
	_settextcolor(avl_msg_color);
    _settextposition(1,1);
    _outtext(msg2);
	unlink(fname);	   
	fp = open(fname,O_CREAT | O_RDWR,S_IREAD | S_IWRITE);
	if (fp == -1)  {
		sprintf(msg2,"Could not open \'%s\'\n", fname);
		AVL_ERROR(msg2);
		AVL_DEL_WINDOW(m2);
		AVL_DEL_WINDOW(m1);
		return;
		}
	size = 100000L;
	if ((buf = malloc(size)) == NULL)  { 
		AVL_ERROR("Out of memory!");
		AVL_DEL_WINDOW(m2);
		AVL_DEL_WINDOW(m1);
		return;
		}
	/* FIll in buffer */
	j = 0;
	temp = avl_block_first_line;
	from_here = avl_block_first_col;
	while ( 1 )  {
		if (j >= size)  {  /*  flush buffer to disk  */
			n = write(fp,buf,size);
			if (n != size) {
				sprintf(msg2,"Could not write \'%s\'\n", fname);
				AVL_ERROR(msg2);
				AVL_DEL_WINDOW(m2);
				AVL_DEL_WINDOW(m1);
				return;
				}
			fsize -= n;
			j = 0;
			}
		if (temp == avl_block_last_line)
			till_here = avl_block_last_col + 1;
		else
			till_here = strlen(temp -> line);
		for(i = 0 + from_here; i < till_here; ++i)
			buf[j++] = temp -> line[i];
		if (temp -> next != avl_block_first_line)
			buf[j++] = '\n';
		else
			break;
		temp = temp -> next;
		} 
	n = write(fp,buf,j);
	if (n != j) {
		sprintf(msg2,"Could not write \'%s\'\n", fname);
		AVL_ERROR(msg2);
		}
	else {
		close(fp);
		free(buf);
		}
	AVL_DEL_WINDOW(m2);
	AVL_DEL_WINDOW(m1);
}
