/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#ifdef __GNUG__
#define WAITPARM (int*)
#else
#define WAITPARM (union wait*)
#endif
#include "config.h"
#include "adamrgprots.h"
#include "miscprots.h"

#ifdef AMIGA
#define fork vfork
#endif

#ifdef vms
/*  Temporary fix to avoid missing adacomp.h
#define fork vfork
#define unlink delete
#include "adacomp.h"
#include descrip
#include <file.h>
#include <types.h>
#include <stat.h>
*/
#else
#include <sys/types.h>
#ifdef IBM_PC
#include <fcntl.h>
#include <process.h>
#else
#include <sys/file.h>
#endif
#endif

#ifdef SYSTEM_V
#include <fcntl.h>
#endif
#include <signal.h>

#ifdef BSD
#include "time.h"
#include <sys/resource.h>
#endif

static int check_status(int, char *, char *);
static char *getsym(char *, char *);
static void arg_dump();
static int run_prog(char *, char **);
static void delete_file(char *);
#ifdef SYSTEM_V
static  int mkdir(char *, int);
#endif
#ifdef vms
static void fold_upper(char *s)								/*;fold_upper*/
#endif

char   *argname;
FILE *MALFILE; /* for use by misc malloc trace */
int     opts_cnt;
char   *other_opts[20];
char   *interface_opts[20];
int     interface_cnt = 0;
int	maxstatus = RC_SUCCESS; /* maximum exit status from called programs */
int     exec_trace = 0;	/* set to print generated command lines */

/* names of executables to use if not defined by environment */
#define PRS_NAME "adaprs"
#define SEM_NAME "adasem"
#define GEN_NAME "adagen"
#define BND_NAME "adabind"

/* status_get extracts program exit code */
#ifdef IBM_PC
#define status_get(s)        (s)
#define system_status_get(s) (s)
#else
#ifdef vms
#define status_get(s)        (s)
#define system_status_get(s) (s)
#else
#define status_get(s)  	     ((s)>>8)
#define system_status_get(s) ((s) & 0xff)
#endif
#endif

char   *base_name;

