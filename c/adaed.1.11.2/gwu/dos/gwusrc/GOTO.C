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

/* goto.c */

#include "externs.h"


void AVL_DO_GOTO(int n)
{
    AVL_EDIT_WINDOW_PTR w;
    AVL_LINE_PTR temp;
    char inp[100];
    w = &avl_windows[avl_window];
    temp = w -> head -> next;
    while ( temp -> line_no != n)  {
        temp = temp -> next;
        if (temp == w -> head)  {
            sprintf(inp,"Program ERROR AVL_GOTO (%d) ...", n);
            AVL_ERROR(inp);
            exit(1);
            }
        }
    w -> current_line = temp;
    w -> txt_col = 0;
    w -> scr_col = 1;
    w -> scr_row = 1;
    _settextposition(1,1);
    AVL_UPDATE_SCREEN();
}

void AVL_GOTO()
{
    AVL_WIN_PTR m;
    AVL_EDIT_WINDOW_PTR w;
    AVL_LINE_PTR temp;
    char inp[100];
    int n;
    w = &avl_windows[avl_window];
    inp[0] = '\0';
    m = AVL_MAKE_WINDOW(" Go to line number: ",23,1,25,30
        ,avl_pro_bk_color,avl_pro_color);
    AVL_PROMPT(1,2,inp, 10);
    if (inp[0] == '\0') {
        AVL_DEL_WINDOW(m);
        return;
        }
    n = atoi(inp);
    temp = w -> head -> next;
    if ((n < (w -> head -> next -> line_no - 1)) ||
        (n > (w -> head -> previous -> line_no - 1)))  {
        AVL_ERROR("Invalid line number.");
        AVL_DEL_WINDOW(m);
        return;
        }
    ++n;
    AVL_DEL_WINDOW(m);
    AVL_DO_GOTO(n);
}
