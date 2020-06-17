/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#ifndef _config_h
#define _config_h

/* header file with common symbols to be used by all phases of Ada/Ed-C
 *
 * Note that some configuration information needs to be defined when porting
 * to a new machine or operating system.
 *
 * Most importantly the following symbolic constants need to be set for
 * each host
 *
 * WORDSIZE32 or WORDSIZE16 -- whether an "int" is 32 bits or 16 bits
 * BSD or SYSTEM_V          -- for unix systems
 * OP_SYS                   -- string representing info on machine/OS config
 * ALIGN2 or ALIGN4         -- systems which need alignment as explained below
 *
 * ALIGN2 defined if data items other than char must be aligned on at least
 * 2 byte boundaries. (e.g. MC68000).
 * ALIGN4 defined if data items other than char must be aligned on at least
 * 4 byte boundaries. (e.g. SPARC).
 */

/* AMIGA NOTE:                                                           */
/* Since some of the source files had differing machine defines due to   */
/* certain incompatibilites, I found no other choice but to force some   */
/* of the defines (or risk missing out on things like ALIGN2...)         */

#define OP_SYS "AmigaDos"
#define NULLFILENAME "nil:"
#define WORDSIZE32

#ifdef mc68000
#define ALIGN2
#endif

#ifdef sun
#define WORDSIZE32
#ifndef BSD
#define BSD
#endif
#ifdef mc68000
#define ALIGN2
#define OP_SYS "Sun 3 - Sun OS"
#endif
#ifdef sparc
#define ALIGN4
#define OP_SYS "Sparc - Sun OS"
#endif
#endif

#ifdef att3b
#define WORDSIZE32
#define SYSTEM_V
#define ALIGN2
#define OP_SYS "ATT 3B"
#endif

#ifdef ALIGN4 
#define ALIGN2
#endif

#ifdef ALIGN2
#define ALIGN_WORD
#endif

#if (defined BSD) || (defined SYSTEM_V)
#ifndef unix
#define unix
#endif
#endif

/* 
 * The following symbolic constants are defined for most implementations
 * and only need to be changed if necessary.
 * 
 * NULLFILENAME -- a name representation a null file
 *
 * Return codes passed between phases:
 *  RC_SUCCESS			everything ok 
 *	RC_ERRORS			errors found, successful termination
 *	RC_INTERNAL_ERROR	internal compiler error
 *	RC_ABORT			e.g. file not found
 *
 * MACHINE_CODE -- defined to turn on capability for generated code trace
 * in the compilation listing 
 *
 * HI_LEVEL_IO -- forces use of the high level i/o package of C rather than
 * the low level i/o package.
 *
 * LIBFILENAME -- the name of the library file. It should be "lib" unless
 * the operating system at hand uses this for another meaning.
 *
 */

#define RC_SUCCESS         0
#define RC_ERRORS          1
#define RC_INTERNAL_ERROR  2
#define RC_ABORT           3
#define MACHINE_CODE
#define HI_LEVEL_IO
#define LIBFILENAME "lib"
#define OM NULL

#ifdef unix
#define DIR_DELIMITER  "/"
/* #define NULLFILENAME "/dev/null" */
#define SUPPORT_PRAGMA_INTERFACE
#endif

#ifdef vms
#define WORDSIZE32
#undef RC_SUCCESS
#undef RC_ERRORS
#undef RC_INTERNAL_ERROR
#undef RC_ABORT
#define RC_SUCCESS          241
#define RC_ERRORS           245
#define RC_INTERNAL_ERROR   249
#define RC_ABORT            253
#define DIR_DELIMITER .
#define OP_SYS "VAX/VMS 5.0"
#define NULLFILENAME "nl:"
/* use low level io since at time the high level io didn't work on VMS */
#undef  HI_LEVEL_IO
#define LO_LEVEL_IO
#undef LIBFILENAME
/* Some vms sites running eunice use lib  to corresond to unix lib  */
#define LIBFILENAME "adalib"
#endif

