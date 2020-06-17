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

/* edauxi.c */
#include "externs.h"


void AVL_MD_TOOLS()
{
}



void AVL_SHIFT_LEFT(char *s, short pos)
{
	int i, j;
	if (pos < 1) return;
	j = strlen(s);
	for (i = pos - 1; i < j; ++i)
		*(s+i) = *(s + i + 1);
}


void AVL_FULL_FN(char *s)
{
	sprintf(s,"%s%c%s",avl_dir_sources,'\\',avl_windows[avl_window].file_name);
}


void AVL_MAKE_FN(char *d, char *s)
{
	int i, j;
	char fn[128];
	for(i = strlen(s) - 1; i >= 0; --i)
		if ((*(s + i) == ':') || (*(s + i) == '\\'))
			break;
	if (i < 0)
		strcpy(d,s);
	else 
		strcpy(d,s+i+1);
}
		


void AVL_FIX()
{
	unsigned int u, *u2;
	int *i;
	FILE *fp, *fopen();
	char line[100];
	AVL_LINE_PTR t;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	w -> current_line = w -> head -> next;
	w -> txt_pos = w -> line_no = 0;
	if (w -> avl_win1 == NULL)  {
		sprintf(line," Compiling %s ", w -> file_name);
		w -> avl_win1 = AVL_MAKE_WINDOW(line,14,4,16,64,7,15);
		_settextposition(3,1);
		_outtext("Compiling line: ");
		_settextposition(1,17);
		}
}

void AVL_GET_CDIR()
{
	avl_curr_directory[0] = '\0';
	_getdcwd(0,avl_curr_directory,128);
}

void AVL_DEFAULT_OPTIONS()
{	
	AVL_GET_CDIR();
	strcpy(avl_dir_sources,avl_curr_directory);
	sprintf(avl_dir_library,"%s",AVL_DEFAULT_LIB);
	avl_heap = 32;
	avl_monitor_on = '\0';
	avl_pgm_stack = 4;
	avl_task_stack = 2;
	avl_scheduller = 0;
	avl_trace_opts[0] = '\0';
	avl_trace_opts[1] = '\0';
	avl_trace_opts[2] = '\0';
	avl_trace_opts[3] = '\0';
	avl_trace_opts[4] = '\0';
	avl_trace_opts[5] = '\0';
	AVL_RESET_COLORS();
	avl_tab_size = 4;
}

void AVL_SAVE_OPTIONS()
{
	AVL_OPTIONS opt; 
	int fp, n;
	unlink(AVL_FN_OPTIONS);	   
	fp = open(AVL_FN_OPTIONS,O_CREAT | O_RDWR,S_IREAD | S_IWRITE);
	if (fp == -1)  {
		AVL_ERROR("Can't save options. Check disk space.");
		return;
		}
	strcpy(opt.sources,avl_dir_sources);
	strcpy(opt.library,avl_dir_library);
	opt.avl_monitor_on = avl_monitor_on;
	opt.tabsize = avl_tab_size;
	opt.avl_heap = avl_heap;
	opt.avl_pgm_stack = avl_pgm_stack;
	opt.avl_task_stack = avl_task_stack;
	opt.avl_scheduller = avl_scheduller;
	opt.avl_trace_opts[0] = avl_trace_opts[0];
	opt.avl_trace_opts[1] = avl_trace_opts[1];
	opt.avl_trace_opts[2] = avl_trace_opts[2];
	opt.avl_trace_opts[3] = avl_trace_opts[3];
	opt.avl_trace_opts[4] = avl_trace_opts[4];
	opt.avl_trace_opts[5] = avl_trace_opts[5];
	opt.avl_txt_color = avl_txt_color;
	opt.avl_txt_bk_color = avl_txt_bk_color;
	opt.avl_wnd_color = avl_wnd_color;
	opt.avl_wnd_title = avl_wnd_title;
	opt.avl_wnd_bk_color = avl_wnd_bk_color;
	opt.avl_que_color = avl_que_color;
	opt.avl_que_bk_color = avl_que_bk_color;
	opt.avl_pro_color = avl_pro_color;
	opt.avl_pro_bk_color = avl_pro_bk_color;
	opt.avl_men_ready = avl_men_ready;
	opt.avl_men_letter = avl_men_letter;
	opt.avl_men_word = avl_men_word;
	opt.avl_men_bk_color = avl_men_bk_color;
	opt.avl_err_color = avl_err_color;
	opt.avl_sta_color = avl_sta_color;
	opt.avl_sta_bk_color = avl_sta_bk_color;
	opt.avl_msg_color = avl_msg_color;
	opt.avl_msg_bk_color = avl_msg_bk_color;

	n = write(fp,&opt,sizeof(AVL_OPTIONS));
	if (n != sizeof(AVL_OPTIONS)) {
		AVL_ERROR("Can't save options. Check disk space.");
		close(fp);
		unlink(AVL_FN_OPTIONS);	   
		}
	else
		close(fp);
}

