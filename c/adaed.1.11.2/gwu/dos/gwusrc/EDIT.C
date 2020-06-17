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

/* edit.c */

#include "externs.h"


int AVL_COL()
{
	AVL_EDIT_WINDOW_PTR w;
	int i, j = 0, n, n2 = 0;
	char *t, line [30];
	w = &avl_windows[avl_window];
	t = w -> current_line -> line;
	n = strlen(t);
	for(i = 0; i <= w -> txt_col; ++i)  {
		if (*(t + i) == '\t' && i != w -> txt_col)  {
			for (n2= 0; !(((j + 1) % w -> tabsize) == 0); ++n2)	
				j++;
			j++;
			}
		else 
			j++;
		}
	return j;
}



void AVL_JOIN_RIGHT() /* Join current line with next */
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, temp2;
	struct rccoord old;
	int pos, pos2;
	w = &avl_windows[avl_window];
	if (w -> current_line -> next != w -> head)  {
		pos2 = w -> txt_col;
		if ((strlen(w -> current_line -> line) + strlen(w -> current_line -> next -> line)) >= AVL_MAX_LINEL)  {
			AVL_ERROR("Lines are too long. Can't perform join!");
			return;
			}
		if (w -> scr_row < (w -> r2 - w -> r1 + 1))  {
			old = _settextposition(w -> scr_row+1,w -> scr_col);
			AVL_DELETE_LINE();
			}
		else
			old = _settextposition(w -> scr_row,w -> scr_col);
		
		temp = w -> current_line -> next;
		strcat(w -> current_line -> line,temp -> line);
		w -> current_line -> next = temp -> next;
		temp -> next -> previous = w -> current_line;
		AVL_MAKE_NUMBER();
		free(temp);
		pos = w -> scr_row;
		temp = temp2 = w -> current_line;
		while (++pos != (w -> r2 - w -> r1 + 1) && temp -> next != w -> head)	{
			temp = temp -> next;
			}
		if (pos == (w -> r2 - w -> r1 + 1) && temp != w -> head)  {
			w -> current_line = temp;
			w -> scr_row = w -> r2 - w -> r1 + 1;
			_settextposition( w -> scr_row, 1);
			AVL_UPDATE_LINE();
			}
		w -> current_line = temp2;
		old = _settextposition(w -> scr_row = old.row,w -> scr_col = old.col);
		w -> txt_col = pos2;
		w -> changed = 1;
		AVL_UPDATE_LINE();
		}
}		

void AVL_JOIN_LEFT() /* Join current line with Previos */
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, temp2;
	struct rccoord old;
	int pos, pos2;
	w = &avl_windows[avl_window];
	if (w -> current_line -> previous != w -> head)  {
		if ((strlen(w -> current_line -> line) + strlen(w -> current_line -> previous -> line)) >= AVL_MAX_LINEL)  {
			AVL_ERROR("Lines are too long. Can't perform join!");
			return;
			}
		old = _settextposition(w -> scr_row+1,w -> scr_col);
		temp = w -> current_line;
		w -> current_line = w -> current_line -> previous;
		AVL_CURSOR_END();
		pos2 = w -> txt_col;
		strcat(w -> current_line -> line, temp -> line);
		w -> current_line -> next = temp -> next;
		temp -> next -> previous = w -> current_line;
		AVL_MAKE_NUMBER();
		free(temp);
		if (w -> scr_row != 1)
			AVL_DELETE_LINE();
		pos = w -> scr_row;
		temp = temp2 = w -> current_line;
		while (++pos != (w -> r2 + 1) && temp -> next != w -> head)	{
			temp = temp -> next;
			}
		if (pos == (w -> r2 + 1) && temp != w -> head)  {
			w -> current_line = temp;
			w -> scr_row = (w -> r2 - 1);
			_settextposition( (w -> r2 - 1), 1);
			AVL_UPDATE_LINE();
			}
		w -> current_line = temp2;
		if (old.row > 1)
			old = _settextposition(w -> scr_row = old.row - 1,w -> scr_col = old.col);
		else
			old = _settextposition(w -> scr_row = old.row,w -> scr_col = old.col);
		w -> txt_col = pos2;
		AVL_UPDATE_LINE();
		w -> changed = 1;
		}
}		


