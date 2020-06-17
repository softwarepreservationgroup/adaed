

/*
    GWMON Parallel Ada Monitor for 386/486 PCs   
    Copyright (C) 1993, Charles W. Kann  & Michael Bliss Feldman
                        ckann@seas.gwu.edu mfeldman@seas.gwu.edu
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/

#include "ed.h"
#include "keydef.h"
/*
	I need the variable "exception_trace" from ivars.h.  I don't want
	to use the whole .h, so I am including this one variable here.
*/
extern int exception_trace;

static int current_task;
static int save_block_number = -1;
static long Run_Total_Time;   /* Total run time (lines) for this run */
static long Run_Current_Time; /* Run time (lines) since last report */
extern rr_flag;
void CWK_SET_TASK();
void CWK_SET_TASK_LINE();
void CWK_GET_KEY();
static void scroll_tasks();

void busy_wait(wait_cnt)
int wait_cnt;
{
    int i, cycles;
    cycles = delay_scaling_factor * delay_array[wait_cnt];
    for ( i = 0; i < cycles; ++i );
}

void CWK_CREATE_TASK(task_no, task_name, task_file)
int task_no;
char *task_name;
char *task_file;
{
    char msg[100];
    int ch;

    if ( task_no > MAX_TSKS ) {
	printf("Can't work with more than %d tasks...", MAX_TSKS-1);
	}
    /*
	Find the task particulars in the task file (For now, hard code them)
    */
	CWK_TASKS[task_no].TN = malloc( strlen(task_name)+1 );
	strcpy( CWK_TASKS[task_no].TN, task_name );
	CWK_TASKS[task_no].FL = 1;
	CWK_TASKS[task_no].LL = 10000;
	CWK_TASKS[task_no].FLOS = 0;
	CWK_TASKS[task_no].CL = 0;
	CWK_TASKS[task_no].MON = CWK_CREATE_MON_WIN(CWK_TASKS[task_no].TN, 
		(monitor_type == 1) ? 1 : 0 );
        CWK_TASKS[task_no].FD = CWK_LOAD_FILE(task_file);
}

/*************************************************************************/
/*									 */
/*	CWK_Switch_Block is used to make sure the correct program block  */
/*	(ie procedure, package, task, etc) is being used.  When a program*/
/*	block becomes current, we want to make sure that we then move to */
/*	use that program block.  Program blocks are entered and exited   */
/*	inta.c.  We call this routine from there.			 */
/*									 */
/*	parameters:							 */
/*		block_number : the block number of this block	  	 */
/*		block_name   : the title of the block (ie proc name)     */
/*		block_file   : file which contains this block            */
/*	 	enter_or_exit: Are we entering or exiting this block     */
/*			       This is only used in procedure monitoring */
/*			       If this is 1 - exiting, 0 - entering	 */
/*									 */
/*************************************************************************/

CWK_Switch_Block( block_number, block_name, block_package, block_file, 
		  enter_or_exit )
