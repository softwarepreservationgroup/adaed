PACKAGE BODY Chop IS
-- Package body for chopstick task, intended for use with
-- dining philosophers. It is really just a simple binary semaphore.
-- Michael B. Feldman, November 1990

  TASK BODY Stick IS

  BEGIN
    
    LOOP
      SELECT
        ACCEPT Pick_Up;
        ACCEPT Put_Down;
      OR
        TERMINATE;
      END SELECT;
    END LOOP;

  END Stick;

END Chop;
