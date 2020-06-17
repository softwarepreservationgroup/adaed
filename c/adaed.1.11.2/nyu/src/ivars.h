#define INIT(v) 




extern   int *stack_segments [MAX_TASKS]; /* one stack segment for each task */
extern   int task_stackptr [MAX_TASKS];   /* stack pointers for task stacks */
extern   unsigned int data_segments_dim;  /* dimension of data_segments */
extern   int **data_segments;             /* one data segment for each cunit */
extern   int *data_seglen;                /* length of data segments */
extern   unsigned int code_segments_dim;  /* dimension of code_segments */
extern   char **code_segments;            /* segments containing code */
extern   int *code_seglen;                /* length of code segments */
extern   int *cur_stack;                  /* current stack segment */
extern   int cur_stackptr;                /* stack ptr for cur_stack */
extern   char *cur_code;                  /* current code segment */
extern   int num_cunits;                  /* number of compilation units */


extern   int cs;       /* index of current code_segment in code_segments */
extern   int ip;       /* instruction pointer in cur_code */
extern   int sp;       /* stack pointer */
extern   int sfp;      /* stack frame pointer */
extern   int bfp;      /* block frame pointer */
extern   int exr;      /* exception register */
extern   int lin;      /* line number for tracing */


extern   int tp;                /* index of current task in stack_segments */
extern   int *heap_next;        /* top of heap for allocation */
extern   int heap_base;         /* segment number of heap */
extern   int *heap_addr;        /* address of beginning of heap */
extern   long next_clock;        /* next time to call clock_interrupts */
extern   long time_offset;       /* cumulative skipped time for simulated time */
extern   int next_clock_flag;   /* request to check the clock (task waiting) */
extern   int simul_time_flag;   /* run with simulated time (versus real time) */
extern   int time_slicing_flag; /* time slicing (vs schedule at system call) */
extern   long time_slice;        /* length of time slice (500 ms default) */
extern   int tasking_trace;     /* TRUE to trace tasking */
extern   int rendezvous_trace;  /* TRUE to trace rendezvous */
extern   int debug_trace;       /* TRUE for general debugging trace */
extern   int exception_trace;   /* TRUE to trace exceptions */
extern   int call_trace;        /* TRUE to trace calls */
extern   int line_trace;        /* TRUE to trace line numbers */
extern   int instruction_trace; /* TRUE to trace instructions */
extern   int context_sw_trace;  /* TRUE to trace context switches */
extern   int rr_flag;           /* TRUE to allow round-robin scheduling */
extern   int rr_counter;        /* counter of statements - round_robin scheme*/
extern   int rr_nb_max_stmts;   /* max value of rr_counter. */


extern   unsigned int data_slots_dim;      /* dimension of data_slots */
extern   char **data_slots;
extern   unsigned int code_slots_dim;      /* dimension of code_slots */
extern   char **code_slots;
extern   unsigned int exception_slots_dim; /* dimension of exception_slots */
extern   char **exception_slots;


extern   int max_mem;                      /* size of a heap segment */
extern   int w;                            /* used by GET_WORD */
extern   struct tt_fx_range *temp_template;/* fixed point work template */

extern   int bse,bas1,bas2,bas3,bas4;      /* address base values */
extern   int off,off1,off2,off3,off4;      /* address offset values */
extern   int *ptr,*ptr1,*ptr2,*ptr3,*ptr4; /* memory addresses */
extern   int value,val1,val2;              /* operands for int operations */
extern   int length1,length2;              /* for array operations */
extern   long lvalue,lval1,lval2;          /* operands for long operations */
extern   long fval1,fval2,fvalue;          /* operands for fixed operations */
extern   float rvalue,rval1,rval2;         /* operands for float operations */
extern   int val_high,val_low;             /* low/high of INT range */
extern   int result,delta;                 /* used in fixed point */
extern   int size;                         /* size of object */
extern   int attribute;                    /* attribute code */
extern   int slot;                         /* for create structure */
extern   int component_size;               /* size of array component */
extern   long now_time;                     /* current time */
extern   int to_type;                      /* type from template */
extern   int discr_list[MAX_DISCR];        /* accumulate list of discriminants */
extern   int nb,i;                         /* loop counters etc */
extern   int overflow;                     /* overflow flag from arith routines */
extern   int nb_discr;                     /* number of discriminants */
extern   int nb_dim;                       /* dimension of array */
extern   int ratio,power;                  /* used in attribute processing */
extern   int d,n;                          /* temporaries */
extern   int old_cs;                       /* old value of cs */
extern   int jump;                         /* jump address */
extern   int lim;                          /* limit in for loop */
extern   struct bf *bfptr;                 /* pointer to block frame */
extern   int *tfptr1,*tfptr2;              /* pointers to task frames */
extern   int cur_col;                      /* temporary for predef */

extern   int break_point;       /* next break point */  

extern   int fix_val1[20];      /* fixed point intermediate values */
extern   int fix_val2[20];     
extern   int fix_resu[20];     
extern   int fix_temp[20];     
extern   int mul_fact[20];     
extern   int div_fact[20];     

extern   int arith_overflow;
extern   int sgn;
extern   int last_task;
extern   int *free_list INIT((int *)0);
extern   int raise_cs;
extern   int raise_lin;
extern   char *raise_reason;




#define MAXFILES 20
extern   struct afcb *(afcbs[MAXFILES]);


extern   int operation;         /* operation for predef. set in the main loop */
extern   int data_exception;    /* exception to raise on data error */
extern   int filenum;           /* number of current file */
extern   int standard_in_file;  /* index of standard input file */
extern   int standard_out_file; /* index of standard output file */
extern   int current_in_file;   /* index of current default input file */
extern   int current_out_file;  /* index of current default output file */
extern   int current_in_file_saved;   
extern   int current_out_file_saved;  
extern   int file_offset;       /* Offset due to file argument in the stack */
extern   struct tfile *tfiles;  /* linked list of temporary files */


extern   char work_string[4096];


extern   int main_task_size;
extern   int new_task_size;
