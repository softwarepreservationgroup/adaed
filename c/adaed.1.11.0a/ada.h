/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#include "config.h"
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <string.h>
#include "constant.h"
#include "kinds.h"

#define TABFILE "ada.t"
#define CONSTANTFILE "constant.h"

#define EOFT "$EOF"
#define ERROR_SYM 0
#define MAXLINE 120
#define MAX_AST 4

#define NUM_LINES 20



/* Structure definitions */
struct namelistmap {
    short num;
    char *name;
    struct namelistmap *nextlist;
    struct namelistmap *nextmap;
    };

struct tok_loc {
    short line;
    short col;
    };

struct token {
    short index;
    struct tok_loc loc;
    };

struct prsstack {
    short symbol;
    struct prsstack *prev;
    union {
	struct ast *ast;
	struct token *token;
	} ptr;
    };
struct two_pool {
    struct two_pool *link;
    union {
	int reduction;
	int state;
	struct ast *node;	
	} val;
    };

struct ast {
    short kind;
    short count ;
    struct tok_loc span;
    union {
	struct two_pool *list;
	struct ast **subast;
	short val;
	} links;
    };

struct tmptok {
    struct token *link;
    };

struct tmpnode {
    short kind;		/* set to AS_FREE when first placed on stack */
    short count;
    struct tok_loc span;
    struct ast *link;
    };

struct tmpast {
    struct ast **link;
    };





/* Declarations of functions needed */
extern struct prsstack *(*prsalloc)(unsigned),*copytoken();
extern char *find_name();
extern void (*prsfree)(struct prsstack *, struct prsstack *);
extern struct two_pool *(*talloc)(unsigned);
extern void (*tfree)(struct two_pool *, struct two_pool *);
extern struct token *(*tokalloc)(unsigned);
extern void (*tokfree)(struct token *);
extern struct ast **new_ast(), **astalloc();
extern struct prsstack *gettok();
extern struct two_pool *makenodelist();


/* Declarations of external variables to be visible */
extern struct tok_loc *lspan, *rspan;
extern struct prsstack *prs_stack, *curtok, *rh[], **tokens;
extern struct two_pool *sta_stack;
extern int tokind, toksiz, tokbottom;
extern FILE  *adafile, *msgfile;
extern struct ast *opt_node, *any_node, *id_node;
extern int lineno, colno, src_index;
extern char *data, *line, source_buf[NUM_LINES][MAXLINE + 1];
extern short lhs[], rhslen[], def_action[]; 
extern short act_tab1[];
extern long act_tab2[];
extern int symcount;
extern struct namelistmap *numtostrtable[], *strtonumtable[], gramsyms[];
extern char islist_node[], islater_declarative_node[], isbody_node[],
	isval_node[], isast_node[],ispredef_pragma[],isimpldef_pragma[],
	isimmediate_decl_pragma[],iscontext_pragma[],iscompilation_pragma[],
	isafter_libunit_pragma[],istask_pragma[],isrepr_pragma[],
	*overloadable_operators[];
extern int redopt, astopt, erropt, termopt, debugopt, trcopt;
extern int err_ct;


/* Macros */
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

#define ISTOKEN(node)	  ((node)->symbol <= EOFT_SYM)



/* Macros for storage allocation */
#define PRSALLOC() (*prsalloc)(sizeof(struct prsstack))
#define PRSFREE (*prsfree)
#define TALLOC() (*talloc)(sizeof(struct two_pool))
#define TFREE (*tfree)
#define TOKALLOC() (*tokalloc)(sizeof(struct token))
#define TOKFREE (*tokfree)


/* Error recovery: */

/* ALWAYS_PREFERRED_SYMS */
#define in_ALWAYS_PREFERRED_SYMS(t) ((t==65) || (t==75) || (t==80) )


/* Constants*/
#define MAX_LOOK_AHEAD	25
#define MIN_LEN 1
#define RESERVED_SYMS 64

/*  
		   DEFINE CANDIDATE TYPES
				  
The map CANDIDATES is a multivalued map from the set
	{'delete','subst','merge','spell-subst','insert'} 
to sets of potential candidates for substitution. Each of these is
represented as a structure containing 
			the token symbol, the number of backup tokens and
			a third value.
													*/
#define DELETE 1
#define MERGE  2
#define SUBST  3
#define INSERT 4 
#define SPELL_SUBST 5 
#define C_BOUND 6


/* Structure definitions */

