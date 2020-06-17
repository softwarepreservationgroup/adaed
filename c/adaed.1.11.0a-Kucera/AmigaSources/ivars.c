/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
/*    +---------------------------------------------------+
      |                                                   |
      |      I N T E R P R E T E R   V A R I A B L E S    |
      |                                                   |
      |                    (C Version)                    |
      |                                                   |
      |   Adapted From Low Level SETL version written by  |
      |                                                   |
      |                    Monte Zweben                   |
      |                 Philippe Kruchten                 |
      |                 Jean-Pierre Rosen                 |
      |                                                   |
      |    Original High Level SETL version written by    |
      |                                                   |
      |                     Clint Goss                    |
      |                  Tracey M. Siesser                |
      |                  Bernard D. Banner                |
      |                  Stephen C. Bryant                |
      |                    Gerry Fisher                   |
      |                                                   |
      |                 C version written by              |
      |                                                   |
      |                  Robert B. K. Dewar               |
      |                                                   |
      +---------------------------------------------------+     */


/* Revision History
  19 Jun 1985, 10:11 (RBKD)
  First complete version, from low level version 10-23-1984
 */
#include <stdlib.h>
#include "config.h"
#include "int.h"
/*  This module contains variables used in all parts of the interpretor */

/*
 *  Memory lay-out
 *  --------------
 */

 int *stack_segments [MAX_TASKS]; /* one stack segment for each task */
 int task_stackptr [MAX_TASKS];   /* stack pointers for task stacks */
 unsigned int data_segments_dim;  /* dimension of data_segments */
 int **data_segments;             /* one data segment for each cunit */
 int *data_seglen;                /* length of data segments */
 unsigned int code_segments_dim;  /* dimension of code_segments */
 char **code_segments;            /* segments containing code */
 int *code_seglen;                /* length of code segments */
 int *cur_stack;                  /* current stack segment */
 int cur_stackptr;                /* stack ptr for cur_stack */
 char *cur_code;                  /* current code segment */
 int num_cunits;                  /* number of compilation units */

/*
 *  Context of a task
 *  -----------------
 */

 int cs;       /* index of current code_segment in code_segments */
 int ip;       /* instruction pointer in cur_code */
 int sp;       /* stack pointer */
 int sfp;      /* stack frame pointer */
 int bfp;      /* block frame pointer */
 int exr;      /* exception register */
 int lin;      /* line number for tracing */

/*
 *  Global context (not preserved by context switch)
 *  ------------------------------------------------
 */

 int tp;                /* index of current task in stack_segments */
 int *heap_next;        /* top of heap for allocation */
 int heap_base;         /* segment number of heap */
 int *heap_addr;        /* address of beginning of heap */
 long next_clock;        /* next time to call clock_interrupts */
 long time_offset;       /* cumulative skipped time for simulated time */
 int next_clock_flag;   /* request to check the clock (task waiting) */
 int simul_time_flag;   /* run with simulated time (versus real time) */
 int time_slicing_flag; /* time slicing (vs schedule at system call) */
 long time_slice;        /* length of time slice (500 ms default) */
 int tasking_trace;     /* TRUE to trace tasking */
 int rendezvous_trace;  /* TRUE to trace rendezvous */
 int signal_trace;      /* TRUE to trace signalling */
 int debug_trace;       /* TRUE for general debugging trace */
 int exception_trace;   /* TRUE to trace exceptions */
 int call_trace;        /* TRUE to trace calls */
 int line_trace;        /* TRUE to trace line numbers */
 int instruction_trace; /* TRUE to trace instructions */

/* Number of slots */

 unsigned int data_slots_dim;      /* dimension of data_slots */
 char **data_slots;
 unsigned int code_slots_dim;      /* dimension of code_slots */
 char **code_slots;
 unsigned int exception_slots_dim; /* dimension of exception_slots */
 char **exception_slots;

