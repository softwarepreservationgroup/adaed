with TEXT_IO, VT100; use TEXT_IO, VT100;
PACKAGE Windows IS 

-- Simple output-only window manager for 24x80 terminals.
-- Adapted by
-- Michael B. Feldman, The George Washington University, November 1990.

   
   ScreenRows : constant integer := 24;
   ScreenColumns : constant integer := 80;

   TYPE WINDOW is private;
   
   SUBTYPE RowRange  is integer range 1.. ScreenRows;
   SUBTYPE ColRange  is integer range 1.. ScreenColumns;
   SUBTYPE RowLength is integer range 1.. ScreenRows;
   SUBTYPE ColLength is integer range 1.. ScreenColumns;
        
        
   PROCEDURE Open (  w: in out WINDOW;          -- Window variable returned 
                       row   : RowRange;        -- Upper left corner        
                       column: ColRange;
                       height: RowLength;       -- Size of window           
                       width : ColLength);

   -- Create a window variable and open the window for writing.  
   -- No checks for overlap of windows are made. 
   
      
   PROCEDURE Close (  w: in out WINDOW);
   -- Close window and clear window variable. 
   
   
   PROCEDURE Title (w     : in out WINDOW;
                    name  : STRING;
                    under : CHARACTER);

   -- Put a title name at the top of the window.  If the parameter 
   -- under <> 0C or ' ', underline the title with the specified character. 
      
      
   PROCEDURE Borders (w   : in out WINDOW;
                      corner, down, across: CHARACTER);

   -- Draw border around current writable area in window with characters
   -- specified.  Call this BEFORE Title.  
   
   
   PROCEDURE GotoRowColumn (w    : in out WINDOW;
                            row  : RowRange;
                            column : ColRange);

   -- Goto the row and column specified.  Coordinates are relative to the
   -- upper left corner of window, which is (1, 1) 
   
   
   PROCEDURE put (w: in out WINDOW; ch: CHARACTER);

   -- put one character to the window.
   -- If end of column, go to the next row.
   -- If end of window, go to the top of the window. 
   
   
   PROCEDURE put_string (w: in out WINDOW;
                          s: STRING);

   -- put a string to window. 


   PROCEDURE new_line (w: in out WINDOW);

   -- Go to beginning of next line.  Next line is
   -- not blanked until next character is written  


   PRIVATE
      type WINDOW is 
          RECORD
             CurrentRow,                  -- Current cursor row 
             firstrow,
             lastrow      : RowRange;
             CurrentColumn,               -- Current cursor column 
             firstcolumn,
             lastcolumn    : ColRange;
          END RECORD;

END Windows;
