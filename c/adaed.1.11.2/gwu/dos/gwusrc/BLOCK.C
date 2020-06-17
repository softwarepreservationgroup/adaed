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

/*  block.c  */

#include <externs.h>


void AVL_PROCESS_COPY()
{
	AVL_EDIT_COPY(1);
}


void AVL_REMOVE_BLOCK()
{
	;
}


void AVL_EDIT_COPY(int no)
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, temp2, temp3;
	struct rccoord old;
	char new_line[(AVL_MAX_LINEL*2+10)];
	char save[AVL_MAX_LINEL + 2];
	int i, j, k;
	if ((avl_block_first_line == NULL) ||
	    (avl_block_last_line == NULL)) {
		AVL_ERROR("Block was not marked");
		return;
		}
	avl_block_first_line2 =
	avl_block_last_line2 = NULL;
	w = &avl_windows[avl_window];
	temp = w -> current_line;
	if (w -> txt_col > (k = strlen(temp -> line)))
		AVL_CURSOR_END();
	for(i = 0; i < w -> txt_col; ++i)  /* Make first line of block */
		new_line[i] = temp -> line[i];
	for(j = avl_block_first_col; avl_block_first_line -> line[j] != '\0'; ++j) {
		new_line[i++] = avl_block_first_line -> line[j];
		if (avl_block_first_line == avl_block_last_line)
			if (j == avl_block_last_col)  
				break;
		}
	/* First line will now be completed */
	if (w -> txt_col != k)
		strcpy(save,&temp -> line[w -> txt_col]);
	else
		strcpy(save,"");
	w -> changed = 1;
	if (avl_block_first_line == avl_block_last_line)  { /* Done */
		for(j = w -> txt_col; temp -> line[j] != '\0'; ++j)
			new_line[i++] = temp -> line[j];
		new_line[i++] = '\0';
		if (i >= AVL_MAX_LINEL)  
		     AVL_ERROR("Block's first line too long. Will truncate!");
		new_line[AVL_MAX_LINEL] = '\0';
		strcpy(temp -> line,new_line);
		AVL_UPDATE_LINE();
		return;
		}
	new_line[i++] = '\0';
	if (i >= AVL_MAX_LINEL)  
	     AVL_ERROR("Block's first line too long. Will truncate!");
	new_line[AVL_MAX_LINEL] = '\0';
	strcpy(temp -> line,new_line);
	AVL_UPDATE_LINE();
	/* There is more than one line */		
	temp2 = temp;
	for(temp = avl_block_first_line -> next; 
	    temp -> previous != avl_block_last_line; 
	    temp = temp -> next)  {
		temp3 = AVL_MAKE_LINE(temp -> line, 1);
		AVL_LINK(temp3,temp2);
		temp2 = temp3;
		}
	for(i = 0; i < avl_block_last_col; ++i)
		new_line[i] = temp3 -> line[i];
	new_line[i++] = '\0';
	strcat(new_line,save);
	if (strlen(new_line) >= AVL_MAX_LINEL)  {
		AVL_ERROR("Last block's line too long. Will truncate!");
		new_line[AVL_MAX_LINEL] = '\0';
		}
	strcpy(temp3 -> line, new_line);
	i = w -> scr_row;
	j = w -> scr_col;
	w -> scr_col = 1;
	AVL_MAKE_NUMBER();
	temp3 = w -> current_line;
	for(temp = w -> current_line -> next; ++(w -> scr_row) <= 24 && temp != w -> head;
	    temp = temp -> next) {
		_settextposition(w -> scr_row,1);
		w -> current_line = temp;
		AVL_UPDATE_LINE();
		}
	w -> current_line = temp3;
	w -> scr_row = i;
	w -> scr_col = j;
	_settextposition(i,j);
}


void AVL_CLEAR_BLOCK()
{
	AVL_LINE_PTR t1, t2;
	avl_message[0] = '\0';
	if (avl_block_first_line == NULL ||
	    avl_block_last_line == NULL) return;
	for(t1 = avl_block_first_line; t1 != avl_block_last_line; t1 = t2) {
		t2 = t1 -> next;
		free(t1);
		}
	free(t1);
	avl_block_first_line2 =
	avl_block_last_line2  = NULL;
	avl_block_first_line = NULL;
	avl_block_last_line = NULL;
}

void AVL_LINK(AVL_LINE_PTR this,AVL_LINE_PTR  at)
{
	this -> previous = at;
	this -> next     = at -> next;
	at -> next -> previous = this;
	at -> next = this;
}

void AVL_MAKE_BLOCK()
{
	AVL_LINE_PTR t1, t2, t3, temp;
	int i, j;
	if (avl_block_first_line -> line_no > avl_block_last_line -> line_no) { /* Swap */
		temp = avl_block_first_line;
		avl_block_first_line = avl_block_first_line2 = avl_block_last_line;
		avl_block_last_line  = avl_block_last_line2  = temp;
		i = avl_block_first_col;
		avl_block_first_col = avl_block_first_col2 = avl_block_last_col;
		avl_block_last_col  = i;
		avl_block_last_col2 = i;
		}
	if (avl_block_first_line == avl_block_last_line)
		if (avl_block_last_col < avl_block_first_col)  { /* Swap only cols */
			i = avl_block_first_col;
			avl_block_first_col = avl_block_first_col2 = avl_block_last_col;
			avl_block_last_col  = avl_block_last_col2  = i;
			}
	t1 = AVL_MAKE_LINE(avl_block_first_line -> line,1);
	t1 -> next = t1 -> previous = t1;
	if (avl_block_first_line == avl_block_last_line)  {
		avl_block_first_line = t1;
		avl_block_last_line = t1;
		return;
		}
	temp = avl_block_first_line -> next;
	avl_block_first_line = t1;
	for(; temp -> previous != avl_block_last_line;
	    temp = temp -> next)  {
		t2 = AVL_MAKE_LINE(temp -> line,1);
		AVL_LINK(t2,t1);
		t1 = t2;
		}
	avl_block_last_line = t1;
}


void AVL_GUIDE_MBLOCK()
{
	if ((avl_block_first_line != NULL) && (avl_block_last_line != NULL)) 
		AVL_CLEAR_BLOCK();
	if (avl_block_first_line == NULL) {
		avl_block_first_line = avl_windows[avl_window].current_line;
		if (avl_block_first_line -> line_no == 0)  
			avl_windows[avl_window].current_line = 
				avl_block_first_line = avl_block_first_line -> next;
		avl_block_first_col  = avl_windows[avl_window].txt_col;
		avl_block_first_line2 = avl_windows[avl_window].current_line;
		avl_block_first_col2  = avl_windows[avl_window].txt_col;
		strcpy(avl_message,"Now mark the block's end");
		AVL_UPDATE_STATUS_LINE();
		}
	else {
		avl_block_last_line = avl_windows[avl_window].current_line;
		if (avl_block_last_line -> line_no == 0)
			avl_windows[avl_window].current_line = 
				avl_block_last_line = avl_block_last_line -> previous;
		avl_block_last_col  = avl_windows[avl_window].txt_col;
		avl_block_last_line2 = avl_windows[avl_window].current_line;
		avl_block_last_col2  = avl_windows[avl_window].txt_col;
		strcpy(avl_message,"Block marked");
		AVL_UPDATE_STATUS_LINE();
		AVL_MAKE_BLOCK();
		}
}
