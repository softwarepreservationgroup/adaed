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

/*  Menus.c  */

#include <externs.h>


/************************************************************************/
/***   Inicializa o Mouse no Video                                    ***/
/*                                                                      */
void AVL_MOUSE_INIT()
{
	union REGS inreg, outreg;
return;
	inreg.x.ax = 0;
	int86(0x33, &inreg, &outreg);
	inreg.x.ax = 1;
	int86(0x33, &inreg, &outreg);
}

void AVL_MOUSE_ON()
{
	union REGS inreg, outreg;
return;
	inreg.x.ax = 1;
	int86(0x33, &inreg, &outreg);
}

void AVL_MOUSE_OFF()
{
	union REGS inreg, outreg;
return;
	inreg.x.ax = 2;
	int86(0x33, &inreg, &outreg);
}


void AVL_MOUSE_SET(int x, int y)
{
	union REGS inreg, outreg;
return;
	inreg.x.ax = 4;
	inreg.x.cx = x;
	inreg.x.dx = y;
	int86(0x33, &inreg, &outreg);
}


/************************************************************************/
/***   Fornece o Status do Mouse - Posicao, Botao Precionado.  	      ***/
/**    Botoes: 0 - esquerda                                            **/
/*             1 - direita          Obs.: Definir estrutura de volta    */
/*             2 - meio                   fora do programa.  xmouse     */
/*                                                           ymouse     */
/*    Retorna:  1 - se botao pressionado                                */
/*              0 - caso contrario ou se nro. do botao for invalido     */
/*                                                                      */
AVL_MOUSE AVL_MOUSE_STATUS()
{
	union REGS inreg, outreg;
	static AVL_MOUSE m;
	short i;
m.status = 0;
return m;

	inreg.x.ax = 3;
	inreg.x.bx = 0;

	int86(0x33, &inreg, &outreg);

	m.x = outreg.x.dx;
	m.y = outreg.x.cx;
	m.x = m.x / 8 + 1;
	m.y = m.y / 8 + 1;
	m.status = outreg.x.bx;

	return(m);
}



int AVL_MOUSE_SELECT(int x1, int y1, int x2, int y2, int *row)
{
	AVL_MOUSE m;
	m = AVL_MOUSE_STATUS();
return 0;
	if (m.x >= x1 && m.x <= x2 && m.y >= y1 && m.y <= y2 && m.status == 1) {
		*row = m.x - x1;
		avl_cur_smenu[avl_cur_menu] = *row;
		AVL_SHOW_SMENU();
		return 1;
		}
	return 0;
}

int AVL_MOUSE_SELECT2(int *item)
{
	AVL_MOUSE m;
	short k, y1, y2;
	m = AVL_MOUSE_STATUS();
return 0;
	if (m.x == 2 && m.status == 1)  
		for(k = 0; k < AVL_MENU_ITEMS; ++k)    {
			y1 = avl_menu[k].c + 1;
			y2 = strlen(avl_menu[k].tit) + y1 - 1;
			if (m.y >= y1 && m.y <= y2 && k != avl_cur_menu)   {
				*item = avl_menu[k].tit[0];
				avl_cur_menu = k;
				AVL_SHOW_MENU();
				return 1;
				}
			}
	*item = avl_menu[avl_cur_menu].tit[0];
	return (m.status == 1) ? 1 : 0;
}


void AVL_SHOW_SMENU()
{
	int i, j, rows = 0, cols = 0;
	short co;
	for(i = 0; *(avl_smenu[avl_cur_menu].s[i].sm) != '\0'; ++i) {
		_settextposition(avl_smenu[avl_cur_menu].r+i,1);
		if (avl_cur_smenu[avl_cur_menu] == i)
			co = _settextcolor(avl_men_ready);
		else
			co = _settextcolor(avl_men_letter);
		_outmem(avl_smenu[avl_cur_menu].s[i].sm,1);
		if (avl_cur_smenu[avl_cur_menu] != i)
			_settextcolor(avl_men_word);
		_outtext(avl_smenu[avl_cur_menu].s[i].sm + 1);
		}
}


