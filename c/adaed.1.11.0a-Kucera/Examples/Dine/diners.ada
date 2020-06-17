WITH Room;
PROCEDURE Diners IS

-- Main procedure for dining philosophers. 
-- The endless loop is a device to keep the main program alive, because
-- some Ada implementations kill off library tasks when the main procedure
-- is completed. This device makes the program entirely portable.

-- Michael B. Feldman, The George Washington University, November 1990.

BEGIN
  Room.Head_Waiter.Open_The_Room;
  LOOP
    DELAY 20.0;
  END LOOP;
END Diners;