int block_number, enter_or_exit;
char *block_name, *block_package, *block_file;
{
	FILE_REC_PTR Save_FD;

	/*
		In the case of task monitoring, if the new procedure is in
		a different source file from the one currently being used,
		move to that source file.
	*/ 
	{
	char msg[80];
	sprintf( msg, "block_number = %d, block_package = %s\n", 
		block_number, block_package );
	_outtext( msg );
	getch();
	}
	if ( task_monitor )
	{
			Save_FD = CWK_TASKS[current_task].FD;
			CWK_TASKS[current_task].FD = CWK_LOAD_FILE(block_file);
			if ( Save_FD != CWK_TASKS[current_task].FD )
	    			CWK_TASKS[current_task].FLOS = 0;
	}

	/*
		Procedure monitoring
	*/
	else
	{
		/*
			The save_block_number remembers the block between
			calls.  The first time through, however, it must be set
		*/
		if ( save_block_number == -1 )
			save_block_number = block_number;

		/*
			If this block hasn't been created, and we have a valid
			block, create it.
		*/
		if  ( CWK_BLKS[block_number].MON == NULL ) 
			CWK_CREATE_BLOCK(block_number, block_name, block_file );

		/*
			Only worry about monitor windows if the monitor is
			running.
		*/

		if ( monitor_type != 1 )
			return;
		
		if ( monitor_window_type == 3 )
		{
			save_block_number = block_number;
			CWK_DRAW_PROC_WIN( CWK_BLKS[block_number].MON, 
				enter_or_exit );

	    		CWK_BLKS[save_block_number].FLOS = 0;
#if 0
			CWK_SET_TASK_LINE( CWK_BLKS[block_number].CL, 0 );
#endif

			/*
				If the mode is to be in task step mode,
				then wait for a key to be pressed to
				move to the next step, otherwise, wait
				a second to keep the screen from flashing.
			*/

			if ( task_step )
	     			CWK_GET_KEY( 1 );
			else
				busy_wait( MAX_DELAY-1 );
		}
		else
			save_block_number = block_number;
			
	}
}

CWK_LEAVE_BLOCK( block_number )
int block_number;
{
	if ( monitor_type != 1 )
		return;
	if ( ! task_monitor )
		CWK_REMOVE_BLOCK( CWK_BLKS[block_number].MON);
}

/*************************************************************************/
/*************************************************************************/
    
void CWK_DISTROY_TASK(task_no)
int task_no;
{
	CWK_TASKS[task_no].TN = "";
	CWK_TASKS[task_no].FL = 0;
	CWK_TASKS[task_no].LL = 0;
	CWK_DEL_MON_WIN(CWK_TASKS[task_no].MON);
}
    
void CWK_CHANGE_TASK_STAT(task_no, stat, task_name, task_file )
int task_no;
char stat;
char *task_name, *task_file;
{
	int FLOS;
	FLOS = CWK_TASKS[task_no].FLOS;

	if ( ! task_monitor )
		return;

	CWK_TASKS[task_no].ST = stat;

	switch( stat ) {
	    case 'C' :      /* Create a new task */
		CWK_CREATE_TASK( task_no, task_name, task_file );
		CWK_TASKS[task_no].ST = '*';
		break;
	    case 'A' :     /* Activate a task */
		CWK_SET_TASK( task_no );
		CWK_TASKS[task_no].ST = '*';
		if ( ( task_step ) && ( CWK_TASKS[task_no].MON->WON == 1 ) )
	     		CWK_GET_KEY( 1 );
		break;
	    case 'S' :    /* Change to a new task */
		CWK_SET_TASK( task_no );
		CWK_TASKS[task_no].ST = '*';
		break;
	    case 'D' :    /* Delaying task       */
		if ( monitor_type == 1 )
		{
			CWK_CUR_LINE( CWK_TASKS[task_no].MON, 
				CWK_TASKS[task_no].CL- FLOS, " D " );
			if ( ( task_step ) && 
			     ( CWK_TASKS[task_no].MON->WON == 1 ) )
	     			CWK_GET_KEY( 1 );
		}
		break;
	    case 'R' :    /* Waiting Rendezvous  */
		if ( monitor_type == 1 )
		{
			CWK_CUR_LINE( CWK_TASKS[task_no].MON,
				CWK_TASKS[task_no].CL- FLOS, " R " );
			if ( ( task_step ) && 
			     ( CWK_TASKS[task_no].MON->WON == 1 ) )
	     			CWK_GET_KEY( 1 );
		}
		break;
	    case 'M' :    /* Rendezvous Met  */
		if ( monitor_type == 1 )
		{
			CWK_CUR_LINE( CWK_TASKS[task_no].MON,
				CWK_TASKS[task_no].CL- FLOS, " M " );
			if ( ( task_step ) && 
			     ( CWK_TASKS[task_no].MON->WON == 1 ) )
	     			CWK_GET_KEY( 1 );
		}
		break;
	    case 'E' :    /* Rendezvous Met  */
		CWK_DISTROY_TASK( task_no );
		break;
	    }
}

