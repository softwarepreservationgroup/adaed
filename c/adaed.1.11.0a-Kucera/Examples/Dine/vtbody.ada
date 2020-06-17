with TEXT_IO; use  TEXT_IO;
package body VT100 is
  use ASCII;
----------------------------------------------------------
-- Procedures for drawing pictures on VT100
-- ClearScreen and SetCursorAt are trminal-specific
----------------------------------------------------------

  procedure ClearScreen is
  begin
--      PUT( ESC & "[H[J" );
        PUT( FF );
  end ClearScreen;

  procedure SetCursorAt(A: WIDTH; D : DEPTH) is

  begin
        NEW_LINE;
        PUT( ESC & "[" );
        PUT( D, 1 );
        PUT( ';' );
        PUT( A, 1 );
        PUT( 'H' );
  end SetCursorAt;

end VT100;

-- .......................................................................... --