main(int argc, char **argv)
{
    int     c,fp;
    int     status, ok = TRUE;
    extern int  optind;
    extern char *optarg;
#ifdef vms
    extern char *strjoin(); /* vms only */
    char   *strchr();
    char   *DIRECTORY_START = "[.";
#endif
    char   *PRS, *SEM, *GEN, *BND; 
    char   *arg_name;
    char   *lib_name;
    char   *list_name;
    char   *source_name;
    char   *msg_name;
    char   *tmp_name;
    char   *s_temp;
    char   *l_name;
    char   *basep;
    int	   prefix_len, base_len, suffix_len;
    char   *lib_opt_str, *main_unit_name;
    char   *object_files = "";
    char   *sem_options, *gen_options;
    int     bind_opt = 0, main_opt = 0, save_msg_opt = 0 ;
    int     list_opt = FALSE;   /* set to generate a listing */
    char   *list_arg;		/* for passing list_opt to mrg */
    int     lib_opt = FALSE;    /* set to TRUE if -l specified */
    int     newlib_opt = FALSE; /* set to TRUE if -n specified */
    int	    time_limit = 15;    /* default time limit in minutes */
#ifdef vms
    char        buffer[50];
    short       rlength;
    struct      dsc$descriptor_s entity_desc;
    struct      dsc$descriptor_s value_desc;
    struct      dsc$descriptor_s string_desc;
    struct      dsc$descriptor_s old_filespec;
    struct      dsc$descriptor_s new_filespec;
#endif
#ifdef BSD
    struct rlimit rlp;
#endif

/* initializations */
    arg_name = (char *) 0;
    lib_name = (char *) 0;
    sem_options = "";
    gen_options = "";

#ifdef vms
         entity_desc.dsc$b_dtype = DSC$K_DTYPE_T;
         entity_desc.dsc$b_class = DSC$K_CLASS_S;
         value_desc.dsc$b_dtype = DSC$K_DTYPE_T;
         value_desc.dsc$b_class = DSC$K_CLASS_S;
         value_desc.dsc$a_pointer = buffer;
         value_desc.dsc$w_length = 50;

         entity_desc.dsc$a_pointer = "VERBOSE";
         entity_desc.dsc$w_length = 7;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("VERBOSE status %d\n",status);
#endif
         exec_trace = status & 1;
         if (exec_trace) fprintf(stderr,"Command line: ADACOMP /VERBOSE");

         entity_desc.dsc$a_pointer = "ADALINES";
         entity_desc.dsc$w_length = 8;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("ADALINES status %d\n",status);
#endif
         if (status & 1) {
             gen_options = strjoin(gen_options,"l");
             if (exec_trace) fprintf(stderr,"/ADALINES");
         }
         
         entity_desc.dsc$a_pointer = "BIND";
         entity_desc.dsc$w_length = 4;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("BIND status %d\n",status);
#endif
         bind_opt = (status & 1);
         if (bind_opt && exec_trace) fprintf(stderr,"/BIND");
         
         entity_desc.dsc$a_pointer = "FILE";
         entity_desc.dsc$w_length = 4;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("FILE status %d\n",status);
#endif
         if (status & 1) {
             status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
             value_desc.dsc$a_pointer[rlength] = '\0';
             arg_name = strjoin(value_desc.dsc$a_pointer,"");
             if (exec_trace) fprintf(stderr," %s ",arg_name);
#ifdef DEBUG
             printf("FILE %s\n", arg_name);
#endif
         }
         
         entity_desc.dsc$a_pointer = "LIBRARY";
         entity_desc.dsc$w_length = 7;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("LIBRARY status %d\n",status);
#endif
         lib_opt = status & 1;
         if (lib_opt) {
             status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
             value_desc.dsc$a_pointer[rlength] = '\0';
             lib_name = strjoin(value_desc.dsc$a_pointer,"");
             if (exec_trace) fprintf(stderr,"/LIBRARY=%s",lib_name);
#ifdef DEBUG
             printf("LIBRARY %s\n", lib_name);
#endif
         }
         
         entity_desc.dsc$a_pointer = "NEWLIBRARY";
         entity_desc.dsc$w_length = 10;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("NEWLIBRARY status %d\n",status);
#endif
         newlib_opt = status & 1;
         if (newlib_opt && exec_trace) fprintf(stderr,"/NEWLIBRARY");

         entity_desc.dsc$a_pointer = "LISTING";
         entity_desc.dsc$w_length = 7;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("LISTING status %d\n",status);
#endif
         list_opt = status & 1;
         if (list_opt && exec_trace) fprintf(stderr,"/LISTING");

         entity_desc.dsc$a_pointer = "MAIN_UNIT";
         entity_desc.dsc$w_length = 9;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("MAIN_UNIT status %d\n",status);
#endif
         main_opt = status & 1;
         if (main_opt) {
             status = CLI$GET_VALUE(&entity_desc, &value_desc, &rlength);
             value_desc.dsc$a_pointer[rlength] = '\0';
             main_unit_name = strjoin(value_desc.dsc$a_pointer,"");
             fold_upper(main_unit_name);
             if (exec_trace) fprintf(stderr,"/MAIN_UNIT=%s",main_unit_name);
#ifdef DEBUG
             printf("MAIN_UNIT %s\n", main_unit_name);
#endif
         }

         entity_desc.dsc$a_pointer = "MESSAGES";
         entity_desc.dsc$w_length = 8;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("MESSAGES status %d\n",status);
#endif
         save_msg_opt = status & 1;
         if (save_msg_opt && exec_trace) fprintf(stderr,"/MESSAGES");

         entity_desc.dsc$a_pointer = "MACHINE_CODE";
         entity_desc.dsc$w_length = 12;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("MACHINE_CODE status %d\n",status);
#endif
         if (status & 1) {
             gen_options = strjoin(gen_options,"g");
             if (exec_trace) fprintf(stderr,"/MACHINE_CODE");
         }
         if (exec_trace) fprintf(stderr,"\n");

         entity_desc.dsc$a_pointer = "PREDEF";
         entity_desc.dsc$w_length = 6;
         status = CLI$PRESENT(&entity_desc);
#ifdef DEBUG
         printf("PREDEF status %d\n",status);
#endif
         if (status & 1) {
             if (exec_trace) fprintf(stderr,"/PREDEF");
	     s_temp = emalloc(strlen(sem_options) + 2);
  	     strcpy(s_temp, sem_options);
             strcat(s_temp, "p");
	     sem_options = s_temp;
	     s_temp = emalloc(strlen(gen_options) + 2);
	     strcpy(s_temp, gen_options);
             strcat(s_temp, "p");
	     gen_options = s_temp;
	 }
#else
/*
 * command options
 *	-a		generated line number instructions
 *	-b 		bind the unit specified by 'm' option
 *	-g		insert generated code into listing
 *      -i              specify object files and libraries for pragma interface
 *	-l libname	(old) library libname
 *	-m main unit  	specify the main binding unit.
 *			or use default main unit
 *	-n libname	new library libname
 *      -s		create source program listing
 *	-v		trace executed commands and exit status
 *      -M		save message files (for running B tests)
 *  	-P		compile predef
 */

    while((c = getopt(argc, argv, "abgl:m:nsvMPi:")) != EOF) {

	switch(c) {
	    case 'a':
		s_temp = emalloc(strlen(gen_options) + 2);
		strcpy(s_temp, gen_options);
	        strcat(s_temp, "l");
		gen_options = s_temp;
		break;
	    case 'b':
		bind_opt = 1;
		break;
	    case 'g':
		s_temp = emalloc(strlen(gen_options) + 2);
		strcpy(s_temp, gen_options);
	        strcat(s_temp, "g");
		gen_options = s_temp;
		break;
	    case 'l':
		lib_opt = TRUE;
		lib_name = emalloc(strlen(optarg) + 1);
		strcpy(lib_name, optarg);
		break;
	    case 'm':    
		main_opt = 1;
		main_unit_name = emalloc(strlen(optarg) + 1);
		strcpy(main_unit_name, optarg);
		break;
	    case 'n':
		newlib_opt = TRUE;
		break;
            case 'i':
		s_temp = emalloc(strlen(optarg) + 1);
		strcpy(s_temp, optarg);
		interface_opts[interface_cnt++] = s_temp;
                break;
	    case 's':
		list_opt++;
		break;
	    case 'v':
		exec_trace++;
		break;
	    case 'M':
		save_msg_opt = TRUE ;
		break;
	    case 'P':
		s_temp = emalloc(strlen(sem_options) + 2);
		strcpy(s_temp, sem_options);
	        strcat(s_temp, "p");
		sem_options = s_temp;
		s_temp = emalloc(strlen(gen_options) + 2);
		strcpy(s_temp, gen_options);
	        strcat(s_temp, "p");
		gen_options = s_temp;
		break;
	    case '?':
		exit(RC_ABORT);
		break;
	    default:
		fprintf(stderr, "Unknown Option: %c\n", c);
		exit(RC_ABORT);
	}
    }
    if (optind < argc)
	arg_name = argv[optind];
    if (arg_name == (char *) 0) {
	fprintf(stderr,"Invalid Usage: No ada file specified\n");
	exit(RC_ABORT);
    }
#endif
    if (!lib_opt) { /* if -l not specified, try to get from environment */
#ifdef vms
       lib_name = getenv("ADAEDLIB");
#else
       lib_name = getenv("ADALIB");
#endif
       if (lib_name!=(char *)0) {
	   lib_opt++;
	}
	if (lib_opt) {
#ifdef vms
	    printf("library defined by ADAEDLIB: %s\n",lib_name);
#else
	    printf("library defined by ADALIB: %s\n",lib_name);
#endif
	}
    }
    if (!lib_opt) {
#ifdef vms
       LIB$SIGNAL(MSG_USAGE);
#else
       fprintf(stderr,
		"Invalid Usage: please specify a library\n");
       exit(RC_ABORT);
#endif
    }
#ifdef BSD
    getrlimit(RLIMIT_CPU,&rlp);
    (&rlp)->rlim_cur = time_limit*60;     /* limit to time_limit mins */
    setrlimit(RLIMIT_CPU,&rlp);
#endif

    basep = parsefile(arg_name, &prefix_len, &base_len, &suffix_len);
    /* check for presence of ada file;  if none, make it ada */
    if (suffix_len ==0) {
	source_name = emalloc(strlen(arg_name) + 4 + 1);
	strcpy(source_name, arg_name);
	strcat(source_name, ".ada");
    }
    else {
	source_name = arg_name;
    }
    base_name = emalloc(base_len + 1);
    strncpy(base_name, basep, base_len);
    if ((fp = open(source_name,O_RDONLY,0700)) < 0) {
#ifdef vms
        string_desc.dsc$w_length = strlen(source_name);
        string_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        string_desc.dsc$b_class = DSC$K_CLASS_S;
        string_desc.dsc$a_pointer = source_name;
        LIB$SIGNAL(MSG_ADAFILE,1,&string_desc);
        exit();
#else
	fprintf(stderr,"Cannot access file %s\n",source_name);
	exit(RC_ABORT);
#endif
    }
    close(fp);


    umask(0);
    if (newlib_opt){
        if (exec_trace) {
	    fprintf(stderr, "mkdir %s ", lib_name);
        }
        status = mkdir(lib_name, '\377');
        if (exec_trace) {
	    fprintf(stderr, " ? %d\n", status);
        }
    }
    status = 0;
    if (status) {
#ifdef vms
        string_desc.dsc$w_length = strlen(lib_name);
        string_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        string_desc.dsc$b_class = DSC$K_CLASS_S;
        string_desc.dsc$a_pointer = lib_name;
        LIB$SIGNAL(MSG_NOLIBRARY,1,&string_desc);
        exit();
#else
        fprintf(stderr,"%s cannot be used as a library\n", lib_name);
        exit(RC_ABORT);
#endif
    }
    if (!newlib_opt) {
        /* check for presence of library file */
#ifdef vms
	l_name = emalloc(strlen(lib_name + strlen(LIBFILENAME) + 4));
        if (strchr(lib_name,'[')) {
	   strcpy(l_name, lib_name);
	}
	else {
	   strcpy(l_name, DIRECTORY_START);
           strcat(l_name, lib_name);
	}
#else
	l_name = emalloc(strlen(lib_name) + strlen(LIBFILENAME) + 2);
	strcpy(l_name, lib_name);
#endif

#ifdef AMIGA
    strcat(l_name, "/");
#endif
#ifdef BSD
	strcat(l_name, "/");
#endif
#ifdef SYSTEM_V
	strcat(l_name, "/");
#endif
#ifdef IBM_PC
	strcat(l_name, "/");
#endif
#ifdef vms
        if (!strchr(lib_name,'['))
	   strcat(l_name, "]");
#endif
	strcat(l_name, LIBFILENAME);

        if ((fp = open(l_name,O_RDONLY,0700)) < 0) {
#ifdef vms
            string_desc.dsc$w_length = strlen(lib_name);
            string_desc.dsc$b_dtype = DSC$K_DTYPE_T;
            string_desc.dsc$b_class = DSC$K_CLASS_S;
            string_desc.dsc$a_pointer = lib_name;
            LIB$SIGNAL(MSG_NOLIBRARY,1,&string_desc);
            exit();
#else
            fprintf(stderr,"%s cannot be used as a library\n", lib_name);
            exit(RC_ABORT);
#endif
        }
	efree(l_name);
        close(fp);
    }

    /* format library option as expected by adasem & adagen */
    lib_opt_str = ((newlib_opt) ? "-nl" : "-l");

    PRS = getsym("PRS", PRS_NAME);
    other_opts[opts_cnt = 0] = PRS;
    other_opts[++opts_cnt] = lib_opt_str;
    other_opts[++opts_cnt] = lib_name;
    other_opts[++opts_cnt] = source_name;
    other_opts[++opts_cnt] = (char *) 0;
    if (exec_trace)
        arg_dump();
    status = run_prog(PRS, other_opts);

    if (exec_trace)
        fprintf(stderr, " ? %d\n", status);
    ok = check_status(status, "PRS", arg_name);
    if (ok) {
	SEM = getsym("SEM",SEM_NAME);
	other_opts[opts_cnt = 0] = SEM;
	/* check for parsing errors (adaprs exits with RC_ERRORS) */
	if (status_get(status) == RC_ERRORS) {
	    s_temp = emalloc(strlen(sem_options) + 2);
	    strcpy(s_temp, sem_options);
	    strcat(s_temp, "e");
	    sem_options = s_temp;
	}
	if (strlen(sem_options) != 0) {
	    other_opts[++opts_cnt] = "-s";
	    other_opts[++opts_cnt] = sem_options;
	}
	other_opts[++opts_cnt] = lib_opt_str;
	other_opts[++opts_cnt] = lib_name;
	other_opts[++opts_cnt] = base_name;
	other_opts[++opts_cnt] = (char *) 0;
	if (exec_trace)
	    arg_dump();
	status = run_prog(SEM, other_opts);
	if (exec_trace)
	    fprintf(stderr, " ? %d\n", status);
	ok = check_status(status, "SEM", arg_name);
        /* check for semantic errors (adasem will exit with RC_ERRORS) */
	if (status_get(status)== RC_ERRORS)
	    ok = FALSE;
    }
    /* once SEM run, can delete AST file */
    tmp_name = emalloc(strlen(lib_name) + strlen(base_name) + 7);
#ifdef vms
    if (strchr(lib_name,'[')) {
       strcpy(tmp_name, lib_name);
    }
    else {
       strcpy(tmp_name, DIRECTORY_START);
       strcat(tmp_name, lib_name);
    }
#else
    strcpy(tmp_name, lib_name);
#endif

#ifdef AMIGA
    strcat(tmp_name,"/");
#endif
#ifdef BSD
    strcat(tmp_name,"/");
#endif
#ifdef SYSTEM_V
    strcat(tmp_name,"/");
#endif
#ifdef IBM_PC
    strcat(tmp_name,"/");
#endif
#ifdef vms
    if (!strchr(lib_name,'[')) 
       strcat(tmp_name,"]");
#endif
    strcat(tmp_name, base_name);
    strcat(tmp_name, ".ast");
    delete_file(tmp_name);
    efree(tmp_name);
    if (ok) {
	GEN = getsym("GEN", GEN_NAME);
	other_opts[opts_cnt = 0] = GEN;
	if (strlen(gen_options) != 0) {
	    other_opts[++opts_cnt] = "-g";
	    other_opts[++opts_cnt] = gen_options;
	}
	other_opts[++opts_cnt] = lib_opt_str;
	other_opts[++opts_cnt] = lib_name;
	other_opts[++opts_cnt] = base_name;
	other_opts[++opts_cnt] = (char *) 0;
	if (exec_trace)
	    arg_dump();
	status =  run_prog(GEN, other_opts);
	if (exec_trace)
	    fprintf(stderr, " ? %d\n", status);
	ok = check_status(status, "GEN", arg_name);
    }
    if (ok && bind_opt) { /* run binder if desired */
	BND = getsym("BND", BND_NAME);
	other_opts[opts_cnt = 0] = BND;
	other_opts[++opts_cnt] = "-c"; /* indicate errors in message form */
	other_opts[++opts_cnt] = base_name; /* pass filename for msg listing */

	while(interface_cnt) {
	    other_opts[++opts_cnt] = "-i";
	    other_opts[++opts_cnt] = interface_opts[--interface_cnt];
	}
	if (main_opt) {
	    other_opts[++opts_cnt] = "-m";
	    other_opts[++opts_cnt] = main_unit_name;
	}
	other_opts[++opts_cnt] = lib_name; /* library is current directory */
	other_opts[++opts_cnt] = (char *) 0;
	if (exec_trace)
	    arg_dump();
	status =  run_prog(BND, other_opts);
	if (exec_trace)
	    fprintf(stderr, " ? %d\n", status);
	ok = check_status(status, "BND", arg_name);
    }
#ifdef vms
#ifdef SKIP
/* this rename not needed if file generated in proper place
 * ds 1-17-86
 */
struct      dsc$descriptor_s old_filespec;
struct      dsc$descriptor_s new_filespec;
	list_name = strjoin(base_name,".lis;");
/*	tolist_name = strjoin(dir_name, list_name);*/
        old_filespec.dsc$w_length = strlen(list_name);
        old_filespec.dsc$b_dtype = DSC$K_DTYPE_T;
        old_filespec.dsc$b_class = DSC$K_CLASS_S;
        old_filespec.dsc$a_pointer = list_name;
        new_filespec.dsc$w_length = strlen(tolist_name);
        new_filespec.dsc$b_dtype = DSC$K_DTYPE_T;
        new_filespec.dsc$b_class = DSC$K_CLASS_S;
        new_filespec.dsc$a_pointer = tolist_name;
	status = LIB$RENAME_FILE(&old_filespec, &new_filespec);
#endif
#endif
#ifdef AMIGA
    list_name = emalloc(strlen(base_name) + 4 + 1);
    strcpy(list_name, base_name);
    strcat(list_name, ".lis");
#endif
#ifdef IBM_PC
	list_name = emalloc(strlen(base_name) + 4 + 1);
	strcpy(list_name, base_name);
	strcat(list_name, ".lis");
#endif
#ifdef SYSTEM_V
	list_name = emalloc(strlen(base_name) + 4 + 1);
	strcpy(list_name, base_name);
	strcat(list_name, ".lis");
#endif
#ifdef BSD
	list_name = emalloc(strlen(base_name) + 4 + 1);
	strcpy(list_name, base_name);
	strcat(list_name, ".lis");
#endif
#ifdef vms
	list_name = emalloc(strlen(base_name) + 4 + 1);
	strcpy(list_name, base_name);
	strcat(list_name, ".lis");
#endif
    list_arg = (list_opt>0) ? "1" : "0";
    msg_name = emalloc(strlen(lib_name) + strlen(base_name) + 7);
#ifdef vms
    if (strchr(lib_name,'[')) {
       strcpy(msg_name, lib_name);
    }
    else {
       strcpy(msg_name, DIRECTORY_START);
       strcat(msg_name, lib_name);
    }
#else
    strcpy(msg_name, lib_name);
#endif
#ifdef AMIGA
    strcat(msg_name,"/");
#endif
#ifdef BSD
    strcat(msg_name,"/");
#endif
#ifdef SYSTEM_V
    strcat(msg_name,"/");
#endif
#ifdef IBM_PC
    strcat(msg_name,"/");
#endif
#ifdef vms
    if (!strchr(lib_name,'['))
       strcat(msg_name,"]");
#endif
    strcat(msg_name, base_name);
    strcat(msg_name, ".msg");
    status = mrg(source_name,msg_name, list_name, list_arg);
    efree(list_name);
    if (!save_msg_opt) {
        delete_file(msg_name);
	efree(msg_name);
    }

#ifdef vms
    if (maxstatus == RC_ABORT || maxstatus == RC_INTERNAL_ERROR) {
        string_desc.dsc$w_length = strlen(source_name);
        string_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        string_desc.dsc$b_class = DSC$K_CLASS_S;
        string_desc.dsc$a_pointer = source_name;
        LIB$SIGNAL(MSG_ABORT,1,&string_desc);
    }
    exit();
#else
    exit(maxstatus);
#endif
}

