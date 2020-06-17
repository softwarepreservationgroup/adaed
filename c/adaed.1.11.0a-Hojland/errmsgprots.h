/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

void errmsg_id(char *, Symbol, char *, Node);
void errmsg_str(char *, char *, char *, Node);
void errmsg_nat(char *, Symbol, char *, Node);
void errmsg_type(char *, Symbol, char *, Node);
void errmsg_nval(char *, Node, char *, Node);
void errmsg_id_id(char *, Symbol name1, Symbol name2, char *, Node);
void errmsg_id_type(char *, Symbol, Symbol, char *, Node);
void errmsg_nat_id_str(char *, Symbol, Symbol, char *, char *, Node);
void errmsg_str_id(char *, char *, Symbol, char *, Node);
void errmsg_str_num(char *, char *, int, char *, Node);
void pass1_error_id(char *, Symbol, char *, Node);
void pass1_error_str(char *, char *, char *, Node);
void pass1_error_l(char *msg1, char *msg2, char *, Node);
char *build_full_names(Set);
void errmsgn(int, int, Node);
void id_errmsgn(int, Symbol, int, Node);
void id_id_errmsgn(int, Symbol name1, Symbol name2, int, Node);
void id_type_errmsgn(int, Symbol, Symbol, int, Node);
void l_errmsgn(int msg1, int msg2, int, Node);
void l1_errmsgn(char *msg1, int msg2, int, Node);
void l2_errmsgn(int msg1, char *msg2, int, Node);
void l_id_errmsgn(int msg1, int msg2, Symbol, int, Node);
void l_str_errmsgn(int msg1, int msg2, char *, int, Node);
void nat_errmsgn(int, Symbol, int, Node);
void nat_id_str_errmsgn(int, Symbol, Symbol, char *, int, Node);
void nval_errmsgn(int, Node, int, Node);
void str_errmsgn(int, char *, int, Node);
void str_id_errmsgn(int, char *, Symbol, int, Node);
void str_num_errmsgn(int, char *, int, int, Node);
void type_errmsgn(int, Symbol, int, Node);
void type1_errmsgn(char *, Symbol, int, Node);
void errmsg(char *, char *, Node);
void warning(char *, Node);
