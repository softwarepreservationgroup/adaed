/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#define GEN

#include <stdio.h>
#include <ctype.h>
#include "hdr.h"
#include "vars.h"
#include "gvars.h"
#include "libhdr.h"
#include "segment.h"
#include "ifile.h"
#include "slot.h"
#include "arithprots.h"
#include "dclmapprots.h"
#include "readprots.h"
#include "dbxprots.h"
#include "initprots.h"
#include "blibprots.h"
#include "libprots.h"
#include "glibprots.h"
#include "libfprots.h"
#include "librprots.h"
#include "libwprots.h"
#include "g0aprots.h"
#include "setprots.h"
#include "miscprots.h"
#include "gmiscprots.h"
#include "bmainprots.h"
#ifdef vms
#define vms_BINDER
#endif

#ifdef vms_BINDER
/*
#include descrip
#include "adabind.h"
*/
#endif

static void fold_upper(char *);
static void bpreface();
static void exitf(int);

/* Driver routine for ada gen */
char *argname;

IFILE	*AISFILE, *AXQFILE, *STUBFILE, *LIBFILE, *TREFILE;
FILE *MALFILE;
int list_unit_0 = 0; /* set by '0' option to list unit 0 structure */
int peep_option = 1; /* on for peep_hole optimization */
int adacomp_option = 0; /* set if called from adacomp */

extern Segment	CODE_SEGMENT, DATA_SEGMENT, DATA_SEGMENT_MAIN;
extern Segment   VARIANT_TABLE, FIELD_TABLE ;
char *lib_name;
#ifdef DEBUG
extern int zpadr_opt; /* not for EXPORT */
#endif

