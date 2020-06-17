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

/*  Color.c  */

#include "externs.h"


void AVL_RESET_COLORS()
{
	avl_txt_color = AVL_TEXT_COLOR;
	avl_txt_bk_color = AVL_TEXT_BK_COLOR;
	avl_wnd_color = AVL_WIND_COLOR;
	avl_wnd_title = AVL_WIND_TITLE;
	avl_wnd_bk_color = AVL_WIND_BK_COLOR;
	avl_que_color = AVL_QUES_COLOR;
	avl_que_bk_color = AVL_QUES_BK_COLOR;
	avl_pro_color = AVL_PROM_COLOR;
	avl_pro_bk_color = AVL_PROM_BK_COLOR;
	avl_men_ready = AVL_MENU_READY;
	avl_men_letter = AVL_MENU_LETTER;
	avl_men_word = AVL_MENU_WORD;
	avl_men_bk_color = AVL_MENU_BK_COLOR;
	avl_err_color = AVL_ERRO_COLOR;
	avl_sta_color = AVL_STAT_COLOR;
	avl_sta_bk_color = AVL_STAT_BK_COLOR;
	avl_msg_color = AVL_MESG_COLOR;
	avl_msg_bk_color = AVL_MESG_BK_COLOR;
}

void AVL_SHOW_COLORS_WHERE()
{
	int i, j, n;
	char cor[2];
	sprintf(cor,"%c",16);
	for(i = 0; i < AVL_NO_COLORS_OPTS; ++i)  {
		if (avl_cor_choices[i].isbk)  {
			_setbkcolor(*avl_cor_choices[i].who);
			_settextcolor(avl_men_word);
			}
		else {
			_setbkcolor(avl_men_bk_color);
			_settextcolor(*avl_cor_choices[i].who);
			}
		_settextposition(i + 1, 2);
		_outtext(avl_cor_choices[i].where);
		_setbkcolor(avl_men_bk_color);
		_settextcolor(avl_men_word);
		_settextposition(i + 1, 1);
		_outtext(" ");
		}
	_settextposition(avl_cor_select+1,1);
	_settextcolor(4);
	_outtext(cor);
}
		
 
void AVL_SHOW_COLORS()
{
	int i, j;
	long bk;
	short co;
	char cor[2];
	co = _gettextcolor();
	bk = _getbkcolor();
	_setbkcolor(7);
	strcpy(cor," ");
	for(i = 0; i < AVL_MAX_COLORS; ++i)  {
		_settextposition(9,i+2);
		_outtext(cor);
		}
	sprintf(cor,"%c",3);
	for(i = 0; i < 8; ++i)  {
		_setbkcolor(i);
		for(j = 0; j < AVL_MAX_COLORS; ++j) {
			_settextposition(i+1,j + 2);
			_settextcolor(j);
			_outtext(cor);
			}
		}
	_settextcolor(4);
	_settextposition(avl_cor_bk_sel+2,avl_cor_txt_sel+2);
	sprintf(cor,"%c",24);
	_outtext(cor);
	_settextcolor(co);
	_setbkcolor(bk);
}

void AVL_SET_COLORS()
{
	int i, ch;
	short c1;
	AVL_WIN_PTR w, w2;
	AVL_SHOW_MENU();
	c1 = avl_menu[avl_cur_menu].c - 1;
	w = AVL_MAKE_WINDOW("",3,c1,3+AVL_NO_COLORS_OPTS+1,22+c1,avl_men_bk_color,avl_wnd_color);
	while ( 1 )  {
		AVL_SHOW_COLORS_WHERE();
		ch = getch();
		if (ch == 27)  {
			AVL_DEL_WINDOW(w);
			return;
			}
		if (ch == 13) {
			w2 = AVL_MAKE_WINDOW("",5,c1+6,8+7,AVL_MAX_COLORS+c1+10,avl_men_bk_color,avl_wnd_color);
			while ( 1 )  {
				AVL_SHOW_COLORS();
				ch = getch();
				if (ch == 27) {
					AVL_DEL_WINDOW(w2);
					break;
					}
				if (ch == 13)  {
					if (avl_cor_choices[avl_cor_select].isbk)
						*avl_cor_choices[avl_cor_select].who = avl_cor_bk_sel;
					else
						*avl_cor_choices[avl_cor_select].who = avl_cor_txt_sel;
					AVL_DEL_WINDOW(w2);
					break;
					}
				if ((ch == 0 || ch == 0xE0)) {
					ch = getch();
					switch(ch)  {
						case 72 : /* Up  */ 
							if (--avl_cor_bk_sel < 0) avl_cor_bk_sel = 7;
							break;
						case 80 : /* Down */ 
							if (++avl_cor_bk_sel > 7) avl_cor_bk_sel = 0;
							break;
						case 75 : /* Left */ 
							if (--avl_cor_txt_sel < 0) avl_cor_txt_sel = 15;
							break;
						case 77 : /* Right */ 
							if (++avl_cor_txt_sel > 15) avl_cor_txt_sel = 0;
							break;
						default : putch(7); break;
						}
					}
				else
					putch(7);
				}
			} /* if (ch == 13)  */
		if ((ch == 0 || ch == 0xE0)) {
			ch = getch();
			switch(ch)  {
				case 72 : /* Up  */ 
					if (--avl_cor_select < 0) avl_cor_select = (AVL_NO_COLORS_OPTS - 1);
					break;
				case 80 : /* Down */ 
					if (++avl_cor_select > (AVL_NO_COLORS_OPTS - 1)) avl_cor_select = 0;
					break;
				default : putch(7); break;
				}
			}
		}	
}
