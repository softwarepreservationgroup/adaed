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

/* Parser.c */

#include "externs.h"
#include "defines.h"

char *strupr(char *string);
char *strcat(char *string1, const char *string2);


typedef struct {
	char yytext[AVL_MAX_LINEL + 1];
	AVL_LINE_PTR temp;  /* Full source line */
	int    yyleng;
	int    code;
	int    line_no;
	int    line_col;
	} TOKEN_TYPE;


typedef struct {  /*  I/O structure  */
	char        buffer[AVL_MAX_LINEL + 40];
	char        last_token[AVL_MAX_LINEL + 1];
	char        decimal_point;
	int         size;
	int         ibuf;
	int         line_no;
	int         look_ahead;
	int         look_ahead_c[5];
	int         look_ahead_t;
	TOKEN_TYPE look_ahead_tcode[3];
	} IO_TYPE;



IO_TYPE io_size;

#include "bsearch.c"


void AVL_START_UP()
{
	int     ch;
	char msg[100];
	AVL_EDIT_WINDOW_PTR w;
	w = &avl_windows[avl_window];
	io_size.decimal_point     = '.';
	io_size.look_ahead = 0;
	io_size.line_no = w -> current_line -> line_no;
	io_size.ibuf = w -> txt_col;
	io_size.look_ahead_t = 0;
	sprintf(io_size.buffer, w -> current_line -> line);
	io_size.size = strlen(io_size.buffer);
	sprintf(io_size.last_token,"THIS_IS_THE_END_OF_THE_FILE");
}
	

int AVL_NEXT()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, temp2;
	int     ch;
	w = &avl_windows[avl_window];
	if (io_size.look_ahead) { /* There is a look ahead char in the queue */
		ch = io_size.look_ahead_c[0];
		io_size.look_ahead_c[0] = io_size.look_ahead_c[1];
		io_size.look_ahead_c[1] = io_size.look_ahead_c[2];
		io_size.look_ahead_c[2] = io_size.look_ahead_c[3];
		io_size.look_ahead_c[3] = io_size.look_ahead_c[4];
		io_size.look_ahead -= 1;
		return ch;
		}
	if (io_size.ibuf >= io_size.size)  {
		if (w -> current_line -> next == w -> head)  
			sprintf(io_size.buffer," \n THIS_IS_THE_END_OF_THE_FILE \" \n THIS_IS_THE_END_OF_THE_FILE  ");
		else {
			w -> current_line = w -> current_line -> next;
			sprintf(io_size.buffer,"%s\n", w -> current_line -> line);
			io_size.line_no = w -> current_line -> line_no;
			}
		io_size.size = strlen(io_size.buffer);
		io_size.ibuf = 0;
		}
	ch = io_size.buffer[io_size.ibuf++];
	if (ch == 26) ch = ' ';
	if (ch == 0) return ' ';
	return ch;
}

void BACK(int     ch)
{
	if (io_size.look_ahead > 4)  {
		AVL_ERROR("Too many look ahead bytes. SYSTEM ERROR...");
		exit(1);
		}
	io_size.look_ahead_c[io_size.look_ahead++] = ch;
}


int IS_DIGIT(int ch, int base, int *n)  
{
	int     num;
	char msg[40];
	*n = 0;
	if (ch >= 'A' && ch <= 'F')
		num = ch - 'A' + 11;
	else if (ch >= '0' && ch <= '9') 
		num = ch - '0';
	else 
		return 0;
	sprintf(msg,"%c in %d base ", ch, base);
	if (num < 0 || num >= base)
		AVL_ERROR(strcat("Based number with invalid digit ==> ",msg));
	*n = num;
	return 1;
}

void BACK_T(TOKEN_TYPE tk)
{
	if (io_size.look_ahead_t > 2) {
		AVL_ERROR("Too many look ahead tokens. SYSTEM ERROR...");
		exit(1);
		}
	io_size.look_ahead_tcode[io_size.look_ahead_t++] = tk;
}
	

