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

/* save.c */

#include "externs.h"




void AVL_SAVE_AS()
{
	AVL_WIN_PTR win;
	AVL_WIN_PTR m1, m2;
	AVL_LINE_PTR temp, temp2 = NULL;
	AVL_EDIT_WINDOW_PTR w;
	int fp, i;
	int line_no = 0;
	long fsize;
	char *buf;
	char line[1024], l2[80];
	unsigned int size, n, j;
	int n1, n2, x, from_here = 0, till_here = 0, rp;
	char fname[121];
	static char *msg = " GWAda - Save As... ";
	char msg2[80];
	w = &avl_windows[avl_window];
	n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msg,7,n2,9,n1+n2+2,avl_wnd_bk_color,avl_wnd_color);
	fname[0] = '\0';
	rp = AVL_PROMPT(1,1,fname,60);
	AVL_DEL_WINDOW(m1);
	if (rp)
		return;
	w = &avl_windows[avl_window];
	w -> changed = 0;
	sprintf(line,"Writing %s ...", fname);
	win = AVL_MAKE_WINDOW(" Save As ...",23,1,25,strlen(line)+3,avl_wnd_bk_color,avl_wnd_color);
	_setbkcolor(avl_msg_bk_color);
	_settextcolor(avl_msg_color);
	_settextposition(1,1);
	_outtext(line);
	sprintf(l2,"%s", fname);
	for(i = 0; i < strlen(l2) && l2[i] != '.'; ++i)  
		line[i] = fname[i];
	line[i] = '.';
	line[i + 1] = 'b';
	line[i + 2] = 'a';
	line[i + 3] = 'k';
	line[i + 4] = '\0';
	unlink(line);
	rename(l2, line);
	fp = open(l2,O_CREAT | O_RDWR,S_IREAD | S_IWRITE);
	if (fp == -1)  {
		sprintf(line,"Could not open \'%s\'\n", l2);
		AVL_ERROR(line);
		AVL_DEL_WINDOW(win);
		return;
		}
	size = 100000L;
	if ((buf = malloc(size)) == NULL)   {
		AVL_ERROR("Out of memory. Can't write file to disk!");
		AVL_DEL_WINDOW(win);
		return;
		}
	/* FIll in buffer */
	j = 0;
	temp = w -> head -> next;
	while (temp != w -> head)  {
		if (j >= size)  {  /*  flush buffer to disk  */
			n = write(fp,buf,size);
			if (n != size) {
				AVL_ERROR("Disk full. Make room and then try again!");
				close(fp);
				unlink(fname);
				rename(line, fname);
				AVL_DEL_WINDOW(win);
				return;
				}
			fsize -= n;
			j = 0;
			}
		for(i = 0; i < strlen(temp -> line); ++i)
			buf[j++] = temp -> line[i];
		if (temp -> next != w -> head)
			buf[j++] = '\n';
		temp = temp -> next;
		} 
	n = write(fp,buf,j);
	if (n != j) {
		AVL_ERROR("Disk full. Make room and then try again!");
		close(fp);
		unlink( fname );
		rename(line, fname );
		AVL_DEL_WINDOW(win);
		return;
		}
	close(fp);
	free(buf);
	AVL_DEL_WINDOW(win);
}

void AVL_SAVE()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR win;
	AVL_LINE_PTR temp, temp2 = NULL;
	int fp, i;
	int line_no = 0;
	long fsize;
	char *buf;
	char line[1024], l2[80];
	unsigned int size, n, j;
	w = &avl_windows[avl_window];
	w -> changed = 0;
	sprintf(line,"Writing %s ...", w -> file_name);
	win = AVL_MAKE_WINDOW("",23,1,25,strlen(line)+3,avl_wnd_bk_color,avl_wnd_color);
	_setbkcolor(avl_msg_bk_color);
	_settextcolor(avl_msg_color);
	_settextposition(1,1);
	_outtext(line);
	sprintf(l2,"%s",w -> file_name);
	for(i = 0; i < strlen(l2) && l2[i] != '.'; ++i)  
		line[i] = w -> file_name[i];
	line[i] = '.';
	line[i + 1] = 'b';
	line[i + 2] = 'a';
	line[i + 3] = 'k';
	line[i + 4] = '\0';
	unlink(line);
	rename(l2, line);
	fp = open(l2,O_CREAT | O_RDWR,S_IREAD | S_IWRITE);
	if (fp == -1)  {
		sprintf(line,"Could not open \'%s\'\n", l2);
		AVL_ERROR(line);
		AVL_DEL_WINDOW(win);
		return;
		}
	size = 100000L;
	if ((buf = malloc(size)) == NULL)   {
		AVL_ERROR("Out of memory. Can't write file to disk!");
		AVL_DEL_WINDOW(win);
		return;
		}
	/* FIll in buffer */
	j = 0;
	temp = w -> head -> next;
	while (temp != w -> head)  {
		if (j >= size)  {  /*  flush buffer to disk  */
			n = write(fp,buf,size);
			if (n != size) {
				AVL_ERROR("Disk full. Make room and then try again!");
				close(fp);
				unlink(w -> file_name);
				rename(line, w -> file_name);
				AVL_DEL_WINDOW(win);
				return;
				}
			fsize -= n;
			j = 0;
			}
		for(i = 0; i < strlen(temp -> line); ++i)
			buf[j++] = temp -> line[i];
		if (temp -> next != w -> head)
			buf[j++] = '\n';
		temp = temp -> next;
		} 
	n = write(fp,buf,j);
	if (n != j) {
		AVL_ERROR("Disk full. Make room and then try again!");
		close(fp);
		unlink(w -> file_name);
		rename(line, w -> file_name);
		AVL_DEL_WINDOW(win);
		return;
		}
	close(fp);
	free(buf);
	AVL_DEL_WINDOW(win);
}



