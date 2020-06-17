
/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#include "ada.h"

#define SPAN(node) get_left_span(node),get_right_span(node)

/* Macros for convenient use of the rh array:
AST(n) represents the node in the nth position of the rh array (non-terminal).
IND(n) repesents the index of the terminal in the nth position of the rh
array.
LOC(n) represents the starting location of the terminal in the nth position 
of the rh array.
END_LOC(n) represents the ending location of the terminal in the nth position 
of the rh array.
*/
#define AST(j) (rh[j]->ptr.ast)
#define IND(j) (rh[j]->ptr.token->index)
#define LOC(j) (&rh[j]->ptr.token->loc)
#define END_LOC(j) 	\
	make_span(LOC(j)->line,LOC(j)->col+strlen(namelist(IND(j)))-1)



/* FREEAST recursively frees the nth child of node and its children. */
#define FREEAST(node,n) {if ((node->links.subast)[n] != opt_node) \
	free_everything((node->links.subast)[n]);}


/* These macros are just to make coding easier */
#define NAST1 node->links.subast = new_ast1
#define NAST2 node->links.subast = new_ast2
#define NAST3 node->links.subast = new_ast3
#define NAST4 node->links.subast = new_ast4
#define NN(kind) node = new_node(kind)


/* The following 2 macros are for looping through a circular linked list of
   structs of type two_pool. list is the list to go through, and var is
   the variable name to be used for the current element of the list. */
#define LLOOPTOP(list,var) {if (list != NULL) {struct two_pool *bottom, *var;\
	var = bottom = list; do	 { var = var->link;
#define LLOOPBOTTOM(var) } while (var != bottom); }}
