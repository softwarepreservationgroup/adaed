/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#ifndef _vars_h
#define _vars_h


/* vars.ch - global declarations and initializations */

/*
 *T+ GLOBAL DECLARATIONS and DEF =  IONS
 *S+ Constants
 */

extern  double	ADA_MIN_REAL;
extern  double	ADA_MAX_REAL;
extern  int	ADA_REAL_DIGITS;
extern  int	cdebug2;

extern  Declaredmap declared_all[4];	/* array of standard declareds */
extern  int	full_others;  /* a boolean */
extern  int	fold_context; /* a boolean */
extern  int	node_count;
extern  Node current_node;
extern  Node OPT_NODE;	/*initialized in sem0 */

/* Initialized in SEM =   */
/*
??const qualifiers = {as_qual_range,  as_qual_length,  as_qual_discr,
		    as_qual_arange, as_qual_alength, as_qual_adiscr };
 */

/*S+ Variables*/
 /* declared map from standard environment */
/*base_declared is array of copies of maps for standard0,standard,
umnentionable0, and ascii. base_declared_symbols is corresponding
array of symbol table pointers */
extern  Declaredmap base_declared[4];
extern  Symbol base_declared_symbols[4];
extern  Tuple scope_st;			/* stack of lexical scopes */
extern  Tuple has_return_stk;		/* stack to track return statements */
extern  Tuple newtypes;			/* list of type declaration code to be */
 /* emitted before declaration being processed. */
extern  Tuple lab_seen;			/* set of labels in procedure . */
extern  Tuple current_instances;	/* stack for recursive instantiation check */

extern  Symbol scope_name;		/* unique name for each scope */
extern  char *unit_name;		/* compilation unit information */
extern  Tuple all_vis;			/* Modules whose visibility is required */
				/* all_vis used only in ch. 10 */

extern  Tuple open_scopes;		/* nest of currently open scopes, from inner */
 /* to outer.  Outermost is STANDARD. */
extern  Tuple used_mods;		/* packages appearing in a use clause */
extern  Tuple vis_mods;	/* list of package names visible in this unit*/

extern  int	noop_error;

 /* used by procs init_compunit and compunit. */
extern  Set non_local_names;		/* To collect non_local references in each subp. */

extern  int	out_context;		/* Signals valid appearance of out parameter.  */

extern  Symbol
symbol_and,
symbol_andthen,
symbol_any,
symbol_any_id,
symbol_array_type,
symbol_ascii,
symbol_assign,
symbol_boolean,
symbol_boolean_type,
symbol_callable,
symbol_character,
symbol_character_type,
symbol_constrained,
symbol_constraint_error,
symbol_daccess,
symbol_dfixed,
symbol_discrete_type,
symbol_divfx,
symbol_duration,
symbol_eq,
symbol_exception,
symbol_float,
symbol_ge,
symbol_gt,
symbol_in,
symbol_incomplete,		/* incomplete, for incp_types */
symbol_integer,
symbol_le,
symbol_left,
symbol_limited,			/* limited for priv_types, incp_types */
symbol_limited_private,		/* cf. symbol_limited */
symbol_long_float,
symbol_long_integer,
symbol_lt,
symbol_main_task_type,
symbol_mulfx,
symbol_natural,
symbol_none,
symbol_not,
symbol_ne,
symbol_notin,
symbol_null,
symbol_numeric,
symbol_numeric_error,
symbol_or,
symbol_order_type,
symbol_orelse,
symbol_overloaded,
symbol_positive,
symbol_private,			/* for priv_types, incp_types */
symbol_program_error,
symbol_right,
symbol_short_integer,
symbol_short_integer_base,
symbol_standard,
symbol_standard0,
symbol_storage_error,
symbol_system_error,
symbol_string,
symbol_string_type,
symbol_system,
symbol_tasking_error,
symbol_undef,
symbol_universal_dfixed,
symbol_universal_fixed,
symbol_universal_integer,
symbol_universal_integer_1,
symbol_universal_real,
symbol_unmentionable,
symbol_xor;

/* following used in Chapter 4 mainly for check_type */
extern  Symbol 
symbol_universal_type,
symbol_integer_type,
symbol_real_type,
symbol_composite_type,
symbol_equal_type;


