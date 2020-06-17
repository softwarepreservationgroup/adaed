/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "config.h"
#include "ivars.h"
#include "slot.h"
#include "ifile.h"
#include "intaprots.h"
#include "loadprots.h"
#include "miscprots.h"
#include "libfprots.h"
#ifdef vms
#include descrip
#endif

#ifdef DEBUG
#ifdef DEBUG_STORES
int heap_store_offset = 0;
#endif
#endif

static void fold_upper(char *);

/* global variable needed for imain.c, derived from generator */
FILE *MALFILE;
FILE *efopen();

main(int argc, char **argv)											/*;main*/
{
	int         c, i, n, status;
	int         errflg = 0, nobuffer = 0;
	char        *FILENAME;
	IFILE       *ifile;
	extern int  optind;
	extern char *optarg;
	char        *library_name;
	char	*t_name;
	Axq		axq;
	char	*main_unit = (char *)0;
	char 	*tname;
	int		malloc_trace = 0;
#ifdef vms
	char        buffer[50];
	short       rlength;
	struct      dsc$descriptor_s entity_desc;
	struct      dsc$descriptor_s value_desc;
#endif

	max_mem = MAX_MEM;
#ifdef IBM_PC
	new_task_size = 2048;
	main_task_size = 4096;
#else
	main_task_size = 10000;
	new_task_size = 10000;
#endif
#ifdef vms
	entity_desc.dsc$b_dtype = DSC$K_DTYPE_T;
	entity_desc.dsc$b_class = DSC$K_CLASS_S;
	entity_desc.dsc$a_pointer = "TRACE";
	entity_desc.dsc$w_length = 5;
	value_desc.dsc$b_dtype = DSC$K_DTYPE_T;
	value_desc.dsc$b_class = DSC$K_CLASS_S;
	value_desc.dsc$a_pointer = buffer;
	value_desc.dsc$w_length = 50;
	status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
	printf("TRACE status %d\n",status);
#endif
	while (status & 1) {
		status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
#ifdef DEBUG
		value_desc.dsc$a_pointer[rlength] = '\0';
		printf("TRACE %s\n", value_desc.dsc$a_pointer);
#endif
		switch (value_desc.dsc$a_pointer[0]) {
		case 'A': /* ADALINE    */
			line_trace++;  
			break;
		case 'C': /* CALL       */
			call_trace++;  
			break;
		case 'D': /* DEBUG      */
			debug_trace++;  
			break;
		case 'E': /* EXCEPTION  */
			exception_trace++;  
			break;
		case 'R': /* RENDEZVOUS */
			rendezvous_trace++;  
			break;
		case 'S': /* SIGNAL     */
			signal_trace++;  
			break;
		case 'T': /* TASKING    */
			tasking_trace++;  
			break;
		}
		if (status == 1) break;
	}
	entity_desc.dsc$a_pointer = "HEAP_SIZE";
	entity_desc.dsc$w_length = 9;
	status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
	printf("HEAP_SIZE status %d\n",status);
#endif
	if (status & 1) {
		status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
		value_desc.dsc$a_pointer[rlength] = '\0';
		max_mem = atoi(value_desc.dsc$a_pointer);
#ifdef DEBUG
		printf("HEAP_SIZE %d\n", max_mem);
#endif
		max_mem = 1000 * max_mem;
	}
	entity_desc.dsc$a_pointer = "MAIN_UNIT";
	entity_desc.dsc$w_length = 9;
	status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
	printf("MAIN_UNIT status %d\n",status);
#endif
	if (status & 1) {
		status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
		value_desc.dsc$a_pointer[rlength] = '\0';
		main_unit = strjoin(value_desc.dsc$a_pointer,"");
		fold_upper(main_unit);
#ifdef DEBUG
		printf("MAIN_UNIT %s\n", main_unit);
#endif
	}
	entity_desc.dsc$a_pointer = "LIBRARY";
	entity_desc.dsc$w_length = 7;
	status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
	printf("LIBRARY status %d\n",status);
#endif
	if (status & 1) {
		status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
		value_desc.dsc$a_pointer[rlength] = '\0';
		library_name = strjoin(value_desc.dsc$a_pointer,"");
#ifdef DEBUG
		printf("LIBRARY %s\n", library_name);
#endif
	}
#else
#ifndef DEBUG_STORES
	while((c = getopt(argc, argv, "bf:h:m:p:s:t:")) != EOF) {
#else
	while((c = getopt(argc, argv, "bf:h:m:p:s:t:w:")) != EOF) {
#endif
		/*    user:
		 *      h       heap size in kilobytes
		 *	m	main unit name
		 *	p	main program task stack size
		 *	s	task stack size
		 *	t	tracing, followed by list of options:
		 *                a	Ada lines
		 *                c	calls 
		 *                e	exceptions
		 *                r	rendezvous
		 *                t	tasks
		 *    debug (only):
		 *                d	debug
		 *		  i 	instruction
		 *		  m	malloc
		 *                s	signals
		 *	b	do not buffer standard output
		 *	f	file i/o trace, followed by list of options
		 *		a	trace axq files
		 *		d	do not include descriptors in trace
		 *		n	do not include file numbers in trace
		 *		l	trace lib files
		 *	w	off	trace stores at specified offset in heap
		 */
		switch(c) {
#ifdef DEBUG
		case 'b':	/* do not buffer standard output (for debugging) */
			nobuffer++;
			break;
		case 'f':	/* process ifile trace options */
			n = strlen(optarg);
			for (i = 0;i < n; i++) {
				switch (optarg[i]) {

				case 'a':
					iot_ais_r = 2; 
					break;
				case 'l':
					iot_lib_r = 2; 
					break;
				case 'n': 
					iot_set_opt_number(0);
					break;
				case 'd': 
					iot_set_opt_desc(0); 
					break;
				}
			}
			break;
#ifdef DEBUG_STORES
		case 'w':	/* storage write trace */
			heap_store_offset = atoi(optarg);
			break;
#endif
#endif
		case 'h': /* heap size in kilo bytes */
#ifndef IBM_PC
			max_mem = 1000*atoi(optarg);
#else
			{
				int optval; /* avoid too large value */
				optval = atoi(optarg);
				if (optval > 0 && optval < MAX_MEM/1000) 
					max_mem = 1000*optval;
			}
#endif
			break;
		case 'm': /* specify main unit name */
			main_unit = strjoin(optarg,"");
			fold_upper(main_unit);
			break;
		case 'p': /* main task stack size */
			i = atoi(optarg);
			if (i > 0 && i < 31)	 /* small value gives kilowords */
				main_task_size = i * 1024;
			else if (i > 31)
				main_task_size = i;
			break;
		case 's': /* task stack size */
			i = atoi(optarg);
			if (i > 0 && i < 31)	/* small value gives kilowords */
				new_task_size = i * 1024;
			else if (i > 31)
				new_task_size = i;
			break;
		case 't': /* interpreter trace arguments */
			n = strlen(optarg);
			for (i = 0; i < n; i++) {
				switch(optarg[i]) {
				case 'c': /* calls */
					call_trace++;
					break;
				case 'e': /* exceptions */
					exception_trace++;
					break;
				case 'a': /* Ada lines */
					line_trace++;
					break;
				case 'r': /* rendezvous */
					rendezvous_trace++;
					break;
				case 't': /* tasks */
					tasking_trace++;
					break;
#ifdef DEBUG
				case 'd': /* debug */
					debug_trace++;
					break;
				case 'i': /* instructions */
					instruction_trace++;
					break;
				case 'm': /* malloc */
					malloc_trace++;
					break;
				case 's': /* signals */
					signal_trace++;
					break;
#endif
				default:
					errflg++;
					break;
				}
			}
			break;
		case '?':
			errflg++;
		}
	}
#ifdef DEBUG
	if (debug_trace)
		printf("program, new task stack sizes %d %d\n",
		  main_task_size, new_task_size);
#endif
	if (optind < argc)
		library_name = argv[optind];
	else {
		library_name = getenv("ADALIB");
	}
	if (library_name == (char *)0)
		errflg++;
	if (errflg) {
		fprintf(stderr,
		  "Usage: adaexec -m main_unit -h size -t[acert] library\n");
		exit(RC_ABORT);
	}
#endif
#ifdef DEBUG
	if (nobuffer)
		setbuf(stdout,(char *) 0);/* do not buffer output(for debug) */
	if (malloc_trace) {
		trace_malloc();
		tname = emalloc((unsigned)strlen(library_name) + 5);
		MALFILE = efopen(strcat(strcpy(tname,library_name),".mai"),"w","t");
		efreet(tname,"temp-file-name");
	}
#endif
	FILENAME = library_name;
	t_name = libset(library_name);

	/* AXQFILE is opened by load_axq or library read (TBSL);*/
	axq = (Axq) emalloc((unsigned) sizeof(Axq_s));
	load_slots(FILENAME, &ifile, axq);
	/* second arg to load_lib and load_axq is non-null if file open */
	load_lib(FILENAME, ifile, axq, main_unit, argv);

	status = int_main();
#ifdef vms
	exit();
#else
	exit(status);
#endif
}

static void fold_upper(char *s)					/*;fold_upper*/
{
	char c;

	while (*s) {
		c = *s;
		if (islower(c)) *s = toupper(c);
		s++;
	}
}
