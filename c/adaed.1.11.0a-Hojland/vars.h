#define INIT(v) 
#ifndef _vars_h
#define _vars_h




extern   double	ADA_MIN_REAL INIT((-79228162514264337593543950336.0));/* - 2.0**96 */
extern   double	ADA_MAX_REAL INIT(79228162514264337593543950336.0);/* 2.0**96 */
extern   int	ADA_REAL_DIGITS INIT(6);
extern   int	cdebug2;

extern   Declaredmap declared_all[4];	/* array of standard declareds */
extern   int	full_others;  /* a boolean */
extern   int	fold_context; /* a boolean */
extern   int	node_count;
extern   Node current_node;
extern   Node OPT_NODE;	/*initialized in sem0 */


extern   Declaredmap base_declared[4];
extern   Symbol base_declared_symbols[4];
extern   Tuple scope_st;			/* stack of lexical scopes */
extern   Tuple has_return_stk;		/* stack to track return statements */
extern   Tuple newtypes;			/* list of type declaration code to be */
extern   Tuple lab_seen;			/* set of labels in procedure . */
extern   Tuple current_instances;	/* stack for recursive instantiation check */

extern   Symbol scope_name;		/* unique name for each scope */
extern   char *unit_name;		/* compilation unit information */
extern   Tuple all_vis;			/* Modules whose visibility is required */

extern   Tuple open_scopes;		/* nest of currently open scopes, from inner */
extern   Tuple used_mods;		/* packages appearing in a use clause */
extern   Tuple vis_mods;	/* list of package names visible in this unit*/

extern   int	noop_error;

extern   Set non_local_names;		/* To collect non_local references in each subp. */

extern   int	out_context;		/* Signals valid appearance of out parameter.  */

extern   Symbol
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

extern   Symbol 
symbol_universal_type,
symbol_integer_type,
symbol_real_type,
symbol_composite_type,
symbol_equal_type;


extern   Symbol
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

extern   Tuple unary_sig;
extern   Tuple binary_sig; /* TBSL: to be initialized in sem0*/

extern   int num_predef_units; /* number of units in predef.ada */

extern   int errors INIT(FALSE); /* flag if errors are present*/

extern   int adaval_overflow; 
extern   char *FILENAME;
extern   FILE *MSGFILE INIT(stdout);
extern   Tuple init_nodes;
extern   Tuple init_symbols; /* tuple of symbols created by sem initialization*/
extern   Tuple unit_nodes;
#ifdef TBSL
extern   int unit_nodes_n INIT(0);
#endif
extern   char *PREDEFNAME; /* name of predef file */
extern   char *AISFILENAME;
extern   int lib_option INIT(FALSE);
extern   int new_library INIT(FALSE); /* set if creating library */
extern   struct unit *pUnits[MAX_UNITS+1];
extern   Tuple lib_stub;
extern   Tuple stub_info;

extern   int seq_node_n INIT(0); /* number of nodes allocated */
extern   Tuple seq_node; /* tuple of allocated nodes*/
extern   int seq_symbol_n INIT(0);/* number of symbols allocated*/
extern   Tuple seq_symbol;	/* tuple of allocated symbols */

extern   int unit_number_now INIT(0);
extern   int unit_numbers INIT(0);
extern   int empty_unit_slots INIT(0);

extern   int trapns INIT(0),trapnu INIT(0),trapss INIT(0),trapsu INIT(0);

extern   Tuple aisunits_read;

extern   Nodemap node_map;
extern   Symbol OPT_NAME; /* This is to symbols what OPT_NODE is to nodes*/

extern   int iot_ifile INIT(0); /* current trace level for ifile */
extern   int iot_ofile INIT(0); /* current trace level for ofile */
extern   int iot_ais_r INIT(0); /* trace level for ais files read */
extern   int iot_ais_w INIT(0); /* trace level for ais files written */
extern   int iot_tre_r INIT(0); /* trace level for tre files read */
extern   int iot_tre_w INIT(0); /* trace level for tre files written */
extern   int iot_lib_r INIT(0); /* trace level for lib files read */
extern   int iot_lib_w INIT(0);/* set to trace writing lib file */
extern   char iot_ofile_type;
extern   char iot_ifile_type;

extern   int	compiling_predef INIT(0); /* set if we are compiling predef */

extern   char *string_any_id  INIT("any_id"); 
extern   char *string_ok  INIT("ok");
extern   char *string_ds  INIT("$D$");
extern   int power_of_2_accuracy; 
extern   int power_of_2_power;
extern   Rational power_of_2_small;
extern   Set stubs_to_write;
extern   Tuple NOT_CHOSEN;
#endif
