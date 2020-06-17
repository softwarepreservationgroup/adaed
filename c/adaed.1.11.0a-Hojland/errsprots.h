/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

*/

void syntax_err(struct tok_loc *, struct tok_loc *, char *);
void match_error(int, int, char *, struct tok_loc *, struct tok_loc *);
void prs_warning(struct tok_loc *, struct tok_loc *,  char *);
void lexerr(int, int, int, char *);
void write_pragma(int, struct tok_loc *, struct tok_loc *);
void print_tree(struct ast *);
