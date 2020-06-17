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

/*  Tool.c  */

#include <externs.h>


char avl_from_which_file[AVL_MAX_NAME_SIZE + 1];

#define AVL_TOOL_MNU_ITEMS  6


int AVL_MOUNT_TOOL(char *local);

void AVL_TOOL_REFER_PCK(AVL_T_TOOL_PTR t)
{
	char this[AVL_MAX_LINEL + 1];
	short i;
	sprintf(this,"WITH %s;", t -> more[avl_cur_tool].with);
	for(i = 0; i < strlen(this); ++i)
		AVL_EDIT_INSERT(this[i]);
	AVL_CURSOR_END();
	AVL_EDIT_ENTER();
}

void AVL_TOOL_REFER_PCK_USE(AVL_T_TOOL_PTR t)
{
	char this[AVL_MAX_LINEL + 1];
	short i;
	sprintf(this,"WITH %s; USE %s;"
		, t -> more[avl_cur_tool].with
		, t -> more[avl_cur_tool].with);
	for(i = 0; i < strlen(this); ++i)
		AVL_EDIT_INSERT(this[i]);
	AVL_CURSOR_END();
	AVL_EDIT_ENTER();
}

void AVL_TOOL_INSERT_CODE(AVL_T_TOOL_PTR t)
{
	if (AVL_MOUNT_TOOL(t -> menu[avl_cur_tool].source));
}

void AVL_TOOL_IDENT(int l)
{
	int i;
	for(i = 0; i < l; ++i)
		AVL_EDIT_INSERT(' ');
}

static short avl_tool_in_error = 0;

static char avl_tool_new_package[AVL_MAX_LINEL + 1];

void AVL_TOOL_INSTANTIATE_PCK(AVL_T_TOOL_PTR t)
{
	char *msg = " Enter the New Package Name ";
	char gen_pars[AVL_MAX_GEN_PARS][AVL_MAX_LINEL + 1];
	char msg2[300];
	AVL_WIN_PTR m = NULL;
	char this[AVL_MAX_LINEL + 1];
	char this2[AVL_MAX_LINEL + 1];
	short i, j;
	int where, where2;
	avl_tool_in_error = 1;	
	where = avl_windows[avl_window].txt_col;
	m = AVL_MAKE_WINDOW(msg,7,4,9,5+62,avl_wnd_bk_color,avl_wnd_color);
	if (AVL_PROMPT(1,1,this2,60)) {
		AVL_DEL_WINDOW(m);
		return;
		}
	AVL_DEL_WINDOW(m);
	sprintf(avl_tool_new_package," USE %s;", this2);
	sprintf(msg2," Parameter Passing for Package %s ", this2);
	msg2[72] = '\0';
	m = AVL_MAKE_WINDOW(msg2,4,3,9+t -> more[avl_cur_tool].n_pars,5+73,avl_wnd_bk_color,avl_wnd_color);
	for(i = 0; i < t -> more[avl_cur_tool].n_pars; ++i)  {
again:
		_settextposition(i+1,1);
		sprintf(msg2," Enter %d parameter (%s)\n",i+1,t -> more[avl_cur_tool].gen_pars[i]);
		msg2[72] = '\0';
		_outtext(msg2);
		gen_pars[i][0] = '\0';
		if (AVL_PROMPT(i+2,3,gen_pars[i],60)) {
			AVL_DEL_WINDOW(m);
			return;
			}
		sprintf(msg2,"Is parameter no. %d ok (Y/N) ?",i+1);
		if (AVL_QUESTION(msg2) == 'N') goto again;
		_settextposition(i+1,1);
		t -> more[avl_cur_tool].gen_pars[i][0] = toupper(t -> more[avl_cur_tool].gen_pars[i][0]);
		sprintf(msg2," %d %s: %-69s"
			, i+1
			, t -> more[avl_cur_tool].gen_pars[i]
			, gen_pars[i]);
		msg2[72] = '\0';
		_outtext(msg2);
		_settextposition(i+2,1);
		sprintf(msg2,"%-72s"," ");
		_outtext(msg2);
		}
	AVL_DEL_WINDOW(m);
	sprintf(this,"WITH %s;", t -> more[avl_cur_tool].with);
	AVL_TOOL_IDENT(where);
	for(i = 0; i < strlen(this); ++i)
		AVL_EDIT_INSERT(this[i]);
	AVL_CURSOR_END();
	AVL_EDIT_ENTER();
	AVL_TOOL_IDENT(where);
	sprintf(this,"PACKAGE %s IS NEW %s(", this2, t -> more[avl_cur_tool].pname);
	for(i = 0; i < strlen(this); ++i)
		AVL_EDIT_INSERT(this[i]);
	where2 = avl_windows[avl_window].txt_col;
	for(i = 0; i < t -> more[avl_cur_tool].n_pars; ++i)  {
		if (i != 0)  {
			avl_windows[avl_window].txt_col = 0;
			avl_windows[avl_window].current_line -> line[0] = '\0';
			AVL_TOOL_IDENT(where2-1);
			AVL_EDIT_INSERT(',');
			}
		for(j = 0; j < strlen(gen_pars[i]); ++j)
			AVL_EDIT_INSERT(gen_pars[i][j]);
		if ((i + 1) == t -> more[avl_cur_tool].n_pars) {
			AVL_EDIT_INSERT(')');
			AVL_EDIT_INSERT(';');
			}
		else {
			AVL_CURSOR_END();
			AVL_EDIT_ENTER();
			}
		}
	avl_tool_in_error = 0;	
}


