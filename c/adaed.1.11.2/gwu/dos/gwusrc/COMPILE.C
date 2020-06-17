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

/*  Compile.c  */

#include <externs.h>

extern void AVL_MAKE_UNIT_DATE(AVL_EDIT_WINDOW_PTR w);

char *AVL_RUN_SELECT(AVL_UNITDT_PTR w)
{
	AVL_WIN_PTR temp = NULL, temp2 = NULL;
	short rows, cols;
	int i, j, k, n = 0, nc = 0;

	char msg[50];
	char *ru;

	static AVL_BIND_SIZE r[50];
	AVL_BIND_SIZE c[50];
	 
	/* Pick obsolete units for compilation */
	for(i = 0; i < w -> ns && n < 50; ++i)  
		if (w -> s[i].status == 'A' && w -> s[i].is_main == ' ' &&
		    w -> s[i].fu[0] == 'b')   {
			AVL_MAKE_BNAME(w -> s[i].fu, msg);
			ru = msg;
			j = strlen(ru);
			if (j > 10 ) {
				j = j - 10;
				if (!strcmp("_idle_task",ru+j))  
					*(ru+j) = '\0';
				}
			strcpy(r[n].name,msg);
			++n;
			}				

	if (n > 0)
		i = AVL_UNIT(r,n);
	else {
		i = -1;
		AVL_ERROR("First compile any executable program unit.");
		}
	AVL_DEL_WINDOW(temp2);
	if (i >= 0)
		return r[i].name;
	return NULL;
}


int AVL_EX_UNIT(char *name, char *opts[17])
{
	int cmd_ret_code, i;
	char msg[100];
	cmd_ret_code = spawnvp(P_WAIT,name,opts);
	if (cmd_ret_code == -1)  {
		switch( errno )  {
			case E2BIG : sprintf(msg,"E2BIG"); break;
			case EINVAL: strcpy(msg,"The mode argument is invalid."); break;
			case ENOENT: sprintf(msg,"File or path name not found: %s",name); break;
			case ENOEXEC : sprintf(msg,
"The specified file is not an executable or has an inv. exec-file format: '%s'",name); break;
			case ENOMEM : strcpy(msg,
"Not enough memory is available to execute the child process."); break;
			default : sprintf(msg,"errno = %d, could not know...", errno); break;
			}
		AVL_ERROR(msg);
		return 0;
		}
	return 1;
}



int AVL_COMP_SETUP(AVL_EDIT_WINDOW_PTR w)
{
	AVL_WIN_PTR temp;
	FILE *fp, *fopen();
	struct find_t c_file;

	char lookat[AVL_MAX_FILE_NAME+1];
	short i;
	char msg[AVL_MAX_FILE_NAME+100];
	unsigned att;

	sprintf(lookat,"%s",avl_dir_library);
	i = _dos_findfirst( lookat, _A_SUBDIR, &c_file );
	if (!i)  {
		i = _dos_getfileattr(lookat,&att);
		if (att != _A_SUBDIR)  {
			sprintf(msg," Library %s does not exist! ",avl_dir_library);
			msg[77] = '\0';
			temp = AVL_MAKE_WINDOW(msg,5,9,12,71,avl_wnd_bk_color,avl_wnd_color);
			_outtext(" Your current directory contains a file that has the same\n");
			_outtext(" name of the current library. Before creating a new library\n");
			_outtext(" (Alt-O N) either change this file name or change the\n");
			_outtext(" current library name (Alt-O D).\n\n");
			_outtext("Press any key to continue...");
			i = getch();
			if (i == 0) i = getch();
			AVL_DEL_WINDOW(temp);
			return 1;
			}
		sprintf(msg,"%s\\LIB",avl_dir_library);
		fp = fopen(msg,"r");
		if (fp == NULL) {
			sprintf(msg," Library %s has been corrupted! ",avl_dir_library);
			msg[77] = '\0';
			temp = AVL_MAKE_WINDOW(msg,5,9,11,71,avl_wnd_bk_color,avl_wnd_color);
			_outtext(" Your current library has been corrupted. It is suggested\n");
			_outtext(" a new library, that is, create a new library by pressing\n");
			_outtext(" the Alt-O N keys.\n\n");
			_outtext("Press any key to continue...");
			i = getch();
			if (i == 0) i = getch();
			AVL_DEL_WINDOW(temp);
			return 1;
			}
		}
		
	if (i)  {
		sprintf(msg," Library %s does not exist! ",avl_dir_library);
		msg[77] = '\0';
		temp = AVL_MAKE_WINDOW(msg,5,9,11,71,avl_wnd_bk_color,avl_wnd_color);
		_outtext(" A library must be created before compiling any program.\n");
		_outtext(" Pressing the Alt-O N keys will create a library for you.\n");
		_outtext(" To change the current library press the Alt-O D keys.\n\n");
		_outtext("Press any key to continue...");
		i = getch();
		if (i == 0) i = getch();
		AVL_DEL_WINDOW(temp);
		return 1;
		}

    avl_blank_line[0] = '\0';
	w -> fix = AVL_FIX;
	w -> options = AVL_READ_OPTIONS;
	w -> delete = AVL_DEL_WINDOW;
	w -> error = AVL_ERROR;
	w -> line_no = 0;
	w -> no_errors = 0;
    w -> avl_win1 = NULL;
	if (w -> changed)
		AVL_SAVE();
	w -> changed = '\0';
	return 0;
}

