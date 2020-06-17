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

/*  Pars.c  */

#include <externs.h>

static avl_cur_par = 0;


void AVL_SET_MONITOR() 
{
    AVL_WIN_PTR hw, hw2;
    short h, co;
    char opts[6];
    char msg[80];
    int ch;
    char old;
    old = avl_monitor_on;
	hw2 = AVL_MAKE_WINDOW("",18,7,21,77,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Set the Parallel Monitoring On/Off:\n");
	_outtext(" ESC-Cancel  ENTER-Ok  Type the digit to set/reset the option.");
	_settextposition(2,2); 
	co = _settextcolor(avl_men_ready);
	_outtext("ESC");
	_settextposition(2,14); 
	_outtext("ENTER");
	_settextcolor(co);
	hw = AVL_MAKE_WINDOW(" Monitoring ",5,2,7,79,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" 1. Off  Source line monitoring");
	_settextcursor(0x2000);
	while ( 1 )   {
		_settextposition(1,5); 
		if (avl_monitor_on) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		ch = getch();
		if (ch == '1')   {
			h = ch - '1';
			avl_monitor_on = !avl_monitor_on;
			}
		else if (ch == 27) {avl_monitor_on = old; break; }
		else if (ch == 13)  
			break;
		else if (ch == 0) ch = getch();
		}
	AVL_DEL_WINDOW(hw);
	AVL_DEL_WINDOW(hw2);
		
}


void AVL_SET_TRACE() 
{
    AVL_WIN_PTR hw, hw2;
    short h, co;
    char opts[6];
    char msg[80];
    int ch;
	hw2 = AVL_MAKE_WINDOW("",18,9,21,78,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Specify the kind of trace requested for the program's execution:\n");
	_outtext(" ESC-Cancel    ENTER-Ok    Type the digit to set/reset an option.");
	_settextposition(2,2); 
	co = _settextcolor(avl_men_ready);
	_outtext("ESC");
	_settextposition(2,16); 
	_outtext("ENTER");
	_settextcolor(co);
	hw = AVL_MAKE_WINDOW(" Trace ",5,2,13,79,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" 1. Off  Display the source line numbers.\n");
	_outtext("         There is no need to recompile the program.\n");
	_outtext(" 2. Off  Display the name of each routine as it is entered and exited.\n");
	_outtext(" 3. Off  Display the name of each exception as it is raised.\n");
	_outtext(" 4. Off  Trace each rendezvous.\n");
	_outtext(" 5. Off  Display the context-switches between tasks.\n");
	_outtext(" 6. Off  Display task creation, activation, and termination.");
	for(h = 0; h < 6; ++h)
		opts[h] = avl_trace_opts[h];
	_settextcursor(0x2000);
	while ( 1 )   {
		_settextposition(1,5); 
		if (opts[0]) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		_settextposition(3,5); 
		if (opts[1]) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		_settextposition(4,5); 
		if (opts[2]) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		_settextposition(5,5); 
		if (opts[3]) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		_settextposition(6,5); 
		if (opts[4]) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		_settextposition(7,5); 
		if (opts[5]) {
			_settextcolor(avl_men_ready);
			_outtext("On "); 
			}
		else  { 
			_settextcolor(avl_men_letter);
			_outtext("Off");
			}
		ch = getch();
		if (ch >= '1' && ch <= '6')   {
			h = ch - '1';
			opts[h] = !opts[h];
			}
		else if (ch == 27) break;
		else if (ch == 13)  {
			for(h = 0; h < 6; ++h)
				avl_trace_opts[h] = opts[h];
			break;
			}
		}
	AVL_DEL_WINDOW(hw);
	AVL_DEL_WINDOW(hw2);
		
/* -t [acerst]  */
}

void AVL_SET_SCHEDULER() 
{
    AVL_WIN_PTR hw, hw2;
    short h;
    char msg[80];
	hw2 = AVL_MAKE_WINDOW("",18,3,24,77,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Invoke a 'round-robin' scheduling policy instead of the 'run-until-\n");
	_outtext(" -block' default scheduling policy and specify the maximum number of\n");
	_outtext(" interpreted statements that a given task can execute before switching\n");
	_outtext(" to another task. (the minimum value is 1).\n");
	_outtext(" A value 0 sets the 'run-until-block' schedulling policy.");
	hw = AVL_MAKE_WINDOW(" Scheduller ",13,20,13+2
	                                     ,63,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 )   {
		sprintf(msg,"%d", avl_scheduller);
		if (AVL_PROMPT(1,3,msg,3)) break;
		if (strlen(msg) > 0) {
			h = atoi(msg);
			if (h < 0) AVL_ERROR("Invalid number of interpreted statements."); 
			else {
				avl_scheduller = h;
				break;
				}
			}
		}
	AVL_DEL_WINDOW(hw);
	AVL_DEL_WINDOW(hw2);
		
/* -r number_of_statements (an integer)*/
}


void AVL_SET_TASK_STACK() 
{
    AVL_WIN_PTR hw, hw2;
    short h;
    char msg[80];
	hw2 = AVL_MAKE_WINDOW("",18,3,24,76,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Specifies the size in bytes of the stack size for each new task.\n");
	_outtext(" The default is 2048. Argument values in the range 1..31 are multiplied\n");
	_outtext(" by 1024 to obtain the stack size in bytes. Use of small stack sizes\n");
	_outtext(" may be appropriate when executing programs with many tasks, each of\n");
	_outtext(" which can run with a small stack.  The maximum value is 32767.");
	hw = AVL_MAKE_WINDOW(" Task Stack ",13,20,13+2
	                                     ,63,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 )   {
		sprintf(msg,"%d", avl_task_stack);
		if (AVL_PROMPT(1,3,msg,3)) break;
		if (strlen(msg) > 0) {
			h = atoi(msg);
			if (h <= 0) AVL_ERROR("Invalid task stack."); 
			else {
				avl_task_stack = h;
				break;
				}
			}
		}
	AVL_DEL_WINDOW(hw);
	AVL_DEL_WINDOW(hw2);
		
/*  -s tstack  */
}


void AVL_SET_HEAP_SIZE()
{
    AVL_WIN_PTR hw, hw2;
    short h;
    char msg[80];
	hw2 = AVL_MAKE_WINDOW("",18,4,24,75,avl_wnd_bk_color,avl_wnd_color);
    _outtext(" Specifies the size of the heap increment in kilowords, by which the\n");
    _outtext(" heap is expanded when the elaboration of a new entity requires it.\n");
    _outtext(" The heap expands to fill available memory. The default increment is\n");
    _outtext(" 32. This parameter limits the size of the largest object that can be\n");
    _outtext(" created in a program.");
	hw = AVL_MAKE_WINDOW(" Heap Size ",13,20,13+2
	                                     ,63,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 )   {
		sprintf(msg,"%d", avl_heap);
		if (AVL_PROMPT(1,3,msg,3)) break;
		if (strlen(msg) > 0) {
			h = atoi(msg);
			if (h <= 0) AVL_ERROR("Invalid heap size."); 
			else {
				avl_heap = h;
				break;
				}
			}
		}
	AVL_DEL_WINDOW(hw);
	AVL_DEL_WINDOW(hw2);
		
/*  -h heap  */
}



void AVL_SET_PGM_STACK() 
{
    AVL_WIN_PTR hw, hw2;
    short h;
    char msg[80];
	hw2 = AVL_MAKE_WINDOW("",19,5,24,75,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Specifies the size in bytes of the program stack size for the main\n");
	_outtext(" task. The default is 4096. Argument values in the range 1..31 are\n");
	_outtext(" multiplied by 1024 to obtain the stack size in bytes. The maximum\n");
	_outtext(" value is 32767. ");
	hw = AVL_MAKE_WINDOW(" Program Stack ",13,20,13+2
	                                      ,63,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 )   {
		sprintf(msg,"%d", avl_pgm_stack);
		if (AVL_PROMPT(1,3,msg,3)) break;
		if (strlen(msg) > 0) {
			h = atoi(msg);
			if (h <= 0) AVL_ERROR("Invalid stack size."); 
			else {
				avl_pgm_stack = h;
				break;
				}
			}
		}
	AVL_DEL_WINDOW(hw);
	AVL_DEL_WINDOW(hw2);
		
/*  -p pstack */
}

void AVL_SET_PARAMETERS()
{
	AVL_BIND_SIZE c[7];
	PROC_NAME rot[7] = {AVL_SET_MONITOR
	                   ,AVL_SET_HEAP_SIZE
	                   ,AVL_SET_PGM_STACK
	                   ,AVL_SET_TASK_STACK
	                   ,AVL_SET_SCHEDULER
	                   ,AVL_SET_TRACE
	                   ,NULL};
	int i;
	int save_unit;
    int ch, cols = 16, rows = 6;
    AVL_WIN_PTR hw, hw2;
	strcpy(c[0].name,"Monitoring");
	strcpy(c[1].name,"Heap Size");
	strcpy(c[2].name,"Program Stack");
	strcpy(c[3].name,"Task Stack");
	strcpy(c[4].name,"Scheduller");
	strcpy(c[5].name,"Trace");
	save_unit = avl_cur_unit;
	avl_cur_unit = avl_cur_par;
    avl_size = rows;

	hw2 = AVL_MAKE_WINDOW("",15,55,15+9,55+24,avl_wnd_bk_color,avl_wnd_color);
	_outtext(" Use the arrow keys to\n");
	_outtext(" go over the  options.\n");
	_outtext("\n");
	_outtext(" Press  <enter>  to\n");
	_outtext(" select an option or\n");
	_outtext(" type the option's no.\n");
	_outtext("\n");
	_outtext(" Press ESC to  cancel.");
	hw = AVL_MAKE_WINDOW(" Parameters ",3,avl_menu[6].c,3+rows+1,avl_menu[6].c+cols+5,avl_wnd_bk_color,avl_wnd_color);
	while ( 1 )  {
		AVL_SHOW_UNIT(c,cols);
		ch = getch();
		if ((ch >= '1' && ch <= '6') || (ch == 13)) {
			if (ch != 13)
				avl_cur_unit += (ch - '0' - 1);
			avl_cur_par = avl_cur_unit;
			avl_cur_unit = save_unit;
			rot[avl_cur_par]();
			}
		else {
			if (ch == 0 || ch == 0xE0) {
				ch = getch();
                switch( ch ) {
                   	case 72 : /* Up */ 
                   		if (--avl_cur_unit < 0)  
							avl_cur_unit = avl_size - 1;
                   		break;
                   	case 80 : /* Down */ 
                   		if (++avl_cur_unit >= avl_size)
                   			avl_cur_unit = 0;
                   		break;
                   	default : putchar(7); break;
                   	}
                continue;
                }
			if (ch == 27)  {
				AVL_DEL_WINDOW(hw);
				AVL_DEL_WINDOW(hw2);
				avl_cur_par = avl_cur_unit;
				avl_cur_unit = save_unit;
				return;
				}
			putch(7);
			continue;
			}
		}
}


