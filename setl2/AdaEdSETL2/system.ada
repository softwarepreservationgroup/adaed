package SYSTEM is

   type NAME    is (ADA_ED); 
   type ADDRESS is new INTEGER;

   SYSTEM_NAME  : constant NAME := ADA_ED;
   STORAGE_UNIT : constant      := 32;
   MEMORY_SIZE  : constant      := 2**30 - 1;

   -- System Dependent Named Numbers:

   MIN_INT      : constant      := -(2**30 - 1);
   MAX_INT      : constant      := 2**30 - 1;
   MAX_DIGITS   : constant      := 33;
   MAX_MANTISSA : constant      := 31;
   FINE_DELTA   : constant      := 2.0 ** (-31);
   TICK         : constant      := 0.01;

   -- Other System Dependent Declarations

   subtype PRIORITY is INTEGER range 0 .. 9;

   SYSTEM_ERROR : exception;

end SYSTEM;

