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

/* Ed.h */
#include <malloc.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <dos.h>
#include <bios.h>
#include <errno.h>
#include <stdio.h>
#include <conio.h>
#include <direct.h>
#include <process.h>
#include <stdlib.h>
#include "\gwada\env\edit.h"
#include "\gwada\mon\monitor.h"
#include "\gwada\mon\mon_ext.h"

#ifndef AVL_MOUSE_IN
#define AVL_MOUSE_IN
typedef struct {
	short x;
	short y;
	short status;
	} AVL_MOUSE, *AVL_MOUSE_PTR;
#endif


AVL_MOUSE AVL_MOUSE_STATUS();
void AVL_MOUSE_SET(int x, int y);
void AVL_MOUSE_INIT();
void AVL_MOUSE_ON();
void AVL_MOUSE_OFF();

void AVL_HOT_KEYS();
void AVL_WVIDEO(unsigned char x, unsigned char a, short *c);
int AVL_COMP_SETUP(AVL_EDIT_WINDOW_PTR w);
void AVL_COMPILE_CURRENT();
void AVL_COMP_ORDER();
void AVL_BODY();

void AVL_RESTORE_ENV();
void AVL_COMP_SMENU(int *rows, int *cols);
void AVL_SHOW_MENU();
void AVL_SHOW_SMENU();
void AVL_CLEAR_LINE();
void AVL_SAVE_AS();
void AVL_REMOVE_BLOCK();
void AVL_CALL_TOOL();
void AVL_MOUNT_TEMPLATE(char *fname);
void AVL_TEMPLATE_ADA_PGM();
void AVL_TEMPLATE_ADA_VECTOR();
void AVL_TEMPLATE_ADA_ACCESS();
void AVL_TEMPLATE_ADA_RECORD();
void AVL_TEMPLATE_ADA_MATRIX();
void AVL_TEMPLATE_ADA_TASK();
void AVL_TEMPLATE_ADA_PACKAGE();
void AVL_TEMPLATE_ADA_GENERIC();
void AVL_TEMPLATE_ADA_SERVER();
void AVL_TEMPLATE_ADA_LOOP();
void AVL_TEMPLATE_ADA_WHILE();
void AVL_TEMPLATE_ADA_IF();
void AVL_TEMPLATE_ADA_CASE();
void AVL_TEMPLATE_ADA_FUNCTION();
void AVL_TEMPLATE_ADA_PROCEDURE();

void AVL_HELP_INTRO();   
void AVL_HELP_LEXIE();   
void AVL_HELP_DCLTY();   
void AVL_HELP_NEXPS();   
void AVL_HELP_STMTS();   
void AVL_HELP_SUBPG();   
void AVL_HELP_PACKS();   
void AVL_HELP_VRULE();   
void AVL_HELP_TASKS();   
void AVL_HELP_PSCIS();   
void AVL_HELP_EXCEP();   
void AVL_HELP_GENER();   
void AVL_HELP_RCIDF();   
void AVL_HELP_INOUT();   
void AVL_HELP_ANEXA();   
void AVL_HELP_ANEXB();   
void AVL_HELP_ANEXC();   
void AVL_HELP_ANEXD();   
void AVL_HELP_ANEXE();   
void AVL_HELP_ANEXF();   

void AVL_FIND(int no,char *what);
void AVL_SHOW_ASCII();
void AVL_LIST_LIB();
char *AVL_RUN_SELECT(AVL_UNITDT_PTR w);
int AVL_UNIT(AVL_BIND_PTR w, int r);
void AVL_DO_DEL_LINE();
void AVL_NEW_LIB();
void AVL_BIND();
void AVL_ERROR(char *s);
void AVL_TELL(char *s);
void AVL_MAKE_NUMBER();
void AVL_FIX();
void AVL_RESTORE_I23();
void AVL_INSTALL_I23();
void AVL_DO_GOTO(int n);
void AVL_PAUSE(short l);
void AVL_RESET_COLORS();
void AVL_OPEN();
void AVL_OPEN_ERROR();
void AVL_CLOSE();
void AVL_HELP(char *s);
void AVL_FULL_FN(char *s);
void AVL_LINK();
void AVL_DELETE_LINE();
void AVL_ABOUT_GWADA();
void AVL_DOS_CMD();
void AVL_PROCESS_FIND();
void AVL_PROCESS_REPLACE();
void AVL_GUIDE_MBLOCK();
void AVL_PROCESS_COPY();

void AVL_SET_DIRECTORIES(); /* Options */
void AVL_SET_PARAMETERS();
void AVL_SET_COLORS();
void AVL_LOAD_OPTIONS();
void AVL_SAVE_OPTIONS();
void AVL_READ_OPTIONS();
void AVL_DEFAULT_OPTIONS();
void AVL_FREE_ALL();
void AVL_SET_TAB();
void AVL_PROCESS_LOAD();

void AVL_HELP_EDITOR(); /* Help */
void AVL_HELP_COMPILER();
void AVL_HELP_INTERPRETER();
void AVL_HELP_WINDOW();
void AVL_HELP_BIND();
void AVL_HELP_FILE();
void AVL_HELP_OPTIONS();
void AVL_HELP_GWADA();

void AVL_MD_TOOLS();
void AVL_MDH_TOOLS();
void AVL_GET_CDIR();
void AVL_LOAD_FILE(char *fn);
int AVL_PROMPT(short lno, short col,char *s, short max);
void AVL_SHIFT_LEFT(char *s, short pos);
void AVL_REPLACE(int no);
void AVL_WRITE_OUT();
void AVL_INSERT_CODES();
void AVL_CLEAR_BLOCK();
void AVL_MAKE_BLOCK();
void AVL_SCROLL();
void AVL_EXIT();
void AVL_RENUMBER(AVL_LINE_PTR from, int c, int n);
void AVL_LINE_INSERT(AVL_LINE_PTR this,AVL_LINE_PTR  *at);
AVL_LINE_PTR AVL_MAKE_LINE(char *l, int lno);

void AVL_SAVE();
void AVL_EDIT_DEL_LINE(int no);
void AVL_READ_IN();
void AVL_UPDATE_SCREEN();
void AVL_UPDATE_CURSOR();
void AVL_EDIT_DEL_LEFT(int no);
void AVL_EDIT_DEL_RIGHT(int no);
void AVL_CURSOR_HOME();
void AVL_CURSOR_END();
void AVL_CURSOR_PGUP();
void AVL_CURSOR_PGDN();
void AVL_CURSOR_LEFT(int no);
void AVL_CURSOR_RIGHT(int no);
void AVL_CURSOR_UP(int no);
void AVL_CURSOR_DOWN(int no);
void AVL_EDIT_ENTER();
void AVL_EDIT_COPY(int no);
void AVL_HSCROLL();
void AVL_UPDATE_STATUS_LINE();
void AVL_ENVIRONMENT(int key);
void AVL_INIT_WINDOW(AVL_EDIT_WINDOW_PTR w, AVL_LINE_PTR first);
char *AVL_MAKE_TXT_LINE();
void AVL_UPDATE_LINE();
void AVL_EDIT_INSERT(int ch);
void AVL_EDIT(char mode);
void AVL_RUN();
void AVL_WINDOW();
void AVL_COMPILE();
char *AVL_BORDER(char *s,short r1, short c1, short r2, short c2,short bk, short co);

AVL_WIN_PTR AVL_MAKE_WINDOW(char *s,short r1,short c1,short r2,short c2,short bk, short co);
void AVL_DEL_WINDOW(AVL_WIN_PTR w);
void AVL_GOTO();