int main(int argc, char **argv)
{
	int		c, i, n, iot_level = 2;
	int		errflg = 0, nobuffer = 0, mflag = 0;
	extern int  optind;
	extern char *optarg;
	char	*t_name;
	char	*fname, *tfname, *source_name;
	int		r_trace = TRUE, w_trace = TRUE; /* trace modes for -f option */
#ifdef vms_BINDER
	int         status;
	char        buffer[50];
	short       rlength;
	struct      dsc$descriptor_s entity_desc;
	struct      dsc$descriptor_s value_desc;
#endif

	AISFILE = (IFILE *)0;
	AXQFILE = (IFILE *)0;
	LIBFILE = (IFILE *)0;
	STUBFILE = (IFILE *)0;
	TREFILE = (IFILE *)0;

	MAINunit = "";
	interface_files = "";

	while ((c = getopt (argc, argv, "c:g:f:m:i:")) != EOF)
		/*
		 *    user:
		 *	c	set if called from adacomp (errors in msg format).
		 *	f	file i/o trace, followed by list of options
		 *		a	trace ais files
		 *		d	do not include descriptors in trace
		 *		n	do not include file numbers in trace
		 *		r	subsequent traces for reading only
		 *		t	trace tre files
		 *		w	subsequenc traces for writing only
		 *		(traces initially for both r and w, use of r or w
		 *		limits further files traces to just that mode)
		 *		1	set trace level to 1
		 *		2	set trace level to 2
		 *	g	debugging, followed by list of options:
		 *		0	show structure of unit 0
		 *		M	malloc trace (including init_sem)
		 *		b	do not buffer standard output
		 *		e	flag signalling errors in the parsing phase
		 *		g	list generated code
		 *		l	show line numbers in generated code
		 *		m	malloc trace (after init_sem)
		 *		z	call trapini to initialize traps
		 *      i   to specify object files and libraries for pragma interface
		 *  	l	using library
		 *	m	main unit name
		 *  	n	new library
		 */
		switch (c) {
		case 'c': 
			adacomp_option++;
			source_name = malloc(strlen(optarg)+1);
			strcpy(source_name, optarg);
			break;
		case 'i':
			interface_files = strjoin(interface_files, optarg);
			interface_files = strjoin(interface_files, " ");
			break;
		case 'l': /* using existing library */
			break;
		case 'm': /* specify main unit name */
			MAINunit = malloc(strlen(optarg)+1);
			strcpy(MAINunit, optarg);
			fold_upper(MAINunit);
			break;
		case 'n': /* indicates new library */
			new_library = TRUE;
			break;
#ifdef DEBUG
		case 'f':	/* process ifile trace options */
			n = strlen(optarg);
			for (i = 0; i < n; i++) {
				switch (optarg[i]) {

				case 'o':
					/* turn off file offset trace */
					iot_off_info(0);
					break;
				case 'a':
					if (w_trace) iot_ais_w = iot_level;
					if (r_trace) iot_ais_r = iot_level;
					break;
				case 't':
					if (w_trace) iot_tre_w = iot_level;
					if (r_trace) iot_tre_r = iot_level;
					break;
				case 'l':
					if (w_trace) iot_lib_w = iot_level;
					if (r_trace) iot_lib_r = iot_level;
					break;
				case 'n': 
					iot_set_opt_number(0);
					break;
				case 'd': 
					iot_set_opt_desc(0); 
					break;
				case 'r': 
					w_trace= FALSE; 
					r_trace= TRUE; 
					break;
				case 'w': 
					r_trace = FALSE; 
					w_trace = TRUE; 
					break;
				case '1': 
					iot_level = 1; 
					break;
				case '2': 
					iot_level = 2; 
					break;
				}
			}
			break;
#endif
		case 'g': /* gen debug options */
			n = strlen(optarg);
			for (i = 0; i < n; i++) {
				switch((int)optarg[i]) {
#ifdef DEBUG
				case 'a':
					zpadr_opt = 0; /* do not print addresses in zpadr */
					break;
#endif
				case 'g':
					list_code++;
					break;
				case 'l':
					line_option++;
					break;
#ifdef DEBUG
				case 'b': /* do not buffer output */
					nobuffer++;
					break;
				case 'd': /* force debugging output */
					debug_flag++;
					break;
				case 'e':
					errors = TRUE;
					break;
				case 'o': /* disable optimization (peep) */
					peep_option = 0;
					break;
				case 'm': /* malloc trace */
					mflag++;
					break;
				case '0': /* read trace including unit 0 */
					list_unit_0++;
					break;
				case 'z': 
					trapini();
					break;
#endif
				}
			}
			break;
		case '?':
			errflg++;
		}
#ifdef IBM_PC
	if (!adacomp_option) {
		fprintf(stderr, "NYU Binder Version 1.7.2,");
		fprintf(stderr, " Copyright (C) 1985-1987 by New York University\n");
	}
#endif
	fname = (char *)0;
	if (optind < argc)
		fname = argv[optind];
	/* if fname not given, get from environment. */
	if (!errflg && fname == (char *)0) {
		fname = getenv("ADALIB");
		if (fname!= (char *)0 && !adacomp_option) {
#ifdef IBM_PC
			fprintf(stderr, "L");
#else
			fprintf(stderr, "l");
#endif
			fprintf(stderr, "ibrary defined by ADALIB: %s\n", fname);
		}
	}
	if (fname == (char *)0 || errflg) {
		fprintf (stderr, "Usage: adabind [-m main_unit] [library]\n");
		exitp(RC_ABORT);
	}
	lib_name = emalloc(strlen(fname) + 1);
	strcpy(lib_name, fname);
	t_name = libset(lib_name); /* set library */
	gen_option = FALSE; /* bind only */

#ifdef vms_BINDER
	if (!adacomp_option) {
		entity_desc.dsc$b_dtype = DSC$K_DTYPE_T;
		entity_desc.dsc$b_class = DSC$K_CLASS_S;
		value_desc.dsc$b_dtype = DSC$K_DTYPE_T;
		value_desc.dsc$b_class = DSC$K_CLASS_S;
		value_desc.dsc$a_pointer = buffer;
		value_desc.dsc$w_length = 50;
		entity_desc.dsc$a_pointer = "C";
		entity_desc.dsc$w_length = 1;
		status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
		printf("C status %d\n", status);
#endif
		if (status & 1) {
			status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
			value_desc.dsc$a_pointer[rlength] = '\0';
			adacomp_option++;
			source_name  = strjoin(value_desc.dsc$a_pointer, "");
#ifdef DEBUG
			printf("C %s\n", source_name);
#endif
		}


		entity_desc.dsc$b_dtype = DSC$K_DTYPE_T;
		entity_desc.dsc$b_class = DSC$K_CLASS_S;
		value_desc.dsc$b_dtype = DSC$K_DTYPE_T;
		value_desc.dsc$b_class = DSC$K_CLASS_S;
		value_desc.dsc$a_pointer = buffer;
		value_desc.dsc$w_length = 50;
		entity_desc.dsc$a_pointer = "MAIN_UNIT";
		entity_desc.dsc$w_length = 9;
		status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
		printf("MAIN_UNIT status %d\n", status);
#endif
		if (status & 1) {
			status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
			value_desc.dsc$a_pointer[rlength] = '\0';
			MAINunit = strjoin(value_desc.dsc$a_pointer, "");
			fold_upper(MAINunit);
#ifdef DEBUG
			printf("MAIN_UNIT %s\n", MAINunit);
#endif
		}
		entity_desc.dsc$a_pointer = "LIBRARY";
		entity_desc.dsc$w_length = 7;
		status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
		printf("LIBRARY status %d\n", status);
#endif
		if (status & 1) {
			status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
			value_desc.dsc$a_pointer[rlength] = '\0';
			fname = strjoin(value_desc.dsc$a_pointer, "");
#ifdef DEBUG
			printf("LIBRARY %s\n", fname);
#endif
		}
		lib_name = emalloc(strlen(fname) + 1);
		strcpy(lib_name, fname);
		t_name = libset(lib_name); /* set library */
		gen_option = FALSE; /* bind only */
	}
#endif
	tup_init(); /* initialize set and tuple procedures */
#ifdef DEBUG
	if (mflag) {
		trace_malloc();
		/* can't use strjoin to setup efopen arg as want trace ! */
		/*MALFILE = efopen(strjoin(FILENAME, ".mas"), "w", "t"); */
		tfname = malloc(strlen(fname) +4 + 1);
		MALFILE = efopen(strcat(strcpy(tfname, fname), ".mag"), "w", "t");
		free(tfname);
	}
#endif
	FILENAME = (fname != (char *)0) ? strjoin(fname, "") : fname;

	PREDEFNAME = predef_env();
	if (nobuffer) {
		setbuf (stdout, (char *) 0);	/* do not buffer output (for debug) */
	}
	rat_init(); /* initialize arithmetic and rational package*/
	dstrings_init(2048, 256); /* initialize dstrings package */
	init_sem();
	DATA_SEGMENT_MAIN = main_data_segment();
	aisunits_read = tup_new(0);
	init_symbols = tup_exp(init_symbols, seq_symbol_n);
	for (i = 1; i <= seq_symbol_n; i++)
		init_symbols[i] = seq_symbol[i];

	num_predef_units = init_predef();
	/*
	 * When the separate compilation facility is being used all references to
	 * AIS files will be made via the directory in LIBFILE. AISFILENAME is set
	 * to a number.
	 */
	if (new_library) 
		AISFILENAME = "1";
	else
		AISFILENAME = lib_aisname(); /* retrieve name from library */
	/* open the appropriate files using the suffix .axq for axq files and
	 * .trc for tree file. 
	 *
	 * Open MESSAGEFILE with suffixe ".msg" if a file name specified;
	 * otherwise, if a file name not required, and one is not given,
	 * used stderr.
	 */
	AXQFILE  = ifopen(AISFILENAME, "axq", "w", "a", iot_ais_w, 0);
	if (adacomp_option) {
		MSGFILE  = efopenl(source_name, "msg", "a", "t");
		/* unbuffer output for debugging purposes */
		setbuf(MSGFILE, (char *) 0);
	}
	else {
		MSGFILE = stdout;
	}
	bpreface();

	/* Code formerly procedure finit() in init.c is now put here directly */
	if (!errors) {
		write_glib();
		cleanup_files();
	}

	exitf(RC_SUCCESS);
}