int AVL_DO_COMPILATION(char *s, int pause)
{
	AVL_EDIT_WINDOW_PTR w;
	int lex, syntax, semantic, bind, gen, nopt = 0, n, i;
	FILE *fp, *fopen();
	AVL_WIN_PTR temp;
	char msg[100];
	char *opts[17];
	char compiling[121];
	avl_w = w = &avl_windows[avl_window];
    fp = fopen("COMPILE.CK$","w");
    fclose(fp);
	temp = AVL_MAKE_WINDOW(" N.Y.U. Ada Compiler "
		,5,1,8,80,avl_wnd_bk_color,avl_wnd_color);

	sprintf(compiling," Compiling %s, please wait ...\n", s);
	_outtext(compiling);
	opts[nopt++] = "Adacomp";
    opts[nopt++] = "-a";
    opts[nopt++] = "-s";
	opts[nopt++] = "-l";
	opts[nopt++] = avl_dir_library;
	sprintf(msg,"%s",s);
	opts[nopt++] = msg;
	opts[nopt++] = NULL;
	unlink("gwada.smm");
	if (AVL_EX_UNIT("ADACOMP", opts))  ;
	AVL_DEL_WINDOW(temp);
	avl_open_error_file = 0;
	fp = fopen("GWADA.SMM","r");
	if (fp == NULL)  
		w -> no_errors = 0;
	else {
		fscanf(fp, "%d", &w -> no_errors);
		fscanf(fp, "%d", &lex);
		fscanf(fp, "%d", &syntax);
		fscanf(fp, "%d", &semantic);
		fscanf(fp, "%d", &bind);
		fscanf(fp, "%d", &gen);
		fclose(fp);
		}

	if (w -> no_errors > 0)  {
		n = 0;
		if (lex) ++n;
		if (syntax) ++n;
		if (semantic) ++n;
		if (bind) ++n;
		if (gen) ++n;
		temp = AVL_MAKE_WINDOW(" N.Y.U. Ada Compiler - Error Summary "
			,5,1,7+n,80,avl_wnd_bk_color,avl_wnd_color);
		_settextposition(1,2);
		sprintf(msg,"  %d error%s detected:", w -> no_errors, (w -> no_errors == 1) ? "" : "s");
		_outtext(msg); 
		n = 0;
		_settextposition(2,2);
		if (lex > 0) { 
			++n;
			sprintf(msg,"        %d lexical error%s. ", lex, (lex == 1) ? "" : "s");
			_outtext(msg); 
			}
		if (syntax > 0) { 
			_settextposition(2+n++,2);
			sprintf(msg,"        %d syntactical error%s. ", syntax, (syntax == 1) ? "" : "s");
			_outtext(msg); 
			}
		if (semantic > 0) { 
			_settextposition(2+n++,2);
			sprintf(msg,"        %d semantical error%s. ", semantic, (semantic == 1) ? "" : "s");
			_outtext(msg); 
			}
		if (bind > 0) { 
			_settextposition(2+n++,2);
			sprintf(msg,"        %d binding error%s. ", bind, (bind == 1) ? "" : "s");
			_outtext(msg); 
			}
		if (gen > 0) { 
			_settextposition(2+n++,2);
			sprintf(msg,"        %d code generating error%s. ", gen, (gen == 1) ? "" : "s");
			_outtext(msg); 
			}
		if (pause) {
	 		i = getch(); if (i == 0) i = getch();
	 		}
		AVL_DEL_WINDOW(temp);
		avl_open_error_file = 1;
		lex = 1;
		}
	else  {
		temp = AVL_MAKE_WINDOW(" N.Y.U. Ada Compiler ",5,1,9,80,avl_wnd_bk_color,avl_wnd_color);
		_settextposition(1,2);
	    fp = fopen("COMPILE.CK$","r");
	    if (fp != NULL)  {
	    	sprintf(msg," Compilation failed! Check installation. ");
		    fclose(fp);
		    lex = 1;
		    }
	    else {
			sprintf(msg," Program %s compiled with success.\n\n  Press any key to continue...", s);
		 	lex = 0;
		 	}
		_outtext(msg);
		if (pause) {
	 		i = getch(); if (i == 0) i = getch();
	 		}
		AVL_DEL_WINDOW(temp);
		}
	return lex;
}					