void AVL_TOOL_INSTANTIATE_PCK_USE(AVL_T_TOOL_PTR t)
{
	int where, i;
	where = avl_windows[avl_window].txt_col;
	AVL_TOOL_INSTANTIATE_PCK(t);
	if (avl_tool_in_error) 
		return;
	AVL_TOOL_IDENT(where);
	for(i = 0; i < strlen(avl_tool_new_package); ++i)
		AVL_EDIT_INSERT(avl_tool_new_package[i]);
	AVL_CURSOR_END();
	AVL_EDIT_ENTER();
}

void AVL_TOOL_HELP(AVL_T_TOOL_PTR t)
{
	AVL_HELP(t -> more[avl_cur_tool].help);
}



TOOL_SIZE avl_tool_mnu[AVL_TOOL_MNU_ITEMS] = 
	{{"A Refer Package            ",AVL_TOOL_REFER_PCK}
	,{"B Refer Package & USE      ",AVL_TOOL_REFER_PCK_USE}
	,{"C Insert Source Code       ",AVL_TOOL_INSERT_CODE}
	,{"D Instantiate Package      ",AVL_TOOL_INSTANTIATE_PCK}
	,{"E Instantiate Package & USE",AVL_TOOL_INSTANTIATE_PCK_USE}
	,{"F Help                   "  , AVL_TOOL_HELP}
	};
	


