/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

*/

void free_everything(struct ast *);
void set_span(struct ast *, struct tok_loc *);
struct two_pool *initlist(struct ast *);
struct ast **new_ast1(struct ast *);
struct ast **new_ast2(struct ast *, struct ast *);
struct ast **new_ast3(struct ast *, struct ast *, struct ast *);
struct ast **new_ast4(struct ast *, struct ast *, struct ast *, struct ast *);
struct two_pool *concatl(struct two_pool *,struct two_pool *);
struct two_pool *concatl3(struct two_pool *, struct two_pool *,
  struct two_pool *);
void append(struct ast *, struct ast *);
void prepend(struct ast *, struct ast *);
struct ast *binary_operator(struct ast *, struct ast *, struct ast *);
struct ast *unary_operator(struct ast *, struct ast *);
int check_expanded_name(struct ast *);
void check_discrete_range(struct ast *);
void pragmalist_warning(struct ast *);
void check_pragmas(struct ast *, int (*allowed_test)(int));
int isoverloadable_op(char *);
void make_id(int );
int immediate_decl_pragmas(int );
int compilation_pragmas(int );
int after_libunit_pragmas(int );
int task_pragmas(int );
int task_repr_pragmas(int );
int context_pragmas(int );
int null_pragmas(int);
void check_choices(struct ast *, char *);
struct two_pool *remove_duplicate_labels(struct two_pool *);
void ins_as_line_no(struct ast *);
void end_as_line_no(struct ast *, struct prsstack *);
unsigned long labelshash(struct ast *);
void newlabels(struct ast *, struct two_pool *);
struct two_pool *getlabels(struct ast *);
void erase_labels(struct ast *);
struct two_pool *copylist(struct two_pool *);
void free_labels();
#ifdef DEBUG
void dump_labels(struct ast *);
#endif 