/* Global Variables (for main interpreter) */

 int max_mem;                      /* size of a heap segment */
 int w;                            /* used by GET_WORD */
 struct tt_fx_range *temp_template;/* fixed point work template */

 int bse,bas1,bas2,bas3,bas4;      /* address base values */
 int off,off1,off2,off3,off4;      /* address offset values */
 int *ptr,*ptr1,*ptr2,*ptr3,*ptr4; /* memory addresses */
 int value,val1,val2;              /* operands for int operations */
 int length1,length2;              /* for array operations */
 long lvalue,lval1,lval2;          /* operands for long operations */
 long fval1,fval2,fvalue;          /* operands for fixed operations */
 float rvalue,rval1,rval2;         /* operands for float operations */
 int val_high,val_low;             /* low/high of INT range */
 int result,delta;                 /* used in fixed point */
 int size;                         /* size of object */
 int attribute;                    /* attribute code */
 int slot;                         /* for create structure */
 int component_size;               /* size of array component */
 long now_time;                     /* current time */
 int to_type;                      /* type from template */
 int discr_list[MAX_DISCR];        /* accumulate list of discriminants */
 int nb,i;                         /* loop counters etc */
 int overflow;                     /* overflow flag from arith routines */
 int nb_discr;                     /* number of discriminants */
 int nb_dim;                       /* dimension of array */
 int ratio,power;                  /* used in attribute processing */
 int d,n;                          /* temporaries */
 int old_cs;                       /* old value of cs */
 int jump;                         /* jump address */
 int lim;                          /* limit in for loop */
 struct bf *bfptr;                 /* pointer to block frame */
 int *tfptr1,*tfptr2;              /* pointers to task frames */
 int cur_col;                      /* temporary for predef */

/*
 * I/O trace flags used to get trace prints when reading and writing
 * intermediate files. A trace level is one of the following:
 *               0           no trace
 *               1           brief trace
 *               2           full trace
 */
 int iot_ifile  =  (0); /* current trace level for ifile */
 int iot_ais_r  =  (0); /* trace level for ais files read */
 int iot_lib_r  =  (0); /* trace level for lib files read */
 char iot_ifile_type;

 int break_point;       /* next break point */  

 int fix_val1[20];      /* fixed point intermediate values */
 int fix_val2[20];     
 int fix_resu[20];     
 int fix_temp[20];     
 int mul_fact[20];     
 int div_fact[20];     

 int arith_overflow;
 int sgn;
 int last_task;
 int *free_list  =  ((int *)0);
 int raise_cs;
 int raise_lin;
 char *raise_reason;

/*
 *                             P R E D E F
 */

/* Define maximum number of files and AFCB vector to match */

/*
 * Note that file numbers are 1's origin subscripts into this vector, so that
 * the file number value of 1 corresponds to the first entry, i.e. afcbs[0]
 */

#define MAXFILES 20
 struct afcb *(afcbs[MAXFILES]);

/* Variables for predef */

 int operation;         /* operation for predef. set in the main loop */
 int data_exception;    /* exception to raise on data error */
 int filenum;           /* number of current file */
 int standard_in_file;  /* index of standard input file */
 int standard_out_file; /* index of standard output file */
 int current_in_file;   /* index of current default input file */
 int current_out_file;  /* index of current default output file */
 int current_in_file_saved;   
	 /* duplicate index of current default input file */
 int current_out_file_saved;  
	 /* duplicate index of current default output file */
 int file_offset;       /* Offset due to file argument in the stack */
                         /* 2 if explicit file, 0 if default file */
 struct tfile *tfiles;  /* linked list of temporary files */

/*
 * The area work_string is used to hold an ASCIIZ string. It is a strictly
 * temporary area which can be destroyed by almost any function. Anyone
 * who needs to keep this string value must copy it using make_string.
 *
 * Note that the maximum length of strings in the Ada system is 4K bytes.
*/

 char work_string[4096];

/* The task stack size for created tasks is given by new_task_size,
 * and the size of the main task is given by main_task_size.
 * These are initialized in imain.c; and can be set by program options
 * -s and -p, respectively.
 */

 int main_task_size;
 int new_task_size;
