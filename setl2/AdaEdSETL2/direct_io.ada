with IO_EXCEPTIONS;
generic
    type ELEMENT_TYPE is private;

package DIRECT_IO is

    type  FILE_TYPE  is limited private;

    type    FILE_MODE       is (IN_FILE, INOUT_FILE, OUT_FILE);
    type    COUNT           is range 0 .. INTEGER'LAST;
    subtype POSITIVE_COUNT  is COUNT range 1 .. COUNT'LAST;    
      

    -- File management


    procedure CREATE   (FILE : in out FILE_TYPE;
                        MODE : in FILE_MODE := INOUT_FILE;
                        NAME : in STRING := "";
                        FORM : in STRING := "");
    pragma IO_interface(CREATE, DIO_CREATE, ELEMENT_TYPE);

    procedure OPEN     (FILE : in out FILE_TYPE;
                        MODE : in FILE_MODE;
                        NAME : in STRING;
                        FORM : in STRING := "");
    pragma IO_interface(OPEN, DIO_OPEN, ELEMENT_TYPE);

    procedure CLOSE    (FILE : in out FILE_TYPE);
    pragma IO_interface(CLOSE, DIO_CLOSE);

    procedure DELETE   (FILE : in out FILE_TYPE);
    pragma IO_interface(DELETE, DIO_DELETE);

    procedure RESET    (FILE : in out FILE_TYPE; MODE : in  FILE_MODE);
    pragma IO_interface(RESET, DIO_RESET_MODE);

    procedure RESET    (FILE : in out FILE_TYPE);
    pragma IO_interface(RESET, DIO_RESET);

    function  MODE     (FILE : in FILE_TYPE)  return FILE_MODE;
    pragma IO_interface(MODE, DIO_MODE);

    function  NAME     (FILE : in FILE_TYPE)  return STRING;
    pragma IO_interface(NAME, DIO_NAME);

    function  FORM     (FILE : in FILE_TYPE)  return STRING;
    pragma IO_interface(FORM, DIO_FORM);
    
    function  IS_OPEN  (FILE : in FILE_TYPE)  return BOOLEAN;
    pragma IO_interface(IS_OPEN, DIO_IS_OPEN);

    -- Input and Output Operations:

    procedure READ   (FILE : in FILE_TYPE; ITEM : out ELEMENT_TYPE);
    pragma IO_interface(READ, DIO_READ, ELEMENT_TYPE);
    procedure READ   (FILE : in FILE_TYPE; ITEM : out ELEMENT_TYPE;
                                           FROM : in POSITIVE_COUNT);
    pragma IO_interface(READ, DIO_READ_FROM, ELEMENT_TYPE);

    procedure WRITE  (FILE : in FILE_TYPE;  ITEM : in ELEMENT_TYPE);
    pragma IO_interface(WRITE, DIO_WRITE, ELEMENT_TYPE);
    procedure WRITE  (FILE : in FILE_TYPE;  ITEM : in ELEMENT_TYPE;
                                            TO   : in POSITIVE_COUNT);
    pragma IO_interface(WRITE, DIO_WRITE_TO, ELEMENT_TYPE);

    procedure SET_INDEX(FILE : in FILE_TYPE; TO :in POSITIVE_COUNT);
    pragma IO_interface(SET_INDEX, DIO_SET_INDEX);
   
    function  INDEX    (FILE : in FILE_TYPE)  return POSITIVE_COUNT;
    pragma IO_interface(INDEX, DIO_INDEX);

    function  SIZE     (FILE : in FILE_TYPE)  return COUNT;
    pragma IO_interface(SIZE, DIO_SIZE);
      
    function  END_OF_FILE(FILE : in FILE_TYPE) return BOOLEAN;
    pragma IO_interface(END_OF_FILE, DIO_END_OF_FILE);

    -- Exceptions:

    STATUS_ERROR : exception renames IO_EXCEPTIONS.STATUS_ERROR;
    MODE_ERROR   : exception renames IO_EXCEPTIONS.MODE_ERROR;
    NAME_ERROR	 : exception renames IO_EXCEPTIONS.NAME_ERROR;
    USE_ERROR	 : exception renames IO_EXCEPTIONS.USE_ERROR;
    DEVICE_ERROR : exception renames IO_EXCEPTIONS.DEVICE_ERROR;
    END_ERROR	 : exception renames IO_EXCEPTIONS.END_ERROR;
    DATA_ERROR	 : exception renames IO_EXCEPTIONS.DATA_ERROR;

private
    type FILE_TYPE is array (1 .. 1) of INTEGER;

end DIRECT_IO;

package body DIRECT_IO is end ;