extern  Symbol
symbol_addu,  /* +u */
symbol_subu,  /* -u */
symbol_abs,  /* abs */
symbol_add,  /* + */
symbol_sub,  /* - */
symbol_mul,  /* * */
symbol_div,  /* / */
symbol_mod,  /* mod */
symbol_rem,  /* rem */
symbol_exp,  /* ** */
symbol_cat,  /* & */
/* new symbols for the catenation operations */
symbol_cat_cc, /* &cc */
symbol_cat_ac, /* &ac */
symbol_cat_ca, /* &ca */
symbol_modi,  /* modi */
symbol_remi,  /* remi */
symbol_addui,  /* +ui */
symbol_subui,  /* -ui */
symbol_absi,  /* absi */
symbol_addi,  /* +i */
symbol_subi,  /* -i */
symbol_muli,  /* *i */
symbol_divi,  /* /i */
symbol_addufl,	/* +ufl */
symbol_subufl,	/* -ufl */
symbol_absfl,  /* absfl */
symbol_addfl,  /* +fl */
symbol_subfl,  /* -fl */
symbol_mulfl,  /* *fl */
symbol_divfl,  /* /fl */
symbol_addufx,	/* +ufx */
symbol_subufx,	/* -ufx */
symbol_absfx,  /* absfx */
symbol_addfx,  /* +fx */
symbol_subfx,  /* -fx */
symbol_mulfxi,	/* *fxi */
symbol_mulifx,	/* *ifx */
symbol_divfxi,	/* /fxi */
symbol_mulfli,	/* *fli */
symbol_mulifl,	/* *ifl */
symbol_divfli,	/* /fli */
symbol_expi,  /* **i */
symbol_expfl;  /* **fl */

extern  Tuple unary_sig;
extern  Tuple binary_sig; /* TBSL: to be initialized in sem0*/

extern  int num_predef_units; /* number of units in predef.ada */

extern  int errors; /* flag if errors are present*/

/* adaval_overflow is set to indicate overflow from adaval; corresponds
 * to SETL ADAVAL returning 'OVERFLOW'
 */
extern  int adaval_overflow; 
extern  char *FILENAME;
extern  FILE *MSGFILE;
/* added 24 sep 84*/
/* init_nodes is tuple of nodes needed by save_tree (10) */
extern  Tuple init_nodes;
extern  Tuple init_symbols; /* tuple of symbols created by sem initialization*/
/* varying length tuple, unit_nodes_n gives current length */
extern  Tuple unit_nodes;
#ifdef TBSL
extern  int unit_nodes_n;
#endif
extern  char *PREDEFNAME; /* name of predef file */
extern  char *AISFILENAME;
extern  int lib_option;
extern  int new_library; /* set if creating library */
extern  struct unit *pUnits[MAX_UNITS+1];
extern  Tuple lib_stub;
extern  Tuple stub_info;

extern  int seq_node_n; /* number of nodes allocated */
extern  Tuple seq_node; /* tuple of allocated nodes*/
extern  int seq_symbol_n;/* number of symbols allocated*/
extern  Tuple seq_symbol;	/* tuple of allocated symbols */

/* added 12 oct 84 to track nodes read from ast */
extern  int unit_number_now;
extern  int unit_numbers;
extern  int empty_unit_slots;

/* following variables used to trap references to selected node or symbol.
 * Node specified by trapns and trapnu (giving node sequence and unit),
 * symbol specified by trapss and trapsu (giving symbol sequence and unit).
 * These are meaningful only if either nonzero. When a trapped node referenced,
 * then procedure trap is called.
 * Code to watch for traps is planted at several places, notably sym_new,
 * node_new, zptupsym, zptupnod (these latter two do not call trap).
 */
extern  int trapns,trapnu,trapss,trapsu;

extern  Tuple aisunits_read;

extern  Nodemap node_map;
/* OPT_NAME is used by code_generator but defined here so it can be
 * referenced from libr procedures */
extern  Symbol OPT_NAME; /* This is to symbols what OPT_NODE is to nodes*/

/* I/O trace flags used to get trace prints when reading and writing
 * intermediate files. A trace level is one of the following:
 *	0	no trace
 *	1	brief trace
 *	2	full trace.
 */
extern  int iot_ifile; /* current trace level for ifile */
extern  int iot_ofile; /* current trace level for ofile */
extern  int iot_ais_r; /* trace level for ais files read */
extern  int iot_ais_w; /* trace level for ais files written */
extern  int iot_tre_r; /* trace level for tre files read */
extern  int iot_tre_w; /* trace level for tre files written */
extern  int iot_lib_r; /* trace level for lib files read */
extern  int iot_lib_w;/* set to trace writing lib file */
extern  char iot_ofile_type;
extern  char iot_ifile_type;

/* compiling predef */
extern  int	compiling_predef; /* set if we are compiling predef */

/* the following are used to avoid having string literals in code that
 * might be overlaid on the PC 
 */
extern  char *string_any_id; 
extern  char *string_ok;
extern  char *string_ds;
/* variables to report result from power_of_2(), thus avoiding
 * need to compute a tuple, as is done in SETL version.
 */
extern  int power_of_2_accuracy; 
extern  int power_of_2_power;
extern  Rational power_of_2_small;
extern  Set stubs_to_write;
extern  Tuple NOT_CHOSEN;
#endif