static void fold_upper(char *s)								/*;fold_upper*/
{
	register char c;

	while (c = *s) {
		if (islower(c)) *s = toupper(c);
		s++;
	}
}

void fold_lower(char *s)										/*;fold_lower*/
{
	register char c;

	while (c = *s) {
		if (isupper(c)) *s = tolower(c);
		s++;
	}
}

static void bpreface()											/*;bpreface*/
{
	/* bpreface is version of preface for use with binder */

	int	i;
	Tuple	aisread_tup;

	aisread_tup = tup_new(0);
	initialize_1();
	/* 1- Load PREDEF */

	TASKS_DECLARED = FALSE;
	/* 2- Generate user program */

	initialize_2();

	ada_line = 9998;
	/* if binding, make ais_read tupe correspond to library */
	aisread_tup = tup_new(0);
	for (i = 11; i <= unit_numbers; i++)
		aisread_tup = tup_with(aisread_tup, pUnits[i]->name);

#ifdef EXPORT
	list_code = 0;
#endif
	if (binder(aisread_tup))
		store_axq(AXQFILE, unit_number_now);
	ifclose(AXQFILE);
	if (errors) {
#ifdef DEBUG
		user_info("Binding stopped");
#endif
		exitf(RC_ERRORS);
	}
}

static void exitf(int status)										/*;exitf*/
{
	/* exit after closing any open files */
	ifoclose(AXQFILE);
	ifoclose(LIBFILE);
	ifoclose(STUBFILE);
	exitp(status);
}

void user_error(char *reason)									/*;user_error*/
{
	errors++;
	if (adacomp_option) {
		list_hdr(ERR_SEMANTIC);
		fprintf(MSGFILE, " %s\n", reason);
	}
	else
		printf(" %s\n", reason);
}

void user_info(char *line)										/*;user_info*/
{
	/* In SETL USER_INFO macro is defined to be
     * PRINTA(GENfile, INFORMATION, ada_line, 0, ada_line, 0, '	'+line)    endm;
	 * where the argument is always a unit_name passed to formatted name
	 * In C, we call user_info and fill in needed info
	 */

	if (adacomp_option) {
		list_hdr(INFORMATION);
		fprintf(MSGFILE, "%s\n", line);
	}
	else {
		printf("%s\n", line);
	}
}
