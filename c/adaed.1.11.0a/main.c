/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#include "hdr.h"
#include "vars.h"
#include "libhdr.h"
#include "ifile.h"
#include "libprots.h"
#include "libfprots.h"
#include "librprots.h"
#include "libwprots.h"
#include "setprots.h"
#include "dbxprots.h"
#include "arithprots.h"
#include "miscprots.h"
#include "smiscprots.h"
#include "chapprots.h"
#include "astreadprots.h"
#include "dclmapprots.h"
#include "sspansprots.h"

IFILE *TREFILE;
IFILE *AISFILE, *STUBFILE;
IFILE *LIBFILE;
FILE *MALFILE;
int list_unit_0=0; /* on to list structure of unit 0 */

static int malloc_usage_list=0;
static unsigned malloc_bytes;

static void exitf(int);

#ifdef DEBUG
extern int zpadr_opt;
#endif

int main (int argc, char **argv)									/*;main*/
{
	/* Driver routine for ada sem */

	Node nod;
	int		c, i, n;
	int		errflg = 0, nobuffer = 0;
	int		mflag = 0;
	int		compiling_predef = 0;
	int		dir_option=FALSE;
	int		iot_prs_r=0; /* prs tree input trace */
	int		iot_level=2; /* default trace level */
	extern int	optind;
	extern char *optarg;
	IFILE	*astfile;
	char	*fname, *tfname;
	char	*lib_name, *t_name;
	int		r_trace=TRUE, w_trace =TRUE;

	AISFILE = (IFILE *)0;
	STUBFILE= (IFILE *)0;

	while ((c = getopt (argc, argv, "f:l:ns:")) != EOF)
		/*   user:
		 *  	l	using library
		 *  	n	new library
		 *   debugging: 
		 *	f	file i/o trace, followed by list of options
		 *		a	trace ais files
		 *		d	do not include descriptors in trace
		 *		n	do not include file numbers in trace
		 *		p	trace ast file
		 *		r	subsequent traces for reading only
		 *		t	trace tre files
		 *		w	subsequent traces for writing only
		 *		(traces initially for both r and w, use of r or w
		 *		limits further files traces to just that mode)
		 *		1	set trace level to 1
		 *		2	set trace level to 2
		 *	s	debugging, followed by list of options:
		 *		0	show structure of unit 0
		 *		M	malloc trace (including init_sem)
		 *		b	do not buffer standard output
		 *		d	debug trace
		 *		e	flag signalling errors in the parsing phase
		 *		m	malloc trace (after init_sem)
		 *		p	compiling predef units
		 *		z	call trapini to initialize traps
		 */

		switch (c) {
		case 'l': /* using existing library */
			lib_option = TRUE;
			lib_name = emalloc((unsigned) (strlen(optarg) + 1));
			strcpy(lib_name, optarg);
			break;
		case 'n': /* indicates new library */
			new_library = TRUE;
			lib_option = TRUE;
			break;
#ifdef DEBUG
		case 'f':	/* process ifile trace options */
			n = strlen(optarg);
			for (i=0; i<n; i++) {
				switch (optarg[i]) {

				case 'o': /* suppress offset info */
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
				case 'p': 
					iot_prs_r = iot_level;
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
		case 's':
			n = strlen(optarg);
			for (i=0; i <= n; i++) {
				switch(optarg[i]) {

#ifdef DEBUG
				case 'a':
					zpadr_opt = 0;
					break;
#endif
				case 'e':
					errors = TRUE;
					break;
				case 'p': /* compiling predef units */
					compiling_predef++;
					break;
#ifdef DEBUG
				case '0':  /* dump unit 0 */
					list_unit_0++;
					break;
				case 'b': /* do not buffer output */
					nobuffer++;
					break;
				case 'd': /* force debugging output */
					cdebug2 = 4;
					break;
				case 'm': /* turn on malloc trace */
					mflag++;
					break;
				case 't': /* save_tree trace */
					zpadr_opt=0;
					save_trace_init();
					break;
				case 'u': /* malloc usage */
					malloc_usage_list++;
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
	fname = (char *)0;
	if (optind < argc)
		fname = argv[optind];
	if (fname == (char *)0) errflg++;
	if (errflg) {
		printf ("Usage: adasem -n -l library -s [AMRabdemprtwz] file\n");
		exitp(RC_ABORT);
	}
#ifdef DEBUG
	if (mflag) {
		trace_malloc();
		/* can't use strjoin to setup efopen arg as want trace ! */
		/*MALFILE = efopen(strjoin(FILENAME, ".mas"), "w", "t"); */
		tfname = malloc((unsigned) (strlen(fname) + 4 + 1));
		MALFILE = efopen(strcat(strcpy(tfname, fname), ".mas"), "w", "t");
		free(tfname);
	}
#endif
	tup_init(); /* initialize tuple package */
	FILENAME = strjoin(fname, "");
	PREDEFNAME = predef_env();
	if (nobuffer) {
		setbuf (stdout, (char *)0); /* do not buffer output (for debug) */
	}
	dstrings_init(2048, 512); /* initialize dstrings package */

	rat_init(); /* initialize arithmetic and rational package*/
	init_sem();

	aisunits_read = tup_new(0);
	init_symbols = tup_exp(init_symbols, (unsigned)  seq_symbol_n);
	for (i=1; i<= seq_symbol_n; i++)
		init_symbols[i] = seq_symbol[i];

	t_name = libset(lib_name);
	MSGFILE  = efopenl(FILENAME, "msg", "a", "t");
	if (!compiling_predef) init_predef();
	/* When the separate compilation facility is being used all references to
	 * AIS files will be made via the directory in LIBFILE. AISFILENAME is set
	 * to a number if the library is used, otherwise it is the FILENAME.  
	 */
	if (lib_option) {
		if (compiling_predef)
			AISFILENAME = "0";
		else if (new_library) 
			AISFILENAME = "1";
		else {
			/* here to get AIS name from lib. Note the library is left open*/
			AISFILENAME = lib_aisname();
			read_lib();
		}
	}
	else {
		AISFILENAME = FILENAME;
	}
	/* open the appropriate files using the suffix  .aic for ais files and
     * .trc for tree file. 
     */
	astfile  = ifopen(FILENAME, "ast", "r", "p", iot_prs_r, 0);
	AISFILE  = ifopen(AISFILENAME, "aic", "w", "a", iot_ais_w, 0);
	TREFILE  = ifopen(AISFILENAME, "trc", "w", "t", iot_tre_w, 0);
	/* delete any existing stubfile for current AISFILENAME */
	ifdelete(strjoin(AISFILENAME, ".st1"));
	/* unbuffer output for debugging purposes */
	setbuf(MSGFILE, (char *) 0);
	/* ignore first unit (due to pragmas) */
	nod =astreadu(astfile);
	if (N_KIND(nod) == as_astend) exitf(errors ? RC_ERRORS : RC_SUCCESS);
	for (;;) {
		unit_number_now = unit_numbers + 1;
		seq_node_n = 0; /* reset node count */
		seq_symbol_n = 0; /* reset symbol count */
		nod =astreadu(astfile);
		if (N_KIND(nod) == as_astend) break;
		adasem(nod);
	}

	exitf(errors ? RC_ERRORS : RC_SUCCESS);
}
	
static void exitf(int status)										/*;exitf*/
{
	/* exit after closing any unclosed files */

	ifoclose(AISFILE);
	ifoclose(STUBFILE);
	ifoclose(TREFILE);
	if (malloc_usage_list) {


#ifdef MSTAT
		mstats("malloc use");
#endif
		/*printf("malloc got %u bytes\n", malloc_bytes); */
	}
	exitp(status);
}

/* The following procedures  errmsg() and warning() - should properly 
 * be in errmsg.c, but are placed here to avoid a problem with 
 * overlaying on PC, so that errmsg can be overlaid with eval. 
 * There are calls to errmsg() and warning() in eval that cause loop
 * on PC if these procedures in errmsg.
 */

void errmsg(char *msg, char *lrm_sec, Node node)				/*;errmsg*/
{
	/* Semantic errors */

	int begline, begcol, endline, endcol;
	Span lspan, rspan;

	if (cdebug2 > 3) TO_ERRFILE("AT PROC : errmsg(msg, lrm_sec, node); ");

	if (node == OPT_NODE) node = current_node;
	if (node != (Node)0) {
		lspan = get_left_span(node);
		rspan = get_right_span(node);
		begline = lspan->line;
		begcol	= lspan->col;
		endline = rspan->line;
		endcol	= rspan->col;
	}
	else 
		/* this is in case rcv null node - put message at beginning of file*/
		/* only temp-eventually, all calls to errmsg should have valid node */
		begline = begcol = endline = endcol = 1;

	fprintf(MSGFILE, "%d %d %d %d %d\t%s", ERR_SEMANTIC, begline, begcol,
	  endline, endcol, msg);
	if (!streq(lrm_sec, "none"))
		fprintf(MSGFILE, " (RM %s)", lrm_sec);
	fprintf(MSGFILE, "\n");
	errors = TRUE;
}

void warning(char *msg, Node node)								/*;warning*/
{
	int begline, begcol, endline, endcol;
	Span lspan, rspan;

	if (cdebug2 > 3) TO_ERRFILE("AT PROC : warning(msg);");

	if (node == OPT_NODE) node = current_node;
	if (node != (Node)0) {
		lspan = get_left_span(node);
		rspan = get_right_span(node);
		begline = lspan->line;
		begcol	= lspan->col;
		endline = rspan->line;
		endcol	= rspan->col;
	}
	else 
		/* this is in case rcv null node - put message at beginning of file*/
		/* only temp-eventually, all calls to errmsg should have valid node */
		begline = begcol = endline = endcol = 1;

	fprintf(MSGFILE, "%d %d %d %d %d\t%s", ERR_WARNING, begline, begcol,
	  endline, endcol, msg);
	fprintf(MSGFILE, "\n");
}