int AVL_OFFSET()
{
	AVL_EDIT_WINDOW_PTR w;
	int n, i, off = 0;
	w = &avl_windows[avl_window];
	n = AVL_COL();
	off = 0;
	for(i = (w -> c2 - w -> c1 + 1); i < n; i += 20)
		off += 20;
	return ( off );
}

int AVL_COUNT()
{
	int i = 0;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	avl_line_temp = w -> current_line;
	for (; avl_line_temp != w -> head 
	       && i < w -> r2; avl_line_temp = avl_line_temp -> next)
		++i;
	avl_line_temp = avl_line_temp -> previous;
	return i;
}

void AVL_OPEN_LINE()
{
   short left, top, right, bottom;
   struct rccoord rc;

   _gettextwindow( &top, &left, &bottom, &right );
   rc = _gettextposition();
   _settextwindow( rc.row+1, left, bottom, right );
   _scrolltextwindow( _GSCROLLDOWN );
   _settextwindow( top, left, bottom, right );
   _settextposition( rc.row+1, rc.col );
}

void AVL_EDIT_ENTER()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp;
	struct rccoord old;
	short i, j, k;
	old = _gettextposition();
	w = &avl_windows[avl_window];
	for(i = 0, j = 0; w -> current_line -> line [i] == ' '; ++i);
	j = i;
	temp = calloc(1,sizeof(AVL_LINE_SIZE));
	temp -> previous = w -> current_line;
	temp -> next     = w -> current_line -> next;
	w -> current_line -> next -> previous = temp;
	w -> current_line -> next = temp;
	strcpy(temp -> line, w -> current_line -> line + w -> txt_col);
	w -> current_line -> line [w -> txt_col] = '\0';
	AVL_MAKE_NUMBER();
	w -> txt_col = 0;
	AVL_OPEN_LINE();
	_settextposition(w -> scr_row,w -> scr_col = 1);
	AVL_UPDATE_LINE();
	if (w -> scr_row == (w -> r2 - w -> r1 + 1)) 
		_scrolltextwindow( 1 );
	else
		w -> scr_row += 1;
	w -> current_line = temp; 
	w -> changed = 1;
	if (strlen(temp -> line) > 0)
		j = 0;  
	for(i = 0; i < j; ++i)
		w -> current_line -> line[i] = ' ';
	if (j > 0)
		w -> current_line -> line[i] = '\0';
	_settextposition(w -> scr_row,w -> scr_col = (j < 78) ? j + 1 : 1);
	w -> txt_col = (j < 78) ? j : 0;
	AVL_UPDATE_LINE();
	if (w -> offset != 0)  {
		w -> offset = 0;
		AVL_SCROLL();
		}
}

void AVL_SCROLL()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp;
	struct rccoord old;
	short i, j, k;
	AVL_WIN_PTR t, t2;
	w = &avl_windows[avl_window];
	old = _gettextposition();
	temp = w -> current_line;
	j = old.row;
/*
	if ((w -> txt_col == strlen(temp -> line)) && (w -> txt_col > 0))
		w -> txt_col -= 1;
*/
	k = w -> txt_col;
	for(i = j; i > 1; --i)  {
		w -> current_line = w -> current_line -> previous;
		}
	w -> scr_row = 1;
	avl_hscroll_on = 1;
	_settextposition(1,1);
	AVL_UPDATE_SCREEN();
	_settextposition(old.row,old.col);
	w -> current_line = temp;
	w -> scr_row = j;
	w -> scr_col = old.col;
	w -> txt_col = k;
	avl_hscroll_on = 0;
	AVL_UPDATE_CURSOR();
}



