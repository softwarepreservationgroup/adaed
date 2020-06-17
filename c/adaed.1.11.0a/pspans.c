/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#include "ada.h"
#include "miscprots.h"
#include "adalexprots.h"
#include "pspansprots.h"

extern char *emalloc();

int is_terminal_node(short node_kind)					/*;is_terminal_node*/
{
	return (
	    node_kind == AS_STRING ||
	    node_kind == AS_STRING_LITERAL ||
	    node_kind == AS_CHARACTER_LITERAL ||
	    node_kind == AS_INT_LITERAL ||
	    node_kind == AS_REAL_LITERAL ||
	    node_kind == AS_SIMPLE_NAME ||
	    node_kind == AS_OPERATOR ||
	    node_kind == AS_MODE ||
	    node_kind == AS_PACKAGE_STUB ||
	    node_kind == AS_TASK_STUB ||
	    node_kind == AS_NULL ||
	    node_kind == AS_NULL_S ||
	    node_kind == AS_OTHERS ||
	    node_kind == AS_GENERIC ||
	    node_kind == AS_LINE_NO 
	    );
}

struct tok_loc *get_left_span(struct ast *node)			/*;get_left_span*/
{
	short nkind,first;
	struct two_pool *firstelem;
	struct ast **astptr;

	nkind = node->kind;
	if (is_terminal_node(nkind))
		return (&node->span);
	else if (nkind == AS_OPT)
		return (&node->span);
	else if (nkind == AS_EXIT) {
		return get_left_span(node->links.subast[3]);
	}
	else if (nkind == AS_RETURN) {
		return get_left_span(node->links.subast[3]);
	}
	else if (nkind == AS_RAISE) {
		return get_left_span(node->links.subast[1]);
	}
	else if (nkind == AS_OTHERS_CHOICE) {
		return get_left_span(node->links.subast[2]);
	}
	else if (islist_node[nkind]) {
		if (node->links.list==NULL) /* only for parser */
			return (&node->span);
		else {
			firstelem = node->links.list->link;/* first list element */
			if (firstelem->val.node == opt_node)
				chaos("get_left_span: list element opt_node");
			return get_left_span(firstelem->val.node);
		}
	}
	else if (isast_node[nkind]) {
		astptr = node->links.subast;
		first = -1;
		if (astptr[0] != opt_node && astptr[0] != (struct ast *)0)
			first = 0;
		else if (N_AST2_DEFINED(nkind)) {
			if (astptr[1] != opt_node && astptr[1] != (struct ast *)0)
				first = 1;
			else if (N_AST3_DEFINED(nkind)) {
				if (astptr[2] != opt_node && astptr[2] != (struct ast *)0)
					first = 2;
				else if (N_AST4_DEFINED(nkind)) {
					if (astptr[3] != opt_node && astptr[3] != (struct ast *)0)
						first = 3;
				}
			}
		}
		if (first >= 0)
			return get_left_span(astptr[first]);
		else {
			printf("node kind %d\n",nkind);
			chaos("get_left_span: all ast's are opt_node");
		}
	}
	else printf("get_left_span: bad node kind %d\n",nkind);
	chaos("get_left_span");
	return (struct tok_loc *)0;
}

struct tok_loc *get_right_span(struct ast *node)		/*;get_right_span*/
{
	short nkind,last,length=1;
	struct ast **astptr;
	struct two_pool *lastelem;

	nkind = node->kind;
	if (is_terminal_node(nkind)) {
		if (isval_node[nkind])
			/* as_null, as_null_s, as_others, as_others_choice
			 * have no N_VAL field defined
			 */
			length = strlen(namelist(node->links.val));
		return (make_span(node->span.line, node->span.col+length-1));
	}
	else if (nkind == AS_OPT)
		return (&node->span);
	else if (nkind == AS_EXIT) {
		if (node->links.subast[1] != opt_node)
			return get_right_span(node->links.subast[1]);
		else if (node->links.subast[0] != opt_node)
			return get_right_span(node->links.subast[0]);
		else return get_right_span(node->links.subast[3]);
	}
	else if (nkind == AS_RETURN) {
		if (node->links.subast[0] != opt_node)
			return get_right_span(node->links.subast[0]);
		else return get_right_span(node->links.subast[3]);
	}
	else if (nkind == AS_RAISE) {
		if (node->links.subast[0] != opt_node)
			return get_right_span(node->links.subast[0]);
		else return get_right_span(node->links.subast[1]);
	}
	else if (nkind == AS_OTHERS_CHOICE) {
		if (node->links.subast[1] != opt_node)
			return get_right_span(node->links.subast[1]);
		else if (node->links.subast[0] != opt_node)
			return get_right_span(node->links.subast[0]);
		else return get_right_span(node->links.subast[2]);
	}
	else if (islist_node[nkind]) {
		if (node->links.list==NULL) /* only for parser */
			return (&node->span);
		else {
			lastelem = node->links.list;
#ifdef TODO
			-- this finds next element in list. we need to search backwards !!!
			    while ((lastelem)->val.node == opt_node) {
				lastelem = lastelem->link; /* last list element */
				if (lastelem == node->links.list)
					chaos("get_right_span: all list elements opt_node");
			}
#endif
			if (lastelem->val.node == opt_node)
				chaos("get_right_span: list element opt_node");
			return get_right_span((lastelem)->val.node);
		}
	}
	else if (isast_node[nkind]) {
		astptr = node->links.subast;
		if (N_AST4_DEFINED(nkind) && astptr[3] != opt_node 
		    && astptr[3] != (struct ast *)0)
			last = 3;
		else if (N_AST3_DEFINED(nkind) && 
		    astptr[2] != opt_node && astptr[2] != (struct ast *)0)
			last = 2;
		else if (N_AST2_DEFINED(nkind) && 
		    astptr[1] != opt_node && astptr[1] != (struct ast *)0)
			last = 1;
		else if (astptr[0] == opt_node && astptr[0] == (struct ast *)0)
			chaos("get_right_span: all ast's are opt_node");
		else last = 0;
		return get_right_span(astptr[last]);
	}
	else printf("get_right_span: bad node kind %d\n",nkind);
	chaos("get_right_span");
	return (struct tok_loc *)0;
}

struct tok_loc *make_span(short line, short col)			/*;make_span*/
{
	struct tok_loc *tok;

	tok = (struct tok_loc *) emalloc(sizeof(struct tok_loc));
	tok->line = line;
	tok->col  = col;
	return (tok);
}
