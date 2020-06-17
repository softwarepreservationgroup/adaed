/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#define GEN

#include "hdr.h"
#include <stdio.h>

/* ada_min_integer; ada_max_integer; are defined in vars.c */

/* SETL GENflag is list_code in C */
 int	list_code  =  (0); /* set if GEN option selected */

#ifdef TBSN
PREDEF_UNITS; /* predefined units */
#endif

 int	debug_flag;
/* set debug_line to line number to cause call to trap_line when
 * starting to generating code for that line 
 * (see maincase.c)		ds 7-19-85
 */
 int	debug_line  =  (0);

/* MAINunit identifies the main unit. In SETL this is specified by
 * MAIN program option. In C it will be character string
 */
 char *MAINunit; /* name of main unit (from MAIN opo */
 char *interface_files;


 int	bind_option; /* bind option */
 int   bind_only_option  =  (0); /* set when binding only, no generation */
 int	line_option  =  (0); /* LINE option use -L to set */
 int	gen_option  =  (1); /* GEN option: set if want to generate code */


 int ada_line; /* line number for error file (mixed_case) */
 int save_ada_line; /* used to save value of ada_line  */


#ifdef TBSN
/* STIME and BTIME are used to hold elapsed times in SETL version
 * and are reals there. They are not needed in first C version.
 */
STIME; /* ?? time */
BTIME; /* binding time */
#endif

 int 	NB_INSTRUCTIONS; /* number of instructions generated */
 int 	NB_STATEMENTS; /* number of statements processed?? */

 int 	ERROR_IN_UNIT; /* boolean set if errors in unit */
 int 	STUBS_IN_UNIT; /* boolean set if stubs in unit */

/* In SETL, TARGET indicates target machine and is either SETL or IBMPC
 * In C will be integer
 */
 int 	TARGET; 


 Node	FIRST_NODE;		 /* first node in a compilation unit */

 Tuple 	UNIT_FIRST_NODE;	 /* map { unit -> FIRST_NODE } */
/* Represent as 'tuple map' in C */


 Tuple    RENAME_MAP;          
/* map { generic_name -> instance_name } 
 * This is kept as a tuple in C with successive pairs of elements giving
 * domain and range values (tuple as map).
 */

#ifdef TBSN
-- this is not referenced		ds 22-feb-85
	Node ROOT_NODE;           /* root node of unit to be expanded */
#endif

/* Generated code is built up in CODE_SEGMENT, generated data in
 * DATA_SEGMENT. In SETL these are tuples, and will also be tuples
 * in first C version.
 * DATA_SEGMENT_MAIN is data segment for main code slot, initialized
 * by segment_main_data();
 * Since the type of these (Segment) is not known to all files
 * the variables CODE_SEGMENT, DATA_SEGMENT, DATA_SEGMENT_MAIN
 * VARIANT_TABLE and FIELD_TABLE are defined in init.c and
 * declared as extern's explicitly where needed.
 */


 Tuple    CODE_SLOTS;          /* map showing code_slots occupation */
                         /*  { procedure_names -> number } */

 Tuple	DATA_SLOTS;          /* map showing data_slots occupation */
                         /*  { compilation_unit_names -> number } */

 Tuple	EXCEPTION_SLOTS;     /* map showing exception_slots occupation */
   		         /*  { exception_names -> number } */
 Tuple    CODE_SEGMENT_MAP;    /* map { number -> [actual code] } */
 Tuple    DATA_SEGMENT_MAP;    /* map { number -> [actual data] } */


 int    CURRENT_DATA_SEGMENT;   /* number of current data segment */
 int    CURRENT_CODE_SEGMENT;   /* number of current code  segment */
/* UNIT_NAME is variable unit_name declared in vars.c */
	/* name of the current compilation unit */

/*
 * GENERATED_OBJECTS is a set of symbols in the SETL version. It is
 * kept as a tuple in the C version. The most common use is in the
 * form
 *	GENERATED_OBJECTS with:= sym;
 * which we will write in C as
 *	generate_object(sym);
 * This will permit option of choosing more efficient data structure
 * later.
 */
 Tuple    GENERATED_OBJECTS;

/*  Symbol table and extended symbol table : */


#ifdef TBSN
/* MISC will be symbol table fields in C version */
    MISC,           /* map used to store miscellanous infos like: */
                    /* for types: */
                    /*   a boolean indicating that they contain tasks */
/* NOTE: Info in MISC is save in library (AXQ files) */

#endif

/* REFERENCE_MAP and LOCAL_REFERENCE_MAP are used in SETL to give
 * the address assigned to symbols. 
 * LOCAL_REFERENCE_MAP is 'sparse' map from symbols to offsets; note
 * in particular that it is copied.
 * See ghdr.c for description of realization of local_reference_map.
 */
 Tuple LOCAL_REFERENCE_MAP;

/* reference_of sets these globals */
 int REFERENCE_SEGMENT;
 int REFERENCE_OFFSET;

/* CONSTANT_MAP is a map from constant values to generated symbols. It
 * is referenced only in procedure get_constant_name. The representation
 * in C is still not clear. 
 *
 * Constant map is optimization that is not needed in C 
 * It is referenced ony in get_constant_name to reuse locations having
 * the same value. 
 */
