PACKAGE Chop IS
-- Specification for a chopstick task, intended for use with 
-- dining philosophers. It is really just a binary semaphore.
-- Michael B. Feldman, The George Washington University, November 1990.

  TASK TYPE Stick IS
    ENTRY Pick_Up;
    ENTRY Put_Down;
  END Stick;

END Chop;