int AVL_GET_SMENU_OPT()
{
	int ch, i, j;
	short att, c1;
	int rows, cols;
	AVL_WIN_PTR w = NULL;
	AVL_SHOW_MENU();
	AVL_COMP_SMENU(&rows,&cols);
	AVL_MOUSE_ON();
	AVL_MOUSE_SET(2,2);
	avl_no_up_corner = 1;
	c1 = avl_menu[avl_cur_menu].c - 1;
	ch = 78 - (c1 + cols);
	if (ch < 0) c1 += ch;
	if (!(rows == 0 && cols == 0))
		w = AVL_MAKE_WINDOW("",3,c1,rows+4,c1+cols+1,avl_men_bk_color,avl_wnd_color);
	_settextcursor(0x2000);
	while ( 1 ) {
		if (w != NULL)
			AVL_SHOW_SMENU();
		_settextposition(1,1);
again:
		if (!kbhit())  {
			if (AVL_MOUSE_SELECT(4,c1+1,rows+3,c1+cols,&i))  {
				AVL_DEL_WINDOW(w);
				if (avl_smenu[avl_cur_menu].s[i].ret_edit) {
					ch = 0;
					AVL_DEL_WINDOW(avl_env_win);
					AVL_RESTORE_ENV();
					}
				else
					ch = *(avl_smenu[avl_cur_menu].s[i].sm);
				avl_smenu[avl_cur_menu].s[i].proc();
/*				if ((avl_cur_menu == 0) && (avl_cur_smenu[avl_cur_menu] == 3))
					AVL_BORDER(" GWAda Development Environment ",1,1,3,80,avl_men_bk_color,avl_wnd_color);
*/
				avl_no_up_corner = 0;
				AVL_MOUSE_OFF();
				return ch;
				}
			goto again;
			}
		ch = getch();
		ch = toupper(ch);
		if (ch >= 'A' && ch <= 'Z')   {
			if (w == NULL)  {
				AVL_ERROR("Invalid action");
				continue;
				}
			for (i = 0; i < rows; ++i)
				if (*(avl_smenu[avl_cur_menu].s[i].sm) == ch) {
					avl_cur_smenu[avl_cur_menu] = i;
					AVL_DEL_WINDOW(w);
					if (avl_smenu[avl_cur_menu].s[i].ret_edit) {
						ch = 0;
						AVL_DEL_WINDOW(avl_env_win);
						AVL_RESTORE_ENV();
						}
					avl_smenu[avl_cur_menu].s[i].proc();
/*					if ((avl_cur_menu == 0) && (avl_cur_smenu[avl_cur_menu] == 3))
						AVL_BORDER(" GWAda Development Environment ",1,1,3,80,avl_men_bk_color,avl_wnd_color);
*/
					avl_no_up_corner = 0;
					AVL_MOUSE_OFF();
					return ch;
					}
			continue;
			}
		else {
			switch( ch ) {
				case  27  : /* Abort Menu */
					AVL_DEL_WINDOW(w);
					avl_no_up_corner = 0;
					AVL_MOUSE_OFF();
					return 1;
				case  13  : /* Engage current selection */
					AVL_DEL_WINDOW(w);
					ch =  *(avl_smenu[avl_cur_menu].s[avl_cur_smenu[avl_cur_menu]].sm);
					if (avl_smenu[avl_cur_menu].s[avl_cur_smenu[avl_cur_menu]].ret_edit) {
						ch = 0;
						AVL_DEL_WINDOW(avl_env_win);
						AVL_RESTORE_ENV();
						}
					avl_smenu[avl_cur_menu].s[avl_cur_smenu[avl_cur_menu]].proc();
/*					if ((avl_cur_menu == 0) && (avl_cur_smenu[avl_cur_menu] == 3))
						AVL_BORDER(" GWAda Development Environment ",1,1,3,80,avl_men_bk_color,avl_wnd_color);
*/
					avl_no_up_corner = 0;
					AVL_MOUSE_OFF();
					return ch;
				case 0   : 
				case 0XE0: {
					ch = getch();
					switch(ch)  {
						case 72 : /* Up  */ 
							if (avl_cur_smenu[avl_cur_menu] > 0)
								--avl_cur_smenu[avl_cur_menu];
							else
								avl_cur_smenu[avl_cur_menu] = rows - 1;
							break;
						case 80 : /* Down */ 
							if (avl_cur_smenu[avl_cur_menu] < (rows - 1))
								++avl_cur_smenu[avl_cur_menu];
							else
								avl_cur_smenu[avl_cur_menu] = 0;
							break;
						case 75 : /* Left  */ 
							if (avl_cur_menu > 0) 
								--avl_cur_menu; 
							else
								avl_cur_menu = 8;
							AVL_DEL_WINDOW(w);
							w = NULL;
							AVL_SHOW_MENU();
							AVL_COMP_SMENU(&rows,&cols);
							c1 = avl_menu[avl_cur_menu].c - 1;
							ch = 78 - (c1 + cols);
							if (ch < 0) c1 += ch;
							if (!(rows == 0 && cols == 0))
								w = AVL_MAKE_WINDOW("",3,c1,rows+4,c1+cols+1,avl_men_bk_color,avl_wnd_color);
							_settextcursor(0x2000);
							break;
						case 77 : /* Right */ 
							if (avl_cur_menu < 8) 
								++avl_cur_menu; 
							else
								avl_cur_menu = 0;
							AVL_DEL_WINDOW(w);
							w = NULL;
							AVL_SHOW_MENU();
							AVL_COMP_SMENU(&rows,&cols);
							c1 = avl_menu[avl_cur_menu].c - 1;
							ch = 78 - (c1 + cols);
							if (ch < 0) c1 += ch;
							if (!(rows == 0 && cols == 0))
								w = AVL_MAKE_WINDOW("",3,c1,rows+4,c1+cols+1,avl_men_bk_color,avl_wnd_color);
							else
								w = NULL;
							_settextcursor(0x2000);
							break;
						default : putch(7); break;
						}
					break;
					}
				default : putch(7); break;
				}
			}
		}  /*  While  */
}

