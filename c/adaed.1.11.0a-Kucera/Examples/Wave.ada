--From schweige@taurus.cs.nps.navy.mil Sun Mar  1 21:15:18 1992
--Return-Path: <schweige@taurus.cs.nps.navy.mil>
--Received: from taurus.cs.nps.navy.mil (taurus-campus.cs.nps.navy.mil) by beno.CSS.GOV (4.1/SMI-4.1)
--        id AA18219; Sun, 1 Mar 92 21:15:16 EST
--Received: from aldebaran.cs.nps.navy.mil.nps.navy.mil by taurus.cs.nps.navy.mil (4.1/cs.nps-1.0)
--        id AA12307; Sun, 1 Mar 92 18:14:45 PST
--Date: Sun, 1 Mar 92 18:14:45 PST
--From: schweige@taurus.cs.nps.navy.mil (Jeffrey M. Schweiger)
--Message-Id: <9203020214.AA12307@taurus.cs.nps.navy.mil>
--To: black@beno.CSS.GOV
--Subject: More Ada
--Status: R

-- Title          : Lab 2
-- Author         : Jeffrey M. Schweiger
-- Date           : 6 Apr 1989
-- Revised        : 10 Apr 1989
-- Course         : CS-2970
-- System         : Zenith 248
-- Compiler       : InterAda
-- Description    : This package is used to simplify access to the procedures
--                  GET and PUT for the standard types INTEGER and FLOAT.
--                  This is a non-standard package used by example programs
--                  in the text book 'Ada from the Beginning.'

with TEXT_IO;
package BASIC_NUM_IO is
  package STANDARD_INTEGER_IO is new TEXT_IO.INTEGER_IO(INTEGER);

  DEFAULT_WIDTH : TEXT_IO.FIELD := INTEGER'WIDTH;
  DEFAULT_BASE  : TEXT_IO.NUMBER_BASE := 10;

  procedure GET (ITEM        :out INTEGER;
                 WIDTH       :in TEXT_IO.FIELD := 0)
                 renames STANDARD_INTEGER_IO.GET;

  procedure PUT (ITEM        :in INTEGER;
                 WIDTH       :in TEXT_IO.FIELD := DEFAULT_WIDTH;
                 BASE        :IN TEXT_IO.NUMBER_BASE := DEFAULT_BASE)
                 renames STANDARD_INTEGER_IO.PUT;

  package STANDARD_FLOAT_IO is new TEXT_IO.FLOAT_IO(FLOAT);

  DEFAULT_FORE : TEXT_IO.FIELD := 2;
  DEFAULT_AFT  : TEXT_IO.FIELD := FLOAT'DIGITS - 1;
  DEFAULT_EXP  : TEXT_IO.FIELD := 3;

  procedure GET (ITEM        :out FLOAT;
                 WIDTH       :in TEXT_IO.FIELD := 0)
                 renames STANDARD_FLOAT_IO.GET;

  procedure PUT (ITEM        :in FLOAT;
                 FORE        :in TEXT_IO.FIELD := DEFAULT_FORE;
                 AFT         :in TEXT_IO.FIELD := DEFAULT_AFT;
                 EXP         :in TEXT_IO.FIELD := DEFAULT_EXP)
                 renames STANDARD_FLOAT_IO.PUT;

end BASIC_NUM_IO;


------------
-- Title          : Project 1
-- Author         : Jeffrey M. Schweiger
-- Date           : 12 Apr 1989
-- Revised        : 12 Apr 1989
-- Course         : CS-2970
-- System         : Zenith 248
-- Compiler       : InterAda
-- Description    : Given an input frequency in MHz, this program will compute
--                  the wavelength in meters of the frequency, the length of
--                  of the vertical radiator, and the length of the radials in
--                  meters and feet for a ground-plane antenna.

with TEXT_IO,BASIC_NUM_IO;
use TEXT_IO,BASIC_NUM_IO;
procedure GROUND_PLANE_ANTENNA is
  FREQUENCY, WAVELENGTH, VERTICAL_RADIATOR, RADIAL: FLOAT;
  -- FREQUENCY in MHz, WAVELENGTH in meters, VERTICAL_RADIATOR, RADIAL
  -- in feet.
  SPEED_OF_LIGHT: constant := 3.0E8;
  -- SPEED_OF_LIGHT in meters per second.

begin
  -- read input data.
  PUT ("Enter desired frequency in MHz: ");
  GET (FREQUENCY);
  -- calculate wavelength.
  WAVELENGTH := SPEED_OF_LIGHT / (FREQUENCY * 1.0E6);
  -- output frequency and wavelength.
  NEW_LINE;
  PUT_LINE ("Ground-Plane Antenna");
  PUT ("Frequency is:  ");
  SET_COL (25);
  PUT (FREQUENCY, FORE => 4, AFT => 2, EXP => 0);
  PUT_LINE (" MHz.");
  PUT ("Wavelength is:  ");
  SET_COL (25);
  PUT (WAVELENGTH, FORE => 4, AFT => 2, EXP => 0);
  PUT_LINE (" m.");
  -- calculate and output length of vertical radiator.
  VERTICAL_RADIATOR := 234.0 / FREQUENCY;
  PUT ("Vertical Radiator is:  ");
  SET_COL (25);
  PUT (VERTICAL_RADIATOR/3.281, FORE => 4, AFT => 2, EXP => 0);
  PUT (" m");
  PUT (VERTICAL_RADIATOR, FORE => 4, AFT => 2, EXP => 0);
  PUT_LINE (" ft.");
  -- calculate and output length of radials.
  RADIAL := 240.0 / FREQUENCY;
  PUT ("Radials are:  ");
  SET_COL (25);
  PUT (RADIAL/3.281, FORE => 4, AFT => 2, EXP => 0);
  PUT (" m");
  PUT (RADIAL, FORE => 4, AFT => 2, EXP => 0);
  PUT_LINE (" ft.");

end GROUND_PLANE_ANTENNA;