void CWK_SET_TASK_LINE( line_no, getkey_or_not )
int line_no, getkey_or_not;
{
	int i;
	int FLOS;
	static text_string[50], format_string[50];

	TASK_REC *temp_task;
	int task_no;

	if ( task_monitor )
	{
		task_no = current_task;
		temp_task = CWK_TASKS;
	}
	else
	{
		task_no = save_block_number;
		temp_task = CWK_BLKS;
	}

        if ( temp_task[task_no].FD->lines_in_file < line_no )
	{
		return;
	}

	if ( ( temp_task[task_no].MON->WON != 1 )  ||
	     ( monitor_type != 1 ) )
	{
		return;
	}

	FLOS = temp_task[task_no].FLOS;
	if ( FLOS != 0 ) 
	    CWK_CUR_LINE(temp_task[task_no].MON, 
		   temp_task[task_no].CL - FLOS, "   ");
	if (( FLOS == 0 ) ||
	   (( line_no < FLOS ) || (line_no > FLOS + WIN_SIZE))) {
		FLOS = line_no - (WIN_SIZE / 2);
		if (FLOS < 1) 
			FLOS = 1;
		temp_task[task_no].FLOS = FLOS;
		for ( i = 0; i < WIN_SIZE; ++i )
			{
			/*   Blank out the current line                     */

	    		CWK_CUR_LINE(temp_task[task_no].MON, i+1, "   ");
			sprintf( format_string,"%c%d.%ds", '%', 
				LINE_SIZE+4-Block_Level,
				LINE_SIZE+4-Block_Level );
			sprintf( text_string, format_string, " " );
		 	CWK_WRITE_LINE(temp_task[task_no].MON, i+1,  
				text_string );
#if 0
			if ( ( monitor_window_type == 0 ) || 
			     ( monitor_window_type == 2 ) )
		 		CWK_WRITE_LINE(temp_task[task_no].MON, i+1, 
				"                                " );
			else
		 		CWK_WRITE_LINE(temp_task[task_no].MON, i+1, 
				"                                                                    " );

#endif
			/* Stop putting out lines when you have reached */
			/* the end of the file so you don't access bad  */
			/* memory                                       */

        		if (temp_task[task_no].FD->lines_in_file < 
					FLOS+i)
			{
				continue;
			}

			/* Now write out the line to the screen.  Note that */
			/* the use of different window types was stuffed in */
			/* later int he program life, and so the string size*/
			/* for a window is handled here, not in cwkgraph    */
			/* where it belongs!				    */

			sprintf( format_string,"%c3.3d %c-%d.%ds", '%', '%', 
				LINE_SIZE-Block_Level, LINE_SIZE-Block_Level );
			sprintf( text_string, format_string, FLOS+i,
				    temp_task[task_no].FD->lines[FLOS+i]);
#if 0
			if ( ( monitor_window_type == 0 ) || 
			     ( monitor_window_type == 2 ) )
				sprintf( text_string, "%3.3d %-28.28s", FLOS+i,
				    temp_task[task_no].FD->lines[FLOS+i]);
			else
				sprintf( text_string, "%3.3d %-68.68s", FLOS+i,
				    temp_task[task_no].FD->lines[FLOS+i]);
#endif
		 	CWK_WRITE_LINE(temp_task[task_no].MON, i+1, 
					text_string);
			}
		}
	
	temp_task[task_no].CL = line_no;
	CWK_CUR_LINE(temp_task[task_no].MON, line_no - FLOS, "-->");

	if ( getkey_or_not )
	    CWK_GET_KEY( 0 );
}

void CWK_SET_TASK(task_no)
int task_no;
{
	int ch;

	current_task = task_no;
}

/***********************************************************************/
/* This section of code handles the control screen .                   */
/***********************************************************************/

