/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#include "hdr.h"
#include "vars.h"
#include "attr.h"
#include "ifile.h"
#include "chapprots.h"
#include "libfprots.h"
#include "setprots.h"
#include "sspansprots.h"
#include "miscprots.h"
#include "smiscprots.h"
#include "astreadprots.h"
/* procedures to read the ast */
#define ZERO_NODE  9999

static Node astreadn(IFILE *);
static void attrnum(Node);

Node astreadu(IFILE *astfile)									/*;astreadu*/
{
	/* read next unit from file astfile. Build all necessary
	 * nodes and return the root node of the unit. This can be done in one
	 * pass since all sub-ast and list nodes appear before their "parents".
	 * The global tuple seq_node maps node sequence nubmers to their locations
	 * and is used to relocate pointers upon reading in nodes. 
	 * seq_node_n refers to the last node allocated.
	 */

	Node	node, comp_unit;

	for (;;) {
		node = astreadn(astfile); /* read next node */
		if (N_KIND(node) == as_astend) return node;
		if (N_KIND(node)== as_astnull) { /* if null node */
			comp_unit = (Node) seq_node[seq_node_n];
			return comp_unit;
		}
	}
}

static Node astreadn(IFILE *astfile)							/*;astreadn*/
{
	/* Return next node from ast file */
	/* Return node with kind 0 for null node, kind as_astend for EOF */

	int	c;
	int	n, astcount, listcount, val;
	int	iv, count;
	unsigned int kind;
	short	span0, span1;
	Tuple	list;
	Node	newn, nod;
	char	*newstr;

	count = getnum(astfile, "count");
	if (count == -1) {
		nod = node_new_noseq((unsigned) as_astend);
		return nod;
	}
	if (count == 0) {
		nod = node_new_noseq((unsigned) as_astnull);
		return nod;
	}

	/* Here at start of tuple */
	kind = (unsigned) getnum(astfile, "kind");
	/* allocate and initialize node */
	if (count <= seq_node_n) {
		/* copy of previously seen node, read in and ignore */
		newn = node_new_noseq(kind);
	}
	else if (count == seq_node_n + 1) {  /* next node in sequence */
		newn = node_new(kind);
	}
	else { /* unexpected node number */
		chaos("astreadn: node out of sequence");
	}
	if (newn == (Node) 0) {
		printf("astread cannot allocate node kind %d\n", kind);
		chaos("astread - bad kind");
	}
	if (is_terminal_node(kind)) {
		span0 = getnum(astfile, "span-line");
		span1 = getnum(astfile, "span-col");
	}
	if (kind != as_line_no) {
		/* as_line_no nodes do not have ast or list fields */
		astcount = getnum(astfile, "astcount");
		/* we need to distinguish the zero entered as initial value of AST
		 * field from the 0 used in input format to indicate OPT_NODE. Hence
		 * we mark nodes with ZERO_NODE, and then change values explicitly
		 * entered as zero to OPT_NODE
		 */
		N_AST1(newn) = (Node)ZERO_NODE;
		N_AST2(newn) = (Node)ZERO_NODE;
		N_AST3(newn) = (Node)ZERO_NODE;
		N_AST4(newn) = (Node)ZERO_NODE;
		/* Now read ast count*/
		for (n = 1; n <= astcount; n++) {
			c = getnum(astfile, "ast-count");
			if (c >= N_SEQ(newn))
				printf("astreadn: forward pointer in ast\n");
			switch (n) {
			case(1):	
				N_AST1(newn) = (Node) c; 
				break;
			case(2):	
				N_AST2(newn) = (Node) c; 
				break;
			case(3):	
				N_AST3(newn) = (Node) c; 
				break;
			case(4):	
				N_AST4(newn) = (Node) c; 
				break;
			default: 
				chaos("astreadn too many ast entries");
			};
		}
		if (N_AST1(newn) == (Node)0) N_AST1(newn) = OPT_NODE;
		else if (N_AST1(newn) != (Node)ZERO_NODE)
			N_AST1(newn) = (Node) seq_node[(int)N_AST1(newn)];
		else N_AST1(newn) = (Node)0;

		if (N_AST2(newn) == (Node)0) N_AST2(newn) = OPT_NODE;
		else if (N_AST2(newn) != (Node)ZERO_NODE)
			N_AST2(newn) = (Node) seq_node[(int)N_AST2(newn)];
		else N_AST2(newn) = (Node)0;

		if (N_AST3(newn) == (Node)0) N_AST3(newn) = OPT_NODE;
		else if (N_AST3(newn) != (Node)ZERO_NODE)
			N_AST3(newn) = (Node) seq_node[(int)N_AST3(newn)];
		else N_AST3(newn) = (Node)0;

		if (N_AST4(newn) == (Node) 0) N_AST4(newn) = OPT_NODE;
		else if (N_AST4(newn) != (Node)ZERO_NODE)
			N_AST4(newn) = (Node) seq_node[(int)N_AST4(newn)];
		else N_AST4(newn) = (Node)0;

		/* convert attribute string to internal code */
		if (N_KIND(newn) == as_attribute || N_KIND(newn) == as_range_attribute)
			attrnum(newn);
		/* now process list field */
		listcount = getnum(astfile, "if-list");
		if (listcount >= 0) {
			list = tup_new(listcount);
			for (n = 1; n <= listcount; n++) {
				val = getnum(astfile, "list-val");
				if (val >= N_SEQ(newn))
					printf("astreadn: forward pointer in ast\n");
				list[n] = (char *) seq_node[val];
			}
			N_LIST(newn) = list;
		}

		iv = getnum(astfile, "iv");
		if (iv == 0 ) goto set_span;

		newstr = getstr(astfile, "val");

		/* If mode, needing argument type conversion*/
		if (N_KIND(newn) == as_mode) {
			/* Input from tree has string. We convert to appropriate code
			 * according to the length of the string. This is sufficient, and
			 * avoids having to look at the characters in the string.
			 */
			n = strlen(newstr);
			switch(n) {
			case (0):	
				N_VAL(newn) = (char * ) 0; 
				break;
			case (2):	
				N_VAL(newn) = (char * ) na_in; 
				break;
			case (3):	
				N_VAL(newn) = (char * ) na_out; 
				break;
			case (5):	
				N_VAL(newn) = (char * ) na_inout; 
				break;
			default:	
				printf(" node type as_mode invalid argument type %s\n", newstr);
				/* how to indicate node error - TBSL */
			}
		}
		else {
			N_VAL(newn) = newstr;
		}
	} /* end of processing "non as_line_no" nodes */
	else {
		/* here, handle as_line_no which has just one integer field */
		c = getnum(astfile, "line-no");
		N_VAL(newn) = (char *) c;
	}
set_span:
	if (is_terminal_node(kind)) {
		N_SPAN0(newn) = span0;
		N_SPAN1(newn) = span1;
	}
	return newn;
}

