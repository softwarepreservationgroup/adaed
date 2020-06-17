package CALENDAR is

  type TIME is private ;

   subtype YEAR_NUMBER  is INTEGER  range 1901 .. 2099;
   subtype MONTH_NUMBER is INTEGER  range 1 .. 12;
   subtype DAY_NUMBER   is INTEGER  range 1 .. 31;
   subtype DAY_DURATION is DURATION range 0.0 .. 86_400.0;

   function CLOCK return TIME;
   pragma IO_interface(CLOCK, CLOCK);

   function YEAR   (DATE    : TIME) return YEAR_NUMBER;
   pragma IO_interface(YEAR, YEAR);
   function MONTH  (DATE    : TIME) return MONTH_NUMBER;
   pragma IO_interface(MONTH, MONTH);
   function DAY    (DATE    : TIME) return DAY_NUMBER;
   pragma IO_interface(DAY, DAY);
   function SECONDS(DATE    : TIME) return DAY_DURATION;
   pragma IO_interface(SECONDS, SECONDS);

   procedure SPLIT (DATE    : in  TIME;
                    YEAR    : out YEAR_NUMBER;
                    MONTH   : out MONTH_NUMBER;
                    DAY     : out DAY_NUMBER;
                    SECONDS : out DAY_DURATION);
   pragma IO_interface(SPLIT, SPLIT);

   function TIME_OF(YEAR    : YEAR_NUMBER;
                    MONTH   : MONTH_NUMBER;
                    DAY     : DAY_NUMBER;
                    SECONDS : DAY_DURATION := 0.0) return TIME;
   pragma IO_interface(TIME_OF, TIME_OF);

   function "+"  (LEFT : TIME;     RIGHT : DURATION) return TIME;
   pragma IO_interface("+", ADD_TIME_DUR);
   function "+"  (LEFT : DURATION; RIGHT : TIME)     return TIME;
   pragma IO_interface("+", ADD_DUR_TIME);
   function "-"  (LEFT : TIME;     RIGHT : DURATION) return TIME;
   pragma IO_interface("-", SUB_TIME_DUR);
   function "-"  (LEFT : TIME;     RIGHT : TIME)     return DURATION;
   pragma IO_interface("-", SUB_TIME_TIME);

   function "<"  (LEFT, RIGHT : TIME) return BOOLEAN;
   pragma IO_interface("<", LT_TIME);
   function "<=" (LEFT, RIGHT : TIME) return BOOLEAN;
   pragma IO_interface("<=", LE_TIME);
   function ">"  (LEFT, RIGHT : TIME) return BOOLEAN;
   pragma IO_interface(">", GT_TIME);
   function ">=" (LEFT, RIGHT : TIME) return BOOLEAN;
   pragma IO_interface(">=", GE_TIME);

   TIME_ERROR : exception;   --   can be raised by TIME_OF, "+", "-"

private
   type TIME is array (1 .. 4) of INTEGER;
end CALENDAR;

package body CALENDAR is end CALENDAR ;