void AVL_GET_TOOLS(AVL_T_TOOL_PTR tool)
{
	FILE *fp, *fopen();
	char s[200], s2[200];
	char *pl;
	int i, j, k, l;
	tool -> no_menu = 0;
	pl = getenv("ADAED");
	if (pl == NULL)
		pl=getenv("adaed");
	if (pl != NULL)  
		sprintf(s,"%s%c%s",pl,'\\',AVL_TOOL_FNAME);
	else  {
		AVL_ERROR("Set the ADAED environment variable first (see readme.gw).");
		return;
		}		
	fp = fopen(s,"r");
	if (fp == NULL)  {
		sprintf(s2,"Can't access file \'%s\'", s);
		AVL_ERROR(s2);
		return;
		}
	while ( fgets(s2,200,fp) != NULL)  {
		if (tool -> no_menu >= AVL_TOOL_MAX)  {
			sprintf(s2,"Can't process more than %d tools. Rest ignored!", AVL_TOOL_MAX);
			AVL_ERROR(s2);
			break;
			}
		for(i = 0; s2[i] != '@' && s2[i] != '$' && i < AVL_TOOL_MAX_MENU; ++i)
			tool -> menu[tool -> no_menu].menu_item[i] = s2[i];
		tool -> menu[tool -> no_menu].menu_item[i] = '\0';
		while (s2[i] != '$') ++i;
		++i;
		for(j = 0; !((s2[i] == '\0') || (s2[i] == ' ') || (s2[i] == '\n')) && j < AVL_MAX_FILE_NAME; ++j, ++i)
			tool -> menu[tool -> no_menu].source[j] = s2[i];
		tool -> menu[tool -> no_menu].source[j] = '\0';
		while (s2[i] != '$') ++i;
		++i;
		for(j = 0; !((s2[i] == '\0') || (s2[i] == ' ') || (s2[i] == '\n')) && j < AVL_MAX_LINEL; ++j, ++i)
			tool -> more[tool -> no_menu].help[j] = s2[i];
		tool -> more[tool -> no_menu].help[j] = '\0';
		while (s2[i] != '$') ++i;
		++i;
		tool -> more[tool -> no_menu].type = s2[i];
		/* Get the package name */
		while (s2[i] != '$') ++i;
		++i;
		for(j = 0; !((s2[i] == ' ') || (s2[i] == '\0') || (s2[i] == '\n')) && j < AVL_MAX_LINEL; ++j, ++i)
			tool -> more[tool -> no_menu].with[j] = s2[i];
		tool -> more[tool -> no_menu].with[j] = '\0';
		if (tool -> more[tool -> no_menu].type == 'I')  {
			/* Get the full package name  */
			while (s2[i] != '$') ++i;
			++i;
			for(j = 0; !((s2[i] == ' ') || (s2[i] == '\0') || (s2[i] == '\n')) && j < AVL_MAX_LINEL; ++j, ++i)
				tool -> more[tool -> no_menu].pname[j] = s2[i];
			tool -> more[tool -> no_menu].pname[j] = '\0';
			while (s2[i] != '$') ++i;
			++i;
			/* Now get the number of parameters */
			tool -> more[tool -> no_menu].n_pars = 0;
			while(isdigit(s2[i])) 
				tool -> more[tool -> no_menu].n_pars = 
					tool -> more[tool -> no_menu].n_pars * 10 
					                                     + (s2[i++] - '0');
			/* Get an user message to prompt for each parameter */
			for(k = 0; k < tool -> more[tool -> no_menu].n_pars && 
			   fgets(tool -> more[tool -> no_menu].gen_pars[k],200,fp) != NULL; 
			   ++k)  {
				l = strlen(tool -> more[tool -> no_menu].gen_pars[k]); 
				if (tool -> more[tool -> no_menu].gen_pars[k][l - 1] == '\n')
					tool -> more[tool -> no_menu].gen_pars[k][l - 1] = '\0'; 
				}
			if (k != tool -> more[tool -> no_menu].n_pars)  {
				sprintf(s,"Invalid package parameter descriptor : %s", 
					tool -> menu[tool -> no_menu].menu_item);
				AVL_ERROR(s);
				sprintf(s,"Process suspended while working with item no. %d", tool -> no_menu);
				AVL_ERROR(s);
				fclose(fp);
				return;
				}
			}				
		tool -> no_menu += 1;
		}
	fclose(fp);
}


void AVL_SHOW_TOOL(AVL_T_TOOL_PTR w, int cols)
{
	short i, first, last, k;	
	short co;
	k = avl_cur_tool / 9;
	first = k * 9;
	last = first + 8;
	if (last >= avl_size) 
		last = avl_size - 1;
	_settextposition(1,1);
	_settextcursor(0x2000);  /*  turn cursor off */
	_outtext("\n\n\n\n\n\n\n\n\n\n\n\n");
	k = 1;
	for(i = first; i <= last; ++i)  {
		_settextposition(k,1);
		if (i == avl_cur_tool)
			co = _settextcolor(avl_men_ready);
		else
			co = _settextcolor(avl_men_letter);
		_outtext(w -> menu[i].menu_item);
		++k;
		}
	_settextcolor(co);
}
                
