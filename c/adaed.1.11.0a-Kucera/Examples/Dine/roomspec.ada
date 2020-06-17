WITH Chop;
WITH Phil;
PACKAGE Room IS

-- Specification of dining room package, which exports a set of
-- chopsticks and a head waiter task.
--
-- Michael B. Feldman, The George Washington University, November 1990.


  Table_Size: CONSTANT := 5;
  SUBTYPE Table_Type IS Positive RANGE 1..Table_Size;

  Sticks:     ARRAY(Table_Type) OF Chop.Stick;

  TASK Head_Waiter IS
    ENTRY Open_The_Room;
    ENTRY Report_State(Which_Phil: Table_Type;
                       State: Phil.States;
                       How_Long: Natural := 0);
  END Head_Waiter;

END Room;
