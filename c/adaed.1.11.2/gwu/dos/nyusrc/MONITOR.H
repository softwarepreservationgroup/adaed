#define MAX_TSKS 40
#define MAX_BLKS 200
#define MAX_LINES 2000
#define MAX_DELAY 10

typedef void (*PROC_NM)();

typedef struct {
	int x1, y1, x2, y2;
	} COORD;

typedef struct {
	int mon_win ;
	int WON;
	char *title;
	} MON_REC, *MON_REC_PTR;

typedef struct {
	FILE	*fd;
	char 	*FN;
	char    *lines[MAX_LINES];
	int	lines_in_file;
	} FILE_REC, *FILE_REC_PTR;

typedef struct {
          int ACTIVE ; 	/* TRUE <==> MONITOR TASK ON  */
          int DontDo ; 	/* TRUE <==> Task Type        */
	  char *TN   ;  /* TASK NAME  */
          int FL     ;  /* TASK FIRST LINE IN FILE */
          int LL     ; 	/* TASK LAST LINE IN FILE */
          int CL     ; 	/* TASK CURRENT LINE  */
	  int FLOS   ;  /* FIRST LINE ON SCREEN */
	  long Tot_Time; /* Total execution time (lines) for this task */
	  long Cur_Time; /* Execution time for this task since last report */
          char ST    ; 	/* TASK STATUS  */
          int GRDIR  ;  /* IF TRUE GOES RIGHT */
	  MON_REC_PTR MON;  /* Monitor record on the screen */
	  FILE_REC_PTR FD;  /* File this task is located in */
	} TASK_REC, *TASK_REC_PTR;

MON_REC_PTR CWK_CREATE_MON_WIN();
FILE_REC_PTR CWK_LOAD_FILE();
