-- This will extract the files from a ".src" file on SIMTEL20

-------- SIMTEL20 Ada Software Repository Prologue ------------
--                                                           -*
-- Unit name    : UNPAGE
-- Version      : 1.0
-- Author       : Mitre Corp.
-- DDN Address  : wis_ada at mitre
-- Date created : 21 JAN 85
-- Release date : 29 JAN 85
-- Last update  : 21 JAN 85
-- Machine/System Compiled/Run on : Intellimac 7000M
--                                  UNIX
--                                  Telesoft unvalidated
--                                                           -*
---------------------------------------------------------------
--                                                           -*
-- Keywords     :  Table builder, Text formatter
--
----------------:
--
-- Abstract     :
--      UNPAGE is a simple little program which complements the
-- UNIX page command.  The UNIX page command can be used to combine
-- several source files, interspersing file headers of the form:
--
--                    ::::::::::
--                    FILENAME
--                    ::::::::::
--
--      UNPAGE reads such a file breaking the subfiles into separate
-- files as indicated by the filename headers.  UNPAGE has been
-- enhanced to also recognize file headers which have the format of
-- an Ada comment:
--
--                    --::::::::::
--                    --FILENAME
--                    --::::::::::
----------------:
--                                                           -*
------------------ Revision history ---------------------------
--                                                           -*
-- DATE         VERSION AUTHOR           HISTORY
-- 12/15/84     1.0     Mitre Corp       Initial Release
-- 01/21/85     1.0     Mitre Corp       Add recognition of
--                                       --:::::::::::
--                                                           -*
------------------ Distribution and Copyright -----------------
--                                                           -*
-- This prologue must be included in all copies of this software.
--
-- This software is released to the Public Domain (note:
--   software released to the Public Domain is not subject
--   to copyright protection).
--
-- Restrictions on use or distribution:  Although there are
--      no current plans to provide maintenance for UNPAGE,
--      we would appreciate your reporting problems and
--      experiences to:
--
--                wis_ada at mitre (net address)
--
--      or call at:
--
--                (703)  883-7697
--                                                           -*
------------------ Disclaimer ---------------------------------
--                                                           -*
-- This software and its documentation are provided "AS IS" and
-- without any expressed or implied warranties whatsoever.
-- No warranties as to performance, merchantability, or fitness
-- for a particular purpose exist.
--
-- Because of the diversity of conditions and hardware under
-- which this software may be used, no warranty of fitness for
-- a particular purpose is offered.  The user is advised to
-- test the software thoroughly before relying on it.  The user
-- must assume the entire risk and liability of using this
-- software.
--
-- In no event shall any person or organization of people be
-- held responsible for any direct, indirect, consequential
-- or inconsequential damages or lost profits.
--                                                           -*
-------------------END-PROLOGUE--------------------------------
with TEXT_IO; use TEXT_IO;
procedure UNPAGE is
  MAX_STRING   : constant INTEGER := 255;
  NEW_FILE     : FILE_TYPE;
  PAGED_FILE   : FILE_TYPE;
  LINE         : STRING(1..MAX_STRING);
  LAST         : INTEGER;

  procedure WRITE_ERROR
    (PART1 : in STRING;
     PART2 : in STRING) is
  begin
    NEW_LINE;
    PUT("**ERROR**");
    PUT(PART1);
    PUT(PART2);
    NEW_LINE;
  end WRITE_ERROR;



  procedure OPEN_INPUT is
    TAB_NAME  : STRING(1..MAX_STRING);
  begin
    NEW_LINE;
    PUT("ENTER NAME OF PAGE-CATENATED FILE =>");
    GET_LINE(TAB_NAME, LAST);
    OPEN(PAGED_FILE, IN_FILE, TAB_NAME(1..LAST));
  end;


  function FILE_NAME_LINE return BOOLEAN is
  begin
    if LAST >= 10 and then (LINE(1..10) = "::::::::::" or
                            LINE(1..10) = "--::::::::")then
      return TRUE;
    else
      return FALSE;
    end if;
  end FILE_NAME_LINE;


  procedure OPEN_NEW_FILE is
    NAME_START : integer;
  begin
    GET_LINE(PAGED_FILE, LINE, LAST);
    PUT_LINE(LINE(1..LAST));

    if LINE(1..2) = "--" then
      NAME_START := 3;
    else
      NAME_START := 1;
    end if;
    CREATE (NEW_FILE, OUT_FILE, LINE(NAME_START..LAST));

    --Skip over the trailing ":::::::::::::" line
    GET_LINE(PAGED_FILE, LINE, LAST);
  end OPEN_NEW_FILE;



begin

  --Prompt user for paged file name. Then get name of first file to be processed
  OPEN_INPUT;

  GET_LINE(PAGED_FILE, LINE, LAST);
  if FILE_NAME_LINE then
    OPEN_NEW_FILE;
    loop
        GET_LINE(PAGED_FILE, LINE, LAST);
        if FILE_NAME_LINE then
          CLOSE(NEW_FILE);
          OPEN_NEW_FILE;
        else
          PUT_LINE(NEW_FILE, LINE(1..LAST));
        end if;
    end loop;
  else
    WRITE_ERROR("FILE IS NOT IN PAGED FORMAT, EXECUTION ABORTED", ".");
  end if;

exception
  when END_ERROR => CLOSE(NEW_FILE);
                    CLOSE(PAGED_FILE);
end UNPAGE;