void AVL_COMPILE_CURRENT()
{
	AVL_EDIT_WINDOW_PTR w;
	avl_w = w = &avl_windows[avl_window];
    if (AVL_COMP_SETUP(w)) return;
	strcpy(current_file_name,w -> file_name);
	if (AVL_DO_COMPILATION(w -> file_name,1));
}


/*
void AVL_COMP_ORDER()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR temp;
	AVL_WIN_PTR m1, m2;
	char *opts[19];
	int nopt = 0;
	char msg[100];
	char s[121];
	char s2[121];
	char search_space[10][128];
	static char *msgc = "GWAda - Consider which set of files? ";
	int i, j, n, n1, n2;
	int no_units, larger;
	static int first = 0;
	static char lastfn[121];
	static char fname[121];
	char compiling[121];
    FILE *fp2, *fopen();
	avl_w = w = &avl_windows[avl_window];
	n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msgc,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	if (first == 0)
		sprintf(lastfn,"%s%c*.ada",avl_dir_sources,92);
	first = 1;
	strcpy(fname,lastfn);

	if (AVL_PROMPT(1,1,fname,60))  {
		AVL_DEL_WINDOW(m1);
		return;
		}

	strcpy(lastfn,fname);
	for(i = 0, j = 0, n = 0; i < strlen(fname); ++i)  
		if (isspace(fname[i])) {
			search_space[n++][j] = '\0';
			j = 0;
			while(isspace(fname[i])) ++i;
			--i;
			}
		else
			search_space[n][j++] = fname[i];
	search_space[n++][j] = '\0';
	AVL_DEL_WINDOW(m1);
	temp = AVL_MAKE_WINDOW(" GWAda Bottom Up Parts Compiler "
		,2,1,9,80,avl_wnd_bk_color,avl_wnd_color);

	sprintf(compiling," Building compilation order for %s\n Please wait ...\n"
		, w -> file_name );
	_outtext(compiling);
	opts[nopt++] = "GWADACO";
	opts[nopt++] = avl_dir_library;
    opts[nopt++] = w -> file_name;
	for(i = 0; i < n; ++i)
	    opts[nopt++] = search_space[i];
	opts[nopt++] = NULL;
	if (AVL_EX_UNIT("GWADACO", opts))  ;
	fp2 = fopen("AVLCOMPO.DAT","r");
	if (fp2 == NULL) {
		AVL_ERROR("Request failed! Check installation or set of files");
		AVL_DEL_WINDOW(temp);
		return;
		}
	AVL_DEL_WINDOW(temp);
	j = 0;
	fscanf(fp2,"%d %d", &no_units, &larger);
	sprintf(compiling," Units Compiled ");
	if (larger < strlen(compiling)) larger = strlen(compiling);
	larger += 2;
	if (larger > 74) larger = 74;
	if (no_units > 18) no_units = 18;
	i = (76 - larger);
	n = 23 - no_units;
	m2 = AVL_MAKE_WINDOW(compiling,n,i,n+no_units+1,i+larger+2,avl_txt_bk_color,avl_txt_color);
	while ( fscanf(fp2,"%s",s2) != EOF) {
		sprintf(s, "%s",s2);
		sprintf(compiling,"\n %s",s);
		strcpy(current_file_name,s);

	    if (AVL_COMP_SETUP(w)) {
			fclose(fp2);
			AVL_DEL_WINDOW(m2);
			return;
			}
	    	
		if (AVL_DO_COMPILATION(s,0)) {
			fclose(fp2);
			AVL_DEL_WINDOW(m2);
			return;
			}
		_outtext(compiling);
		++j;
		}					
	if (j == 0)
		AVL_ERROR("There is nothing to compile");
	else {
		temp = AVL_MAKE_WINDOW(" N.Y.U. Ada Compiler ",5,1,9,80,avl_wnd_bk_color,avl_wnd_color);
		_settextposition(1,2);
		sprintf(msg," %d unit%s compiled with success.\n\n  Press any key to continue..."
			, j, (j > 1) ? "s" : "" );
		_outtext(msg);
		i = getch(); if (i == 0) i = getch();
		AVL_DEL_WINDOW(temp);
		}
	AVL_DEL_WINDOW(m2);
	fclose(fp2);
}
	
*/

