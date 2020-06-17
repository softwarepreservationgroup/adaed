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

/*  Codes.c  */

#include "externs.h"

void AVL_BUILD_CODES()
{
        short i, j, k = 0;
        /*  Mount Table  */
        for(i = 0; i < 8; ++i)
                for(j = 0; j < 40; ++j)  {
                        avl_mat_code[i][j] = ' ';
                        if (k < 157)
                                avl_mat_code[i][++j] =  avl_codes[k++];
                        else
                                avl_mat_code[i][++j] = ' ';
                        }
}


void AVL_SHOW_CODES()
{
        short i, j, k = 0, co;
        char line[41];
        sprintf(line,"%40s"," ");
        co = _settextcolor(avl_men_letter);
        for(i = 0; i < 8; ++i)  {
                _settextposition(i + 1 , 1);
                _outtext(line);
                }
        for(i = 0; i < 8; ++i)  {
                for(j = 0; j < 40; ++j)
                        line[j] = avl_mat_code[i][j];
                line[40] = '\0';
                _settextposition(i+1,1);
                _outmem(line,40);
                }
        _settextposition(avl_codes_row+1,avl_codes_col*2+1);
        sprintf(line,"%c",16);
        _settextcolor(4);
        _outtext(line);
        _settextcolor(co);
}


void AVL_INSERT_CODES()
{
        AVL_WIN_PTR w;
        int ch;
        AVL_BUILD_CODES();
        w = AVL_MAKE_WINDOW(" Special Characters ",3,30,12,72,avl_men_bk_color,avl_wnd_color);
        while ( 1 )  {
                AVL_SHOW_CODES();
                ch = getch();
                switch ( ch ) {
                        case 27 : 
                                AVL_DEL_WINDOW(w);
                                return;
                        case 13 :
                                AVL_DEL_WINDOW(w);
                                AVL_EDIT_INSERT(avl_mat_code[avl_codes_row][avl_codes_col*2+1]);
                                return;
                        case  0   :
                        case 0xE0 : {
                                ch = getch();
                                switch(ch)  {
                                       case 72 : /* Up  */ 
                                                if (--avl_codes_row < 0) avl_codes_row = 7;
                                                break;
                                       case 80 : /* Down */ 
                                                if (++avl_codes_row > 7) avl_codes_row = 0;
                                                break;
                                       case 75 : /* Left */ 
                                                if (--avl_codes_col < 0) avl_codes_col = 19;
                                                break;
                                       case 77 : /* Right */ 
                                                if (++avl_codes_col > 19) avl_codes_col = 0;
                                                break;
                                       default : putch(7); break;
                                       }
                                continue;
                                }
                        default : putch(7); break;
                        }
                }
}

