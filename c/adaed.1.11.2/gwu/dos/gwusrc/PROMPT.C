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

/*  prompt.c  */

#include <externs.h>

int AVL_PROMPT(short lno, short col,char *s, short max)
{
	char *msg;
	char *msg2;
	char fmt[20], change = 0;
	short i = 0, j, k, att, co, wait = 0;
	int ch, lim, ch1, ch2;
	long bk;
	struct rccoord old;
	msg = malloc(max+10);
	msg2 = malloc(max+10);
	if (msg == NULL || msg2 == NULL)  {
		AVL_ERROR("Memory fault!");
		exit(1);
		}
	bk = _setbkcolor(avl_pro_bk_color);
	co = _settextcolor(avl_pro_color);
	old = _settextposition(lno,col);
	att = _settextcursor(0x0707);	
	sprintf(fmt,"%c-%ds",'%',max);
	sprintf(msg,fmt,s);
	_settextposition(lno,col);
	_outtext(msg);
	lim = strlen(s);
	for(i = 0; i < max; ++i) msg[i] = '\0';
	i = 0;
	strcpy(msg,s);
	_settextposition(lno,col);
	ch1 = getch();
	if (isprint(ch1)) {
		msg2[0] = msg[0] = '\0';
		lim = 0;
		}
	wait = 1;
	while ( 1 )  {
		_settextposition(lno,col);
		sprintf(msg2,fmt,msg);
		_outtext(msg2);
		_settextposition(lno,i+col);
		if (!wait)  
			ch1 = getch();
		else
			wait = 0;
		if (ch1 == 0) {
			ch2 = getch();
			if (ch2 == 75)  /*  left arrow  */
				if (i <= 0) putchar(7);
				else --i;
			else if (ch2 == 77) /*  right arrow  */
				if (i >= lim) putchar(7);
				else ++i; 
			else if (ch2 == 71)  /* Home  */
				i = 0;
			else if (ch2 == 79)  /* End  */
				i = strlen(msg);
			else if (ch2 == 83) {  /* Del */
				if (i >= 0 && i < lim)   {
					--lim;
					change = 1;
					for (j = i; j < lim; ++j)
						msg[j] = msg[j + 1];
					msg[lim] = '\0';
					}
				}
			else putchar(7);
			}
		else if (ch1 == 8) /* Backspace */
				if (i <= 0) putchar(7);
				else {
					--lim;
					for (j = i - 1; j < lim; ++j)
						msg[j] = msg[j + 1];
					msg[lim] = '\0';
					--i;
					}
		else if (ch1 == 9)  putchar(7); /* Tab */
		else if (ch1 == '\n' || ch1 == 13) {
			msg[i] = '\0';
			break;
			}
		else if (ch1 == 27)  { msg[0] = '\0'; break; } /* ESC */
		else if (!isprint(ch1)) putchar(7);
		else {
			if (i >= max) {
				putchar(7);
				continue;
				}
			_settextposition(lno,col+i);
			putch(ch1);
			msg[i++] = ch1;
			if (i > lim) 
				msg[i] = '\0';
			}
		lim = strlen(msg);
		sprintf(msg2,fmt,msg);
		_settextposition(lno,col);
		_outtext(msg2);
		if (ch1 == 27) {
			msg[0] = '\0';
			break;
			}

		}

	if (strlen(msg) > 0)
		strcpy(s,msg);
	*(s+max) = '\0';
	_setbkcolor(bk);
	_settextcolor(co);
	_settextposition(old.row,old.col);
	_settextcursor(att);	
	free(msg);
	free(msg2);
	if (ch1 == 27 || s[0] == '\0') return 1;
	else return 0;
}