void AVL_UPDATE_STATUS_LINE()
{
	AVL_EDIT_WINDOW_PTR w;
	char status[10];
	char line[181];
	char fn[121];
	short att, attold;
	short i, n;
	char *p;
	w = &avl_windows[avl_window];
	attold = _gettextcursor();
	if (w -> no_status == 'N') return;
	if (avl_window < AVL_MAX_WINDOWS)
		switch (tolower(w -> edit_mode))  {
			case 'x' : strcpy(status,"Ovr"); att = 0x0007; break;
			case 'i' : strcpy(status,"Ins"); att = 0x0707; break;
			default  : w -> edit_mode = 'i'; strcpy(status,"Ins"); att = 0x0707; break;
			}
	else
		strcpy(status,"Hlp");
	AVL_MAKE_FN(fn, w -> file_name);
	sprintf(line,"Line %3d Col %2d %-35s %s %-12s F1-Help             "
		,  w -> current_line -> line_no - 1
		, w -> txt_col + 1
		, avl_message
		, status
		, fn
		);
	n = strlen(line);
	if (n > 80) n = 80;
	p = line;
	for(i = 1; i <= n; ++i, ++p)
/*		AVL_PUT(*p,w -> r1 - 1,i,avl_sta_bk_color,avl_sta_color);   */
		AVL_WVIDEO(*p,(unsigned char) (avl_sta_bk_color << 4 | avl_sta_color)
			, AVL_MAP(w -> r1 - 1,i));
	if (attold != att)
		_settextcursor(att);	
}


void AVL_INIT_WINDOW(AVL_EDIT_WINDOW_PTR w, AVL_LINE_PTR first)
{
	short att;
	att = _settextcursor(0x0607);
	avl_find_txt[0] = '\0';
	avl_replace_txt[0] = '\0';
	avl_block_first_line = NULL;
	avl_block_last_line = NULL;
	avl_block_first_col = 0;
	avl_block_last_col = 0;
	sprintf(avl_blank_line,"%80s"," ");
	w -> head         = first -> previous;
	w -> current_line = first;
	w -> no_status = 'Y';
	w -> r1     = 2;
	w -> c1     = 1;
	w -> r2     = 25;
	w -> c2     = 80;
	w -> offset = 0;
	w -> edit_mode = 'i';
	w -> tabsize   = avl_tab_size;
	w -> scr_col   = 1;
	w -> scr_row   = 1;
	w -> txt_col   = 0;
	w -> line_no = 0;
	avl_message[0] = '\0';
    _settextwindow(w -> r1, w -> c1, w -> r2, w -> c2);
	_setbkcolor( avl_txt_bk_color );
	_settextcolor( avl_txt_color );
}

char *AVL_MAKE_TXT_LINE()
{
	char *d;
	int i, j = 0, n, n2 = 0;
	AVL_EDIT_WINDOW_PTR w;
	char *t, *t2;
	w = &avl_windows[avl_window];
	t = w -> current_line -> line;
	d = w -> expanded_line;
	n = strlen(t);
	for(i = 0; i < n; ++i)  {
		if (*(t + i) == '\t')  {
			for (n2= 0; !(((j + 1) % w -> tabsize) == 0); ++n2)
				d[j++] = ' ';
			d[j++] = ' ';
			}
		else 
			d[j++] = *(t + i);
		}
	d[j] = '\0';		
	return d;
}		

