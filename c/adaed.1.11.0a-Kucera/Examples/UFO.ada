--From schweige@taurus.cs.nps.navy.mil Sun Mar  1 21:16:15 1992
--Return-Path: <schweige@taurus.cs.nps.navy.mil>
--Received: from taurus.cs.nps.navy.mil (taurus-campus.cs.nps.navy.mil) by beno.CSS.GOV (4.1/SMI-4.1)
--        id AA18224; Sun, 1 Mar 92 21:16:13 EST
--Received: from aldebaran.cs.nps.navy.mil.nps.navy.mil by taurus.cs.nps.navy.mil (4.1/cs.nps-1.0)
--        id AA12312; Sun, 1 Mar 92 18:15:44 PST
--Date: Sun, 1 Mar 92 18:15:44 PST
--From: schweige@taurus.cs.nps.navy.mil (Jeffrey M. Schweiger)
--Message-Id: <9203020215.AA12312@taurus.cs.nps.navy.mil>
--To: black@beno.CSS.GOV
--Subject: Again, more Ada
--Status: R

-- Title          : Project 2
-- Author         : Jeffrey M. Schweiger
-- Date           : 20 Apr 1989
-- Revised        : 20 Apr 1989
-- Course         : CS-2970
-- System         : Zenith 248
-- Compiler       : InterAda
-- Description    : Given a input file with data of UFO sightings during one
--                  twenty-four hour period, consisting of time of day of the
--                  sighting, and estimated altitude of the sighting, output
--                  list of all sightings, and highlight data on the first,
--                  minimum altitude, and last sighting of the day.

with TEXT_IO;
use TEXT_IO;

procedure UNIDENTIFIED_FLYING_OBJECTS is

  FILE_NAME       : constant STRING := "UFO.DAT";
  THE_FILE        : FILE_TYPE;

  HOURS_FIRST,
  MINUTES_FIRST,
  SECONDS_FIRST,
  ALTITUDE_FIRST  : INTEGER;
  -- Data for first sighting of the day

  HOURS_MIN,
  MINUTES_MIN,
  SECONDS_MIN,
  ALTITUDE_MIN    : INTEGER;
  -- Data for sighting with closest point to the earth for the day

  HOURS_LAST,
  MINUTES_LAST,
  SECONDS_LAST,
  ALTITUDE_LAST   : INTEGER;
  -- Data for last sighting of the day

  -- Input and output procedures for INTEGERS
  package INTEGER_INOUT is new INTEGER_IO(INTEGER);
  use INTEGER_INOUT;


  -- Create procedure to print leading zeros for time output

  procedure PUT_TIME(TIME  : INTEGER) is
    begin
      if TIME < 10 then
        PUT("0");
        PUT(TIME, WIDTH => 1);
      else
        PUT(TIME, WIDTH => 2);
      end if;
    end PUT_TIME;



begin

  -- print header information
  SET_COL(13);
  PUT("Daily Unidentified Flying Object (UFO) Sighting Report");
  NEW_LINE(3);
  PUT ("Time of Sighting");
  SET_COL(27);
  PUT_LINE("Altitude");


  -- open file for input and read first observation

  OPEN(THE_FILE, MODE => IN_FILE, NAME => FILE_NAME);

  GET(THE_FILE, HOURS_FIRST);
  GET(THE_FILE, MINUTES_FIRST);
  GET(THE_FILE, SECONDS_FIRST);
  GET(THE_FILE, ALTITUDE_FIRST);
  SKIP_LINE(THE_FILE);

  -- Write first observation of the day

  SET_COL(4);
  PUT_TIME(HOURS_FIRST);
  PUT(":");
  PUT_TIME(MINUTES_FIRST);
  PUT(":");
  PUT_TIME(SECONDS_FIRST);

  SET_COL(25);  -- move to altitude column

  PUT(ALTITUDE_FIRST, WIDTH => 4);
  PUT_LINE(" meters.");


  -- assign first observation data as initial minimum sighting data

  HOURS_MIN := HOURS_FIRST;
  MINUTES_MIN := MINUTES_FIRST;
  SECONDS_MIN := SECONDS_FIRST;
  ALTITUDE_MIN := ALTITUDE_FIRST;


  -- assign first observation data as initial last sighting data

  HOURS_LAST := HOURS_FIRST;
  MINUTES_LAST := MINUTES_FIRST;
  SECONDS_LAST := SECONDS_FIRST;
  ALTITUDE_LAST := ALTITUDE_FIRST;


  -- Read data file until end of file

  while not END_OF_FILE(THE_FILE) loop

    -- Read from the disk file, current observation is last observation

    GET(THE_FILE, HOURS_LAST);
    GET(THE_FILE, MINUTES_LAST);
    GET(THE_FILE, SECONDS_LAST);
    GET(THE_FILE, ALTITUDE_LAST);

    SKIP_LINE(THE_FILE);


    -- Write current observation to screen

    SET_COL(4);
    PUT_TIME(HOURS_LAST);
    PUT(":");
    PUT_TIME(MINUTES_LAST);
    PUT(":");
    PUT_TIME(SECONDS_LAST);
    SET_COL(25);
    PUT(ALTITUDE_LAST, WIDTH => 4);
    PUT_LINE(" meters.");


    -- If current observation altitude is less than previous minimum
    -- altitude, assign new minimum

    if ALTITUDE_LAST < ALTITUDE_MIN then

      HOURS_MIN := HOURS_LAST;
      MINUTES_MIN := MINUTES_LAST;
      SECONDS_MIN := SECONDS_LAST;
      ALTITUDE_MIN := ALTITUDE_LAST;

    end if;

  -- end data read loop

  end loop;

  CLOSE(THE_FILE);


  -- Write time and altitude of first sighting of the day

  NEW_LINE(2);
  PUT("The first sighting of the day was at ");
  PUT_TIME(HOURS_FIRST);
  PUT(":");
  PUT_TIME(MINUTES_FIRST);
  PUT(":");
  PUT_TIME(SECONDS_FIRST);
  PUT(".  Altitude was ");
  PUT(ALTITUDE_FIRST, WIDTH => 4);
  PUT_LINE(" meters.");
  NEW_LINE(2);


  -- Write time minimum altitude was first observed and the altitude

  PUT("Minimum altitude was first observed at ");
  PUT_TIME(HOURS_MIN);
  PUT(":");
  PUT_TIME(MINUTES_MIN);
  PUT(":");
  PUT_TIME(SECONDS_MIN);
  PUT(".  Minimum altitude was ");
  PUT(ALTITUDE_MIN, WIDTH => 4);
  PUT_LINE(" meters.");
  NEW_LINE(2);


  -- Write time and altitude of last sighting of the day

  PUT("The last sighting of the day was at ");
  PUT_TIME(HOURS_LAST);
  PUT(":");
  PUT_TIME(MINUTES_LAST);
  PUT(":");
  PUT_TIME(SECONDS_LAST);
  PUT(".  Altitude was ");
  PUT(ALTITUDE_LAST, WIDTH => 4);
  PUT_LINE(" meters.");


end UNIDENTIFIED_FLYING_OBJECTS;
