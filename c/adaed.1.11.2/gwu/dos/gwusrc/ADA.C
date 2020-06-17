/* ada.c */

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

void AVL_CENTER(int line, char *msg)
{
        int i;
        i = (80 - strlen(msg)) / 2;                
        _settextposition(line,i);
        _outtext(msg);
}


void main(int nn, char *aa[])
{
        AVL_WIN_PTR m;
        AVL_LINE_PTR head = NULL, temp;
        AVL_MOUSE mouse;
        char line[100];
        int x ;
        int no = 0;
        int ch;

        AVL_EDIT_WINDOW_PTR w;
        AVL_INSTALL_I23();
/*              system("mouse");
*/
        sprintf(avl_version_mark,"Version %s 93",GWAda_Version);
        w = &avl_windows[avl_window];
        AVL_READ_OPTIONS();
        AVL_GET_CDIR();
        _settextrows( 25 );
        _clearscreen( _GCLEARSCREEN );
        m = AVL_MAKE_WINDOW("",1,1,24,80,avl_txt_bk_color,avl_txt_color);
        _settextcursor(0x2000);  /*  turn cursor off */
        sprintf(line,"GWAda Development Environment  -  Version %s  (C) 1992, 1993",GWAda_Version);
        AVL_CENTER( 3,line);
        AVL_CENTER( 5,"  Michael B. Feldman, Ph.D.      Arthur V. Lopes, Ph.D.   ");
        AVL_CENTER( 6,"    mfeldman@seas.gwu.edu          lopes@seas.gwu.edu     ");
        AVL_CENTER( 8,"              Charles Kann, Ph.D. Candidate               ");
        AVL_CENTER( 9,"                   ckann@seas.gwu.edu                     ");
        AVL_CENTER(12," Department of Electrical Engineering and Computer Science");
        AVL_CENTER(13,"        School of Engineering and Applied Science         ");
        AVL_CENTER(14,"             The George Washington University             ");
        AVL_CENTER(15,"                  Washington, D.C. 20052                  ");
        AVL_CENTER(16,"                         U. S. A.                         ");
        AVL_CENTER(18,"A Project Co-Sponsored by ARPA (Contract# FY3592-93-10234)");
        AVL_CENTER(19,"Encapsulating the NYU Ada 83 Translator");
        AVL_CENTER(20,"Freely distributable without cost or obligation");
        AVL_CENTER(22,"Press any key to start ...");
        _settextcursor(0x2000);  /*  turn cursor off */
        AVL_MOUSE_INIT();
        AVL_MOUSE_ON();
        while (!kbhit())  {
                mouse = AVL_MOUSE_STATUS();
                if (mouse.status == 1) break;
                }
        if (mouse.status != 1)  {
                x = getch(); 
                if (x == 0) x = getch();
                }
        AVL_DEL_WINDOW(m);
        if (nn == 1)    {
                AVL_LOAD_FILE("NONAME.Ada");
                strcpy(current_file_name,"NONAME.Ada");
                }
        else  {
                AVL_LOAD_FILE(*(aa+1));
                strcpy(current_file_name,*(aa+1));
                }
        avl_nwindows += 1;
        AVL_MOUSE_OFF();
        AVL_EDIT('i');
        AVL_RESTORE_I23();
        AVL_MOUSE_OFF();
}