void AVL_UPDATE_LINE()
{
	short i, j, k;
	char *d;
	char *p;
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	d = AVL_MAKE_TXT_LINE();
	j = strlen(d);
	if (w -> txt_col > (k = strlen(d)))
		w -> txt_col = k;
	if (w -> txt_col < 0) 
		w -> txt_col = 0;
	if (j <= w -> offset)
		j = 0;
	else
		j = j - w -> offset + 1;
	k = w -> scr_row + w -> r1 - 1;
	i = (j > (w -> c2 - w -> c1 + 1)) ? (w -> c2 - w -> c1 + 1) : j;
	p = d + w -> offset;
	for(j = 1; j <= i; ++j, ++p)
		AVL_WVIDEO(*p,(unsigned char) (avl_txt_bk_color << 4 | avl_txt_color) 
			, AVL_MAP(k,j));
/*		AVL_PUT(*p,k,j,avl_txt_bk_color,avl_txt_color);   */
	for(; j <= (w -> c2 - w -> c1 + 1); ++j)
		AVL_WVIDEO(' ',(unsigned char) (avl_txt_bk_color << 4 | avl_txt_color), AVL_MAP(k,j));
/*		AVL_PUT(' ',k,j,avl_txt_bk_color,avl_txt_color);  */
	strcpy(w -> current_line -> line,d);
	if (!avl_hscroll_on)
		AVL_UPDATE_CURSOR();
}


void show_sit(char *s)
{
	struct rccoord old;
	static short x = 0;
	AVL_EDIT_WINDOW_PTR w;
	int n, j;
	char msg[200];
	if (x >= 6) x = 0;
	w = &avl_windows[avl_window];
	n = strlen(w -> current_line -> line);
	old = _gettextposition();
	_settextposition(10+x++,1);
	sprintf(msg,"%s offset=%d scr_col=%d txt_col=%d len=%d char=%c[%d]", s, 
		w -> offset, w -> scr_col, w -> txt_col, n 
		, w -> current_line -> line [w -> txt_col]
		, w -> current_line -> line [w -> txt_col]);
	_outtext(msg);
	j = getch();
	_settextposition(old.row,old.col);
}

void AVL_EDIT_INSERT(int ch)
{
	AVL_EDIT_WINDOW_PTR w;
	short n, j, oldoffset;
	char *p;
	w = &avl_windows[avl_window];
	oldoffset = w -> offset;
	n = strlen(w -> current_line -> line);
/*	if (w -> txt_col >= n)  
		AVL_CURSOR_END();
*/
	if (n >= AVL_MAX_LINEL) { putchar(7); return; }
	if (w -> edit_mode == 'i') {   /*  Insert mode */
		if ((n + 1) >= AVL_MAX_LINEL) { putchar(7); return; }
		for (j = n; j >= w -> txt_col; --j)
			w -> current_line -> line[j + 1] = w -> current_line -> line[j];
		w -> current_line -> line[w -> txt_col] = ch;
		}
	else {
		w -> current_line -> line[w -> txt_col] = ch;
		if (w -> txt_col >= n)  {
			w -> current_line -> line[w -> txt_col+1] = '\0';
			}
		}
	w -> txt_col += 1;
	w -> offset = AVL_OFFSET();
	if (w -> offset != oldoffset)  
		AVL_SCROLL();
	w -> buffer_size += 1;
	w -> changed = 1;
	n = strlen(w -> current_line -> line);
	AVL_UPDATE_LINE();
	j = strlen(w -> current_line -> line);
	if (j != n)  {
		w -> txt_col += (j - n);
		AVL_UPDATE_CURSOR();
		}
/*show_sit("Depois ==> ");
*/
}

	AVL_FLIP_MODE()
{
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	switch (tolower(w -> edit_mode))  {
		case 'c' : w -> edit_mode = 'i';  break;
		case 'i' : w -> edit_mode = 'x';  break;
		case 'x' : w -> edit_mode = 'i';  break;
		default  : AVL_ERROR("Invalid status mode..."); exit(1);
		}
	AVL_UPDATE_STATUS_LINE();
}