void AVL_READ_OPTIONS()
{
	AVL_OPTIONS opt; 
	int fp, n;
	fp = open(AVL_FN_OPTIONS,O_BINARY | O_RDWR, S_IREAD | S_IWRITE);
	if (fp == -1)  {
		AVL_DEFAULT_OPTIONS();
		return;
		}
	n = read(fp,&opt,sizeof(AVL_OPTIONS));
	if (n != sizeof(AVL_OPTIONS))  {
		close(fp);
		unlink(AVL_FN_OPTIONS);
		AVL_DEFAULT_OPTIONS();
		return;
		}
	avl_heap = opt.avl_heap;
	avl_monitor_on = opt.avl_monitor_on;
	avl_pgm_stack = opt.avl_pgm_stack;
	avl_task_stack = opt.avl_task_stack;
	avl_scheduller = opt.avl_scheduller;
	avl_trace_opts[0] = opt.avl_trace_opts[0];
	avl_trace_opts[1] = opt.avl_trace_opts[1];
	avl_trace_opts[2] = opt.avl_trace_opts[2];
	avl_trace_opts[3] = opt.avl_trace_opts[3];
	avl_trace_opts[4] = opt.avl_trace_opts[4];
	avl_trace_opts[5] = opt.avl_trace_opts[5];
	avl_txt_color = opt.avl_txt_color;
	avl_txt_bk_color = opt.avl_txt_bk_color;
	avl_wnd_color = opt.avl_wnd_color;
	avl_wnd_title = opt.avl_wnd_title;
	avl_wnd_bk_color = opt.avl_wnd_bk_color;
	avl_que_color = opt.avl_que_color;
	avl_que_bk_color = opt.avl_que_bk_color;
	avl_pro_color = opt.avl_pro_color;
	avl_pro_bk_color = opt.avl_pro_bk_color;
	avl_men_ready= opt.avl_men_ready;
	avl_men_letter = opt.avl_men_letter;
	avl_men_word = opt.avl_men_word;
	avl_men_bk_color = opt.avl_men_bk_color;
	avl_err_color = opt.avl_err_color;
	avl_sta_color = opt.avl_sta_color;
	avl_sta_bk_color = opt.avl_sta_bk_color;
	avl_msg_color = opt.avl_msg_color;
	avl_msg_bk_color = opt.avl_msg_bk_color;
	strcpy(avl_dir_sources,opt.sources);
	strcpy(avl_dir_library,opt.library);
	avl_tab_size = opt.tabsize;
	close(fp);
}



AVL_LINE_PTR AVL_MAKE_LINE(char *l, int lno)
{
	AVL_LINE_PTR temp;
	if (strlen(l) >= AVL_MAX_LINEL)  {
		AVL_ERROR("Line too long. Truncated to 256 characters.");
		*(l+256) = '\0';
		}
	temp = (AVL_LINE_PTR) calloc(1,sizeof(AVL_LINE_SIZE));
	if (temp == NULL) {
		AVL_ERROR("Out of memory...");
		exit(1);
		}
	strcpy(temp -> line,l);
	temp -> line_no = lno;
	temp -> line_attrib = '\0';
	temp -> previous =
	temp -> next     = NULL;
	return temp;
}

void AVL_RENUMBER(AVL_LINE_PTR from, int c, int n)
{
	int count;
	AVL_LINE_PTR temp;
	count = c; 
	temp = from;
	while (temp -> line_no != 1)  {
		temp -> line_no += count;
		count += n;
		temp = temp -> next;
		}
}

void AVL_LINE_INSERT(AVL_LINE_PTR this,AVL_LINE_PTR  *at)
{
	if (*at == NULL)  {  /*  First line  */
		*at = this;
		this -> previous =
		this -> next     = this;
		this -> line_no  = 1;
		}	
	else {
		this -> previous = *at;
		this -> next     = (*at) -> next;
		((*at) -> next) -> previous = this;
		(*at) -> next = this;
		AVL_RENUMBER(this, (*at) -> line_no + 1, 1);
		}
}
