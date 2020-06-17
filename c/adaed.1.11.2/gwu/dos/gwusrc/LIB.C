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

/* Lib.c */

#include <externs.h>

void AVL_SHOW_UNIT2(AVL_UNITDT_PTR w)
{
	int i, first, last, k;	
	short co;
	char msg[100];

	k = avl_cur_unit / 12;
	first = k * 12;
	last = first + 11;
	if (last >= avl_size) 
		last = avl_size - 1;

	
	_settextposition(1,1);
	_outtext("\n\n\n\n\n\n\n\n\n\n\n\n");
	k = 1;
	for(i = first; i <= last; ++i)  {
		_settextposition(k,1);
		if (i == avl_cur_unit)
			co = _settextcolor(avl_men_ready);
		else
			co = _settextcolor(avl_men_letter);
		sprintf(msg," %s %-6s %c%c/%c%c/%c%c %c%c:%c%c %-38s"
			, (w -> s[i].status  == 'O') ? "Obsolete" : "Active  "
			, (w -> s[i].is_main == 'M') ? "(Main)" : ""
			, w -> s[i].cdate[0] / 10 + '0'
			, w -> s[i].cdate[0] % 10 + '0'
			, w -> s[i].cdate[1] / 10 + '0'
			, w -> s[i].cdate[1] % 10 + '0'
			, w -> s[i].cdate[2] / 10 + '0'
			, w -> s[i].cdate[2] % 10 + '0'
			, w -> s[i].cdate[3] / 10 + '0'
			, w -> s[i].cdate[3] % 10 + '0'
			, w -> s[i].cdate[4] / 10 + '0'
			, w -> s[i].cdate[4] % 10 + '0'
			, w -> s[i].fu
			);
		_outtext(msg);
		++k;
		}
	_settextcolor(co);
}
                
void AVL_UNIT2(AVL_UNITDT_PTR w)
{
	int x;
	int r;
	int no = 0;
	int ch, cols = 80, rows;
	AVL_WIN_PTR hw, hw2;
	r = w -> ns;
	if (r < 12)
	   	rows = r;
	else 
		rows = 12;
	avl_size = r;
	avl_cur_unit = 0;

	hw2 = AVL_MAKE_WINDOW("",18,55,18+5,55+24,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Use the arrow keys to\n");
	_outtext(" go  over  the  units.\n");
	_outtext("\n");
	_outtext(" Press ESC to  cancel.");

	hw = AVL_MAKE_WINDOW(" Library's Contents ",4,1,rows+6,80,avl_wnd_bk_color,avl_wnd_color);

	while ( 1 )  {
		AVL_SHOW_UNIT2(w);
		ch = getch();
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
			return;
			}
		putch(7);
		continue;
		}
}			

								
void  AVL_LIB_SELECT(AVL_UNITDT_PTR w)
{
	short rows, cols;
	int i;
	char msg[50];

	for(i = 0; i < w -> ns; ++i)  
		if (!strcmp(w -> s[i].fu,"subprogram DUMMY")) {
			AVL_DELETE_ONE(i,w);
			--i;
			continue;
			}
}

void AVL_MAKE_UNIT_DATE(AVL_UNITDT_PTR w)
{
	FILE *fp, *fopen();
	char status[20];
	char type[20];
	char date[20];
	char timex[20];
	char unit[60];
	char line[128];
	int i, j, k;
	w -> ns = 0;
	fp = fopen("avllib.$$$","r");
	k = 0;
	if (fp == NULL)  
		return;
	if (fgets(line,127,fp) == NULL)  return;
	do {
		if (fgets(line,127,fp) == NULL)  {
			if (feof(fp)) break;
			AVL_ERROR("Error reading library. You should make a new library.");
			fclose(fp);
			return;
			}
		if (line[0] == 13) continue;
		if (line[0] == 10) continue;
		if (line[0] == '\n') continue;
		if (strlen(line) < 1) continue;
		for(i = j = 0; line[i] != ' '; ++i, ++j)
			status[j] = line[i];
		status[j] = '\0';
		i = 11;
		if (line[i] != ' ')  {
			for(j = 0; line[i] != ' '; ++i, ++j)
				type[j] = line[i];
			type[j] = '\0';
			}
		else strcpy(type,"  ");
		i = 21;
		for(j = 0; line[i] != ' '; ++i, ++j)
			date[j] = line[i];
		date[j] = '\0';
		i = 30;
		for(j = 0; line[i] != ' '; ++i, ++j)
			timex[j] = line[i];
		timex[j] = '\0';
		i = 37;
		for(j = 0; i < strlen(line); ++i, ++j)
			unit[j] = line[i];
		if (unit[j - 1] == '\n') --j;
		unit[j] = '\0';
		w -> s[k].status = status[0];
		w -> s[k].is_main = type[1];
		w -> s[k].cdate[0] = (date[0] - '0') * 10 + date[1] - '0';
		w -> s[k].cdate[1] = (date[3] - '0') * 10 + date[4] - '0';
		w -> s[k].cdate[2] = (date[6] - '0') * 10 + date[7] - '0';
		w -> s[k].cdate[3] = (timex[0] - '0') * 10 + timex[1] - '0';
		w -> s[k].cdate[4] = (timex[3] - '0') * 10 + timex[4] - '0';
		strcpy(w -> s[k].fu,unit);
		++k;
		} while (!feof(fp));
	w -> ns = k;
	fclose(fp);
}

		
void AVL_LIST_LIB()
{
/*	AVL_EDIT_WINDOW_PTR w;  */
	AVL_WIN_PTR temp;
	AVL_UNITDT w;
	char msg[100];
	char *bunit;
	char *opts[17];
	int cmd_ret_code, i;
    FILE *fp, *fopen();
    avl_blank_line[0] = '\0';
/*	w = &avl_windows[avl_window];  */
	w.ns = 0;
	opts[0] = "Adalib2";
	opts[1] = "-l";
	opts[2] = avl_dir_library;
	opts[3] = NULL;
	temp = AVL_MAKE_WINDOW(" List Library ",5,1,9,80,avl_wnd_bk_color,avl_wnd_color);
	_outtext("  Searching library. Please, wait a moment...\n");
	if (AVL_EX_UNIT("ADALIB2", opts))  {
		AVL_DEL_WINDOW(temp);
		msg[0] = '\0';
		AVL_MAKE_UNIT_DATE(&w);
		if (w.ns > 0)  {
			AVL_LIB_SELECT(&w);
			if (w.ns != 0)  
				AVL_UNIT2(&w);
			else
				AVL_ERROR("Library empty");
			}			
		else 
			AVL_ERROR("Library empty");
		}
	avl_open_error_file = 0;
}