void AVL_HOT_KEYS()
{
	AVL_WIN_PTR win;
	win = AVL_MAKE_WINDOW(" Hot Keys ",4,3,17,76,avl_wnd_bk_color,avl_wnd_color);
	/*          1         2         3         4         5         6         7 */
	/* 1234567890123456789012345678901234567890123456789012345678901234567890 */
	/* 

         Alternate keys: File Edit Compile Bind Run Window Options Ada Help
                         F4 quit

	                     Example: Alt-F ==> activates the File Menu

	     Editing keys:   F5      goto line        Ctrl-G  delete char
	                     Ctrl-KB begin block      Ctrl-Y  delete line      
	                     Ctrl-KK end block        Ctrl-QR go begin text
	                     Ctrl-KC copy block       Ctrl-RC go end text
	                     Ctrl-KS save             Ins     insert/over. mode
            
	*/
	_settextcolor(avl_men_letter);
	_settextposition(2,3);  _outtext("Alternate keys: ");
	_settextcolor(avl_men_ready); _outtext("F");
	_settextcolor(avl_men_letter); _outtext("ile ");
	_settextcolor(avl_men_ready); _outtext("E");
	_settextcolor(avl_men_letter); _outtext("dit ");
	_settextcolor(avl_men_ready); _outtext("C");
	_settextcolor(avl_men_letter); _outtext("ompile ");
	_settextcolor(avl_men_ready); _outtext("B");
	_settextcolor(avl_men_letter); _outtext("ind ");
	_settextcolor(avl_men_ready); _outtext("R");
	_settextcolor(avl_men_letter); _outtext("un ");
	_settextcolor(avl_men_ready); _outtext("W");
	_settextcolor(avl_men_letter); _outtext("indow ");
	_settextcolor(avl_men_ready); _outtext("O");
	_settextcolor(avl_men_letter); _outtext("ptions ");
	_settextcolor(avl_men_ready); _outtext("A");
	_settextcolor(avl_men_letter); _outtext("da ");
	_settextcolor(avl_men_ready); _outtext("H");
	_settextcolor(avl_men_letter); _outtext("elp");
	_settextposition(3,19); _settextcolor(avl_men_ready); _outtext("F4");
 	_settextcolor(avl_men_letter); _outtext(" quit");
	_settextposition(5,19); _outtext("Example: ");
	_settextcolor(avl_men_ready); _outtext("Alt-F");
	_settextcolor(avl_men_letter); _outtext(" ==> activates the File Menu");
	_settextposition(7,3);  _outtext("Editing keys:   ");
	_settextcolor(avl_men_ready); _outtext("F5");
	_settextcolor(avl_men_letter); _outtext("      goto line        ");
	_settextcolor(avl_men_ready); _outtext("Ctrl-G");
	_settextcolor(avl_men_letter); _outtext("  delete char");
	_settextposition(8,19); _settextcolor(avl_men_ready); _outtext("Ctrl-KB");
	_settextcolor(avl_men_letter); _outtext(" begin block      ");
	_settextcolor(avl_men_ready); _outtext("Ctrl-Y");
	_settextcolor(avl_men_letter); _outtext("  delete line");
	_settextposition(9,19); _settextcolor(avl_men_ready); _outtext("Ctrl-KK");
	_settextcolor(avl_men_letter); _outtext(" end block        ");
	_settextcolor(avl_men_ready); _outtext("Ctrl-QR");
	_settextcolor(avl_men_letter); _outtext(" go begin text");
	_settextposition(10,19); _settextcolor(avl_men_ready); _outtext("Ctrl-KC");
	_settextcolor(avl_men_letter); _outtext(" copy block       ");
	_settextcolor(avl_men_ready); _outtext("Ctrl-RC");
	_settextcolor(avl_men_letter); _outtext(" go end text");
	_settextposition(11,19);_settextcolor(avl_men_ready);  _outtext("Ctrl-KS");
	_settextcolor(avl_men_letter); _outtext(" save             ");
	_settextcolor(avl_men_ready); _outtext("Ins");
	_settextcolor(avl_men_letter); _outtext("     insert/over. mode");
	AVL_PAUSE(20);
	AVL_DEL_WINDOW(win);
}