TOKEN_TYPE AVL_SCAN()
{
	int     ch, lch;
	int     is_real = 0;
	int     is_based = 0;
	int     base = 10;
	int     is_expo = 0;
	int     string_mark;
	int     temp;
	static TOKEN_TYPE tk;
	AVL_EDIT_WINDOW_PTR w;
	char yytext[AVL_MAX_LINEL + 1];
	w = &avl_windows[avl_window];

	if (io_size.look_ahead_t) {
		tk = io_size.look_ahead_tcode[0];
		io_size.look_ahead_tcode[0] = io_size.look_ahead_tcode[1];
		io_size.look_ahead_tcode[1] = io_size.look_ahead_tcode[2];
		io_size.look_ahead_t -= 1;
		return tk;
		}

	tk.yyleng = 0;
	tk.yytext[0] = '\0';
again:
	/* First skip blanks */
	while (isspace(ch = AVL_NEXT()));
	/* Test comment */
	if (ch == '-') {
		ch = AVL_NEXT();
		if (ch == '-') { /* Now reject til end of line */
			while ((ch = AVL_NEXT()) != '\n');
			goto again;
			}
		BACK('-');
		BACK(ch);
		}
	else BACK(ch);
	ch = AVL_NEXT();

	/* Now let's see what we got */

	tk.line_no  = io_size.line_no;
	tk.line_col = io_size.ibuf;
	tk.temp = w -> current_line;
	if (isalpha(ch)) {  /* Got an object's name */ 
		lch = ' ';
		do {
			if (tk.yyleng >= AVL_MAX_LINEL)  {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("Symbol too long => ", tk.yytext));
				}
			if (ch == '-' && lch == '-') {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("Do not repeat _ ==> ", tk.yytext));
				}
			tk.yytext[tk.yyleng++] = ch;
			lch = ch;
			ch = AVL_NEXT();
			} while (isalpha(ch) || isdigit(ch) || ch == '_');
		tk.yytext[tk.yyleng] = '\0';
		strcpy(yytext,tk.yytext);
		strupr(yytext);
		if (lch == '_') 
			AVL_ERROR(strcat("_ can't terminate an object's name ==> ", tk.yytext));
		BACK(ch);
		if (tk.code == XATTMARK) {  /* See if previous token was an ' */
			lch = IS_ATT(yytext);
			if (lch == 0)
				AVL_ERROR(strcat("Invalid attribute ==> ", tk.yytext));
			tk.code = lch;
			}
		else {
			if (!strcmp(yytext,"REM"))
				tk.code = XMULT;
			else if (!strcmp(yytext,"MOD"))
				tk.code = XMULT;
			else if (!strcmp(yytext,"AND"))
				tk.code = XLOG;
			else if (!strcmp(yytext,"OR"))
				tk.code = XLOG;
			else
				tk.code = IS_KEYWORD(yytext);
			if (tk.code == 0)
				if (strcmp(io_size.last_token,yytext))
					tk.code = XID;
			}
		return tk;
		}

						
	if (isdigit(ch))  {  /* Got a numeric literal */
		lch = ' ';
		do {
			if (tk.yyleng >= AVL_MAX_LINEL)  {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("Numeric literal too long ==> ", tk.yytext));
				}
			if (lch == ch && ch == '_') {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("Two _ repeated within numeric literal ==> ", tk.yytext));
				}
			if (ch != '_')
				tk.yytext[tk.yyleng++] = ch;
			lch = ch;
			ch = AVL_NEXT();
			} while (isdigit(ch) || ch == '_');
		if (lch == '_') {
			tk.yytext[tk.yyleng] = '\0';
			AVL_ERROR(strcat("_ can't terminate an integer ==> ", tk.yytext));
			}
		if (ch == ':' || ch == '#')  { /* Got a based number */
			is_based = ch;
			tk.yytext[tk.yyleng] = '\0';
			base = atoi(tk.yytext);
			if (base < 2 || base > 16) 
				AVL_ERROR(strcat("Based literal with invalid base ==> ", tk.yytext));
			tk.yyleng = 0;
			ch = AVL_NEXT();
			if (!IS_DIGIT(ch,base,&temp))
				AVL_ERROR("A digit was expected");
			lch = ' ';
			do {
				if (lch == ch && ch == '_') {
					tk.yytext[tk.yyleng] = '\0';
					AVL_ERROR(strcat("Two _ repeated within numeric literal ==> ", tk.yytext));
					}
				if (ch != '_')
					tk.yytext[tk.yyleng++] = ch;
				lch = ch;
				ch = AVL_NEXT();
				}  while (IS_DIGIT(ch,base,&temp) || ch == '_');
			if (lch == '_') {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("_ can't terminate an integer ==> ", tk.yytext));
				}
			}
		if (ch == io_size.decimal_point) { /* Got a real number? */
			if (ch == '.')  {
				ch = AVL_NEXT();
				if (ch == '.')  {
					BACK('.');
					BACK('.');
					tk.yytext[tk.yyleng] = '\0';
					tk.code = XINTEGERLIT;
					return tk;
					}
				BACK(ch);
				}
			is_real = 1;
			tk.yytext[tk.yyleng++] = '.';
			ch = AVL_NEXT();
			if (!IS_DIGIT(ch,base,&temp))  {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("A digit should follow a decimal point ==> ", tk.yytext));
				}
			lch = ' ';
			do {
				if (lch == ch && ch == '_') {
					tk.yytext[tk.yyleng] = '\0';
					AVL_ERROR(strcat("Two _ repeated within numeric literal ==> ", tk.yytext));
					}
				if (ch != '_')
					tk.yytext[tk.yyleng++] = ch;
				lch = ch;
				ch = AVL_NEXT();
				}  while (IS_DIGIT(ch,base,&temp) || ch == '_');
			if (lch == '_') {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("_ can't terminate an integer ==> ", tk.yytext));
				}
			}
		
		if (is_based && is_based != ch) {
			tk.yytext[tk.yyleng] = '\0';
			AVL_ERROR(strcat("Missing based number delimiter ==> ", tk.yytext));
			}
		else if (is_based)
			ch = AVL_NEXT();
		if (toupper(ch) == 'E') {  /* Got an exponent */
			is_expo = 1;
			tk.yytext[tk.yyleng++] = ch;
			ch = AVL_NEXT();
			if (ch == '-' || ch == '+')  {
				if (!is_real && ch == '-')  {
					tk.yytext[tk.yyleng] = '\0';
					AVL_ERROR(strcat("E-<integer> can't be used in an integer number ==> ", tk.yytext));
					}
				tk.yytext[tk.yyleng++] = ch;
				ch = AVL_NEXT();
				}
			if (!isdigit(ch))  {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("A digit should follow a decimal point ==> ", tk.yytext));
				}
			do {
				if (lch == ch && ch == '_') {
					tk.yytext[tk.yyleng] = '\0';
					AVL_ERROR(strcat("Two _ repeated within numeric literal ==> ", tk.yytext));
					}
				if (ch != '_')
					tk.yytext[tk.yyleng++] = ch;
				lch = ch;
				ch = AVL_NEXT();
				}  while (isdigit(ch) || ch == '_');
			if (lch == '_') {
				tk.yytext[tk.yyleng] = '\0';
				AVL_ERROR(strcat("_ can't terminate an integer ==> ", tk.yytext));
				}
			}
		tk.yytext[tk.yyleng] = '\0';
		if (is_real)
			tk.code = XREALLIT;
		else 
			tk.code = XINTEGERLIT;
		BACK(ch);
		return tk;
		}	/*  Done with numeric literal  */
	

	switch(ch)  {

		case ':' :  {
			ch = AVL_NEXT();
			if (ch == '=')  {
				tk.code = XASSIGN;
				strcpy(tk.yytext,":=");
				}
			else {
				BACK(ch);
				tk.code = ':';
				strcpy(tk.yytext,":");
				}
			return tk;
			}
	
		case ';' : case ',' : case '(' :
		case ')' : case '|' : case '!' : case '&' : {
			tk.code = ch;
			if (ch == '!') ch = '|';
			sprintf(tk.yytext,"%c",ch);
			return(tk);
			}

		case '\"' : case '%' :  {  /* Got a string */
			string_mark = ch;
			tk.yytext[tk.yyleng++] = ch;
			while ( 1 )   {
				ch = AVL_NEXT();
				if (ch == string_mark)  {
					ch = AVL_NEXT();
					if (ch != string_mark)  { /* End of string */
						tk.yytext[tk.yyleng++] = string_mark;
						tk.yytext[tk.yyleng] = '\0';
						BACK(ch);
						tk.code = XSTRLIT;
						return tk;
						}
					}
				if (tk.yyleng >= AVL_MAX_LINEL)  {
					tk.yytext[tk.yyleng] = '\0';
					AVL_ERROR(strcat("String literal too long ==> ", tk.yytext));
					}
				if (ch == '\n')  {
					tk.yytext[tk.yyleng] = '\0';
					AVL_ERROR(strcat("String literal between lines ==> ", tk.yytext));
					}
				tk.yytext[tk.yyleng++] = ch;
				}
			}

		case '+' : case '-' :  {
			sprintf(tk.yytext,"%c",ch);
			tk.code = XARIT;
			return tk;
			}

		case '/' :  {
			ch = AVL_NEXT();
			if (ch != '=')   {
				tk.code = XMULT;
				strcpy(tk.yytext,"/");
				return tk;
				}
			sprintf(tk.yytext,"/=");
			tk.code = XREL;
			return tk;
			}

		case '*' : {
			ch = AVL_NEXT();
			if (ch != '*')  
				BACK(ch);
			else {
				strcpy(tk.yytext,"**");
				tk.code = XEXP;
				return tk;
				}
			ch = '*';
			sprintf(tk.yytext,"%c",ch);
			tk.code = XMULT;
			return tk;
			}

		case '.' :  {
			ch = AVL_NEXT();
			if (ch == '.')  {
				strcpy(tk.yytext,"..");
				tk.code = XDOTDOT;
				return tk;
				}
			BACK(ch);
			strcpy(tk.yytext,".");
			tk.code = '.';
			return tk;
			}

		case '\'' :   {  /* Got an attribute ?  */
			ch = AVL_NEXT();
			lch = AVL_NEXT();
			if (lch == '\'') { /* Got a character literal!  */
				sprintf(tk.yytext,"%c",ch);
				tk.code = XCHARLIT;
				return tk;
				}
			tk.code = XATTMARK;
			strcpy(tk.yytext,"'");
			BACK(lch);
			BACK(ch);
			return tk;
			}

		case '=' : {
			ch = AVL_NEXT();
			if (ch == '>')  {
				strcpy(tk.yytext,"=>");
				tk.code = XEQGT;
				}
			else 
				BACK(ch);
			strcpy(tk.yytext,"=");
			tk.code = XREL;
			return tk;
			}

		case '>' : case '<' : {
			if (ch == '<')  {
				int k;
				k = ch;
				ch = AVL_NEXT();
				if (ch == '>')  {
					strcpy(tk.yytext,"<>");
					tk.code = XLTGT;
					return tk;
					}
				BACK(ch);
				ch = k;
				}
			tk.code = XREL;
			tk.yytext[0] = ch;
			tk.yytext[1] = '\0';
			tk.yytext[2] = '\0';
			ch = AVL_NEXT();
			if (ch == '=')  
				tk.yytext[1] = ch;
			else
				BACK(ch);
			return tk;
			}
								
		default : {
			sprintf(tk.yytext,"[decimal=%d]",ch);
			AVL_ERROR(strcat("Invalid character ==> ", tk.yytext));
			}
		}
}


