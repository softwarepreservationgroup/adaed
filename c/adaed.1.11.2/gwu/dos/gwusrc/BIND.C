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

/* Bind.c */

#include <externs.h>

extern void AVL_MAKE_UNIT_DATE(AVL_EDIT_WINDOW_PTR w);

short first, last;

void AVL_SHOW_UNIT(AVL_BIND_PTR w, short cols)
{
	short i,  k;	
	short co;
	char msg[35];
	char fmt[20];
	sprintf(fmt," %cc %c-%ds", '%', '%', cols);

	k = avl_cur_unit / 9;
	first = k * 9;
	last = first + 8;
	if (last >= avl_size) 
		last = avl_size - 1;
	
	_settextposition(1,1);
	_outtext("\n\n\n\n\n\n\n\n\n\n\n\n");
	_outtext("\n\n\n\n\n\n\n\n\n\n\n\n");
	k = 1;
	for(i = first; i <= last; ++i)  {
		_settextposition(k,1);
		if (i == avl_cur_unit)
			co = _settextcolor(avl_men_ready);
		else
			co = _settextcolor(avl_men_letter);
		sprintf(msg,fmt, k + '0', w[i].name);
		_outtext(msg);
		++k;
		}
	_settextcolor(co);
}
                

int AVL_UNIT(AVL_BIND_PTR w, int r)
{
        short x ;
        short no = 0;
        short ch, cols = 0, rows;
        AVL_WIN_PTR hw, hw2;
        if (r < 10)
        	rows = r;
        else 
        	rows = 9;
        avl_size = r;
        avl_cur_unit = 0;

        /* Compute the maximum window's length */
        for(x = 0; x < r; ++x)  
        	if ((no = strlen(w[x].name)) > cols)
        		cols = no;

		cols += 4;
		hw2 = AVL_MAKE_WINDOW("",15,55,15+9,55+24,avl_wnd_bk_color,avl_wnd_color);
		_outtext(" Use the arrow keys to\n");
		_outtext(" go  over  the  units.\n");
		_outtext("\n");
		_outtext(" Press  <enter>  to\n");
		_outtext(" select  an  unit  or\n");
		_outtext(" type  the  unit's no.\n");
		_outtext("\n");
		_outtext(" Press ESC to  cancel.");
		hw = AVL_MAKE_WINDOW("",3,avl_menu[3].c,3+rows+1,avl_menu[3].c+cols+5,avl_wnd_bk_color,avl_wnd_color);

		while ( 1 )  {
			AVL_SHOW_UNIT(w,cols-4);
			ch = getch();
			x = (last - first + 1);
			if ((ch >= '1' && ch <= (x + '0')) || (ch == 13)) {
				if (ch != 13)
					avl_cur_unit += (ch - '0' - 1);
				AVL_DEL_WINDOW(hw);
				AVL_DEL_WINDOW(hw2);
				return avl_cur_unit;
				}
			else {
				if (ch == 0 || ch == 0xE0) {
					ch = getch();
                    switch( ch ) {
		               	case 73 : /* Page Up */ 
		               		avl_cur_unit -= rows;
		               		if (avl_cur_unit < 0)  
								avl_cur_unit = avl_size - 1;
		               		break;
		               	case 81 : /* Page Down */ 
		               		avl_cur_unit += rows;
		               		if (avl_cur_unit >= avl_size)
		               			avl_cur_unit = 0;
		               		break;
                    	case 72 : /* Up */ 
                    		if (--avl_cur_unit < 0)  
								avl_cur_unit = avl_size - 1;
                    		break;
                    	case 80 : /* Down */ 
                    		if (++avl_cur_unit >= avl_size)
                    			avl_cur_unit = 0;
                    		break;
                    	default : putchar(7); break;
                    	}
                    continue;
                    }
				if (ch == 27)  {
					AVL_DEL_WINDOW(hw);
					AVL_DEL_WINDOW(hw2);
					return -1;
					}
				putch(7);
				continue;
				}
			}
}			



void AVL_MAKE_BNAME(char *s, char *r)
{
	short i, j = 0;
	char msg[50];
	char *p;
	p = r;
	i = strlen(s) - 1;
	while (*(s + i) != ' ') --i;
	for(++i; *(s + i) != '\0'; ++i, ++r)
		*r = *(s + i);
	*r = '\0';
	r = p;
	if (strlen(p) > 10) {
		i = strlen(p);
		if (*(p + --i) == 'k' &&
		    *(p + --i) == 's' &&
		    *(p + --i) == 'a' &&
		    *(p + --i) == 't' &&
		    *(p + --i) == '_' &&
		    *(p + --i) == 'e' &&
		    *(p + --i) == 'l' &&
		    *(p + --i) == 'd' &&
		    *(p + --i) == 'i' &&
		    *(p + --i) == '_')  
			*(p + i) = '\0';
		}
}	

void AVL_DELETE_ONE(short j, AVL_UNITDT_PTR w)
{
	short k;
	w -> ns -= 1;
	for(k = j; k < w -> ns; ++k) {
		w -> s[k].status   = w -> s[k + 1].status; 
		w -> s[k].is_main  = w -> s[k + 1].is_main;
		strcpy(w -> s[k].fu,w -> s[k + 1].fu);
		w -> s[k].cdate[0] = w -> s[k + 1].cdate[0];
		w -> s[k].cdate[1] = w -> s[k + 1].cdate[1];
		w -> s[k].cdate[2] = w -> s[k + 1].cdate[2];
		w -> s[k].cdate[3] = w -> s[k + 1].cdate[3];
		w -> s[k].cdate[4] = w -> s[k + 1].cdate[4];
		}
}	
	