#ifdef TBSN
Tuple	CONSTANT_MAP;
#endif


 int    LAST_OFFSET;    /* first available offset in current stack frame */
 int    MAX_OFFSET;     /* largest offset in current stack frame */
 int    TASKS_DECLARED; /* flag indicating possible presence of tasks in frame */
 int	SPECS_DECLARED; /* count of # of specs requiring a */
   		    /* body in a library package. */
 Tuple  SUBPROG_SPECS; /* set of subprograms having an explicit spec in the */
   		    /* current program unit. */

 Tuple	SOURCE;         /* the current list of statements to be processed */


/* in C, EMAP is maintained by emap_... procedures: */
 Tuple    EMAP;           /* Various temporary storage: */
                    /* Type:     -> list of dependent deferred types */
                    /* Constant: -> Boolean true if a deferred constant */
/* NOTE: info in EMAP is NOT save in libarry (AXQ files) */
 Tuple	EMAP_VALUE; /* value of emap if defined, set by emap_get */

 int	CURRENT_LEVEL;  /* used for static depth of blocks */

 Tuple    PARAMETER_SET; /* Tuple of symbols for formal parameters */

/* RELAY_SET is a tuple of symbols. */
/* Note; need to review if this need be copied */
/* Note: RELAY_SET should be represented more efficiently than just
 * as tuple if possible, as may be 'large'.
 */
 Tuple    RELAY_SET;

/* DANGLING_RELAY_SETS is a tuple of relay sets */
 Tuple    DANGLING_RELAY_SETS;

 Tuple    SUBPROG_PATCH;
	
 Tuple   CODE_PATCH_SET;
 Tuple    DATA_PATCH_SET;

/* Global variables used for record type elaboration */

 int	CURRENT_FIELD_NUMBER;
 int	CURRENT_FIELD_OFFSET;
 int	REC_WITH_TASKS; /* boolean */
 int	STATIC_REC; /* boolean */
 Tuple   INTERNAL_ACCESSED_TYPES; /* of symbols */

/* Variables used by the binder */
 Tuple  axqfiles_read;	    /* set of already read AXQfiles */
 Tuple  call_lib_unit;	    /* Accumulates code for idle_task to call library */
   			    /* packages. */
 Tuple  PRECEDES_MAP;      /* Map representing relationship between units */
 Tuple  DELAYED_MAP;
 Tuple  compilation_table; /* Table of compilation units giving the order of */
     			    /* compilation. */
 Tuple  late_instances;    /* Map from unit unique name to a set of late */
                            /* instances */
 Tuple interfaced_procedures;
     			    /* set of the part of codes generated for each
         		       intefaced procedures */
 int	 interface_counter  =  (256);
			    /* integer associated with each subprogram which */
			    /* has a pragma INTERFACE */
 int    interface_flag;    /* equals 1 if there are interfaced procedures, 
                                      0 otherwise */

/* Variables used by the peep-hole optimizer */

 int deleted_instructions;
#ifdef TBSN
    optimizable_codes,
#endif


/* "local" variables (do not use these names elsewhere!) */

    Tuple	just_read;
 Node	unit_node;

/* TBSL: symbol_constrained_type added 21 jan - needs to be initialized*/
 Symbol symbol_constrained_type;
 Symbol symbol_accept_return  =  ((Symbol)0); /* see gen_accept */
/* TBSL: see if following really needed for 'used' and 'unused':
  Note; "used" and "unused" are just used for loops can use TRUE and FALSE
  There is label generated at startof loop needed only if exit; if exit
  not present, the label is "unused" and some needless code need not
  be generated.
 */
 Symbol symbol_used,symbol_unused;
/* symbol_main_task_type is defined in adasem (vars.c) */
/* define symbol_main_task used for main_task here */
 Symbol symbol_main_task;
 Symbol symbol_type_mark;
 Symbol symbol_task_block;
 Symbol symbol_mulfix; /* expr.c ...*/

/* rat_tof() returns its results in the globals:*/
/* make RAT_TOF_1 long for initial C version  ds 6-6-86 */
 long RAT_TOF_1,RAT_TOF_2;

 Explicit_ref explicit_ref_0; /* for explicit reference of [0,0] */
/* unit_slots is global tuple maintained by unit_slots_get() and
 * unit_slots_put(). Entries are indexed by unit number. Each entry
 * is tuple of slot numbers; the first three corresponding to OWNED_SLOTS
 * the last two to BORROWED slots.
 */
 Tuple unit_slots   =  ((Tuple) 0);
 int *ivalue_1,*ivalue_10; /* long integer forms of 1 and 10 */
 Const int_const_0; /* Const for integer 0 */
 Rational rat_value_10; /* 10 as rational */
/* global_reference_tupel is used for saving global addresses
 * for trace printout 
 */
 Tuple global_reference_tuple  =  ((Tuple)0);

 Const int_const_null_task; /* for NULL_TASK */
#ifdef BINDER_GEN
 int binder_phase   =  (0); /* set non-zero if binder phase */
#endif
