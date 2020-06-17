/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

*/

int is_terminal_node(short);
struct tok_loc *get_left_span(struct ast *);
struct tok_loc *get_right_span(struct ast *);
struct tok_loc *make_span(short, short);
