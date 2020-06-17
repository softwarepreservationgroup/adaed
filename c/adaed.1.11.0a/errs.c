/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#include "ada.h"
#include "ifile.h"
#include "libfprots.h"
#include "pspansprots.h"
#include "adalexprots.h"
#include "errsprots.h"

extern IFILE *astfile;			/* File pointer for ast file */

static void printerr(struct tok_loc *, struct tok_loc *, char *);
static void err_display(char *);
static void write_error(int, char *, struct tok_loc *, struct tok_loc *);
static void print_tuple(struct ast *);

/* This file contains functions for printing error messages.
   To print an error message given the complete message and spans,
   printerr is used. To print a syntax error, syntax_err is
   used.
   It also contains the procedures used to write the astfile.
   A comment at the end of this file describes the format
   of the astfile.
 */


/* Printerr: print error message to the terminal 
 * this should be called only in case some debugging option is set (-ert)
 * Error message is printed to stdout if termopt set, to errfile if redopt or
 * erropt set 
 * If the error is not on the most recently printed source line, print the line
 * (or last line if it spans multiple lines) first, followed by an underlining
 * or pointer to error location.
 */


static void printerr(struct tok_loc *lspan, struct tok_loc *rspan, char *msg)
															/*;printerr*/
{
	int i;
	char tmp_str[150];

	if (lspan->line==rspan->line || lineno - rspan->line < NUM_LINES)
	{
		/* print source line (or last line ) */
		if (rspan->line != lineno   &&  /* error at current line */
		!(feof(adafile) && lspan->line==lineno-1 &&colno==1) ) {
			/* error is at last line in source */
			sprintf(tmp_str, "%5d:  %s\n", rspan->line, source_buf[
			  (src_index + NUM_LINES - (lineno - rspan->line)) % NUM_LINES]);
			err_display(tmp_str);
		}
		/* underline */
		if (lspan->line != rspan->line) {
			err_display(". . . . ");
			for (i = rspan->col; --i;)
				err_display("-");
			err_display(">\n");
		}
		else { /* error spans more than one line */
			for (i = lspan->col + 8; --i;)
				err_display(" ");
			if (rspan->col - lspan->col <= 1)
				err_display("^");
			else {
				err_display("<");
				for (i = rspan->col - lspan->col - 1; i--;)
					err_display("-");
				err_display(">");
			}
			err_display("\n");
		}
	}
	else {
		sprintf(tmp_str, "-- Between line %d column %d and line %d column %d\n",
		  lspan->line, lspan->col, rspan->line, rspan->col);
		err_display(tmp_str);
	}
	err_display(msg);
	err_display("\n\n");
}

static void err_display(char *str)							/*;err_display*/
{
	if (termopt)
		printf("%s", str);
#ifdef DEBUG
	if (trcopt)
		fprintf(errfile, "%s", str);
#endif
}


/* Syntax_err: Report an error detected during parsing actions */

void syntax_err(struct tok_loc *lspan, struct tok_loc *rspan, char *msg)
															/*;syntax_err*/
{
	char newmsg[300];

	err_ct++;
	write_error(ERR_SYNTAX, msg, lspan, rspan);
	sprintf(newmsg, "*** Syntax error: %s", msg);
	if (debugopt)
		printerr(lspan, rspan, newmsg);
}


void match_error(int id1, int id2, char *construct, struct tok_loc *lspan,
  struct tok_loc *rspan)									/*;match_error*/
{
	/* Match_error: Report an error in matching two identifiers */
	char msg[200];

	sprintf(msg, "%s at beginning of %s does not match %s",
	  namelist(id1), construct, namelist(id2));
	syntax_err(lspan, rspan, msg);
}

void prs_warning(struct tok_loc *lspan, struct tok_loc *rspan,  char *msg)
															/*;prs_warning*/
{
	/* Prs_warning: Report a warning message */
	char newmsg[200];

	write_error(ERR_WARNING, msg, lspan, rspan);
	sprintf(newmsg, "*** Warning: %s", msg);
	if (debugopt)
		printerr(lspan, rspan, newmsg);
}


