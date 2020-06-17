#define INIT(v) 
#define GEN



extern   int	list_code INIT(0); /* set if GEN option selected */

#ifdef TBSN
PREDEF_UNITS; /* predefined units */
#endif

extern   int	debug_flag;
extern   int	debug_line INIT(0);

extern   char *MAINunit; /* name of main unit (from MAIN opo */
extern   char *interface_files;


extern   int	bind_option; /* bind option */
extern   int   bind_only_option INIT(0); /* set when binding only, no generation */
extern   int	line_option INIT(0); /* LINE option use -L to set */
extern   int	gen_option INIT(1); /* GEN option: set if want to generate code */


extern   int ada_line; /* line number for error file (mixed_case) */
extern   int save_ada_line; /* used to save value of ada_line  */


#ifdef TBSN
STIME; /* ?? time */
BTIME; /* binding time */
#endif

extern   int 	NB_INSTRUCTIONS; /* number of instructions generated */
extern   int 	NB_STATEMENTS; /* number of statements processed?? */

extern   int 	ERROR_IN_UNIT; /* boolean set if errors in unit */
extern   int 	STUBS_IN_UNIT; /* boolean set if stubs in unit */

extern   int 	TARGET; 


extern   Node	FIRST_NODE;		 /* first node in a compilation unit */

extern   Tuple 	UNIT_FIRST_NODE;	 /* map { unit -> FIRST_NODE } */


extern   Tuple    RENAME_MAP;          

#ifdef TBSN
-- this is not referenced		ds 22-feb-85
extern  	Node ROOT_NODE;           /* root node of unit to be expanded */
#endif



extern   Tuple    CODE_SLOTS;          /* map showing code_slots occupation */

extern   Tuple	DATA_SLOTS;          /* map showing data_slots occupation */

extern   Tuple	EXCEPTION_SLOTS;     /* map showing exception_slots occupation */
extern   Tuple    CODE_SEGMENT_MAP;    /* map { number -> [actual code] } */
extern   Tuple    DATA_SEGMENT_MAP;    /* map { number -> [actual data] } */


extern   int    CURRENT_DATA_SEGMENT;   /* number of current data segment */
extern   int    CURRENT_CODE_SEGMENT;   /* number of current code  segment */

extern   Tuple    GENERATED_OBJECTS;



#ifdef TBSN
    MISC,           /* map used to store miscellanous infos like: */

#endif

extern   Tuple LOCAL_REFERENCE_MAP;

extern   int REFERENCE_SEGMENT;
extern   int REFERENCE_OFFSET;

#ifdef TBSN
Tuple	CONSTANT_MAP;
#endif


extern   int    LAST_OFFSET;    /* first available offset in current stack frame */
extern   int    MAX_OFFSET;     /* largest offset in current stack frame */
extern   int    TASKS_DECLARED; /* flag indicating possible presence of tasks in frame */
extern   int	SPECS_DECLARED; /* count of # of specs requiring a */
extern   Tuple  SUBPROG_SPECS; /* set of subprograms having an explicit spec in the */

extern   Tuple	SOURCE;         /* the current list of statements to be processed */


extern   Tuple    EMAP;           /* Various temporary storage: */
extern   Tuple	EMAP_VALUE; /* value of emap if defined, set by emap_get */

extern   int	CURRENT_LEVEL;  /* used for static depth of blocks */

extern   Tuple    PARAMETER_SET; /* Tuple of symbols for formal parameters */

extern   Tuple    RELAY_SET;

extern   Tuple    DANGLING_RELAY_SETS;

extern   Tuple    SUBPROG_PATCH;
	
extern   Tuple   CODE_PATCH_SET;
extern   Tuple    DATA_PATCH_SET;


extern   int	CURRENT_FIELD_NUMBER;
extern   int	CURRENT_FIELD_OFFSET;
extern   int	STATIC_REC; /* boolean */
extern   Tuple   INTERNAL_ACCESSED_TYPES; /* of symbols */

extern   Tuple  axqfiles_read;	    /* set of already read AXQfiles */
extern   Tuple  call_lib_unit;	    /* Accumulates code for idle_task to call library */
extern   Tuple  PRECEDES_MAP;      /* Map representing relationship between units */
extern   Tuple  DELAYED_MAP;
extern   Tuple  compilation_table; /* Table of compilation units giving the order of */
extern   Tuple  late_instances;    /* Map from unit unique name to a set of late */
extern   Tuple interfaced_procedures;
extern   int	 interface_counter INIT(256);
extern   int    interface_flag;    /* equals 1 if there are interfaced procedures, 
                                      0 otherwise */


extern   int deleted_instructions;
#ifdef TBSN
    optimizable_codes,
#endif



extern      Tuple	just_read;
extern   Node	unit_node;

extern   Symbol symbol_constrained_type;
extern   Symbol symbol_accept_return INIT((Symbol)0); /* see gen_accept */
extern   Symbol symbol_used,symbol_unused;
extern   Symbol symbol_main_task;
extern   Symbol symbol_type_mark;
extern   Symbol symbol_task_block;
extern   Symbol symbol_mulfix; /* expr.c ...*/

extern   long RAT_TOF_1,RAT_TOF_2;

extern   Explicit_ref explicit_ref_0; /* for explicit reference of [0,0] */
extern   Tuple unit_slots  INIT((Tuple) 0);
extern   int *ivalue_1,*ivalue_10; /* long integer forms of 1 and 10 */
extern   Const int_const_0; /* Const for integer 0 */
extern   Rational rat_value_10; /* 10 as rational */
extern   Tuple global_reference_tuple INIT((Tuple)0);

extern   Const int_const_null_task; /* for NULL_TASK */
#ifdef BINDER_GEN
extern   int binder_phase  INIT(0); /* set non-zero if binder phase */
#endif