int AVL_TOOL(AVL_T_TOOL_PTR w, int r)
{
        int x, i ;
        int no = 0;
        int ch, cols = 0, rows;
		short co;
        char msg[80];
        AVL_WIN_PTR hw = NULL, hw2 = NULL, hw3 = NULL;
		_settextcursor(0x2000);  /*  turn cursor off */
        if (r < 10)
        	rows = r;
        else 
        	rows = 9;
        avl_size = r;
        /* Compute the maximum window's length */
        for(x = 0; x < r; ++x)  
        	if ((no = strlen(w -> menu[x].menu_item)) > cols)
        		cols = no;

		cols += 2;
		hw2 = AVL_MAKE_WINDOW("",15,47,15+9,47+29,avl_wnd_bk_color,avl_wnd_color);
		_outtext(" Use the arrow keys to\n");
		_outtext(" select a tool.\n");
		_outtext("\n");
		_outtext(" Press  the related action\n");
		_outtext(" letter to cause the\n");
		_outtext(" desired effect.\n");
		_outtext("\n");
		_outtext(" Press ESC to  cancel.");
		hw3 = AVL_MAKE_WINDOW(" Actions "
			,3+rows+3,avl_menu[2].c
			,3+rows+3+AVL_TOOL_MNU_ITEMS+1,avl_menu[2].c+3+strlen(avl_tool_mnu[0].tool_opt+2)
			,avl_wnd_bk_color,avl_wnd_color);
		for(i = 0; i < AVL_TOOL_MNU_ITEMS; ++i)  {
			_settextposition(i+1,1);
			co = _settextcolor(avl_men_ready);
			sprintf(msg,"%c",avl_tool_mnu[i].tool_opt[0]);
			_outtext(msg);
			co = _settextcolor(avl_men_letter);
			sprintf(msg,"%s", &avl_tool_mnu[i].tool_opt[1]);
			_outtext(msg);
			}
		hw = AVL_MAKE_WINDOW(" Tool Box ",3,avl_menu[2].c,3+rows+1,avl_menu[2].c+cols+5,avl_wnd_bk_color,avl_wnd_color);

		while ( 1 )  {
			AVL_SHOW_TOOL(w,cols-2);
			ch = getch();
			ch = toupper(ch);
			if (ch >= 'A' && ch <= ('A' + AVL_TOOL_MNU_ITEMS - 1)) {
				if (ch == 'F')  {				
					avl_tool_mnu[ch - 'A'].proc(w);
					continue;
					}
				AVL_DEL_WINDOW(hw);
				AVL_DEL_WINDOW(hw3);
				AVL_DEL_WINDOW(hw2);
				avl_tool_mnu[ch - 'A'].proc(w);
				return 1;
				}
			else {
				if (ch == 0 || ch == 0xE0) {
					ch = getch();
                    switch( ch ) {
                    	case 72 : /* Up */ 
                    		if (--avl_cur_tool < 0)  
								avl_cur_tool = avl_size - 1;
                    		break;
                    	case 80 : /* Down */ 
                    		if (++avl_cur_tool >= avl_size)
                    			avl_cur_tool = 0;
                    		break;
                    	default : putchar(7); break;
                    	}
                    continue;
                    }
				if (ch == 27)  {
					AVL_DEL_WINDOW(hw);
					AVL_DEL_WINDOW(hw3);
					AVL_DEL_WINDOW(hw2);
					return -1;
					}
				putch(7);
				continue;
				}
			}
}			
		
int AVL_MOUNT_TOOL(char *local)
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, head = NULL;
	AVL_WIN_PTR m1;
	char fname[100];
	int n1, n2, x;
	char msg2[80];
	char *pl;
	pl = getenv("ADAED");
	if (pl == NULL)
		pl=getenv("adaed");
	if (pl != NULL)  
		sprintf(fname,"%s%c%s",pl,'\\',local);
	else  {
		AVL_ERROR("Set the ADAED environment variable first (see readme.gw).");
		return;
		}		

	w = &avl_windows[avl_window];
	n1 = w -> txt_col;
	avl_start_pos = AVL_COL() - 1;
	if (avl_start_pos < 0) avl_start_pos = 0;
	w -> txt_col = 0;
	temp = AVL_MAKE_LINE("### DUMMY ###",0);
    AVL_LINE_INSERT(temp,&head);
    x = AVL_LOAD(fname, &head);
	avl_start_pos = 0;

	if (!x) {
		sprintf(msg2,"Can't open \'%s\'", fname);
		AVL_ERROR(msg2);
		w -> txt_col = n1;
		return 0;
		}
	else {
		AVL_CLEAR_BLOCK();
		avl_block_first_line = head -> next;
		avl_block_last_line = head -> previous;
		avl_block_first_col = 0;
		avl_block_last_col = strlen(head -> previous -> line) - 1;
		if (avl_block_last_col < 0) avl_block_last_col = 0;
		AVL_EDIT_COPY(1);
		AVL_CLEAR_BLOCK();
		return 1;
		}
}


void AVL_CALL_TOOL()
{
	AVL_T_TOOL_SIZE t;
	int n;
	char s[100];
	char this[200];
	AVL_GET_TOOLS(&t);
	n = AVL_TOOL(&t,t.no_menu);
	if (n >= 0)  {
		sprintf(s,"%s Inserted", t.menu[avl_cur_tool].menu_item);
		strcpy(avl_message,s);
		}
}