void lexerr(int line, int col1, int col2, char *msg)			/*;lexerr*/
{
	/* Lexerr: Report an error detected by the lexical scanner */
	char nmsg[300];
	struct tok_loc span1, span2;

	err_ct++;
	span1.line = span2.line = line;
	span1.col = col1;
	span2.col = col2;
	write_error(ERR_LEXICAL, msg, &span1, &span2);
	sprintf(nmsg, "*** Lexical Error: %s", msg);
	if (debugopt)
		printerr(&span1, &span2, nmsg);
}

/* This file contains functions dealing with files that need to be
   written so other programs can read them.
*/


/* So that the error messages can be merged with the listing of the
   source file, and so that the pragmas LIST and PAGE are taken into
   consideration properly, msgfile is written with information about
   occurrances of the above. The format is as follows

   error_or_pragma_type lspan rspan
   error_mssage

   where lspan and rspan are each two integers, error_or_pragma_type
   is the type of error or pragma (defined in msgs.h), and error_message
   is the error message in the case of an error. There is no error
   message for pragmas (though there may be a seperate set of lines
   for an error message which is there because of the pragma
*/


void write_pragma(int pragma_type, struct tok_loc *lspan, struct tok_loc *rspan)
															/*;write_pragma*/
{
	/* Write_pragma: Writes data about pragma LIST and PAGE to the msgfile */
	fprintf(msgfile, "%d %d %d %d %d\n", pragma_type, lspan->line,
	  lspan->col, rspan->line, rspan->col);
}

static void write_error(int error_type, char *msg, struct tok_loc *lspan,
  struct tok_loc *rspan)									/*;write_error*/
{
	/* Write_error: Write data about errors to msgfile */
	fprintf(msgfile, "%d %d %d %d %d\t%s\n", error_type, lspan->line,
	  lspan->col, rspan->line, rspan->col, msg);
}

/* Routines for printing the AST to a file in SETL format. */

static int node_count; /* A count for assigning a small integer to each node. */

#define SETLopt_node 0
/* Representation of opt_node in SETL front end */


/* Print_tree: Print the AST in a file so that the SETL semantic phase
   can use it. The AST is written in the form of a list of SETL tuples,
   each representing a node of the tree. The format is
   [kind, spans, ast, val, list] (note that the spans is a tuple containing
   two tuples of the form [line, col1, col2]). The tree is traversed, and
   the nodes are printed in a bottom up fashion. Each node is assigned
   a node number (which is stored in the spans field after it is printed).
   Because each parent must know the node numbers of its children in order
   to print the ast and list, we process the children of each node before
   the node itself. So the root of the tree ends up being the last tuple
   printed.
*/
/* The above was once true, but has been obsolete for quite a while. It
 * has, however, served as a model for the current implementation.
 */

void print_tree(struct ast *root)								/*;print_tree*/
{
	if (root != NULL)
	{
		node_count = 0;
		print_tuple(root);
		putnum(astfile, "root", 0);
	}
}

