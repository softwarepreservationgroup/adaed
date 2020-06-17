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

/*  find.c  */

#include <externs.h>



void AVL_PROCESS_FIND()
{
	AVL_FIND(1,"");
}

void AVL_PROCESS_REPLACE()
{
	AVL_REPLACE(1);
}



void AVL_FIND(int no,char *what)
{
	char f[AVL_MAX_LINEL+10];
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, head = NULL;
	struct rccoord old;
	AVL_WIN_PTR m1 = NULL, m2;
	int n1, n2, x, i, j, k, l, rp;
	static char *msg = " GWAda - Find? ";
	char msg2[80];
	w = &avl_windows[avl_window];
	old = _gettextposition();
	n1 = 62;
	n2 = (80 - n1) / 2;
	if (strlen(what) == 0) {
		m1 = AVL_MAKE_WINDOW(msg,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
		_settextposition(1,1);
		rp = AVL_PROMPT(1,1,avl_find_txt,60);
		if (rp) {
			AVL_DEL_WINDOW(m1);
			return;
			}
		}
	else
		strcpy(avl_find_txt,what);
	temp = w -> current_line;
	i = w -> txt_col;
	while (temp != w -> head)  {
		x = strlen(temp -> line) - (l = strlen(avl_find_txt));
		for(j = i; j < x; ++j)  {
			for(k = 0; k < l; ++k)
				if (toupper(avl_find_txt[k]) != toupper(temp -> line[j + k]))
					break;
			if (k == l) { /* Found string here */
				w -> txt_col = j;
				w -> current_line = temp;
				if (strlen(what)) 
					if (strcmp(what,"$"))  {
						w -> current_line = w -> current_line -> previous;
						w -> current_line = w -> current_line -> previous;
						}
				w -> scr_row = 1;
				AVL_DEL_WINDOW(m1);
				_settextposition(1,1);
				AVL_UPDATE_SCREEN();
				return;
				}
			}
		temp = temp -> next;
		}
	if (!strlen(what))  {
		sprintf(f,"Can't find: %s",avl_find_txt);
		AVL_ERROR(f);
		}
	AVL_DEL_WINDOW(m1);
}

void AVL_DO_REPLACE(int from, AVL_LINE_PTR w)
{
	int i, j, k, n, n2, min;
	j = strlen(avl_find_txt);
	k = strlen(avl_replace_txt);
	n2 = strlen(w -> line);
	min = (j > k) ? k : j;
	for (i = 0; i < min; ++i)
		w -> line[i+from] = avl_replace_txt[i];
	if (j == k) return;
	if (j > k) { /* Have to compress the line */
		n = j - k; 
		for(i = from + min; i <= (n2 - n); ++i)
			w -> line[i] = w -> line[i+n];
		}
	else { /* Have to insert remaining replacement */ 
		n = k - j;
		/*  First shift n positions to the right */
		for(i = n2 + n; i >= (from + min); --i)
			w -> line[i] = w -> line[i-n];
		j = min;
		for(i = from + min; j < k; ++i)
			w -> line[i] = avl_replace_txt[j++];
		}
}		

void AVL_REPLACE(int no)
{
	char f[AVL_MAX_LINEL+10];
	char f2[AVL_MAX_LINEL+10];
	char f3[AVL_MAX_LINEL+10];
	char f3x[AVL_MAX_LINEL+10];
	char rmsg[180];
	char c;
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, last_change, save_line;
	struct rccoord old;
	AVL_WIN_PTR m1, m2, hw2;
	short n1, n2, x, i, j, k, l, question = 0;
	short done = 0, rp, save_txt, save_row, jk;
	static char *msg = " GWAda - Replace what? ";
	static char *msg2 = " GWAda - Replace by? ";
	if (no <= 0) no = 1;
	w = &avl_windows[avl_window];
	old = _gettextposition();
	n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msg,6,n2,8,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	_settextposition(1,1);
	rp = AVL_PROMPT(1,1,avl_find_txt,60);
	if (avl_find_txt[0] == '\0' || rp)  {
		AVL_DEL_WINDOW(m1);
		_settextposition(old.row,old.col);
		return;
		}
	hw2 = AVL_MAKE_WINDOW(" Examples ",17,39,17+6,39+36,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Enter:\n");
	_outtext("    All --- replace all occurences\n");
	_outtext("    3   --- replaces up to 3 times\n");
	_outtext("    3?  --- replaces up to 3 times\n");
	_outtext("            asking confirmation");
	m2 = AVL_MAKE_WINDOW(msg2,10,n2,14,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	_settextposition(1,1);
	rp = AVL_PROMPT(1,1,avl_replace_txt,60);
	if (rp)  {
		AVL_DEL_WINDOW(m2);
		AVL_DEL_WINDOW(hw2);
		AVL_DEL_WINDOW(m1);
		_settextposition(old.row,old.col);
		return;
		}
	_settextposition(3,2);
	_outtext("Replace how many times? ");
	strcpy(f3,"1");
	rp = AVL_PROMPT(3,26,f3,6);
	if (rp)  
	    f3[0] = '\0';
	else {
		strcpy(f3x,f3);
		for(i = 0; i < strlen(f3); ++i)
			if (f3[i] == '?')  {
				f3[i] = '\0';
				question = 1;
				break;
				}
		}
	if ((f3[0] == 'A' || f3[0] == 'a') &&
	    (f3[1] == 'L' || f3[1] == 'l') &&
	    (f3[2] == 'L' || f3[2] == 'l') && f3[3] == '\0') 
	    strcpy(f3,"5000");
	AVL_DEL_WINDOW(m2);
	AVL_DEL_WINDOW(hw2);
	AVL_DEL_WINDOW(m1);
	no = atoi(f3);
	if (f3[0] == '\0' || no == 0)  {
		_settextposition(old.row,old.col);
		return;
		}
	save_line = w -> current_line;
	save_txt = w -> txt_col;
	save_row = w -> scr_row;
	temp = w -> current_line;
	i = w -> txt_col;
	while (temp != w -> head)  {
		x = strlen(temp -> line) - (l = strlen(avl_find_txt));
/* was		for(j = i; j < x; ++j)  {  */
		for(j = i; j <= x; ++j)  {
			for(k = 0; k < l; ++k)
				if (toupper(avl_find_txt[k]) != toupper(temp -> line[j + k]))
					break;
			if (k == l) { /* Found string here */
				if (question)  {
					w -> txt_col = j;
					w -> current_line = temp;
					w -> scr_row = 1;
					AVL_UPDATE_SCREEN();
					AVL_UPDATE_CURSOR();
					AVL_UPDATE_STATUS_LINE();
					for(jk = w -> scr_col; jk < (w -> scr_col + l) && jk < 80 ; ++jk)  {
						c = *AVL_MAP(2,jk);
						AVL_WVIDEO(c,(unsigned char) 
							((avl_men_bk_color + 16) << 4 | avl_men_letter)
							  , AVL_MAP(2,jk));
						}
					_settextposition(1,2);
					sprintf(rmsg,"Replace (Y/N) ? ", avl_replace_txt);
					m1 = AVL_MAKE_WINDOW(msg,6,n2,8,n1+n2,avl_wnd_bk_color,avl_wnd_color);
					_settextposition(1,1);
					_outtext(avl_find_txt);
					m2 = AVL_MAKE_WINDOW(msg2,10,n2,14,n1+n2,avl_wnd_bk_color,avl_wnd_color);
					_settextposition(1,1);
					_outtext(avl_replace_txt);
					_settextposition(3,2);
					_outtext("Replace how many times? ");
					_outtext(f3x);
					if (AVL_QUESTION(rmsg) != 'Y')  {
						AVL_DEL_WINDOW(m2);
						AVL_DEL_WINDOW(m1);
						continue;
						}
					AVL_DEL_WINDOW(m2);
					AVL_DEL_WINDOW(m1);
					}
				w -> changed = 1;
				AVL_DO_REPLACE(j,temp);
				j += (strlen(avl_replace_txt) - 1);
				++done;
				last_change = temp;
				if (--no == 0)  goto job_done;
				}
			}
		temp = temp -> next;
		i = 0;
		}

job_done:

	if (question && done)  {	
		w -> current_line = save_line;
		w -> txt_col = save_txt;
		w -> scr_row = save_row;
		}
	if (!done)  {
		sprintf(f,"Can't replace: %s",avl_find_txt);
		AVL_ERROR(f);
		}
	else  {
		w -> txt_col = j;
		w -> current_line = last_change;
		w -> scr_row = 1;
		_settextposition(old.row,old.col);
		AVL_UPDATE_SCREEN();
		sprintf(f,"Replaced %s %d time%s",avl_find_txt,done, (done > 1) ? "s" : "");
		AVL_TELL(f);
		}
	_settextposition(old.row,old.col);
}