static char *getsym(char *env_name, char *def_value)		/*;getsym*/
{
  /* Retrieve environment variable designating the executable module for
   * a given phase of the compiler.
   * If the variable is not defined, a default is supplied for BSD systems,
   * whereas on vms execution is aborted!
   */
    char   *s;
#ifdef vms
    struct dsc$descriptor_s phase_desc;
#endif

    s = getenv(env_name);
    if (s==(char *)0) {
		char *t = get_libdir();
#ifdef vms
        phase_desc.dsc$w_length = strlen(env_name);
        phase_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        phase_desc.dsc$b_class = DSC$K_CLASS_S;
        phase_desc.dsc$a_pointer = env_name;
        LIB$SIGNAL(MSG_NOENVVAR,1,&phase_desc);
        exit();
#else
		s = emalloc(strlen(t) + strlen(def_value) + 2);
		sprintf(s,"%s/%s", t, def_value);
#endif
    }
    return s;
}

static int check_status(int pstatus, char *phase, char *filename)
															/*;check_status*/
{
#ifdef vms
	struct dsc$descriptor_s err_desc ;
#endif

#ifdef BSD
    if (system_status_get(pstatus) == SIGXCPU) {
		fprintf(stderr, "Ada/Ed cpu time limit exceeded for %s\n",phase);
		return (FALSE);
    }
#endif

#ifdef vms
    /* check for internal compiler error and a signal (system transmitted)
     * that is not IGNORE (1) or BAD_SIGNAL (-1)
     * Check first for user return codes since vms will give precedence
	 * toguarantee what will appear user return codes if there was no crash.
     */
    if (status_get(pstatus)  == RC_SUCCESS) {
		return (TRUE);
    }
    if (status_get(pstatus) == RC_ERRORS){
		maxstatus = RC_ERRORS;
		return (TRUE);
    }
    if (status_get(pstatus)  == RC_ABORT) {
		maxstatus = RC_ABORT;
		return (FALSE);
    }
    if ( (status_get(pstatus)  == RC_INTERNAL_ERROR)
	  || (system_status_get(pstatus) > 1 && system_status_get(pstatus) < 255)) {
		maxstatus = RC_INTERNAL_ERROR;
        err_desc.dsc$w_length = strlen(phase);
        err_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        err_desc.dsc$b_class = DSC$K_CLASS_S;
        err_desc.dsc$a_pointer = phase;
        LIB$SIGNAL(MSG_BUGCHECK,1,&err_desc);
		return (FALSE);
     }
#else
    /* check for internal compiler error and a signal (system transmitted)
     * that is not IGNORE (1) or BAD_SIGNAL (-1)
     * Check first for crash, since have no guarantee what will appear
     * in 'user' section of return code (status_get field)
     */
    if ( (status_get(pstatus)  == RC_INTERNAL_ERROR)
	  || (system_status_get(pstatus) > 1 && system_status_get(pstatus) < 255)) {
		maxstatus = RC_INTERNAL_ERROR;
		fprintf(stderr,"Ada/Ed Internal error(%s) for %s\n", phase, filename);
		return (FALSE);
    }
    if (status_get(pstatus)  == RC_SUCCESS) {
		return (TRUE);
    }
    if (status_get(pstatus) == RC_ERRORS){
		maxstatus = RC_ERRORS;
		return (TRUE);
    }
    if (status_get(pstatus)  == RC_ABORT) {
		maxstatus = RC_ABORT;
		return (FALSE);
    }
#endif
}

