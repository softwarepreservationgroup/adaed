
with TEXT_IO, MY_INT_IO; use  TEXT_IO, MY_INT_IO;
package VT100 is
   use ASCII;
----------------------------------------------------------
-- Procedures for drawing pictures of the solution on VDU.
-- ClearScreen and SetCursorAt are device-specific
----------------------------------------------------------

    SCREEN_DEPTH	: constant INTEGER	:= 24;
    SCREEN_WIDTH	: constant INTEGER	:= 80;

    subtype DEPTH is INTEGER range 1..SCREEN_DEPTH;
    subtype WIDTH is INTEGER range 1..SCREEN_WIDTH;


  procedure ClearScreen; 

  procedure SetCursorAt( A: WIDTH; D : DEPTH);

end VT100;    