#ifdef IBM_PC
#define WORDSIZE32
#define OP_SYS "(MS/DOS 3.0)"
#define NULLFILENAME "NUL"
#define DIR_DELIMITER  "\\"
/* pass error message strings by phrase number */
#define ERRNUM
/* for some reason, the PC doesn't like the name str_errmsgn, so rename */
#define str_errmsgn s_ermsgn
#define PREDEF_PC
/* define DEBUG_INT to enable -i i instruction trace */
/* enable on PC until certified except for EXPORT version */
#ifndef EXPORT
#define DEBUG_INT
#endif
/* define IO_EOF to avoid bug in Metaware library when try to read
 * past end of file; problem is that when call fgetc after EOF seen
 * Metaware library goes into loop. (shields 7-8-86)
 */
#define IO_EOF
#endif

#ifdef WORDSIZE32
#define DIGS	4
#define BAS	10000
#define MAX_INTEGER 2147483647
#define MIN_INTEGER -2147483648
#define ADA_MANTISSA_SIZE	24
/* BITS_SU gives the number of bits in a storage unit. */
#define BITS_SU 32
#endif

#ifdef WORDSIZE16
#define DIGS	2
#define BAS	100
#define MAX_INTEGER	32767
#define MIN_INTEGER	-32768
/* Define MAX_INTEGER_LONG if C ints and longs have different sizes. */
#define MAX_INTEGER_LONG 2147483647
#define MAX_INTEGER_LONG_STRING "2147483647"
#define ADA_MANTISSA_SIZE	24
/* BITS_SU gives the number of bits in a storage unit. */
#define BITS_SU 16
#endif

#define TRUE 1
#define FALSE 0

/* Error codes written by various phases to MSGFILE */
/* Note: None of these should be defined to be 0 */

#define PRAGMA_LIST_ON 1
#define PRAGMA_LIST_OFF 2
#define PRAGMA_LIST_ERR 3
#define PRAGMA_PAGE 4

#define ERR_LEXICAL 10
#define ERR_SYNTAX 11
#define ERR_SEMANTIC 12
#define ERR_WARNING 13
#define INFORMATION 14
#define ERR_BIND    15
#define ERR_COMPILER 16

/* Limits for interpreter */

#define MAX_PRIO		5  /* max priority value, prio = 0..5 */
#define MAX_TASKS     128  /* max number of tasks */
#define MAX_CUNITS     64  /* max number of compilation units */
#define MAX_BODIES    256  /* max subprgs+task bodies+spec/body not in cunit */
#define MAX_ALTS       64  /* maximum open alternatives */
#define MAX_DISCR      64  /* maximum number of discriminants in record */
#define MAX_IDLEN     256  /* maximum identifier length */
#define MAX_LINE_LEN  256  /* maximum line length */

#ifdef IBM_PC
#define MAX_MEM    32000  /* maximum words per heap */
#else
#define MAX_MEM    128000 /* maximum words in heap */
#endif

/* version numbers for library files */
#define AIS_VERSION '1'
#define AST_VERSION '1'
#define LIB_VERSION '1'
#define STUB_VERSION '1'
#define TRE_VERSION '1'

/*  The following is a variable used for deciding whether to optimize
 *  an array aggregate.  If specifies the maximum size of a segment
 *  to be created on the data stack.  If an array aggregate is larger
 *  than this, is will NOT be optimized.
 *  It is also used in type.c in a way that should be reviewed (ds 7-18-86)
 */
#define MAX_STATIC_SIZE 10000

/* define SMALLOC to use smalloc for string allocation */
#define SMALLOC

#ifndef EXPORT
#define DEBUG
#define CHAOS	
#define TRACE
/* define IOT to compile trace code for i/o */
#define IOT
#else
#define chaos(v) 			exit_internal_error()
#define emalloct(n,s)			emalloc(n)
#define malloct(n,s)			malloc(n)
#define erealloct(ptr, size, msg)	erealloc(ptr, size)
#define efreet(p,msg)			efree(p)
#define ecalloct(n,m,msg)		ecalloc(n,m)
#endif

#endif /* _config_h */
