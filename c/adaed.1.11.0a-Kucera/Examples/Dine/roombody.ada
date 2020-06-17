WITH Windows;
WITH Chop;
WITH Phil;
WITH Calendar;
PRAGMA Elaborate(Phil);
PACKAGE BODY Room IS

-- Body of Room package. The room contains a set of philosopher tasks
-- and a set of windows in which philosophers report their states.
-- The task Head_Waiter takes care of the window management.
-- Head_Waiter also assigns chopsticks to philosophers. To avoid deadlock,
-- this version of Head_Waiter commands four of the five philosophers to
-- pick up their left sticks first; the fifth is told to pick up his\
-- right stick first, which breaks the deadlock potential.
-- Michael B. Feldman, The George Washington University, November 1990.


  Phils:      ARRAY(Table_Type) OF Phil.Philosopher;
  Phil_Windows: ARRAY(Table_Type) OF Windows.Window;

  TASK BODY Head_Waiter IS

    T : Integer;
    Start_Time: Calendar.Time;

  BEGIN

    ACCEPT Open_The_Room;
    Start_Time := Calendar.Clock;

    Windows.Open(Phil_Windows(1),1,23,7,30);
    Windows.Borders(Phil_Windows(1),'+','|','-');
    Windows.Title(Phil_Windows(1), "Eddy Dijkstra",'-');
    Phils(1).Come_To_Life(1,1,2);

    Windows.Open(Phil_Windows(3),8,50,7,30);
    Windows.Borders(Phil_Windows(3),'+','|','-');
    Windows.Title(Phil_Windows(3), "Grady Booch",'-');
    Phils(3).Come_To_Life(3,3,4);

    Windows.Open(Phil_Windows(2),8,2,7,30);
    Windows.Borders(Phil_Windows(2),'+','|','-');
    Windows.Title(Phil_Windows(2), "Putnam Texel",'-');
    Phils(2).Come_To_Life(2,2,3);

    Windows.Open(Phil_Windows(5),15,41,7,30);
    Windows.Borders(Phil_Windows(5),'+','|','-');
    Windows.Title(Phil_Windows(5), "Bjarne Stroustrup",'-');
    Phils(5).Come_To_Life(5,1,5);

    Windows.Open(Phil_Windows(4),15,8,7,30);
    Windows.Borders(Phil_Windows(4),'+','|','-');
    Windows.Title(Phil_Windows(4), "Jean Ichbiah",'-');
    Phils(4).Come_To_Life(4,4,5);

    LOOP
      SELECT
        ACCEPT Report_State(Which_Phil: Table_Type;
                         State: Phil.States;
                         How_Long: Natural := 0) DO
          T := Integer(Calendar."-"(Calendar.Clock,Start_Time));
          Windows.Put_String(Phil_Windows(Which_Phil),
            "T=" & Integer'Image(T) & " ");

          CASE State IS
            WHEN Phil.Breathing =>
              Windows.Put_String(Phil_Windows(Which_Phil), "Breathing...");
              Windows.New_Line(Phil_Windows(Which_Phil));

            WHEN Phil.Thinking =>
              Windows.Put_String(Phil_Windows(Which_Phil),
                "Thinking" & Integer'Image(How_Long) & " seconds.");
              Windows.New_Line(Phil_Windows(Which_Phil));

            WHEN Phil.Eating =>
              Windows.Put_String(Phil_Windows(Which_Phil),
                "Eating"   & Integer'Image(How_Long) & " seconds.");
              Windows.New_Line(Phil_Windows(Which_Phil));

            WHEN Phil.Done_Eating =>
              Windows.Put_String(Phil_Windows(Which_Phil), "Yum-yum (burp)");
              Windows.New_Line(Phil_Windows(Which_Phil));

            WHEN Phil.Got_One_Stick =>
              Windows.Put_String(Phil_Windows(Which_Phil),
                "First chopstick" & Integer'Image(How_Long));
              Windows.New_Line(Phil_Windows(Which_Phil));

            WHEN Phil.Got_Other_Stick =>
              Windows.Put_String(Phil_Windows(Which_Phil),
                "Second chopstick" & Integer'Image(How_Long));
              Windows.New_Line(Phil_Windows(Which_Phil));

          END CASE;

         END Report_State;
        OR
          TERMINATE;
        END SELECT;

      END LOOP;

    END Head_Waiter;

END Room;
