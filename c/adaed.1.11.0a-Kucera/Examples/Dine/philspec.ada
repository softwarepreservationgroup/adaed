PACKAGE Phil IS
-- Specification for dining philosopher task.
-- Assumption is that head waiter will name a philosopher and assign
-- the chopsticks to be used by that philosopher.
-- Philosopher states are used by monitoring program (Head_Waiter).
-- Michael B. Feldman, The George Washington University, November 1990.

  
  TASK TYPE Philosopher IS
    
    ENTRY Come_To_Life (My_ID :      Positive; 
                        Chopstick1 : Positive;
                        Chopstick2 : Positive);

  END Philosopher;

  TYPE States IS (Breathing, Thinking, Eating, Done_Eating,
                    Got_One_Stick, Got_Other_Stick);

END Phil;