void print_status()
{
	int iline;
	int save_current_task;

	if ( rr_flag )
        	CWK_CONTROL_TASK( "Round Robin", CWK_TASKS[current_task].TN,
		Run_Total_Time, 0 );
	else
        	CWK_CONTROL_TASK( "Run til blk", CWK_TASKS[current_task].TN,
		Run_Total_Time, 0 );
	scroll_tasks();
	CWK_ERASE_ALL_MON_WIN();
	
	/*    
		Save the current task.  This is because I want to use the
		routine CWK_SET_TASK_LINE to redraw the windows.  It requires
		current_task to be the task redrawn.  So I set it in the loop,
		and at the end restore the value.
	*/

	save_current_task = current_task;
	for ( iline = 0; iline < MAX_TSKS; ++iline ) {
		if ( CWK_TASKS[iline].TN != NULL ) {
#if 0
			CWK_TASKS[iline].Cur_Time = 0;
#endif
			if ( CWK_TASKS[iline].MON->WON == 1 ) {
				current_task = iline;
				CWK_DRAW_MON_WIN( CWK_TASKS[iline].MON );
				CWK_TASKS[iline].FLOS = 0;
				CWK_SET_TASK_LINE( CWK_TASKS[iline].CL, 0 );
			}
			else 
				CWK_TASKS[iline].MON->WON = -1;
		}
	}
	current_task = save_current_task;
}

#define START_LINE 4
#define LINES 8
static void scroll_tasks()
{
	int ch, iline, cur_task, Active_Tasks = 0;
	int stay_flag;
	char msg[100];
	int First_Task_On_Screen = 1;

  	sprintf( msg,"Task #   Task Type        File           Line #   Stat WON  Exe %1.1c", '%');
        CWK_PRINT_CONTROL_LINE( START_LINE - 1, msg );

	Active_Tasks = print_control_lines( First_Task_On_Screen );
	

	iline = START_LINE;
        CWK_PRINT_CONTROL_LINE(iline, "->");
	stay_flag = 1;
	while (stay_flag) {
	    ch = getch();
	    switch (ch ) {
	        case UPARROW : 
        	    CWK_PRINT_CONTROL_LINE(iline, "  ");
		    if ( --iline < START_LINE ) {
		 	if ( First_Task_On_Screen != 1 ) {
			    First_Task_On_Screen -= (LINES - 1);
			    Active_Tasks = print_control_lines( First_Task_On_Screen );
			}
			iline = START_LINE;
		    }
        	    CWK_PRINT_CONTROL_LINE(iline, "->");
		    break;
	        case DOWNARROW : 
        	    CWK_PRINT_CONTROL_LINE(iline, "  ");
		    if ( ++iline >= (START_LINE + Active_Tasks) ) {
			if ( iline <= LINES + START_LINE - 1 )
			    iline = START_LINE + Active_Tasks - 1;
			else {
			    First_Task_On_Screen += (LINES - 1);
			    Active_Tasks = print_control_lines( First_Task_On_Screen );
			    iline = START_LINE;
			}
		    }
        	    CWK_PRINT_CONTROL_LINE(iline, "->");
		    break;
		case 't' :
		case 'T' :
		    cur_task = iline - START_LINE + First_Task_On_Screen;
		    if ( CWK_TASKS[cur_task].MON->WON == 1 )
			CWK_TASKS[cur_task].MON->WON = -1;
		    else if (( CWK_TASKS[cur_task].MON->WON == 0 ) || 
		    	    ( CWK_TASKS[cur_task].MON->WON == -1 ) )
			CWK_TASKS[cur_task].MON->WON = 1;
			sprintf( msg, "-> %2.2d     %-15.15s  %-15.15s %4d     %1.1c    %-3.3s  %3.2f  ", 
			    cur_task, CWK_TASKS[cur_task].TN, 
			    CWK_TASKS[cur_task].FD->FN,
			    CWK_TASKS[cur_task].CL,
			    CWK_TASKS[cur_task].ST, 
			    ( CWK_TASKS[cur_task].MON->WON == 1) ? "yes" : "no",
			    (float) CWK_TASKS[cur_task].Cur_Time /
			    (float)Run_Current_Time );
			
        	    CWK_PRINT_CONTROL_LINE(iline, msg);
		    break;
		case ESC :
		    stay_flag = 0;
		    break;
		default : 
		    break;
	    }
	}
	CWK_DEL_CONTROL();
}

