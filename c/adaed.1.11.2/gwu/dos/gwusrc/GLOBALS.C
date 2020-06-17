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

/* Globals.c */

#include <\gwada\env\ed.h>


short ScreenAdr, PhyScrCols;
char RetraceMode, Aline, Tline, CtrlAttr;


/* Run time options */
char avl_monitor_on = '\0';
short avl_heap;
short avl_pgm_stack;
short avl_task_stack;
short avl_scheduller;
char  avl_trace_opts[6];

int avl_cur_source = 0;
int avl_cur_tool = 0;
int avl_cur_unit = 0;
int avl_rows     = 0;
int avl_size     = 0;
int avl_ctrl_c = 0;
char current_file_name[200];
char do_not_realy_exit = 0;
int	avl_open_error_file = 0;
AVL_LINE_PTR avl_block_first_line2 = NULL;
AVL_LINE_PTR avl_block_last_line2;
int avl_block_first_col2;
int avl_block_last_col2;
AVL_LINE_PTR avl_block_first_line = NULL;
AVL_LINE_PTR avl_block_last_line;
int avl_block_first_col;
int avl_block_last_col;
AVL_EDIT_WINDOW avl_windows[AVL_MAX_WINDOWS+1];
int avl_window = 0;
int avl_nwindows = 0;
int avl_hscroll_on = 0;
int avl_tab_size = 8;
AVL_LINE_PTR avl_line_temp = NULL;
char avl_blank_line[81];
char *avl_border = "Ú¿ÀÙÄ³Ã´";
char avl_message[80];
char avl_curr_directory[128];
char avl_dir_sources[61];
char avl_dir_library[61];
char avl_find_txt[AVL_MAX_LINEL+10];
char avl_replace_txt[AVL_MAX_LINEL+10];
int avl_no_up_corner = 0;
short avl_txt_color;
short avl_txt_bk_color;
short avl_wnd_color;
short avl_wnd_title = AVL_WIND_TITLE;
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
short avl_msg_bk_color;
short avl_msg_color;

AVL_EDIT_WINDOW_PTR avl_w;
AVL_WIN_PTR avl_win1;

char avl_version_mark[30];

AVL_COLOR_CHOICES avl_cor_choices[AVL_NO_COLORS_OPTS] = {
			 {0,"Status             ",&avl_sta_color}
			,{1,"Status Background  ",&avl_sta_bk_color}
			,{0,"Editing Text       ",&avl_txt_color}
			,{1,"Editing Background ",&avl_txt_bk_color}
			,{0,"Window Title       ",&avl_wnd_title}
			,{0,"Window Text        ",&avl_wnd_color}
			,{1,"Window Background  ",&avl_wnd_bk_color}
			,{0,"Question Text      ",&avl_que_color}
			,{1,"Question Background",&avl_que_bk_color}
			,{0,"Prompt Text        ",&avl_pro_color}
			,{1,"Prompt Background  ",&avl_pro_bk_color}
			,{0,"Menu Selection     ",&avl_men_ready}
			,{0,"Menu Letter Option ",&avl_men_letter}
			,{0,"Menu Word          ",&avl_men_word}
			,{1,"Menu Background    ",&avl_men_bk_color}
			,{0,"Error Message      ",&avl_err_color}
			,{0,"Message Text       ",&avl_msg_color}
			,{1,"Message Background ",&avl_msg_bk_color}
			};

int avl_cor_select = 0;
int avl_cor_bk_sel = 0;
int avl_cor_txt_sel = 0;

short avl_codes[157] = {  1,  2,  3,  4,  5,  6,  7,  8, 11, 12
                     , 13, 14, 15, 16, 17, 18, 19, 20, 21, 22
                     , 23, 24, 25, 26, 27, 28, 29, 30, 31,127
                     ,128,129,130,131,132,133,134,135,136,137
                     ,138,139,140,141,142,143,144,145,146,147
                     ,148,149,150,151,152,153,154,155,156,157
                     ,158,159,160,161,162,163,164,165,166,167
                     ,168,169,170,171,172,173,174,175,176,177
                     ,178,179,180,181,182,183,184,185,186,187
                     ,188,189,190,191,192,193,194,195,196,197
                     ,198,199,200,201,202,203,204,205,206,207
                     ,208,209,210,211,212,213,214,215,216,217
                     ,218,219,220,221,222,223,224,225,226,227
                     ,228,229,230,231,232,233,234,235,236,237
                     ,238,239,240,241,242,243,244,245,246,247
                     ,248,249,250,251,252,253,254};

short avl_mat_code[8][40];
short avl_codes_row = 0;
short avl_codes_col = 0;


int avl_cur_menu = 0;

int avl_cur_smenu[10] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};


AVL_WIN_PTR avl_env_win = NULL;

AVL_TIT avl_menu[AVL_MENU_ITEMS] = {
				   {1, 3,"File"}
                  ,{1, 9,"Edit"}
                  ,{1,15,"Compile"}
                  ,{1,24,"Bind"}
                  ,{1,30,"Run"}
                  ,{1,35,"Window"}
                  ,{1,43,"Options"}
                  ,{1,52,"Ada"}
                  ,{1,57,"Help"}
                  ,{1,63,avl_version_mark}
                  }; 

