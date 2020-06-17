PACKAGE BODY Windows is

-- Body of window manager package.
-- Adapted by
-- Michael B. Feldman, The George Washington University, November 1990.

  CursorRow: RowRange := 1;            -- Current cursor position 
  CursorCol: ColRange := 1;
   
   PROCEDURE Open (    w: in out WINDOW;
                       row: RowRange;
                       column: ColRange;
                       height: RowLength;
                       width:  ColLength) is
             --put the window's cursor in upper left corner 
   BEGIN
      w.CurrentRow := row;
      w.firstrow := row;
      w.lastrow := row + height - 1;
      w.CurrentColumn := column;
      w.firstcolumn := column;
      w.lastcolumn := column + width - 1;
   END Open;
   
   PROCEDURE Close (w: in out WINDOW) IS
   BEGIN
      null;
   END Close;
   
   PROCEDURE Title (w     : in out WINDOW;
                    name  : STRING;
                    under : CHARACTER) IS
   -- Put name at the top of the window.  If under <>  ' ', underline
   -- the title. 
      i: ColRange;
   BEGIN
      -- put name on top line 
      w.CurrentColumn := w.firstcolumn;
      w.CurrentRow := w.firstrow;
      put_string (w, name);
      new_line (w);
      -- Underline name if desired, and move the first line of the window
      -- below the title 
      IF under = ' ' THEN
         w.firstrow := w.firstrow + 1;
      ELSE
         FOR i IN w.firstcolumn .. w.lastcolumn LOOP
            put (w, under);
            END LOOP;
         new_line (w);
         w.firstrow := w.firstrow + 2;
      END IF;
   END Title;
   
      
   PROCEDURE GotoRowColumn (w    : in out WINDOW;
                            row  : RowRange;
                            column : ColRange) IS
   -- Relative to writable window boundaries, of course 
   BEGIN
      w.CurrentRow := w.firstrow + row;
      w.CurrentColumn := w.firstcolumn + column;
   END GotoRowColumn;
      
      
   PROCEDURE Borders (w   : in out WINDOW;
                      corner, down, across: CHARACTER) IS
   -- Draw border around current writable area in window with characters.
   -- Call this BEFORE Title.  
      i: RowRange;
      j: ColRange;
   BEGIN
      -- put top line of border 
      SetCursorAt (w.firstcolumn, w.firstrow);
      TEXT_IO.put (corner);
      FOR j IN w.firstcolumn + 1  ..  w.lastcolumn - 1 LOOP
         TEXT_IO.put (across);
         END LOOP;
      TEXT_IO.put (corner);
      
      -- put the two side lines 
      FOR i IN w.firstrow + 1  .. w.lastrow - 1 LOOP
         SetCursorAt (w.firstcolumn, i);
         TEXT_IO.put (down);
         SetCursorAt (w.lastcolumn, i);
         TEXT_IO.put (down);
         END LOOP;
         
      -- put the bottom line of the border 
      SetCursorAt (w.firstcolumn, w.lastrow);
      TEXT_IO.put (corner);
      FOR j IN w.firstcolumn + 1  .. w.lastcolumn - 1 LOOP
         TEXT_IO.put (across);
         END LOOP;
      TEXT_IO.put (corner);
      
      -- Put the cursor at the very end of the window 
      CursorRow := w.lastrow;
      CursorCol := w.lastcolumn + 1;
      
      -- Make the window smaller by one character on each side 
      w.firstrow := w.firstrow + 1;
      w.CurrentRow := w.firstrow;
      w.lastrow := w.lastrow - 1;
      w.firstcolumn := w.firstcolumn + 1;
      w.CurrentColumn := w.firstcolumn;
      w.lastcolumn := w.lastcolumn - 1;
   END Borders;
   

   PROCEDURE EraseToEndOfLine (w: in out WINDOW) IS
      i: ColRange;
   BEGIN
      SetCursorAt (w.CurrentColumn, w.CurrentRow);
      FOR i IN w.CurrentColumn .. w.lastcolumn LOOP
         TEXT_IO.put (' ');
         END LOOP;
      SetCursorAt (w.CurrentColumn, w.CurrentRow);
      CursorCol := w.CurrentColumn;
      CursorRow := w.CurrentRow;
   END EraseToEndOfLine;
   
      
   PROCEDURE put (w: in out WINDOW;
                   ch: CHARACTER) IS
   
   -- If after end of line, move to first character of next line 
   -- If about to write first character on line, blank rest of line.
   -- put character. 
   
   BEGIN
      IF ch = ASCII.CR THEN
         new_line(w);
         RETURN;
         END IF;
         
         -- If at end of current line, move to next line 
         IF w.CurrentColumn > w.lastcolumn THEN
            IF w.CurrentRow = w.lastrow THEN
               w.CurrentRow := w.firstrow;
            ELSE 
               w.CurrentRow := w.CurrentRow + 1;
            END IF;
            w.CurrentColumn := w.firstcolumn;
         END IF;
         
         -- If at w.first char, erase line 
         IF w.CurrentColumn = w.firstcolumn THEN
            EraseToEndOfLine (w);
         END IF;
         
         -- Put physical cursor at window's cursor  
         IF (CursorCol /= w.CurrentColumn) OR (CursorRow /= w.CurrentRow) 
         THEN
            SetCursorAt (w.CurrentColumn, w.CurrentRow);
            CursorRow := w.CurrentRow;
         END IF;
         
         IF ch = ASCII.BS THEN
            -- Special backspace handling 
            IF w.CurrentColumn /= w.firstcolumn THEN
               TEXT_IO.put(ch);
               w.CurrentColumn := w.CurrentColumn - 1;
            END IF;
         ELSE
            TEXT_IO.put (ch);
            w.CurrentColumn := w.CurrentColumn + 1;
         END IF;
         CursorCol := w.CurrentColumn;
   END put;
      
   
   PROCEDURE new_line (w: in out WINDOW) IS
      col: ColRange;
   
   -- If not after line, blank rest of line.
   -- Move to first character of next line 
   
   BEGIN
      IF w.CurrentColumn = 0 THEN
         EraseToEndOfLine (w);
      END IF;
      IF w.CurrentRow = w.lastrow THEN
         w.CurrentRow := w.firstrow;
      ELSE w.CurrentRow := w.CurrentRow + 1;
      END IF;
      w.CurrentColumn := w.firstcolumn;
   END new_line;
   
   
   PROCEDURE put_string (w: in out WINDOW;
                          s: STRING) IS
   BEGIN
      FOR i in s'first .. s'last LOOP
         put (w, s(i));
      END LOOP;
   END put_string;
   

BEGIN -- Windows 
   ClearScreen;
   SetCursorAt (1, 1);
END Windows;
