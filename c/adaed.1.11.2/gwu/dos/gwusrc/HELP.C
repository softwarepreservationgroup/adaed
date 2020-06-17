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
                
void AVL_HELP(char *local)
{
    AVL_LINE_PTR head = NULL, temp;
    int x ;
    char s[100];
    int no = 0;
    int ch, save_w;
    FILE *fp, *fopen();
    AVL_WIN_PTR hw;
    AVL_EDIT_WINDOW_PTR w;
	char c;
	int updt = 1;
	short r1, r2, c1, c2, i, j;
	extern int avl_ctrl_c;
	AVL_LINE_PTR saved;
	short old_bk_color, old_color;
	short old_sta_bk_color, old_sta_color;
	char *pl;
	pl = getenv("ADAED");
	if (pl == NULL)
		pl=getenv("adaed");
	if (pl != NULL)  
		sprintf(s,"%s%c%s",pl,'\\',local);
	else  {
		AVL_ERROR("Set the ADAED environment variable first (see readme.gw).");
		return;
		}		
    save_w = avl_window;
    w = &avl_windows[avl_window=AVL_MAX_WINDOWS];
    hw = AVL_MAKE_WINDOW("",1,1,24,80,7,15);
    _settextrows( 25 );
    _clearscreen( _GCLEARSCREEN );
    w -> head = w -> current_line = NULL;
    AVL_LOAD_FILE(s);
    temp = head;
    while ( 1 )   {  /*  Command mode  */
		ch = getch();
		switch( ch )  {
			case  1  : /* Ctrl-Left == Ctrl-A */ AVL_WORD_BACKWARD(); break;
			case  6  : /* Ctrl-Right == Ctrl-F */ AVL_WORD_FORWARD(); break;
			case  4  : /* Ctrl-D == Right */  AVL_CURSOR_RIGHT(1); break;
			case 14  :  /*  Ctrl-N */
			case 13  :  /*  Enter  */  AVL_CURSOR_DOWN(1); break;
			/*  Control keys */
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
			case 27  :  /*  ESC       */  
               	AVL_FREE_ALL();
               	avl_window = save_w;
                sprintf(avl_message," ");
               	AVL_DEL_WINDOW(hw);
               	return;
			case  0  : {
				ch = getch();
				switch( ch ) {
					/*  Alt keys  */
					case 59 : /* F1 without alternate   */ AVL_HOT_KEYS(); break;
					case 35 : /* Help */ AVL_ENVIRONMENT('H'); break;
					case 107: /* Alt-F4   */ AVL_EXIT(); return;
					case 63 : /* F5   */ AVL_GOTO(); break;	
					case 71 : /* Home */ AVL_CURSOR_HOME(); break;
					case 79 : /* End  */ AVL_CURSOR_END(); break;
					case 73 : /* PgUp */ AVL_CURSOR_PGUP(); break;
					case 81 : /* PgDn */ AVL_CURSOR_PGDN(); break;
					case 75 : /* Left */ AVL_CURSOR_LEFT(no); no = 0; break;
					case 77 : /* Right*/ AVL_CURSOR_RIGHT(no); no = 0; break;
					case 72 : /* Up   */ AVL_CURSOR_UP  (no); no = 0; break;
					case 80 : /* Down */ AVL_CURSOR_DOWN(no); no = 0; break;
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
				putchar(7);
				break;
				}       		
			}

		}
}




void AVL_HELP_INTRO()
{
	AVL_HELP("lrm-01.hlp");
}
   
void AVL_HELP_LEXIE()
{
	AVL_HELP("lrm-02.hlp");
}
   
void AVL_HELP_DCLTY()
{
	AVL_HELP("lrm-03.hlp");
}
   
void AVL_HELP_NEXPS()
{
	AVL_HELP("lrm-04.hlp");
}
   
void AVL_HELP_STMTS()
{
	AVL_HELP("lrm-05.hlp");
}
   
void AVL_HELP_SUBPG()
{
	AVL_HELP("lrm-06.hlp");
}
   
void AVL_HELP_PACKS()
{
	AVL_HELP("lrm-07.hlp");
}
   
void AVL_HELP_VRULE()
{
	AVL_HELP("lrm-08.hlp");
}
   
void AVL_HELP_TASKS()
{
	AVL_HELP("lrm-09.hlp");
}
   
void AVL_HELP_PSCIS()
{
	AVL_HELP("lrm-10.hlp");
}
   
void AVL_HELP_EXCEP()
{
	AVL_HELP("lrm-11.hlp");
}
   
void AVL_HELP_GENER()
{
	AVL_HELP("lrm-12.hlp");
}
   
void AVL_HELP_RCIDF()
{
	AVL_HELP("lrm-13.hlp");
}
   
void AVL_HELP_INOUT()
{
	AVL_HELP("lrm-14.hlp");
}
   
void AVL_HELP_ANEXA()
{
	AVL_HELP("lrm-a.hlp");
}
   
void AVL_HELP_ANEXB()
{
	AVL_HELP("lrm-b.hlp");
}
   
void AVL_HELP_ANEXC()
{
	AVL_HELP("lrm-c.hlp");
}
   
void AVL_HELP_ANEXD()
{
	AVL_HELP("lrm-d.hlp");
}
   
void AVL_HELP_ANEXE()
{
	AVL_HELP("lrm-e.hlp");
}
   
void AVL_HELP_ANEXF()
{
	AVL_HELP("lrm-f.hlp");
}
   


void AVL_HELP_EDITOR()
{
	AVL_HELP("AVL_EDIT.HLP");
}

void AVL_HELP_OPTIONS()
{
	AVL_HELP("AVL_OPTI.HLP");
}

void AVL_HELP_FILE()
{
	AVL_HELP("AVL_FILE.HLP");
}

void AVL_HELP_BIND()
{
	AVL_HELP("AVL_BIND.HLP");
}

void AVL_HELP_GWADA()
{
	AVL_HELP("AVL_GWAD.HLP");
}

void AVL_HELP_COMPILER()
{
	AVL_HELP("AVL_COMP.HLP");
}

void AVL_HELP_INTERPRETER()
{
	AVL_HELP("AVL_INTE.HLP");
}

void AVL_HELP_WINDOW()
{
	AVL_HELP("AVL_WIND.HLP");
}

void AVL_MDH_TOOLS()
{
	AVL_HELP("AVL_MDHE.HLP");
}