int IS_SIMPLE(char *s)
{
	if (!strcmp(s,"TEXT_IO")) return 0;
	if (!strcmp(s,"ASCII")) return 0;
	if (!strcmp(s,"STANDARD")) return 0;
	if (!strcmp(s,"CALENDAR")) return 0;
	if (!strcmp(s,"SYSTEM")) return 0;
	if (!strcmp(s,"MACHINE_CODE")) return 0;
	if (!strcmp(s,"IO_EXCEPTIONS")) return 0;
	if (!strcmp(s,"LOW_LEVEL_IO")) return 0;
	return 1;
}

int pos_p = 0;
int AVL_Append_In_Text;
static AVL_EDIT_WINDOW AVL_Parsing, AVL_Appending;
static int AVL_Parser_Pos = 1, AVL_Parser_Line = -100;

void AVL_OUT_BODY(FILE *fp, TOKEN_TYPE tk)
{
	int i;
	AVL_Parsing = avl_windows[avl_window];
	if (AVL_Parser_Line == -100) {
		AVL_Parser_Line = tk.line_no;
		AVL_Parser_Pos = 1;
		if (AVL_Append_In_Text)  {
			/* Go to last line of current text */
			AVL_DO_GOTO(AVL_Parsing.head -> previous -> line_no);
			AVL_CURSOR_END();
			AVL_EDIT_ENTER(); /* Insert a blank line */
			AVL_EDIT_ENTER(); /* Insert a blank line */
			AVL_Appending = avl_windows[avl_window];		
			}
		}
	if (AVL_Parser_Line != tk.line_no)  {
		AVL_Parser_Pos = 1; 
		AVL_Parser_Line = tk.line_no; 
		if (AVL_Append_In_Text == 0)
			fprintf(fp,"\n"); 
		else {
			avl_windows[avl_window] = AVL_Appending;
			AVL_CURSOR_END();
			AVL_EDIT_ENTER(); /* Insert a blank line */
			avl_windows[avl_window].txt_col = 0;
			AVL_Appending = avl_windows[avl_window];		
			}
		}
	if (pos_p) {
		AVL_Parser_Pos = 1;
		tk.line_col = pos_p;
		}
	avl_windows[avl_window] = AVL_Appending;
	for( ; AVL_Parser_Pos < tk.line_col; ++AVL_Parser_Pos) {
		if (AVL_Append_In_Text == 0)
			fprintf(fp," ");
		else 
			AVL_EDIT_INSERT(' ');
		}
	AVL_Appending = avl_windows[avl_window];		
	if (AVL_Append_In_Text == 0)
		fprintf(fp,"%s", tk.yytext);
	else {
		avl_windows[avl_window] = AVL_Appending;
		for(i=0; i < strlen(tk.yytext); ++i)
			if (tk.yytext[i] == '\n')  {
				AVL_CURSOR_END();
				AVL_EDIT_ENTER();
				}
			else
				AVL_EDIT_INSERT(tk.yytext[i]);
		AVL_Appending = avl_windows[avl_window];		
		}
	avl_windows[avl_window] = AVL_Parsing;
	AVL_Parser_Pos += strlen(tk.yytext);
}

