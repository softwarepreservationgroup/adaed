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

/* load.c */

#include <\gwada\env\externs.h>




void AVL_PROCESS_LOAD() 
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR m1;
	int n1, n2;
	static char *msg = " GWAda - Enter file name to load, please: ";
	char f[61];
	w = &avl_windows[avl_window];
	n1 = strlen(msg) + 10;
	if (n1 < 62) n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msg,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	_settextposition(1,1);
	f[0] = '\0';
	AVL_PROMPT(1,1,f,60);
	AVL_DEL_WINDOW(m1);
	if (strlen(f) > 0) {
		AVL_FREE_ALL();
		AVL_LOAD_FILE(f);
		}	
}



void AVL_LOAD_FILE(char *fn)
{
        AVL_LINE_PTR head = NULL, temp;
        int x ;
        AVL_EDIT_WINDOW_PTR w;
        w = &avl_windows[avl_window];
        temp = AVL_MAKE_LINE("### DUMMY ###",0);
        AVL_LINE_INSERT(temp,&head);
        x = AVL_LOAD(fn, &head);
        if (head == head -> next)  {
        	temp = AVL_MAKE_LINE("",2);
        	AVL_LINK(temp,head);
        	}
        strcpy(avl_windows[avl_window].file_name,fn);
        AVL_INIT_WINDOW(&avl_windows[avl_window], head -> next);
        AVL_CURSOR_HOME();
        AVL_UPDATE_SCREEN();
}


int AVL_LOAD(char *filen, AVL_LINE_PTR *at)
{
	AVL_WIN_PTR win;
	AVL_LINE_PTR temp, temp2 = NULL;
	int fp, i;
	int line_no = 0;
	long fsize;
	char *buf = NULL;
	char garbage = ' ';
	char line[1024];
	unsigned int size, n, j;
	fp = open(filen,O_BINARY | O_RDWR, S_IREAD | S_IWRITE);
	if (fp == -1) return 0;
	sprintf(line,"Reading %s ...", filen);
	win = AVL_MAKE_WINDOW("",23,1,25,strlen(line)+3,avl_wnd_bk_color,avl_wnd_color);
	_setbkcolor(avl_msg_bk_color);
	_settextcolor(avl_msg_color);
	_settextposition(1,1);
	_outtext(line);
	lseek(fp,0L,2);
	fsize = tell(fp);
	if (fsize > 141000L)  {
		AVL_ERROR("Source file too long (> 141,000 characters)!");
		AVL_DEL_WINDOW(win);
		close(fp);
		return 0;
		}
	avl_windows[avl_window].buffer_size = fsize;
	lseek(fp,0L,0);
	size = fsize;
	if (size == 0)
		size = 10;
	if ((buf = malloc(size)) == NULL)    {
		AVL_ERROR("Out of memory to store file.");
		AVL_DEL_WINDOW(win);
		close(fp);
		return -1;
		}
	if (fsize == 0L)
		strcpy(buf,"   ");
	while (fsize > 0L)  {
		n = read(fp,buf,size);
		if (n != size) {
			AVL_ERROR("Can't read file.");
			AVL_DEL_WINDOW(win);
			close(fp);
			return(0);
			}
		j = 0;
		while (j < n)  {
			for(i = 0; i < avl_start_pos; ++i)
				line[i] = ' ';
			for (; !((buf[j] == 13) || (buf[j] == 10) || (j >= n)); ++i, ++j)
				line[i] = buf[j];
			if ((buf[j] == 13) && (buf[j + 1] == 10)) ++j;
			line[i] = '\0';
			++j;
			temp = AVL_MAKE_LINE(line,line_no);
			AVL_LINE_INSERT(temp, (temp2 == NULL) ? at : &temp2);
			temp2 = temp;
			}
		fsize -= n;
		}
	if (buf[n-1] == '\n' || buf[n-1] == 10)  {
		temp = AVL_MAKE_LINE("",line_no);
		AVL_LINE_INSERT(temp, (temp2 == NULL) ? at : &temp2);
		}
	close(fp);
	if (buf != NULL)
		free(buf);
	AVL_DEL_WINDOW(win);
	return 1;
}


