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

#include <graph.h>

#define GWAda_Version       "IV.1"
#define AVL_MAX_GEN_PARS    6
#define AVL_TOOL_FNAME		"avltool.mnu"
#define AVL_TOOL_MAX		550
#define AVL_TOOL_MAX_MENU	40
#define AVL_MAX_UNITS       350
#define AVL_MAX_NAME_SIZE   60
#define AVL_MENU_ITEMS      10
#define AVL_MAX_FILE_NAME   128
#define AVL_MAX_WINDOWS     8
#define AVL_MAX_LINEL       256
#define AVL_MAX_COLORS      16
#define AVL_FN_OPTIONS      "GWAda.opt"
#define AVL_DEFAULT_LIB     "MyLib"
#define AVL_TEXT_COLOR      15  /* White */
#define AVL_TEXT_BK_COLOR   1   /* Blue */
#define AVL_WIND_COLOR      15  /* White */
#define AVL_WIND_BK_COLOR   7   /* Gray */
#define AVL_WIND_TITLE      11  /* Light blue */
#define AVL_QUES_COLOR      1   /* Blue */
#define AVL_QUES_BK_COLOR   7   /* Gray */
#define AVL_PROM_COLOR      15  /* White */
#define AVL_PROM_BK_COLOR   0   /* Black */
#define AVL_MENU_READY      4   /* Red */
#define AVL_MENU_LETTER     1   /* Blue */
#define AVL_MENU_WORD       9   /* light blue */
#define AVL_MENU_BK_COLOR   7   /* Gray */
#define AVL_ERRO_COLOR      4   /* red */
#define AVL_STAT_BK_COLOR   AVL_TEXT_BK_COLOR
#define AVL_STAT_COLOR      11
#define AVL_MESG_COLOR      1
#define AVL_MESG_BK_COLOR   4
#define AVL_NO_COLORS_OPTS  18
#define avl_video_base_address  0xB8000
#define AVL_MAP(r,c)  ((short *) (avl_video_base_address + (r - 1) * 160 + (c - 1) * 2))
#define AVL_PUT(ch,r,c,bk,cor) *AVL_MAP(r,c) = ((char) ch) | ((short) (((bk << 4) | cor) << 8))
#define AVL_ATT(r,c) ((char) ((AVL_MAP(r,c)) >> 8))
#define AVL_CH(r,c)  ((char) *AVL_MAP(r,c))


typedef	struct {
		char st;
		char name[AVL_MAX_NAME_SIZE + 1];
		} *AVL_BIND_PTR, AVL_BIND_SIZE;



typedef struct {
		char status;
		char is_main;
		char cdate[5];
		char fu[AVL_MAX_NAME_SIZE + 1];
		} AVL_UNIT_DATA;

typedef struct {
	AVL_UNIT_DATA s[AVL_MAX_UNITS];
	int ns; 
	} *AVL_UNITDT_PTR, AVL_UNITDT;

typedef struct {
	char sources[AVL_MAX_NAME_SIZE + 1];
	char library[AVL_MAX_NAME_SIZE + 1];
	int tabsize;

	short avl_heap;
	short avl_pgm_stack;
	short avl_task_stack;
	short avl_scheduller;
	char  avl_trace_opts[6];
		
	short avl_txt_color;
	short avl_txt_bk_color;
	short avl_wnd_color;
	short avl_wnd_title;
	short avl_wnd_bk_color;
	short avl_que_color;
	short avl_que_bk_color;
	short avl_pro_color;
	short avl_pro_bk_color;
	short avl_men_ready;
	short avl_men_letter;
	short avl_men_word;
	short avl_men_bk_color;
	short avl_err_color;
	short avl_sta_color;
	short avl_sta_bk_color;
	short avl_msg_color;
	short avl_msg_bk_color;
	char  avl_monitor_on;
	} AVL_OPTIONS;

typedef struct line_node {
	char line[AVL_MAX_LINEL + 1];
	int  line_no;
	char line_attrib;
	struct line_node *previous;
	struct line_node *next;
	} AVL_LINE_SIZE, *AVL_LINE_PTR;

typedef struct {
	char video[4000];
	long bk;
	short co;
	short r1, c1, r2, c2;
	struct rccoord pos;
	} AVL_WIN_SIZE, *AVL_WIN_PTR;

typedef void (*PROC_NAME)();

typedef struct node {
	char file_name[AVL_MAX_FILE_NAME+1];
	long buffer_size;
	short r1, c1,  /*  upper-left corner of window  */
	      r2, c2;  /*  lower-right corner of window */
	int txt_col;
	int scr_col;
	int scr_row;
	int offset;
	char no_status;
	char changed;
	int tabsize;
	int no_errors;
	int line_no; /*  where error was found  */
	int txt_pos; /*  where editing and scanning is in  */
	AVL_LINE_PTR head;
	AVL_LINE_PTR current_line;
	char expanded_line[1025];
	char edit_mode;
	AVL_WIN_SIZE sw;
	struct node *avl_w;
	AVL_WIN_PTR avl_win1;
	PROC_NAME fix;
	PROC_NAME insert;
	PROC_NAME options;
	PROC_NAME delete;
	PROC_NAME error;
	struct node *previous, *next;
	}  AVL_EDIT_WINDOW, *AVL_EDIT_WINDOW_PTR;

typedef struct {
	short r, c;
	char *tit;
	} AVL_TIT;


typedef struct {
	short ret_edit;
	char *sm;
	PROC_NAME proc;
	} AVL_SMENU_SEL;
	
typedef struct {
	int r, c;
	AVL_SMENU_SEL s[23];
	} AVL_STIT;


typedef struct {
	short isbk;
	char *where;
	short *who;
	} AVL_COLOR_CHOICES;


typedef struct {
	char menu_item[AVL_TOOL_MAX_MENU+1];
	char source[AVL_MAX_FILE_NAME+1]; 
	} *AVL_MENU_PTR, AVL_MENU_SIZE;

typedef struct {
	AVL_MENU_SIZE menu[AVL_TOOL_MAX];
	int no_menu;
	} *AVL_TOOL_PTR, AVL_TOOL_SIZE;


typedef struct {
	char  type; /* I = generic unit S = Source code ... */
	char  pname  [AVL_MAX_LINEL + 1];
	char  help   [AVL_MAX_FILE_NAME+1];
	char  with   [AVL_MAX_LINEL + 1];
	char  package[AVL_MAX_LINEL + 1];
	char  gen_pars[AVL_MAX_GEN_PARS][AVL_MAX_LINEL + 1];
	short n_pars;
	} *AVL_T_MENU_PTR, AVL_T_MENU_SIZE;

typedef struct {
	AVL_MENU_SIZE menu[AVL_TOOL_MAX];
	AVL_T_MENU_SIZE more[AVL_TOOL_MAX];
	int no_menu;
	} *AVL_T_TOOL_PTR, AVL_T_TOOL_SIZE;

typedef void (*PROC_TOOL)(AVL_T_TOOL_PTR t);

typedef struct {
	char *tool_opt;
	PROC_TOOL proc;
	} TOOL_SIZE;


typedef struct {
	char name[AVL_TOOL_MAX][13];
	int no_files;
	} AVL_SOURCE_SIZE, *AVL_SOURCE_PTR;
