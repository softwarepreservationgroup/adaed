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

/* Externs.h */

#include <\gwada\env\ed.h>


/* Run time options */
extern char avl_monitor_on;
extern short avl_heap;
extern short avl_pgm_stack;
extern short avl_task_stack;
extern short avl_scheduller;
extern char  avl_trace_opts[6];


extern char current_file_name[];
extern int avl_cur_source;
extern int avl_cur_tool;
extern int avl_start_pos;
extern int avl_cur_unit;
extern int avl_rows;
extern int avl_size;
extern int avl_ctrl_c;
extern char do_not_realy_exit;
extern int	avl_open_error_file;
extern AVL_LINE_PTR avl_block_first_line2;
extern AVL_LINE_PTR avl_block_last_line2;
extern int avl_block_first_col2;
extern int avl_block_last_col2;
extern AVL_LINE_PTR avl_block_first_line;
extern AVL_LINE_PTR avl_block_last_line;
extern int avl_block_first_col;
extern int avl_block_last_col;
extern AVL_EDIT_WINDOW avl_windows[];
extern int avl_window;
extern int avl_nwindows;
extern int avl_hscroll_on;
extern int avl_tab_size;
extern AVL_LINE_PTR avl_line_temp;
extern char avl_blank_line[];
extern char *avl_border;
extern char avl_message[];
extern char avl_curr_directory[];
extern char avl_dir_sources[];
extern char avl_dir_library[];
extern int avl_no_up_corner;
extern short avl_txt_color;
extern short avl_txt_bk_color;
extern short avl_wnd_title;
extern short avl_wnd_color;
extern short avl_wnd_bk_color;
extern short avl_que_color;
extern short avl_que_bk_color;
extern short avl_pro_color;
extern short avl_pro_bk_color;
extern short avl_men_ready;
extern short avl_men_letter;
extern short avl_men_word;
extern short avl_men_bk_color;
extern short avl_err_color;
extern short avl_sta_color;
extern short avl_sta_bk_color;
extern short avl_msg_bk_color;
extern short avl_msg_color;

extern AVL_EDIT_WINDOW_PTR avl_w;
extern AVL_WIN_PTR avl_win1;

extern char avl_find_txt[];
extern char avl_replace_txt[];

extern int avl_cur_menu;

extern int avl_cur_smenu[];

extern AVL_COLOR_CHOICES avl_cor_choices[];
extern int avl_cor_select;
extern int avl_cor_bk_sel;
extern int avl_cor_txt_sel;

extern char avl_version_mark[];

extern short avl_codes[];
extern short avl_mat_code[8][40];
extern short avl_codes_row;
extern short avl_codes_col;


extern AVL_WIN_PTR avl_env_win;

extern AVL_TIT avl_menu[];


extern AVL_STIT avl_smenu[];

	