typedef struct cand {
    short token_sym,backup_toks,t3 ;
    struct cand *next ;
    } CAND, *PCAND;

/*  
		   DEFINE STRING TYPES
								    */
typedef char * STRING ;
typedef struct stringlist {	    /* A list of strings	    */
	STRING message ;	    /* Points to the text	    */
	struct stringlist *next ;   /* Points to the next message   */
}   STRINGLIST ;



/* External variables */
extern FILE *errfile;
extern	int TOKSYMS[] ;
extern	int n_TOKSYMS ;
extern	PCAND CANDIDATES[] ;
extern	int n_CANDIDATES[] ;
extern	int MAX_CHECK ;
extern	int BACKUPTOKS ;
extern struct prsstack *PREVTOK;
extern int n_prs, n_prs_stack, n_sta_stack;
#ifndef IBM_PC
extern	int	ACTION_TOKENS[] ;
#endif
#ifdef IBM_PC
extern	char	ACTION_TOKENS[] ;
#endif
extern	int	ACTION_TOKENS_INDEX[] ;
extern	struct two_pool *POSS_TOK ;
extern int  SHIFT_STATE[];
extern int  SHIFT_STATE_INDEX[];
extern int	primopt,scopeopt,secopt ;



/* External function definitions */

extern	struct cand *(*candalloc)(unsigned);
extern struct prsstack *tokfromlist();
extern struct tok_loc *token_location ();




/* Macros */


#define NEXTTOK (((tokind - tokbottom + 1) % toksiz == 0) ? gettok() : \
	tokfromlist())

#define nexttok tokens[(tokind - 1 + toksiz) % toksiz]

#define ADDTOTOP(tok) tokens[tokind = (tokind + 1) % toksiz] = (tok)
 
#define TOKMINUS (tokind = (tokind - 1 + toksiz) % toksiz)

#define CANDALLOC() (*candalloc)(sizeof(struct cand))

/* #define QUOTE(t) (token_value_des(t) ? "" : "\"") */

#define SEMI_SYM 80

#define l_span(x) (ISTOKEN(x)	\
		? (x->ptr.token->loc)	\
		: (*(get_left_span(x->ptr.ast))))
#define r_span(x) (ISTOKEN(x)	\
		? (*(make_span(x->ptr.token->loc.line,x->ptr.token->loc.col + \
			       strlen(namelist(x->ptr.token->index)) -1))) \
		: (*(get_right_span(x->ptr.ast))))
#define LLOC(x) (ISTOKEN(x)	\
		? &(x->ptr.token->loc)	\
		: (get_left_span(x->ptr.ast)))
#define RLOC(x) (ISTOKEN(x)	\
		? (make_span(x->ptr.token->loc.line,x->ptr.token->loc.col + \
			       strlen(namelist(x->ptr.token->index)) -1)) \
		: (get_right_span(x->ptr.ast)))

#define		NOOP ;

#define	 is_operator(t)	 (( t > CHAR_SYM ) && (t < EOFT_SYM)) 
#define	 is_terminal(t)	 (t <= EOFT_SYM)
#define	 is_reserved(t)	 (( t > 0 ) && ( t <= RESERVED_SYMS ) )
/* is_opener is now procedure in prserr.c */

#define S_TOKSYMS	(MAX_LOOK_AHEAD * 2)

#define TOKSTR(symb)  ((symb != ERROR_SYM) ? namelist(symb) : "ERROR_SYMBOL")

/* added for change is spans format (store only for terminal nodes */

#define N_D_AST1 1
#define N_D_AST2 2
#define N_D_AST3 4
#define N_D_AST4 8
#define N_D_LIST 16
#define N_D_VAL 32
#define N_D_UNQ 64
#define N_D_TYPE 256

#define N_AST1_DEFINED(p) TRUE
#define N_AST2_DEFINED(p) (N_DEFINED[p]&N_D_AST2)
#define N_AST3_DEFINED(p) (N_DEFINED[p]&N_D_AST3)
#define N_AST4_DEFINED(p) (N_DEFINED[p]&N_D_AST4)
#define N_VAL_DEFINED(p) (N_DEFINED[p]&N_D_VAL)
#define N_UNQ_DEFINED(p) (N_DEFINED[p]&N_D_UNQ)
#define N_TYPE_DEFINED(p) (N_DEFINED[p]&N_D_TYPE)
#define N_LIST_DEFINED(p) (N_DEFINED[p]&N_D_LIST)

extern int N_DEFINED[] ;
