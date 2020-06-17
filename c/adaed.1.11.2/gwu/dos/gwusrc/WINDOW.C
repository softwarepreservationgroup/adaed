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

#include "externs.h"

typedef char AVL_WIN[AVL_MAX_WINDOWS][61];

void AVL_SHOW_WIN(AVL_WIN w, int r)
{
	int i;	
	short co;
	for(i = 1; i <= r; ++i)  {
		_settextposition(i,1);
		if ((i - 1) == avl_window)
			co = _settextcolor(avl_men_ready);
		else
			co = _settextcolor(avl_men_letter);
		_outtext(w[i - 1]);
		}
	_settextcolor(co);
}


                
void AVL_WINDOW()
{
        int x ;
        int no = 0;
        int ch, cols = 0, rows = 0;
        AVL_WIN win;
        AVL_WIN_PTR hw, hw2;
        AVL_EDIT_WINDOW_PTR w;
        if (avl_nwindows == 1)  {
        	AVL_ERROR("Use this option when working with more than one text file.");
        	return;
        	}
        rows = avl_nwindows;
        for(x = 0; x < avl_nwindows; ++x)  {
        	if ((no = strlen(avl_windows[x].file_name)) > cols)
        		cols = no;
        		sprintf(win[x],"%d %s", x + 1,avl_windows[x].file_name);
        		}
		hw2 = AVL_MAKE_WINDOW("",13,55,13+9,55+24,avl_wnd_bk_color,avl_wnd_color);
		_outtext(" Use the arrow keys to\n");
		_outtext(" go  over  the  units.\n");
		_outtext("\n");
		_outtext(" Press  <enter>  to\n");
		_outtext(" select  an  unit  or\n");
		_outtext(" type  the  unit's no.\n");
		_outtext("\n");
		_outtext(" Press ESC to  cancel.");
		hw = AVL_MAKE_WINDOW("",3,avl_menu[5].c,3+rows+1,avl_menu[5].c+cols+4,avl_wnd_bk_color,avl_wnd_color);
		while ( 1 )  {
			AVL_SHOW_WIN(win,rows);
			ch = getch();
			if ((ch >= '1' && ch <= (rows + '0')) || (ch == 13)) {
				if (ch != 13)
					avl_window = ch - '0' - 1;
				AVL_DEL_WINDOW(hw);
				AVL_DEL_WINDOW(hw2);
				return;
				}
			else {
				if (ch == 0 || ch == 0xE0) {
					ch = getch();
                    switch( ch ) {
                    	case 72 : /* Up   */ 
                    		if (--avl_window < 0) 
                    			avl_window = avl_nwindows - 1;
                    		break;
                    	case 80 : /* Down */ 
                    		if (++avl_window >= avl_nwindows)
                    			avl_window = 0;
                    		break;
                    	default : putchar(7); break;
                    	}
                    continue;
                    }
				if (ch == 27)  {
					AVL_DEL_WINDOW(hw);
					AVL_DEL_WINDOW(hw2);
					return;
					}
				putch(7);
				continue;
				}
			}
}			

	

char *AVL_BORDER(char *s, short r1, short c1, short r2, short c2,short bk, short co)
{
/*	static char *avl_border = "Ú¿ÀÙÄ³Ã´";  */
	char avl_border[8];
	int i, j, n;
	short color;
	char buf[81];
	char format[10];
	avl_no_up_corner = 0;
	if (avl_no_up_corner)  {
		avl_border[0] = 203;
		avl_border[1] = 203;
		}
	else {
		avl_border[0] = 201;
		avl_border[1] = 187;
		}
	avl_border[2] = 200;
	avl_border[3] = 188;
	avl_border[4] = 205;
	avl_border[5] = 186;
	sprintf(format,"%c-%ds",'%',c2-c1);
	_settextwindow(r1,c1,r2,c2);
	_setbkcolor( bk );
	_settextcolor( co );
	_settextposition(r2-r1+1,c2-c1+1);
	_outmem(avl_border+3,1);
	_scrolltextwindow( -1 );
	sprintf(buf,format," ");
	for(i = 2; i < (r2 - r1 + 1); ++i) {
		_settextposition(i, 1 );
		_outtext(buf);
		}
	for(i = 0; i < 78; ++i)
		buf[i] = *(avl_border+4);
	buf[c2-c1-1] = '\0';
	_settextposition(1 , 2 );
	_outtext(buf);
	_settextposition(r2 - r1 +1, 2 );
	_outtext(buf);
	for(i = 2; i < (r2 - r1 + 1); ++i) {
		_settextposition(i, 1 );
		_outmem(avl_border+5,1);
		_settextposition(i, c2 - c1 + 1);
		_outmem(avl_border+5,1);
		}
	_settextposition(1,1);
	_outmem(avl_border,1);
	_settextposition(1,c2-c1+1);
	_outmem(avl_border+1,1);
	_settextposition(r2-r1+1,1);
	_outmem(avl_border+2,1);
	color = _settextcolor( avl_wnd_title );
	n = (c2 - c1 + 1 - strlen(s)) / 2;
	_settextposition(1,n);
	_outtext(s);
	color = _settextcolor( color );
	_settextwindow(r1+1,c1+1,r2-1,c2-1);
	_settextposition(1,1);
}


AVL_WIN_PTR AVL_MAKE_WINDOW(char *s, short r1,short c1,short r2,short c2,short bk, short co)
{
	AVL_WIN_PTR temp;
	temp = calloc(1,sizeof(AVL_WIN_SIZE));
	if (temp == NULL)  {
		AVL_ERROR("Out of memory!");
		exit(1);
		}
	temp -> bk = _getbkcolor();
	temp -> co = _gettextcolor();
	memmove(temp -> video, 0xb8000, 4000);
	_gettextwindow(&temp -> r1,&temp -> c1,&temp -> r2,&temp -> c2);
	temp -> pos = _gettextposition();
	AVL_BORDER(s,r1,c1,r2,c2,bk,co);
	_setbkcolor(bk);
	_settextcolor(co);
	return temp;
}

void AVL_DEL_WINDOW(AVL_WIN_PTR w)
{
	struct rccoord pos;
	short *s, *d;
	char *p;
	unsigned char c, att;
	short i, j;
	if (w != NULL)  {
/*		memmove(0xb8000, w -> video, 4000);   */
		d = (short *) 0xb8000;
		p =  w -> video;
		s = (short *) p;
		for(i = 0; i < 2000; ++i) {
			c = *s;
			att = *s >> 8;		
			AVL_WVIDEO(c,att,d);
			++s;
			++d;
			}
		_settextwindow(w -> r1,w -> c1,w -> r2,w -> c2);
		_setbkcolor(w -> bk);
		_settextcolor(w -> co);
		pos = _settextposition(w -> pos.row, w -> pos.col);
		free(w);
		}
}
