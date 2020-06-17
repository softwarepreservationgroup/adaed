with IO_EXCEPTIONS;
package TEXT_IO is 
     
  type FILE_TYPE  is limited private;
 
  type FILE_MODE  is (IN_FILE, OUT_FILE);

  type COUNT is range 0 .. 32767;

  subtype POSITIVE_COUNT IS COUNT range 1 .. COUNT'LAST;

  UNBOUNDED : constant COUNT := 0; -- line and page length

  subtype FIELD is INTEGER range 0 .. 100 ;
  subtype NUMBER_BASE is INTEGER range 2 .. 16;

  type TYPE_SET is (LOWER_CASE, UPPER_CASE);

  -- File Management

     
  procedure CREATE (FILE : in out FILE_TYPE;
                    MODE : in FILE_MODE := OUT_FILE;
                    NAME : in STRING    := "";
                    FORM : in STRING    := "");
  pragma IO_interface(CREATE, TIO_CREATE);
    
  procedure OPEN   (FILE : in out FILE_TYPE;
                    MODE : in FILE_MODE;
                    NAME : in STRING;
                    FORM : in STRING := "");
  pragma IO_interface(OPEN, TIO_OPEN);
 
  procedure CLOSE  (FILE : in out FILE_TYPE);
  pragma IO_interface(CLOSE, TIO_CLOSE);
    
  procedure DELETE (FILE : in out FILE_TYPE);
  pragma IO_interface(DELETE, TIO_DELETE);

  procedure RESET  (FILE : in out FILE_TYPE; MODE : in FILE_MODE);
  pragma IO_interface(RESET, TIO_RESET_MODE);

  procedure RESET  (FILE : in out FILE_TYPE);
  pragma IO_interface(RESET, TIO_RESET);

  function MODE (FILE : in FILE_TYPE)     return FILE_MODE;
  pragma IO_interface(MODE, TIO_MODE);

  function NAME (FILE : in FILE_TYPE)     return STRING;
  pragma IO_interface(NAME, TIO_NAME);

  function FORM (FILE : in FILE_TYPE)     return STRING;      
  pragma IO_interface(FORM, TIO_FORM);

  function IS_OPEN (FILE : in FILE_TYPE)  return BOOLEAN;
  pragma IO_interface(IS_OPEN, TIO_IS_OPEN);

  -- Control of default input and output files
     
  procedure SET_INPUT  (FILE : in FILE_TYPE);
  pragma IO_interface(SET_INPUT, SET_INPUT); 

  procedure SET_OUTPUT (FILE : in FILE_TYPE);
  pragma IO_interface(SET_OUTPUT, SET_OUTPUT);

  function STANDARD_INPUT  return FILE_TYPE;
  pragma IO_interface(STANDARD_INPUT, STANDARD_INPUT);

  function STANDARD_OUTPUT return FILE_TYPE;
  pragma IO_interface(STANDARD_OUTPUT, STANDARD_OUTPUT);

  function CURRENT_INPUT  return FILE_TYPE;
  pragma IO_interface(CURRENT_INPUT, CURRENT_INPUT);
  function CURRENT_OUTPUT return FILE_TYPE;
  pragma IO_interface(CURRENT_OUTPUT, CURRENT_OUTPUT);

  -- Specification of line and page lengths

  procedure SET_LINE_LENGTH (FILE : in FILE_TYPE;  TO : in COUNT);
  pragma IO_interface(SET_LINE_LENGTH, SET_LINE_LENGTH_FILE);

  procedure SET_LINE_LENGTH (TO : in COUNT);	-- default output file
  pragma IO_interface(SET_LINE_LENGTH, SET_LINE_LENGTH);

  procedure SET_PAGE_LENGTH (FILE : in FILE_TYPE;  TO : in COUNT);
  pragma IO_interface(SET_PAGE_LENGTH, SET_PAGE_LENGTH_FILE);

  procedure SET_PAGE_LENGTH (TO : in COUNT);    -- default output file
  pragma IO_interface(SET_PAGE_LENGTH, SET_PAGE_LENGTH);

  function LINE_LENGTH (FILE : in FILE_TYPE)  return COUNT;
  pragma IO_interface(LINE_LENGTH, LINE_LENGTH_FILE);

  function LINE_LENGTH return COUNT;  -- default output file
  pragma IO_interface(LINE_LENGTH, LINE_LENGTH);
     
  
  function PAGE_LENGTH (FILE : in FILE_TYPE)  return COUNT;
  pragma IO_interface(PAGE_LENGTH, PAGE_LENGTH_FILE);

  function PAGE_LENGTH return COUNT; -- default output file
  pragma IO_interface(PAGE_LENGTH, PAGE_LENGTH);

  -- Column, Line and Page Control

  procedure NEW_LINE (FILE : in FILE_TYPE;  SPACING : in POSITIVE_COUNT := 1);
  pragma IO_interface(NEW_LINE, NEW_LINE_FILE);

  procedure NEW_LINE (SPACING : in POSITIVE_COUNT := 1); 
  pragma IO_interface(NEW_LINE, NEW_LINE);

  procedure SKIP_LINE (FILE : in FILE_TYPE;  SPACING : in POSITIVE_COUNT := 1);
  pragma IO_interface(SKIP_LINE, SKIP_LINE_FILE);

  procedure SKIP_LINE (SPACING : in POSITIVE_COUNT := 1);
  pragma IO_interface(SKIP_LINE, SKIP_LINE);

  function END_OF_LINE (FILE : in FILE_TYPE) return BOOLEAN;
  pragma IO_interface(END_OF_LINE, END_OF_LINE_FILE);

  function END_OF_LINE return BOOLEAN; -- default input file
  pragma IO_interface(END_OF_LINE, END_OF_LINE);

  procedure NEW_PAGE (FILE : in FILE_TYPE);
  pragma IO_interface(NEW_PAGE, NEW_PAGE_FILE);
  procedure NEW_PAGE; -- default output file
  pragma IO_interface(NEW_PAGE, NEW_PAGE);

  procedure SKIP_PAGE (FILE : in FILE_TYPE);
  pragma IO_interface(SKIP_PAGE, SKIP_PAGE_FILE);
  procedure SKIP_PAGE; -- default input file
  pragma IO_interface(SKIP_PAGE, SKIP_PAGE);

  function END_OF_PAGE (FILE : in FILE_TYPE) return BOOLEAN;
  pragma IO_interface(END_OF_PAGE, END_OF_PAGE_FILE);
  function END_OF_PAGE return BOOLEAN;      
  pragma IO_interface(END_OF_PAGE, END_OF_PAGE);

  function END_OF_FILE (FILE : in FILE_TYPE) return BOOLEAN;
  pragma IO_interface(END_OF_FILE, TIO_END_OF_FILE_FILE);
  function END_OF_FILE return BOOLEAN;      
  pragma IO_interface(END_OF_FILE, TIO_END_OF_FILE);

  procedure SET_COL(FILE : in FILE_TYPE; TO : in POSITIVE_COUNT);
  pragma IO_interface(SET_COL, SET_COL_FILE);
  procedure SET_COL(TO : in POSITIVE_COUNT); -- default output file
  pragma IO_interface(SET_COL, SET_COL);

  procedure SET_LINE(FILE : in FILE_TYPE; TO : in POSITIVE_COUNT);
  pragma IO_interface(SET_LINE, SET_LINE_FILE);
  procedure SET_LINE(TO : in POSITIVE_COUNT); -- default output file
  pragma IO_interface(SET_LINE, SET_LINE);
  
  function COL(FILE : in FILE_TYPE) return POSITIVE_COUNT;
  pragma IO_interface(COL, COL_FILE);
  function COL return POSITIVE_COUNT; -- default output file
  pragma IO_interface(COL, COL);

  function LINE(FILE : in FILE_TYPE) return POSITIVE_COUNT;
  pragma IO_interface(LINE, LINE_FILE);
  function LINE return POSITIVE_COUNT; -- default output file
  pragma IO_interface(LINE, LINE);

  function PAGE(FILE : in FILE_TYPE) return POSITIVE_COUNT;
  pragma IO_interface(PAGE, PAGE_FILE);
  function PAGE return POSITIVE_COUNT; -- default output file
  pragma IO_interface(PAGE, PAGE);


  -- Character Input-Output
 
  procedure GET (FILE : in  FILE_TYPE;  ITEM : out CHARACTER);
  pragma IO_interface(GET, GET_CHAR_FILE_ITEM);
  procedure GET (ITEM : out CHARACTER);
  pragma IO_interface(GET, GET_CHAR_ITEM);
  procedure PUT (FILE : in  FILE_TYPE;  ITEM : in CHARACTER);
  pragma IO_interface(PUT, PUT_CHAR_FILE_ITEM);
  procedure PUT (ITEM : in  CHARACTER);
  pragma IO_interface(PUT, PUT_CHAR_ITEM);

    
  -- String Input-Output
    
  procedure GET (FILE : in  FILE_TYPE;  ITEM : out STRING);
  pragma IO_interface(GET, GET_STRING_FILE_ITEM);
  procedure GET (ITEM : out STRING);    
  pragma IO_interface(GET, GET_STRING_ITEM);
  procedure PUT (FILE : in  FILE_TYPE;  ITEM : in STRING);
  pragma IO_interface(PUT, PUT_STRING_FILE_ITEM);
  procedure PUT (ITEM : in  STRING);
  pragma IO_interface(PUT, PUT_STRING_ITEM);

  procedure GET_LINE (FILE : in FILE_TYPE; ITEM : out STRING;
                                           LAST : out NATURAL);
  pragma IO_interface(GET_LINE, GET_LINE_FILE);
  procedure GET_LINE (ITEM : out  STRING; LAST : out NATURAL);
  pragma IO_interface(GET_LINE, GET_LINE);

  procedure PUT_LINE (FILE : in FILE_TYPE; ITEM : in STRING);
  pragma IO_interface(PUT_LINE, PUT_LINE_FILE);
  procedure PUT_LINE (ITEM : in STRING);
  pragma IO_interface(PUT_LINE, PUT_LINE);
    
  -- Generic package for Input-Output of Integer Types

  generic
    type NUM is range <>;
  package INTEGER_IO is

    DEFAULT_WIDTH : FIELD := NUM'WIDTH;
    DEFAULT_BASE  : NUMBER_BASE := 10;

    procedure GET (FILE  : in FILE_TYPE;  ITEM : out NUM; 
                                          WIDTH : in FIELD := 0);
    pragma IO_interface(GET, GET_INTEGER_FILE_ITEM, NUM);

    procedure GET (ITEM  : out NUM; WIDTH : in FIELD := 0);
    pragma IO_interface(GET, GET_INTEGER_ITEM, NUM);

    procedure PUT (FILE  : in FILE_TYPE;
    		   ITEM  : in NUM;
    		   WIDTH : in FIELD := DEFAULT_WIDTH;
    		   BASE  : in NUMBER_BASE := DEFAULT_BASE);
    pragma IO_interface(PUT, PUT_INTEGER_FILE_ITEM, NUM);
    procedure PUT (ITEM  : in NUM;
    		   WIDTH : in FIELD := DEFAULT_WIDTH;
    		   BASE  : in NUMBER_BASE := DEFAULT_BASE);
    pragma IO_interface(PUT, PUT_INTEGER_ITEM, NUM);
    
    procedure GET (FROM : in STRING; ITEM: out NUM; LAST: out POSITIVE);
    pragma IO_interface(GET, GET_INTEGER_STRING, NUM);
    procedure PUT (TO   : out STRING;
                   ITEM : in  NUM;
                   BASE : in  NUMBER_BASE := DEFAULT_BASE);
    pragma IO_interface(PUT, PUT_INTEGER_STRING, NUM);

  end INTEGER_IO; 


  -- Generic packages for Input-Output of Real Types

  generic
    type NUM is digits <>;
  package FLOAT_IO is

    DEFAULT_FORE : FIELD := 2;
    DEFAULT_AFT  : FIELD := NUM'DIGITS-1;
    DEFAULT_EXP  : FIELD := 3;

    procedure GET (FILE : in FILE_TYPE; ITEM : out NUM;
                                        WIDTH : in FIELD := 0);
    pragma IO_interface(GET, GET_FLOAT_FILE_ITEM, NUM);
    procedure GET (ITEM : out NUM; WIDTH : in FIELD := 0);
    pragma IO_interface(GET, GET_FLOAT_ITEM, NUM);

    procedure PUT (FILE		: in FILE_TYPE;
		   ITEM		: in NUM;
		   FORE        	: in FIELD := DEFAULT_FORE;
		   AFT  	: in FIELD := DEFAULT_AFT;
		   EXP   	: in FIELD := DEFAULT_EXP);
    pragma IO_interface(PUT, PUT_FLOAT_FILE_ITEM, NUM);

    procedure PUT (ITEM		: in NUM;
		   FORE   	: in FIELD := DEFAULT_FORE;
		   AFT  	: in FIELD := DEFAULT_AFT;
		   EXP  	: in FIELD := DEFAULT_EXP);
    pragma IO_interface(PUT, PUT_FLOAT_ITEM ,NUM);
    
    procedure GET (FROM : in STRING; ITEM: out NUM; LAST: out POSITIVE);
    pragma IO_interface(GET, GET_FLOAT_STRING, NUM);
    procedure PUT (TO   : out STRING;
                   ITEM : in NUM;
                   AFT  : in FIELD := DEFAULT_AFT;
                   EXP  : in FIELD := DEFAULT_EXP);
    pragma IO_interface(PUT, PUT_FLOAT_STRING, NUM);

  end FLOAT_IO;

  generic
    type NUM is delta <>;
  package FIXED_IO is

    DEFAULT_FORE : FIELD := NUM'FORE;
    DEFAULT_AFT  : FIELD := NUM'AFT;
    DEFAULT_EXP  : FIELD := 0;

    procedure GET (FILE : in FILE_TYPE; ITEM : out NUM;
                                        WIDTH : in FIELD := 0);
    pragma IO_interface(GET, GET_FIXED_FILE_ITEM, NUM);
    procedure GET (ITEM : out NUM; WIDTH : in FIELD := 0);
    pragma IO_interface(GET, GET_FIXED_ITEM, NUM);

    procedure PUT (FILE		: in FILE_TYPE;
		   ITEM		: in NUM;
		   FORE 	: in FIELD := DEFAULT_FORE;
		   AFT  	: in FIELD := DEFAULT_AFT;
                   EXP          : in FIELD := DEFAULT_EXP);
    pragma IO_interface(PUT, PUT_FIXED_FILE_ITEM, NUM);

    procedure PUT (ITEM		: in NUM;
		   FORE 	: in FIELD := DEFAULT_FORE;
		   AFT  	: in FIELD := DEFAULT_AFT;
                   EXP          : in FIELD := DEFAULT_EXP);
    pragma IO_interface(PUT, PUT_FIXED_ITEM, NUM);

    procedure GET (FROM : in STRING; ITEM: out NUM; LAST: out POSITIVE);
    pragma IO_interface(GET, GET_FIXED_STRING, NUM);
    procedure PUT (TO   : out STRING;
                   ITEM : in  NUM;
                   AFT  : in  FIELD := DEFAULT_AFT;
                   EXP  : in  FIELD := DEFAULT_EXP);
    pragma IO_interface(PUT, PUT_FIXED_STRING, NUM);

  end FIXED_IO;

      
  -- Generic package for Input-Output of Enumeration Types

  generic
    type ENUM is (<>);
  package ENUMERATION_IO is

    DEFAULT_WIDTH   : FIELD := 0;
    DEFAULT_SETTING : TYPE_SET := UPPER_CASE;

    procedure GET (FILE : in FILE_TYPE; ITEM : out ENUM);
    pragma IO_interface(GET, GET_ENUM_FILE_ITEM, ENUM);
    procedure GET (ITEM : out ENUM);
    pragma IO_interface(GET, GET_ENUM_ITEM, ENUM);

    procedure PUT (FILE       :	in FILE_TYPE;
		   ITEM       :	in ENUM;
		   WIDTH      :	in FIELD    := DEFAULT_WIDTH;
		   SET        : in TYPE_SET := DEFAULT_SETTING);
    pragma IO_interface(PUT, PUT_ENUM_FILE_ITEM, ENUM);

    procedure PUT (ITEM       :	in ENUM;
		   WIDTH      :	in FIELD    := DEFAULT_WIDTH;
		   SET        :	in TYPE_SET := DEFAULT_SETTING);
    pragma IO_interface(PUT, PUT_ENUM_ITEM, ENUM);
 
    procedure GET(FROM : in STRING; ITEM: out ENUM; LAST: out POSITIVE);
    pragma IO_interface(GET, GET_ENUM_STRING, ENUM);
    procedure PUT (TO   : out STRING;
                   ITEM : in  ENUM;
                   SET  : in  TYPE_SET := DEFAULT_SETTING);
    pragma IO_interface(PUT, PUT_ENUM_STRING, ENUM);

  end ENUMERATION_IO;


  -- Exceptions:
  --  
  -- These are the exceptions whose names are visible to the   
  -- calling environment.   
     
  STATUS_ERROR	: exception renames IO_EXCEPTIONS.STATUS_ERROR;
  MODE_ERROR    : exception renames IO_EXCEPTIONS.MODE_ERROR;
  NAME_ERROR	: exception renames IO_EXCEPTIONS.NAME_ERROR;
  USE_ERROR	: exception renames IO_EXCEPTIONS.USE_ERROR;
  DEVICE_ERROR	: exception renames IO_EXCEPTIONS.DEVICE_ERROR;
  END_ERROR	: exception renames IO_EXCEPTIONS.END_ERROR;
  DATA_ERROR	: exception renames IO_EXCEPTIONS.DATA_ERROR;
  LAYOUT_ERROR	: exception renames IO_EXCEPTIONS.LAYOUT_ERROR;


    
private

    type FILE_TYPE is array (1 .. 1) of INTEGER;

end TEXT_IO; 

package body TEXT_IO is
  package body INTEGER_IO     is end ;
  package body FLOAT_IO       is end ;
  package body FIXED_IO       is end ;
  package body ENUMERATION_IO is end ;
end TEXT_IO ;