static void attrnum(Node node)										/*;attrnum*/
{
	/* convert attribute string to internal attribute code */
	int		i;
	char	*s, *name;
	static char *attrnames[] = {
		"ADDRESS",
		"AFT",
		"BASE",
		"CALLABLE",
		"CONSTRAINED",
		"O_CONSTRAINED",
		"T_CONSTRAINED",
		"COUNT",
		"DELTA",
		"DIGITS",
		"EMAX",
		"EPSILON",
		"FIRST",
		"O_FIRST",
		"T_FIRST",
		"FIRST_BIT",
		"FORE",
		"IMAGE",
		"LARGE",
		"LAST",
		"O_LAST",
		"T_LAST",
		"LAST_BIT",
		"LENGTH",
		"O_LENGTH",
		"T_LENGTH",
		"MACHINE_EMAX",
		"MACHINE_EMIN",
		"MACHINE_MANTISSA",
		"MACHINE_OVERFLOWS",
		"MACHINE_RADIX",
		"MACHINE_ROUNDS",
		"MANTISSA",
		"POS",
		"POSITION",
		"PRED",
		"RANGE",
		"O_RANGE",
		"T_RANGE",
		"SAFE_EMAX",
		"SAFE_LARGE",
		"SAFE_SMALL",
		"SIZE",
		"O_SIZE",
		"T_SIZE",
		"SMALL",
		"STORAGE_SIZE",
		"SUCC",
		"TERMINATED",
		"VAL",
		"VALUE",
		"WIDTH",
		0	};

	s = N_VAL(N_AST1(node));
	for (i = 0;; i++) {
		name = attrnames[i];
		if (name == (char *)0) break;
		if (streq(name, s)) {
			/* on match, change ast1 node to hold number, not str*/
			N_KIND(N_AST1(node)) = as_number;
			N_VAL(N_AST1(node)) = (char *) i;
			break;
		}
	}
#ifdef IGNORE
	printf("attribute %s\n", s);
	chaos("unable to match attribute name");
#endif
}