void AVL_COMPILE()
{
	AVL_EDIT_WINDOW_PTR w;
	char msg[100];
	char s[121];
	static char *msgc = "GWAda - Compile which file? ";
	char *opts[17];
	int nopt = 0;
	int i, n, n1, n2;
	AVL_WIN_PTR m1;
	AVL_SOURCE_SIZE t;
	static int first = 0;
	static char lastfn[121];
	char fname[121];
	avl_w = w = &avl_windows[avl_window];
	n1 = 62;
	n2 = (80 - n1) / 2;
	m1 = AVL_MAKE_WINDOW(msgc,7,n2,9,n1+n2,avl_wnd_bk_color,avl_wnd_color);
	if (first == 0)
		sprintf(lastfn,"%s%c*.ada",avl_dir_sources,92);
	first = 1;
	strcpy(fname,lastfn);
	if (AVL_PROMPT(1,1,fname,60))  {
		AVL_DEL_WINDOW(m1);
		return;
		}
	strcpy(lastfn,fname);
	AVL_GET_FILE_NAMES(&t,fname);
	AVL_DEL_WINDOW(m1);
	if (t.no_files == 0)  {
		sprintf(s,"Can't find any file under \'%s\'",fname);
		AVL_ERROR(s);
		return;
		}
	s[0] = '\0';
	if ((n = AVL_SOURCES(&t,t.no_files,fname)) < 0) return;
	for(i = strlen(fname) - 1; t.no_files > 0 && i > 0; --i)  {
		if (fname[i] == 92 && i > 0) {
			fname[i+1] = '\0';
			i  = -10;
			break;
			}
		}
	if (i < -1)
		sprintf(s,"%s%s",fname,t.name[n]);
	else
		sprintf(s, "%s",t.name[n]);
	if (s[0] == '\0')
		return;
	strcpy(current_file_name,s);
    if (AVL_COMP_SETUP(w)) return;
	AVL_DO_COMPILATION(s,1);
}

void AVL_NEW_LIB()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_WIN_PTR temp;
	char msg[100];
	char *opts[17];
	int cmd_ret_code, i;
    FILE *fp, *fopen();
	w = &avl_windows[avl_window];
    fp = fopen("DUMMY.ADA","w");
    if (fp == NULL) {
    	AVL_ERROR("Can't create new library! Check disk space...");
    	return;
    	}
    fprintf(fp," PROCEDURE DUMMY IS BEGIN NULL; END DUMMY; ");
    fclose(fp);
	w -> no_errors = 0;
	sprintf(msg,"%s\\LIB",avl_dir_library);
	fp = fopen(msg,"r");
	if (fp == NULL) {
		rmdir(avl_dir_library);
		if (mkdir(avl_dir_library)) {
			sprintf(msg,"Unable to create library '%s'",avl_dir_library);
			AVL_ERROR(msg);
			return;
			}
		}
	else
		fclose(fp);
	temp = AVL_MAKE_WINDOW(" N.Y.U. Ada Compiler "
		,5,1,8,80,avl_wnd_bk_color,avl_wnd_color);
	_outtext("Creating new library, please wait...\n");
	opts[0] = "AdaComp";
	opts[1] = "-n"; 
	opts[2] = "-l";
	opts[3] = avl_dir_library;
	opts[4] = "dummy.ada";
	opts[5] = NULL;
	unlink("gwada.smm");
	if (AVL_EX_UNIT("ADACOMP", opts))  ;
	avl_open_error_file = 0;
	fp = fopen("GWADA.SMM","r");
	if (fp == NULL)  
		w -> no_errors = 0;
	else {
		fscanf(fp, "%d", &w -> no_errors);
		fclose(fp);
		}
	if (w -> no_errors > 0)  
		AVL_ERROR("Library was not created properly!");
	unlink("dummy.ada");
	AVL_DEL_WINDOW(temp);
}