void AVL_DELETE_BNAME(short i,AVL_UNITDT_PTR w,char *s)
{
	short j, k;
	char msg[50];
	for(j = i + 1; j < w -> ns; ++j)  {
		if (w -> s[j].is_main == ' ' && 
			w -> s[j].fu[0] == 'b' &&
			w -> s[j].fu[1] == 'i' &&
			w -> s[j].fu[2] == 'n')  {
			AVL_MAKE_BNAME(w -> s[j].fu,msg);
			if (!strcmp(s,msg)) { 
				AVL_DELETE_ONE(j,w);
				break;
				}
			}
		}
}
								
char *AVL_BIND_SELECT(AVL_UNITDT_PTR w)
{
	short rows, cols;
	short i, j, k, n = 0, nc = 0;

	char msg[50];

	static AVL_BIND_SIZE r[AVL_MAX_UNITS];
	AVL_BIND_SIZE c[AVL_MAX_UNITS];
	 
	/* Pick obsolete units for compilation */
	for(i = 0; i < w -> ns; ++i)  {
		if (!strcmp(w -> s[i].fu,"subprogram DUMMY")) {
			AVL_DELETE_ONE(i,w);
			--i;
			continue;
			}
		if (w -> s[i].status == 'O' && w -> s[i].is_main == 'M')   {
				if (nc >= AVL_MAX_UNITS) {
					AVL_ERROR("Too many units. Reinitialize library!");
					return;
					}
				c[nc].st = 'C';
				AVL_MAKE_BNAME(w -> s[i].fu,c[nc].name);
				/* An obsolete unit may be in "s" twice,
				   therefore, it must be deleted */
				AVL_DELETE_ONE(i,w); /* Now there may be one left */
				AVL_DELETE_BNAME(-1,w,c[nc].name);
				++nc;
				}
		}

	for(i = 0; i < w -> ns; ++i)  
		if (w -> s[i].status == 'A' && w -> s[i].is_main == 'M')   {
			/*  Subprogram ready for binding */
			if (n >= AVL_MAX_UNITS) {
				AVL_ERROR("Too many units. Reinitialize library!");
				return;
				}
			r[n].st = 'B';
			AVL_MAKE_BNAME(w -> s[i].fu,r[n].name);
			AVL_DELETE_ONE(i,w);
			--i;
			++n;
			}


	/*  Delete units that are already binded and ready for execution */
/*  Allow all units to be binded 
	for(i = 0; i < w -> ns; ++i)  
		if (w -> s[i].status == 'A' && w -> s[i].is_main == ' ')   {
			AVL_MAKE_BNAME(w -> s[i].fu, msg);
			for(j = 0; j < n; ++j)  
				if (!strcmp(msg,r[j].name))  {
					--n;
					AVL_DELETE_ONE(i,w);
					for(k = j; k < n; ++k)  {
						r[k].st = r[k + 1].st;
						strcpy(r[k].name,r[k+1].name);
						}
					--i;
					break;
					}
			}				
*/

	if (n > 0)
		i = AVL_UNIT(r,n);
	else {
		i = -1;
		AVL_ERROR("First compile any executable program unit.");
		}
	if (i >= 0)
		return r[i].name;
	return NULL;
}
		

void AVL_BIND()
{
	AVL_EDIT_WINDOW_PTR ww;
	AVL_WIN_PTR temp;
	char msg[100];
	char *bunit;
	char *opts[17];
	AVL_UNITDT w;
	short cmd_ret_code, i;
    FILE *fp, *fopen();
    avl_blank_line[0] = '\0';
/*	w = &avl_windows[avl_window];  */
	w.ns = 0;
	opts[0] = "Adalib2";
	opts[1] = "-l";
	opts[2] = avl_dir_library;
	opts[3] = NULL;
	unlink("gwada.smm");
	temp = AVL_MAKE_WINDOW(" Binding ",5,1,9,80,avl_wnd_bk_color,avl_wnd_color);
	_outtext("  Searching library. Please, wait a moment...\n");
	if (AVL_EX_UNIT("ADALIB2", opts))  {
		AVL_DEL_WINDOW(temp);
		msg[0] = '\0';
		AVL_MAKE_UNIT_DATE(&w);
		if (w.ns > 0)  {
			bunit = AVL_BIND_SELECT(&w);
			if (bunit != NULL)  {
				sprintf(msg," %s\n", bunit);
				temp = AVL_MAKE_WINDOW(" Binding ",5,1,8,80,avl_wnd_bk_color,avl_wnd_color);
				_outtext(msg);
				opts[0] = "Adabind";
				opts[1] = "-m";
				opts[2] = bunit;
				opts[3] = avl_dir_library;
				opts[4] = NULL;
				if (AVL_EX_UNIT("ADABIND", opts))  ;
				AVL_DEL_WINDOW(temp);
				}
			}			
		else  {
			AVL_ERROR("First compile any executable program unit.");
			AVL_DEL_WINDOW(temp);
			}
		}
	avl_open_error_file = 0;
}
	
