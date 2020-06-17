WITH Room;
WITH Random;
PACKAGE BODY Phil IS
  
-- Body of philosopher task. 

-- The philosopher needs to "with" package Room in order to report its states.
-- Package Random is used to generate a (pseudo-)random-length meal and
-- thinking time. This produces very useful nondeterminism.

-- Michael B. Feldman, The George Washington University, November 1990.

  TASK BODY Philosopher IS

    Who_Am_I   : Positive;
    First_Grab : Positive;
    Second_Grab: Positive;
    Meal_Time  : Natural;
    Think_Time : Natural;
    
  BEGIN
    ACCEPT Come_To_Life (My_ID :     Positive; 
                        Chopstick1 : Positive;
                        Chopstick2 : Positive) DO
      Who_Am_I    := My_ID;
      First_Grab  := Chopstick1;
      Second_Grab := Chopstick2;

    END Come_To_Life;

    Room.Head_Waiter.Report_State(Who_Am_I, Breathing);

    LOOP

      Room.Sticks(First_Grab).Pick_Up;
      Room.Head_Waiter.Report_State(Who_Am_I, Got_One_Stick, First_Grab);

      Room.Sticks(Second_Grab).Pick_Up;
      Room.Head_Waiter.Report_State(Who_Am_I, Got_Other_Stick, Second_Grab);

      Meal_Time := Random.Random_Int(10);
      Room.Head_Waiter.Report_State(Who_Am_I, Eating, Meal_Time);

      DELAY Duration(Meal_Time);
      Room.Head_Waiter.Report_State(Who_Am_I, Done_Eating);

      Room.Sticks(First_Grab).Put_Down;
      Room.Sticks(Second_Grab).Put_Down;

      Think_Time := Random.Random_Int(10);
      Room.Head_Waiter.Report_State(Who_Am_I, Thinking, Think_Time);
      DELAY Duration(Think_Time);

    END LOOP;

  END Philosopher;

END Phil;