char env_saved = 0;

void AVL_SAVE_ENV()
{
	AVL_EDIT_WINDOW_PTR wa;
	env_saved = '1';
	wa = &avl_windows[avl_window];
	wa -> sw.bk = _getbkcolor();
	wa -> sw.co = _gettextcolor();
	memmove(wa -> sw.video, 0xb8000, 4000);
	_gettextwindow(&wa -> sw.r1,&wa -> sw.c1,&wa -> sw.r2,&wa -> sw.c2);
	wa -> sw.pos = _gettextposition();
}

void AVL_RESTORE_ENV()
{
	AVL_EDIT_WINDOW_PTR wa;
	short *s, *d;
	char *p;
	unsigned char c, att;
	short i, j;
/*	if (!env_saved) return;
*/
	env_saved = '\0';		
	wa = &avl_windows[avl_window];
	d = (short *) 0xb8000;
	p =  wa -> sw.video;
	s = (short *) p;
	for(i = 0; i < 2000; ++i) {
		c = *s;
		att = *s >> 8;		
		AVL_WVIDEO(c,att,d);
		++s;
		++d;
		}
/*	memmove(0xb8000, wa -> sw.video, 4000);
*/	_settextwindow(wa -> sw.r1,wa -> sw.c1,wa -> sw.r2,wa -> sw.c2);
	_setbkcolor(wa -> sw.bk);
	_settextcolor(wa -> sw.co);
	_settextposition(wa -> sw.pos.row, wa -> sw.pos.col);
}


void AVL_DO_WINDOW()
{
}