void AVL_RUN()
{
	AVL_UNITDT w;
	AVL_EDIT_WINDOW_PTR ww;
	AVL_WIN_PTR temp;
	FILE *fp;
	char msg[100];
	char m2[4][12];
	char t_opts[12];
	short ncmd = 0;	
	short ti = 0;
	char *opts[19];
	char *runit;
	short nopt = 0;
	short cmd_ret_code, i;
    avl_blank_line[0] = '\0';
	ww = &avl_windows[avl_window];
	w.ns = 0;
	opts[0] = "Adalib2";
	opts[1] = "-l";
	opts[2] = avl_dir_library;
	opts[3] = NULL;
	temp = AVL_MAKE_WINDOW(" Run ",5,1,9,80,avl_wnd_bk_color,avl_wnd_color);
	_outtext("  Searching library. Please, wait a moment...\n");
	if (AVL_EX_UNIT("ADALIB2", opts))  ;
	AVL_DEL_WINDOW(temp);
	AVL_MAKE_UNIT_DATE(&w);
	if (w.ns == 0)  {
		AVL_ERROR("First compile any executable program unit.");
		return;
		}

	runit = AVL_RUN_SELECT(&w);
	if (runit == NULL)  
		return;
		temp = AVL_MAKE_WINDOW(" N. Y. U. Ada Interpreter "
		,11,1,14,80,avl_wnd_bk_color,avl_wnd_color);
	_outtext("Loading interpreter, please wait...");
	_settextposition(2,2);
	if (avl_trace_opts[0]) t_opts[ti++] = 'a';
	if (avl_trace_opts[1]) t_opts[ti++] = 'c';
	if (avl_trace_opts[2]) t_opts[ti++] = 'e';
	if (avl_trace_opts[3]) t_opts[ti++] = 'r';
	if (avl_trace_opts[4]) t_opts[ti++] = 's';
	if (avl_trace_opts[5]) t_opts[ti++] = 't';
	if (ti) t_opts[ti] = '\0';

	opts[nopt++] = (avl_monitor_on) ? "gwumon" : "adaexec";
	opts[nopt++] = "-m";
	opts[nopt++] = runit;
	if (!avl_monitor_on)  {
		if (avl_heap) {sprintf(m2[0],"%d",avl_heap); opts[nopt++]="-h"; opts[nopt++]=m2[0];}
		if (avl_pgm_stack) {sprintf(m2[1],"%d",avl_pgm_stack); opts[nopt++]="-p"; opts[nopt++]=m2[1];}
		if (avl_task_stack) {sprintf(m2[2],"%d",avl_task_stack); opts[nopt++]="-s"; opts[nopt++]=m2[2];}
		}
	if (avl_scheduller) {sprintf(m2[3],"%d",avl_scheduller); opts[nopt++]="-r"; opts[nopt++]=m2[3];}
	if (!avl_monitor_on)  
		if (ti) {
			opts[nopt++] = "-t";
			opts[nopt++] = t_opts;
			}
	opts[nopt++] = avl_dir_library;
	opts[nopt++] = NULL;
	_setvideomode( _DEFAULTMODE );
    _settextrows( 25 );
    _clearscreen( _GCLEARSCREEN );
	if (AVL_EX_UNIT((avl_monitor_on) ? "GWUMON" : "ADAEXEC", opts))  {
		printf("\n\nPress any key to return to GWAda ...");
		i = getch(); if (i == 0) i = getch();
		}
	AVL_DEL_WINDOW(temp);
}