void AVL_WORD_BACKWARD()
{
	AVL_EDIT_WINDOW_PTR w;
	char *p, *q;
	w = &avl_windows[avl_window];
	p = &( w -> current_line -> line[w -> txt_col] );
	q = &( w -> current_line -> line[0] );
	while (!(p == q || *p == ' ')) {  /* skip letters going left */
		AVL_CURSOR_LEFT(1);           /* getting out of current word */
		--p;
		}
	if (*p == ' ' && p != q) /* is there a blank? */
		do { /* skip blanks */
			AVL_CURSOR_LEFT(1); 
			--p;
			} while (*p == ' ' && p != q);
	if (p != q)  { /* are we at the right of a symbol? */
		while (!(p == q || *p == ' ')) {
			AVL_CURSOR_LEFT(1); 
			--p;
			}
/*		if (p != q)  {
			AVL_CURSOR_RIGHT(1);   
			++p;
			}
*/
  		}
	if (*p == ' ')  
		do {
			AVL_CURSOR_RIGHT(1);   
			++p;
			} while (*p == ' ');
}

void AVL_WORD_FORWARD()
{
	AVL_EDIT_WINDOW_PTR w;
	char *p;
	w = &avl_windows[avl_window];
	p = &( w -> current_line -> line[w -> txt_col] );
	while (!(*p == '\0' || *p == ' ')) {
		AVL_CURSOR_RIGHT(1); 
		++p;
		}
	if (*p == ' ') 
		do {
			AVL_CURSOR_RIGHT(1); 
			++p;
			} while (*p == ' ');
}	
		
void AVL_DO_HELP_INDEX(char *s, short pos)
{
	short i, k;
	short nopt = 0;
	char word[200];
	char do_call[300];
	char c;
	char *opts[17];
	c = *(s+pos);
	if (!(isalpha(c) || c == '_')) {
		AVL_ERROR("Activate the Search Help (F8) when the cursor is over a word");
		return;
		}
	i = pos;
	while (i >=0 && (isalpha(*(s+i)) || *(s+i) == '_')) --i;
	if (i < 0) i = 0;
	else ++i;
	k = 0;
	while (isalpha(*(s+i)) || *(s+i) == '_') word[k++] = *(s+i++);
	word[k] = '\0';
	opts[nopt++] = "Adahelp";
	opts[nopt++] = word;
	opts[nopt++] = NULL;
	sprintf(do_call,"ADAHELP %s",word);
	system(do_call);
/*
	if (AVL_EX_UNIT("ADAHELP", opts))  ;
*/
}