int
print_control_lines( First_Task_On_Screen )
int First_Task_On_Screen;
{
	int iline, Active_Tasks = 0;
	char msg[80];

	for ( iline=START_LINE; iline < LINES + START_LINE; ++iline ){
		sprintf( msg, "%67s", " ");
		CWK_PRINT_CONTROL_LINE( iline, msg );
	}

	for ( iline=First_Task_On_Screen; 
	      iline < MAX_TSKS  && (iline - First_Task_On_Screen) < LINES; 
	      iline++ )
	{
		if ( CWK_TASKS[iline].TN == NULL )
			continue;
		Active_Tasks++;
		sprintf( msg, "   %2.2d     %-15.15s  %-15.15s %4d     %1.1c    %-3.3s  %3.2f  ", 
			iline, CWK_TASKS[iline].TN, 
			CWK_TASKS[iline].FD->FN,
			CWK_TASKS[iline].CL,
			CWK_TASKS[iline].ST, 
			( CWK_TASKS[iline].MON->WON == 1) ? "yes" : "no",
			(float) CWK_TASKS[iline].Cur_Time /
				(float)Run_Current_Time );
        	CWK_PRINT_CONTROL_LINE( iline+START_LINE-First_Task_On_Screen,
				 	msg );
	}
	return Active_Tasks;
}
#undef LINES
#undef START_LINE
/***********************************************************************/


void CWK_GET_KEY( other_stop )
int other_stop;
{
	int ch = 0;

	if ( other_stop == -1 ) {
	    if (kbhit()) {
	        ch = getch();
	        }
	    }
	else if ( ( step_monitor )  || ( other_stop ) ){
	    ch = getch();
	    }
	else if (kbhit()) {
	    ch = getch();
	    }

	if ( ch != 0 ) {
	    switch ( ch ) {
		case CTRL_L :
		    step_monitor = ++step_monitor % 2;
		    break;
		case UPARROW :
		    step_rate--;
		    if ( step_rate < 0 )
			step_rate = 0;
		    break;
		case DOWNARROW :
		    step_rate++;
		    if ( step_rate >= MAX_DELAY )
			step_rate = MAX_DELAY - 1;
		    break;
	        case CTRL_A :
		    print_status();
		    break;
	        case CTRL_B :
		    CWK_STOP_MONITORING();
		    break;
	        case CTRL_D :
		    /*	With procedure monitoring in, this will allow a */
	            /*	quick fix to put up multiple screens.		*/
		    /*      if ( task_monitor ) */
		    	CWK_SETUP_SCREEN();
		    break;
	        case CTRL_H :
		    print_help();
		    break;
	        case CTRL_K :
		    print_license();
		    break;
	        case CTRL_R :
		    CWK_Resize_Window();
		    break;
	        case CTRL_T :
		    task_step = ++task_step % 2;
		    break;
		case ESC :
			CWK_CLEANUP_MON(1);
			break;
		default :
		    break;
		}
	    }

}

CWK_TIME_TASK()
{
	int iline;

	Run_Total_Time++; Run_Current_Time++; 
	CWK_TASKS[current_task].Tot_Time++; CWK_TASKS[current_task].Cur_Time++;
	busy_wait(step_rate);

	if ( ( Run_Total_Time % Time_To_Read_Key ) == 0 )
	{
		CWK_GET_KEY(-1);
		if ( monitor_exec_time )
			CWK_REPORT_TIME( Run_Total_Time );
	}
	if ( ( Run_Total_Time % Exec_Report_Time ) == 0 )
	{
		if ( monitor_type == 2 )
			CWK_Next_Pct_Rpt();
		Run_Current_Time = 0;
		for ( iline = 0; iline < MAX_TSKS; ++iline ) {
			if ( CWK_TASKS[iline].TN != NULL ) 
				CWK_TASKS[iline].Cur_Time = 0;
		}
	}
}

