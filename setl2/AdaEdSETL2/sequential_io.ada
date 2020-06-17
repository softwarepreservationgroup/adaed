with IO_EXCEPTIONS;
generic
    type ELEMENT_TYPE is private;

package SEQUENTIAL_IO is

    type FILE_TYPE is limited private;
    
    type FILE_MODE is (IN_FILE, OUT_FILE);
      

    -- File management


    procedure CREATE   (FILE : in out FILE_TYPE;
                        MODE : in FILE_MODE := OUT_FILE;
                        NAME : in STRING    := "";
                        FORM : in STRING    := "");
    pragma IO_interface(CREATE, SIO_CREATE, ELEMENT_TYPE);

    procedure OPEN     (FILE : in out FILE_TYPE;
                        MODE : in FILE_MODE;
                        NAME : in STRING;
                        FORM : in STRING := "");
    pragma IO_interface(OPEN, SIO_OPEN, ELEMENT_TYPE);

    procedure CLOSE    (FILE : in out FILE_TYPE);
    pragma IO_interface(CLOSE, SIO_CLOSE);

    procedure DELETE   (FILE : in out FILE_TYPE);
    pragma IO_interface(DELETE, SIO_DELETE);

    procedure RESET    (FILE : in out FILE_TYPE; MODE : in  FILE_MODE);
    pragma IO_interface(RESET, SIO_RESET_MODE);

    procedure RESET    (FILE : in out FILE_TYPE);
    pragma IO_interface(RESET, SIO_RESET);

    function  MODE     (FILE : in FILE_TYPE)  return FILE_MODE;
    pragma IO_interface(MODE, SIO_MODE);

    function  NAME     (FILE : in FILE_TYPE)  return STRING;
    pragma IO_interface(NAME, SIO_NAME);

    function  FORM     (FILE : in FILE_TYPE)  return STRING;
    pragma IO_interface(FORM, SIO_FORM);
    
    function  IS_OPEN  (FILE : in FILE_TYPE)  return BOOLEAN;
    pragma IO_interface(IS_OPEN, SIO_IS_OPEN);

    -- Input and Output Operations:

    procedure READ   (FILE : in FILE_TYPE; ITEM : out ELEMENT_TYPE);
    pragma IO_interface(READ, SIO_READ,ELEMENT_TYPE);

    procedure WRITE  (FILE : in FILE_TYPE; ITEM : in ELEMENT_TYPE);
    pragma IO_interface(WRITE, SIO_WRITE,ELEMENT_TYPE);

    function  END_OF_FILE(FILE : in FILE_TYPE) return BOOLEAN;
    pragma IO_interface(END_OF_FILE, SIO_END_OF_FILE);

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

end SEQUENTIAL_IO;

package body SEQUENTIAL_IO is end ;