void AVL_EDIT(char mode)
{
	int ch, no = 1;
	char c;
	int updt = 1;
	short r1, r2, c1, c2, i, j;
	extern int avl_ctrl_c;
	AVL_LINE_PTR saved;
	AVL_EDIT_WINDOW_PTR w;
	short old_bk_color, old_color;
	short old_sta_bk_color, old_sta_color;
	w = &avl_windows[avl_window];
	w -> edit_mode = mode;
	strcpy(avl_message,"Press ESC to activate top menu");
	AVL_UPDATE_CURSOR();
	AVL_UPDATE_STATUS_LINE();
	old_color = avl_txt_color;
	old_bk_color = avl_txt_bk_color;
	old_sta_color = avl_sta_color;
	old_sta_bk_color = avl_sta_bk_color;
	while ( 1 )  {
		w = &avl_windows[avl_window];
		if (!((old_sta_bk_color == avl_sta_bk_color) && (old_sta_color == avl_sta_color))) {
			for(j = 1; j <= 80; ++j)  {
				c = *AVL_MAP(1,j);
				AVL_WVIDEO(c,(unsigned char) (avl_sta_bk_color << 4 | avl_sta_color), AVL_MAP(1,j));
				}
			old_sta_bk_color = avl_sta_bk_color;
			old_sta_color = avl_sta_color;
			}
		if (!((old_bk_color == avl_txt_bk_color) && (old_color == avl_txt_color))) {
			_gettextwindow(&r1,&c1,&r2,&c2);
			for(i = r1; i <= r2; ++i)
				for(j = c1; j <= c2; ++j)  {
					c = *AVL_MAP(i,j);
					AVL_WVIDEO(c,(unsigned char) (avl_txt_bk_color << 4 | avl_txt_color), AVL_MAP(i,j));
					}
			old_bk_color = avl_txt_bk_color;
			old_color = avl_txt_color;
			}

		if (avl_open_error_file)  {
			AVL_ENVIRONMENT('F');
			avl_open_error_file = 0;
			continue;
			}

		if (!kbhit()) {
			if (updt)  {
				AVL_UPDATE_CURSOR();
				AVL_UPDATE_STATUS_LINE();
				updt = 0;
				}
			continue;
			}
		updt = 1;
		ch = getch();
		switch( ch )  {
			case  1  : /* Ctrl-Left == Ctrl-A */ AVL_WORD_BACKWARD(); break;
			case  6  : /* Ctrl-Right == Ctrl-F */ AVL_WORD_FORWARD(); break;
			case  4  : /* Ctrl-D == Right */  AVL_CURSOR_RIGHT(1); break;
			case  8  : { /*  Backspace */  
			    if (w -> txt_col > 0) {
					w -> txt_col -= 1;
					if (w -> offset != AVL_OFFSET()) {
						w -> offset = AVL_OFFSET();
						AVL_SCROLL();
						}
					AVL_EDIT_DEL_RIGHT(no); 
					no = 0; 
					}
				else {  /* Join with the previous line */
					w -> changed = 1;
					w -> offset = 0;
					AVL_JOIN_LEFT();
					}
				break;
				}
			case  9  :  /*  Tab       */  AVL_EDIT_INSERT(ch); break;
			case 14  :  /*  Ctrl-N */
			case 13  :  /*  Enter     */  AVL_EDIT_ENTER(); break;
			/*  Control keys */
			case  7 : /* Delete current char - Ctrl G */
				AVL_EDIT_DEL_RIGHT(no); 
				no = 0; 
				break;
			case 11 : /* Mark Block - Ctrl K */
				ch = toupper(getch());
				if (ch == 'B' || ch == 'K') {
					AVL_GUIDE_MBLOCK ();
					break;
					}
				else if (ch == 'C')  {
					AVL_PROCESS_COPY();
					break;
					}
				else if (ch == 'S')  {
					AVL_SAVE();
					break;
					}
				if (ch == 0) ch = getch();
				AVL_ERROR("Use only Ctrl-k b, Ctrl-k k, Ctrl-k c and Crtl-k s");
				break;
			case 17 : /* Go to begin/end of text file  Ctrl-Q R or C or Y */
				avl_ctrl_c = 0;
				ch = toupper(getch());
				if (ch == 89 || ch == 25)   {  /* Got a Ctrl-Q Y */
					w -> current_line -> line [w -> txt_col] = '\0';
					AVL_UPDATE_LINE();
					break;
					}
				AVL_UPDATE_LINE();
				if (ch == 'C' || avl_ctrl_c) {
					if (w -> head -> previous != w -> head)
						AVL_DO_GOTO(w -> head -> previous -> line_no);
					break;
					}
				else if (ch == 'R' || ch == 18) {
					avl_ctrl_c = 0;
					if (w -> head -> next != w -> head)
						AVL_DO_GOTO(w -> head -> next -> line_no);
					break;
					}
				if (ch == 0) ch = getch();
				AVL_ERROR("Use only Ctrl-q r, Ctrl-q c or Ctrl-q y");
				break;
			case 22 : /* Ctrl-V == Ins key */ AVL_FLIP_MODE(); break;
			case 25 : /* Delete current line - Ctrl Y */
				AVL_EDIT_DEL_LINE(no); 
				no = 0; 
				break;
			case 27  :  /*  ESC       */  
				if (avl_cur_menu == 7 && avl_window == AVL_MAX_WINDOWS) 
					return;
				else {
					if (do_not_realy_exit) {
						AVL_EXIT();
						return;
						}
					AVL_ENVIRONMENT(0); 
					AVL_UPDATE_CURSOR();
					AVL_UPDATE_STATUS_LINE();
					updt = 1;
					break;
					}

				break;
			case  0  : {
				ch = getch();
				switch( ch ) {
					/*  Alt keys  */
					case 33 : /* File */ AVL_ENVIRONMENT('F'); break;
					case 59 : /* F1 without alternate   */ AVL_HOT_KEYS(); break;
					case 60 : /* F2 without alternate */ AVL_ENVIRONMENT('F'); break;
					case 66 : /* F8 without alternate */ AVL_DO_HELP_INDEX(w -> current_line -> line,w -> txt_col); break;
					case 18 : /* Edit */ AVL_ENVIRONMENT('E'); break;
					case 46 : /* Comp */ AVL_ENVIRONMENT('C'); updt = 1; break;
					case 48 : /* Bind */ AVL_ENVIRONMENT('B'); break;
					case 19 : /* Run  */ AVL_ENVIRONMENT('R'); break;
					case 17 : /* Wind */ AVL_ENVIRONMENT('W'); updt = 1; break;
					case 24 : /* Opti */ AVL_ENVIRONMENT('O'); break;
					case 30 : /* Ada  */ AVL_ENVIRONMENT('A'); break;
					case 35 : /* Help */ AVL_ENVIRONMENT('H'); break;
					case 107: /* Alt-F4   */ AVL_EXIT(); return;
					case 63 : /* F5   */ AVL_GOTO(); break;	
					case 65 : /* F7 */ AVL_BODY(); break;
					case 68 : /* F10 */
						if (avl_cur_menu == 7 && avl_window == AVL_MAX_WINDOWS) 
							return;
						else {
							if (do_not_realy_exit) {
								AVL_EXIT();
								return;
								}
							AVL_ENVIRONMENT(0); 
							AVL_UPDATE_CURSOR();
							AVL_UPDATE_STATUS_LINE();
							updt = 1;
							break;
							}

					case 82 : /* Ins  */ AVL_FLIP_MODE(); break;
					case 83 : /* Del  */ AVL_EDIT_DEL_RIGHT(no); no = 0; break;
					case 71 : /* Home */ AVL_CURSOR_HOME(); break;
					case 79 : /* End  */ AVL_CURSOR_END(); break;
					case 73 : /* PgUp */ AVL_CURSOR_PGUP(); break;
					case 81 : /* PgDn */ AVL_CURSOR_PGDN(); break;
					case 75 : /* Left */ AVL_CURSOR_LEFT(no); no = 0; break;
					case 77 : /* Right*/ AVL_CURSOR_RIGHT(no); no = 0; break;
					case 72 : /* Up   */ AVL_CURSOR_UP  (no); no = 0; break;
					case 80 : /* Down */ AVL_CURSOR_DOWN(no); no = 0; break;
					case 85 : /* SHIFT+F2 == Ctrl-K S */ AVL_SAVE(); break;
					case 92 : /* Shift+F9 == Alt-F D */ AVL_DOS_CMD(); break;
					case 115 : /* Ctrl-Left == Ctrl-A */ AVL_WORD_BACKWARD(); break;
					case 116 : /* Ctrl-Right == Ctrl-F */ AVL_WORD_FORWARD(); break;
					case 117 : /* Ctrl-End */
						AVL_DO_GOTO(w -> head -> previous -> line_no);
						break;
					case 119 : /* Ctrl-Home */
						AVL_DO_GOTO(w -> head -> next -> line_no);
						break;
					default : putchar(7); break;
					}
				break;
				}
			default : {
				if (ch >= 32 && ch <= 126)  /* ' ' && '~' */ 
					AVL_EDIT_INSERT(ch);
				else
					putchar(7);
				break;
				}       		
			}
		}
}