void AVL_ENVIRONMENT(int key)
{
	int ch, opt;
	short att;
	AVL_WIN_PTR w = NULL;
	AVL_SAVE_ENV();
	avl_env_win = NULL;
	if (avl_open_error_file)  {
		avl_open_error_file = 0;
		avl_cur_menu = 0; 
		AVL_OPEN_ERROR();
		AVL_FIND(1,"*** ERROR: ");
		return;
		}
	att = _settextcursor(0x2000);  /*  turn cursor off */
	w = avl_env_win = AVL_MAKE_WINDOW(" GWAda Development Environment ",1,1,3,80,avl_men_bk_color,avl_wnd_color);
	if (key) {
		switch( key ) {
			case 'F' : avl_cur_menu = 0; break;
			case 'E' : avl_cur_menu = 1; break;
			case 'C' : avl_cur_menu = 2; break;
			case 'B' : avl_cur_menu = 3; break;
			case 'R' : avl_cur_menu = 4; break;
			case 'W' : avl_cur_menu = 5; break;
			case 'O' : avl_cur_menu = 6; break;
			case 'A' : avl_cur_menu = 7; break;
			case 'H' : avl_cur_menu = 8; break;
			default  : {
				if (key == 0) key = getch();
				key = 0;
				AVL_ERROR("Invalid selection");
				break;
				}
			}
		}
	while ( 1 ) {

		AVL_SHOW_MENU(); /*  Display top menu  */
		AVL_MOUSE_ON();
		AVL_MOUSE_SET(2,2);
		_settextcursor(0x2000);
		_settextposition(1,1);
		if (key) {
			ch = key;
			key = 0;
			}
		else  
			if (!kbhit())   {
		
				if (!AVL_MOUSE_SELECT2(&ch))  
					continue;
				}
			else {
				ch = getch();
				ch = toupper(ch);
				}
back_here:
		switch( ch ) {
			case  27  : /* ESC : Abort Menu */
				_settextcursor(att);
				AVL_DEL_WINDOW(w);
				AVL_RESTORE_ENV();
				AVL_MOUSE_OFF();
				return;
			case  13  : /* Engage current selection */
				AVL_SHOW_MENU();
				if (avl_cur_menu == 3)  {
					AVL_BIND();
					_settextcursor( att );
					break;
					}
				if (avl_cur_menu == 4)  {
					AVL_RUN();
					_settextcursor( att );
					break;
					}
				if (avl_cur_menu == 5)  {
					AVL_WINDOW();
					AVL_DEL_WINDOW(w);
					AVL_RESTORE_ENV();
					AVL_MOUSE_OFF();
					return;
					}
				opt = AVL_GET_SMENU_OPT();
				if (avl_cur_menu == 2 && avl_open_error_file)  {
					AVL_DEL_WINDOW(w);
					AVL_RESTORE_ENV();
					AVL_MOUSE_OFF();
					return;
					}
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 'F' : 
				avl_cur_menu = 0;
				AVL_SHOW_MENU();
				opt = AVL_GET_SMENU_OPT();
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 'E' : 
				avl_cur_menu = 1;
				AVL_SHOW_MENU();
				opt = AVL_GET_SMENU_OPT();
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 'C' : 
				avl_cur_menu = 2;
				AVL_SHOW_MENU();
				opt = AVL_GET_SMENU_OPT();
				if (avl_open_error_file)  {
					AVL_DEL_WINDOW(w);
					AVL_RESTORE_ENV();
					AVL_MOUSE_OFF();
					return;
					}
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 'B' :  /* Bind */
				avl_cur_menu = 3;
				AVL_SHOW_MENU();
				AVL_BIND();
				_settextcursor( att );
				break;
			case 'R' : 
				avl_cur_menu = 4;
				AVL_SHOW_MENU();
				AVL_RUN();
				_settextcursor( att );
				break;
			case 'W' : 
				avl_cur_menu = 5;
				AVL_SHOW_MENU();
				AVL_WINDOW();
				AVL_DEL_WINDOW(avl_env_win);
				AVL_RESTORE_ENV();
				AVL_MOUSE_OFF();
				return;
			case 'O' :
				avl_cur_menu = 6;
				AVL_SHOW_MENU();
				opt = AVL_GET_SMENU_OPT();
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 'A' :
				avl_cur_menu = 7;
				AVL_SHOW_MENU();
				opt = AVL_GET_SMENU_OPT();
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 'H' : 
				avl_cur_menu = 8;
				AVL_SHOW_MENU();
				opt = AVL_GET_SMENU_OPT();
				_settextcursor( att );
				if (opt == 0)  {
					AVL_MOUSE_OFF();
					return;
					}
				break;
			case 0   : 
			case 0XE0: {
				ch = getch();
				switch(ch)  {
					case 75 : /* Left  */ 
						if (avl_cur_menu > 0) 
							--avl_cur_menu; 
						else
							avl_cur_menu = 8;
						break;
					case 77 : /* Right */ 
						if (avl_cur_menu < 8) 
							++avl_cur_menu; 
						else
							avl_cur_menu = 0;
						break;
					case 33 : /* File */ ch = 'F'; goto back_here;
					case 18 : /* Edit */ ch = 'E'; goto back_here;
					case 46 : /* Comp */ ch = 'C'; goto back_here;
					case 48 : /* Bind */ ch = 'B'; goto back_here;
					case 19 : /* Run  */ ch = 'R'; goto back_here;
					case 17 : /* Wind */ ch = 'W'; goto back_here;
					case 24 : /* Opti */ ch = 'O'; goto back_here;
					case 30 : /* Ada  */ ch = 'A'; goto back_here;
					case 35 : /* Help */ ch = 'H'; goto back_here;
					case 59 : /* F1   */ AVL_HOT_KEYS(); break;
					case 107: /* Alt-F4   */ AVL_EXIT(); 
					                         AVL_MOUSE_OFF();
					                         return;
					default : putch(7); break;
					}
				}
			}
		}
}

void AVL_COMP_SMENU(int *rows, int *cols)
{
	int i, j;
	*rows = *cols = 0;
	for(i = 0; *(avl_smenu[avl_cur_menu].s[i].sm) != '\0'; ++i) {
		j = strlen(avl_smenu[avl_cur_menu].s[i].sm);
		*rows += 1;
		if (j > *cols) *cols = j;
		}
}


void AVL_SHOW_MENU()
{
	int i;
	short co;

	for (i = 0; i < AVL_MENU_ITEMS; ++i)  {
		_settextposition(avl_menu[i].r,avl_menu[i].c);
		if (i != 9)
			if (i == avl_cur_menu)
				co = _settextcolor(avl_men_ready);
			else
				co = _settextcolor(avl_men_letter);
		else 
			co = _settextcolor(15);
		_outmem(avl_menu[i].tit,1);
		if ((i != 9) && (i != avl_cur_menu))
			_settextcolor(avl_men_word);
		_settextposition(avl_menu[i].r,avl_menu[i].c+1);
		_outtext(avl_menu[i].tit+1);
		_settextcolor(co);
		}
}