int avl_start_pos = 0;

void AVL_HELP_LRM()
{
	char *opts[17];
	int nopt = 0;
	int cmd_ret_code;
	opts[nopt++] = "AdaHelp";
    opts[nopt++] = NULL;
/*	if (AVL_EX_UNIT("ADAHELP", opts))  ;
*/
	system("adahelp");
}

void AVL_DO_WINDOW();
	
AVL_STIT avl_smenu[9]  =  { /* File */
						   {1, 2,{{1,"Open   ",AVL_OPEN}
						         ,{1,"Close  ",AVL_CLOSE}
						         ,{0,"Save   ",AVL_SAVE}
						         ,{0,"Save As",AVL_SAVE_AS}
						         ,{0,"Info   ",AVL_ABOUT_GWADA}
						         ,{0,"DOS Cmd",AVL_DOS_CMD}
						         ,{0,"Quit  ",AVL_EXIT}
						         ,{0,"",NULL}}}
						  ,{1, 8,{{1,"Find        ",AVL_PROCESS_FIND}
						         ,{1,"Replace     ",AVL_PROCESS_REPLACE}
						         ,{1,"Insert File ",AVL_READ_IN}
						         ,{1,"Delete Line ",AVL_DO_DEL_LINE}
						         ,{1,"Special Code",AVL_INSERT_CODES}
						         ,{1,"Mark Block  ",AVL_GUIDE_MBLOCK}
/*						         ,{1,"Remove Block",AVL_REMOVE_BLOCK}
*/						         ,{1,"Write Block ",AVL_WRITE_OUT}
						         ,{1,"Copy Block ",AVL_PROCESS_COPY}
						         ,{0,"", NULL}}}
						  ,{1,14,{{0,"Current Unit",AVL_COMPILE_CURRENT} /* Compile */
/*						         ,{0,"Bottom Up",AVL_COMP_ORDER}  
*/						         ,{0,"Select Unit",AVL_COMPILE} 
						         ,{0,"", NULL}}}
						  ,{1,23,{{1,"",AVL_BIND}}}    /* Bind */
						  ,{1,29,{{1,"",AVL_RUN}}} /* Run */
						  ,{1,33,{{1,"",AVL_DO_WINDOW}}} /* Window */
						  ,{1,42,{{0,"ASCII Table      ",AVL_SHOW_ASCII} /* Options */
						         ,{0,"Colors           ",AVL_SET_COLORS} 
						         ,{0,"Directories      ",AVL_SET_DIRECTORIES}
						         ,{0,"Identation Length",AVL_SET_TAB}
						         ,{0,"List Library     ",AVL_LIST_LIB}
						         ,{0,"New Library      ",AVL_NEW_LIB}
						         ,{0,"Parameters (Run) ",AVL_SET_PARAMETERS}
						         ,{0,"Read Options     ",AVL_READ_OPTIONS}
						         ,{0,"Set Defaults     ",AVL_DEFAULT_OPTIONS}
						         ,{0,"Write Options   ",AVL_SAVE_OPTIONS}
						         ,{0,"",NULL}}}
						  ,{1,51,{  /*  Ada  */
						          {1,"Ada Program" ,AVL_TEMPLATE_ADA_PGM}
						         ,{1,"Access type" ,AVL_TEMPLATE_ADA_ACCESS}
						         ,{1,"Vector type" ,AVL_TEMPLATE_ADA_VECTOR}
						         ,{1,"Matrix type" ,AVL_TEMPLATE_ADA_MATRIX}
						         ,{1,"Record type" ,AVL_TEMPLATE_ADA_RECORD}
						         ,{1,"Loop       " ,AVL_TEMPLATE_ADA_LOOP}
						         ,{1,"While      " ,AVL_TEMPLATE_ADA_WHILE}
						         ,{1,"Case       " ,AVL_TEMPLATE_ADA_CASE}
						         ,{1,"If         " ,AVL_TEMPLATE_ADA_IF}
						         ,{1,"Function   " ,AVL_TEMPLATE_ADA_FUNCTION}
						         ,{1,"Procedure  " ,AVL_TEMPLATE_ADA_PROCEDURE}
						         ,{1,"Task       " ,AVL_TEMPLATE_ADA_TASK}
						         ,{1,"Server     " ,AVL_TEMPLATE_ADA_SERVER}
						         ,{1,"Package   " ,AVL_TEMPLATE_ADA_PACKAGE}
/*						         ,{1,"Tool Box  " ,AVL_CALL_TOOL}
*/						         ,{0,"",NULL}}}
						  ,{1,46,{  /*  Help  */
						          {0,"Help Facility",AVL_HELP_LRM}    
						         ,{0,"Fast Help ",AVL_HOT_KEYS}    
						         ,{0,"",NULL}}}
						  };




	