void AVL_SKIP_RECORD()
{
	TOKEN_TYPE tk;
	tk = AVL_SCAN();
	while ( tk.code != 0 )  {
		if (tk.code == XRECORD) AVL_SKIP_RECORD();
		else if (tk.code == XEND)  {
			tk = AVL_SCAN();
			if (tk.code == XRECORD) return;
			else if (tk.code == XCASE) tk = AVL_SCAN();
			else {
				AVL_ERROR("First make sure the package spec is correct.");
				tk.code = 0;
				BACK_T(tk);
				return;
				}
			}
		if (tk.code == XPROCEDURE || tk.code == XFUNCTION || tk.code == XTASK)  {
			tk.code = 0;
			BACK_T(tk);
			return;
			}
		tk = AVL_SCAN();
		}
}

int AVL_FAIL(FILE *fp)			
{
	fclose(fp);
	AVL_ERROR("Syntax error within package spec.");
	return -1;
}

int AVL_PARSER()
{
	TOKEN_TYPE tk, tk2, tk3, tk4, tk5, tk6;
	static int first = 0;
	static char fname[161];
	static char lastfn[121];
	static char *msg = " GWAda - Create Body in File? ";
	char msg2[161];
	AVL_WIN_PTR m = NULL;
	int nt = 0;
	int i, j, code;
	short with_entry = 0;
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, temp2;
	FILE *fopen(), *fp = NULL;
	w = &avl_windows[avl_window];
	pos_p = 0;
	AVL_START_UP();

	tk = AVL_SCAN();
	if (tk.code != XPACKAGE)
		tk = AVL_SCAN();
	switch(tk.code)  {
		case XPACKAGE :  /* Where packages are defined? */
			tk2 = AVL_SCAN(); /* got package's name */
			if (tk2.code == XBODY) 
				break;
			tk3 = AVL_SCAN();  /* got is? */
			if (tk3.code != XIS)     
				break;
			tk4 = AVL_SCAN();
			if (tk4.code == XNEW) 
				return 0;
			tk6 = tk2;
			/* Got a package speck */
			sprintf(msg2,"Append the template at the end of this file (Y/N) ? ");
			if (AVL_QUESTION(msg2) == 'Y')  
				AVL_Append_In_Text = 1;
			else {
				AVL_Append_In_Text = 0;
				/* Now ask for a package body name */
				m = AVL_MAKE_WINDOW(msg,7,4,9,5+62,avl_wnd_bk_color,avl_wnd_color);
				if (first == 0)
					sprintf(lastfn,"%s%c*.ada",avl_dir_sources,92);
				first = 1;
				strcpy(fname,lastfn);
				if (AVL_PROMPT(1,1,fname,60)) {
					AVL_DEL_WINDOW(m);
					return -1;
					}
				strcpy(lastfn,fname);
				if ((fp = fopen(fname,"r")) != NULL)  {
					sprintf(msg2,"File %s already exists. Overwrite (Y/N) ? ", fname);
					if (AVL_QUESTION(msg2) != 'Y')  {
						AVL_DEL_WINDOW(m);
						return -1;
						}
					fclose(fp);
					unlink(fname);
					}
				fp = fopen(fname,"w");
				if (fp == NULL)  {
					sprintf(msg2,"Can't create file %s", fname);
					AVL_ERROR(msg2);
					AVL_DEL_WINDOW(m);
					return -1;
					}
				}
			AVL_OUT_BODY(fp,tk); /* Output PACKAGE in the correct text position */			
			strcpy(tk.yytext," BODY "); AVL_OUT_BODY(fp,tk); 
			AVL_OUT_BODY(fp,tk2); /* Output PACKAGE name */			
			strcpy(tk.yytext," IS\n"); 
			AVL_OUT_BODY(fp,tk); 
			while ( tk4.code != 0 )  {
				switch( tk4.code ) {
					case XTASK      : {
						tk5 = tk4;
						with_entry = 0;
						AVL_OUT_BODY(fp,tk4);
						strcpy(tk4.yytext," BODY ");
						AVL_OUT_BODY(fp,tk4);
						tk2 = AVL_SCAN(); /* tk2 hold the task's name */
						if (tk2.code == 0) return AVL_FAIL(fp);
						if (tk2.code == XTYPE) {
							tk2 = AVL_SCAN();
							if (tk2.code == 0) return AVL_FAIL(fp);
							}
						AVL_OUT_BODY(fp,tk2);
						tk3 = tk2;
						strcpy(tk3.yytext," IS");
						AVL_OUT_BODY(fp,tk3);
						strcpy(tk5.yytext,"BEGIN");
						tk5.line_no = 0;
						AVL_OUT_BODY(fp,tk5);
						tk3 = AVL_SCAN(); 
						if (tk3.code == 0) return AVL_FAIL(fp);
						if (tk3.code == ';')  {
							tk6 = tk5;
							tk6.line_no += 1;
							tk6.line_col += avl_tab_size;
							sprintf(tk6.yytext,"NULL;");
							AVL_OUT_BODY(fp,tk6);
							tk6.line_no += 1;
							tk6.line_col -= avl_tab_size;
							sprintf(tk6.yytext,"END %s;\n",tk2.yytext);
							AVL_OUT_BODY(fp,tk6);
							break;
							}

						tk3 = AVL_SCAN(); 
						if (tk3.code == 0) return AVL_FAIL(fp);
						while (tk3.code == XENTRY)  {
							tk6 = tk3;
							with_entry = 1;
							strcpy(tk3.yytext,"ACCEPT ");
							AVL_OUT_BODY(fp,tk3);
							tk4 = AVL_SCAN();
							if (tk4.code == 0) return AVL_FAIL(fp);
							AVL_OUT_BODY(fp,tk4);  /* output entry name */
							tk3 = AVL_SCAN();
							if (tk3.code == ';')  {
								AVL_OUT_BODY(fp,tk3); 
								tk3 = AVL_SCAN();
								continue;
								}
							if (tk3.code == '(')  {
								while (tk3.code != ')')  {
									if (tk3.code == 0) return AVL_FAIL(fp);
									tk.line_col += 1;
									AVL_OUT_BODY(fp,tk3); 
									tk3 = AVL_SCAN();
									}
								AVL_OUT_BODY(fp,tk3); 
								tk3 = AVL_SCAN();
								}
							tk5 = tk3;
							strcpy(tk5.yytext," DO");
							AVL_OUT_BODY(fp,tk5);
							tk6.line_no = 0;
							tk6.line_col += avl_tab_size;
							sprintf(tk6.yytext,"NULL;");
							AVL_OUT_BODY(fp,tk6);
							tk6.line_no += 1;
							tk6.line_col -= avl_tab_size;
							sprintf(tk6.yytext,"END %s;",tk4.yytext);
							AVL_OUT_BODY(fp,tk6);
							tk3 = AVL_SCAN();
							}
						while (tk3.code != XEND) {
							if (tk3.code == 0) return AVL_FAIL(fp);
							tk3 = AVL_SCAN();
							}
						if (!with_entry)  {
							tk4 = tk3;
							tk4.line_no = 0;
							tk4.line_col += avl_tab_size;
							sprintf(tk4.yytext,"NULL;");
							AVL_OUT_BODY(fp,tk4);
							}
						sprintf(tk3.yytext,"END %s;\n",tk2.yytext);
						AVL_OUT_BODY(fp,tk3);
						break;
						}
					case XPROCEDURE :
					case XFUNCTION  :  {
						AVL_OUT_BODY(fp,tk4);
						tk2 = AVL_SCAN(); /* tk2 hold the routine's name */
						if (tk2.code == 0) return AVL_FAIL(fp);
						AVL_OUT_BODY(fp,tk2);
						tk3 = AVL_SCAN(); 
						if (tk3.code == 0) return AVL_FAIL(fp);
						if (tk3.code == '(')  {
							AVL_OUT_BODY(fp,tk3);
							tk3 = AVL_SCAN();
/*							pos_p = tk3.line_col;   */
							while (tk3.code != ')')  {
								if (tk3.code == 0) return AVL_FAIL(fp);
/*								tk3.line_col = pos_p;   */
								AVL_OUT_BODY(fp,tk3); 
								tk3 = AVL_SCAN();
								}
							AVL_OUT_BODY(fp,tk3); 
							tk3 = AVL_SCAN();
							}
						if (tk3.code == 0) return AVL_FAIL(fp);
						if (tk3.code == XRETURN)  {
							AVL_OUT_BODY(fp,tk3);
							tk3 = AVL_SCAN();
							if (tk3.code == 0) return AVL_FAIL(fp);
							AVL_OUT_BODY(fp,tk3);
							tk5 = tk3;
							strcpy(tk5.yytext," IS");
							AVL_OUT_BODY(fp,tk5);
							tk4.line_no = 0;
							tk4.line_col += avl_tab_size;
							sprintf(tk4.yytext,"Result : %s;",tk3.yytext); 
							AVL_OUT_BODY(fp,tk4);
							strcpy(tk4.yytext,"BEGIN"); 
							tk4.line_no += 1;
							tk4.line_col -= avl_tab_size;
							AVL_OUT_BODY(fp,tk4);
							tk4.line_no += 1;
							tk4.line_col += avl_tab_size;
							sprintf(tk4.yytext,"RETURN Result;");
							AVL_OUT_BODY(fp,tk4);
							}
						else {
							tk5 = tk3;
							strcpy(tk5.yytext," IS");
							AVL_OUT_BODY(fp,tk5);
							tk4.line_no = 0;
							strcpy(tk4.yytext,"BEGIN"); 
							AVL_OUT_BODY(fp,tk4);
							tk4.line_no += 1;
							tk4.line_col += avl_tab_size;
							sprintf(tk4.yytext,"NULL;");
							AVL_OUT_BODY(fp,tk4);
							}
						sprintf(tk4.yytext,"END %s;\n",tk2.yytext);
						tk4.line_no += 1;
						tk4.line_col -= avl_tab_size;
						AVL_OUT_BODY(fp,tk4);
						break;
						}
					case XRECORD :
						AVL_SKIP_RECORD();
						break;
					case XEND    :
						AVL_OUT_BODY(fp,tk4);
						tk = AVL_SCAN();
						if (tk.code == 0) return AVL_FAIL(fp);
						if (tk.code != ';') {
							AVL_OUT_BODY(fp,tk);
							tk = AVL_SCAN();
							}
						if (tk.code == 0) return AVL_FAIL(fp);
						AVL_OUT_BODY(fp,tk);
						if (AVL_Append_In_Text == 0)  {
							fprintf(fp,"\n");
							fclose(fp);
							}
						else {
							AVL_Parsing = avl_windows[avl_window];		
							avl_windows[avl_window] = AVL_Appending;
							AVL_EDIT_ENTER();
							AVL_Appending = avl_windows[avl_window];		
							avl_windows[avl_window] = AVL_Parsing;
							}

						fp = NULL;
					    AVL_DEL_WINDOW(m);
						return 1;
					default :
						break;
					}
				tk4 = AVL_SCAN();
				}
		default :
			break;
		}
	if (fp != NULL)
		AVL_ERROR("Invalid syntax for package spec.");
	sprintf(tk4.yytext,"END;");
	tk4.line_no += 1000;
	tk4.line_col = 1;
	AVL_OUT_BODY(fp,tk4);
    AVL_DEL_WINDOW(m);
	return 0;
}	

void AVL_BODY()
{
	AVL_EDIT_WINDOW_PTR w;
	int old_txt;
	AVL_LINE_PTR temp, temp2;
	w = &avl_windows[avl_window];
	AVL_Appending = avl_windows[avl_window];
	AVL_Parser_Pos = 1;
	AVL_Parser_Line = -100;
	old_txt = w -> txt_col;
	temp = w -> current_line;
	AVL_WORD_BACKWARD();
	if (!AVL_PARSER())
		AVL_ERROR("Press F7 only when the cursor is over a valid PACKAGE spec.");
	if (AVL_Append_In_Text)  {
		avl_windows[avl_window] = AVL_Appending;
		AVL_UPDATE_SCREEN();
		}
	else {
		w -> current_line = temp;
		w -> txt_col = old_txt;
		}
}