/***********************************************************************/
/*    This routine turns off the monitoring windows.                   */
/***********************************************************************/

CWK_STOP_MONITORING()
{
	int iline;

	/*  Stop the monitoring of the lines */
	if ( monitor_type == 1 )
	{
	CWK_ERASE_ALL_MON_WIN();
	    for ( iline = 1; iline < MAX_TSKS; ++iline ) {
		if ( CWK_TASKS[iline].TN != NULL ) 
			CWK_TASKS[iline].MON->WON = -1;
	    }
	}

	/*  Stop the monitoring of execution percentage */
	else if ( monitor_type == 2 )
	{
		CWK_DEL_EXEC_PCT();
	}

	monitor_type = 0;
}

/***********************************************************************/
/*   CWK_Next_Pct_Rpt reports the Pct of CPU time run since the last   */
/*   report.                                                           */
/***********************************************************************/

CWK_Next_Pct_Rpt()
{
	char msg[81], msg1[81];
	int iline;
	
	CWK_EXEC_PCT();

	for ( iline = 0; iline < ( MAX_TSKS / 4 ); ++iline )
	{
		msg[0] = '\0';
		if ( CWK_TASKS[(4*iline)+1].TN != NULL )
		{
			sprintf( msg, "%2.2d  %-7.7s %1.1c %3.2f ", 
				(4*iline)+1, CWK_TASKS[(4*iline)+1].TN, 
				CWK_TASKS[(4*iline)+1].ST, 
				(float) CWK_TASKS[(4*iline)+1].Cur_Time /
					(float)Run_Current_Time );
		}
		if ( CWK_TASKS[(4*iline)+2].TN != NULL )
		{
			sprintf( msg1, "%2.2d  %-7.7s %1.1c %3.2f ", 
				(4*iline)+2, CWK_TASKS[(4*iline)+2].TN, 
				CWK_TASKS[(4*iline)+2].ST, 
				(float) CWK_TASKS[(4*iline)+2].Cur_Time /
					(float)Run_Current_Time );
		strcat( msg, msg1 );
		}
		if ( CWK_TASKS[(4*iline)+3].TN != NULL )
		{
			sprintf( msg1, "%2.2d  %-7.7s %1.1c %3.2f ", 
				(4*iline)+3, CWK_TASKS[(4*iline)+3].TN, 
				CWK_TASKS[(4*iline)+3].ST, 
				(float) CWK_TASKS[(4*iline)+3].Cur_Time /
					(float)Run_Current_Time );
		strcat( msg, msg1 );
		}
		if ( CWK_TASKS[(4*iline)+4].TN != NULL )
		{
			sprintf( msg1, "%2.2d  %-7.7s %1.1c %3.2f ", 
				(4*iline)+4, CWK_TASKS[(4*iline)+4].TN, 
				CWK_TASKS[(4*iline)+4].ST, 
				(float) CWK_TASKS[(4*iline)+4].Cur_Time /
					(float)Run_Current_Time );
		strcat( msg, msg1 );
		}
        	CWK_EXEC_PRINT_LINE( iline+2, msg );
	}
}

/***********************************************************************/
/*   CWK_Exception_Raised reports exceptions to the screen when running*/
/*   the monitor.						       */
/***********************************************************************/

CWK_Exception_Raised( task_id, msg )
int task_id;
char *msg;
{
	if ( ! task_monitor )
	{
		CWK_Report_Exception( "main", msg );
		return;
	}
	else
	{
	    if ( task_id == 0 )
		task_id = current_task;
	    if ( exception_trace )
		CWK_Report_Exception( CWK_TASKS[task_id].TN, msg );
	}
}