static void arg_dump()											/*;arg_dump*/
{
/*list generated command*/
    int     i;
    fprintf(stderr, "%s ", other_opts[0]);
    for (i = 1; i < opts_cnt; i++) {
		fprintf(stderr, " %s", other_opts[i]);
    }
    fprintf(stderr,"\n");
}

static int run_prog(char *prog, char **args)					/*;run_prog*/
{
	int status;
#ifdef vms
   struct dsc$descriptor_s string_desc;
#endif

#ifdef IBM_PC
	status = spawnv(P_WAIT, prog, args);
#else
	if (fork() == 0)
#ifdef vms
	    if (execv(prog,other_opts)) {
        	string_desc.dsc$w_length = strlen(prog);
        	string_desc.dsc$b_dtype = DSC$K_DTYPE_T;
        	string_desc.dsc$b_class = DSC$K_CLASS_S;
        	string_desc.dsc$a_pointer = prog;
        	LIB$SIGNAL(MSG_NOEXECUTE,1,&string_desc);
            exit();
	    }
#else
	    if (execvp(prog , other_opts)) {
			fprintf(stderr,"cannot execute %s\n", prog);
			exit(RC_ABORT);
	    }
#endif
	wait( WAITPARM &status);
#endif
	return status;
}

static void delete_file(char *file_name)					/* ;delete_file */
{
	int status;

#ifdef vms
    extern char *strjoin();

    file_name = strjoin(file_name,";");
#endif
    status = unlink(file_name);
    if (exec_trace)
		fprintf(stderr,"unlink %s ? %d\n",file_name, status);
}

#ifdef SYSTEM_V
#include <sys/stat.h>
/* no mkdir available, mknod doesn't work, so use system */
char syscommand[100];		/* argument for system() call */
static  int mkdir(char *lib_name, int mode)							/*;mkdir*/
{
    int status;
    struct stat statrec;

	if (stat(lib_name,&statrec)) {
	   /* stat returns nonzero value if cannot find file. This check
	    * is to avoid calling mkdir on an existing directory (since
	    * SYSTEM_V complains)
	    */
	   sprintf(syscommand,"mkdir %s",lib_name);
	   system(syscommand);
	   return (0);
	}
	else return (-1);
}
#endif

#ifdef vms
static void fold_upper(char *s)								/*;fold_upper*/
{
    char c;

    while (*s) {
     	c = *s;
	if (islower(c)) *s = toupper(c);
	s++;
    }
}
#endif