static void print_tuple(struct ast *node)					/*;print_tuple*/
{
	struct two_pool *listptr;
	int i;

#ifdef PDEBUG
	if (trcopt)
		fprintf(errfile, "called with node of kind = %d\n", node->kind);
#endif
	/* Call self recursively on children before printing node's own tuple */
	if (islist_node[node->kind]) {
		if (node->links.list != NULL) {
			listptr = node->links.list;
			do {
				listptr = listptr->link;
				print_tuple(listptr->val.node);
			}	    while(listptr != node->links.list);
		}
	}
	else if (isast_node[node->kind] || node->kind==AS_OTHERS_CHOICE) {
		/* KLUDGE until can rerun makeinit */
		for (i = 0; i < MAX_AST && (node->links.subast)[i] != NULL; i++)
			if ((node->links.subast)[i] != opt_node)
				print_tuple((node->links.subast)[i]);
	}


#ifdef PDEBUG
	if (trcopt)
		fprintf(errfile, "Doing stuff for node %d\n", node_count + 1);
#endif

	/* Store the node number in the count field so its parent can access it. */

	if (node->count == 0) node->count = ++node_count;

	/* Write the count and kind (N_KIND) */
	putnum(astfile, "count", node->count);
	putnum(astfile, "kind", node->kind);

	/* Write the spans (SPANS) only for terminal nodes */
	if (is_terminal_node(node->kind)) {
		putnum(astfile, "span-line", node->span.line);
		putnum(astfile, "span-col", node->span.col);
	}

	/* Special case here for AS_LINE_NO since we only want the line
   number written out */

	if (node->kind == AS_LINE_NO)
		putnum(astfile, "line-no", node->links.val);
	else {

		/* Print the (sub)ast (N_AST) */
		if (isast_node[node->kind] || node->kind==AS_OTHERS_CHOICE) {
			/* KLUDGE until can rerun makeinit */
			for (i=0 ; (i < MAX_AST) && ((node->links.subast)[i] != NULL); i++);
			putnum(astfile, "subast", i);

			i = 0;
			while (1) {
				putnum(astfile, "node",
				  ((node->links.subast)[i] != opt_node) ? 
				  ((node->links.subast)[i]->count) : SETLopt_node);
				if (!(++i < MAX_AST && (node->links.subast)[i] != NULL))
					break;
			}
		}
		else {
			putnum(astfile, "none", 0);
		}
		/* Print the list (on a new line, max 12 per line)	(N_LIST) */
		if (islist_node[node->kind]) {

			if (node->links.list != NULL) {
				i = 0 ;
				listptr = node->links.list ;
				while( 1 ) {
					i++ ;
					listptr = listptr->link ;
					if (listptr == node->links.list)
						break;
				}
				putnum(astfile, "list-size", i);
				listptr = node->links.list;
				i = 1;
				while (1) {
					listptr = listptr->link;
					putnum(astfile, "count", listptr->val.node->count);
					if (listptr == node->links.list)
						break;
				}
			}
			else {
				putnum(astfile, "none", 0);
			}
		}
		else {
			putnum(astfile, "not-list", -1);
		}
		/* Write the val (N_VAL) */
		if (isval_node[node->kind]) {
			putnum(astfile, "is-val", 1); /* indicate a VAL field */
			putstr(astfile, "val", namelist(node->links.val));
		}
		else {
			putnum(astfile, "no-val", 0); /* indicate no VAL field */
		}
	}

#ifdef PDEBUG
	if (trcopt)
		fprintf(errfile, "Returning for node %d\n", node_count);
#endif
}

/*
 * The format of the asftile is as follows. The file is a text file in
 * the form of a SETL data set. Each entry is a tuple. The end of a 
 * compilation unit is denoted by a null tuple. Each tuple begins
 * with six integers:
 * 	kind
 * 	line1
 * 	col1
 * 	line2
 * 	col2
 * 	astcount
 * If astcount is zero there are no (sub)ast nodes; otherwise there
 * follow astcount integers giving the nodes in the (sub)ast.
 * Next comes the list count. If this is -1 there is no list.
 * If the list count is 0 there is a list but it has no entries.
 * If the list count is positive, it is followed that number of
 * integers giving the node numbers of the items in the list.
 * Last, if the VAL field is defined for this kind of node, there
 * comes the VAL field in the form of a SETL string.
 * 
 * The current format differs from the above in that integer valus
 * are followed by comma characters and there is in some cases a
 * comma after a string literal where a right bracket ']' should occur.
 * 
 * Within each compilation unit, nodes are referenced in the astfile
 * by integers starting with 1. The last (non-null) tuple is the root
 * of the compilation unit.
 * 
 * Binary format.
 * The binary format uses short integers for integer values.
 * The string '[]' in SETL form is represented by (short) integer 0.
 * The VAL field is written as follows:
 * 	0	if no VAL
 * 	1+n	if VAL field, in which case the count is follows
 * 		by the n characters representing the value (no \0)
 * 		and end.
 */
