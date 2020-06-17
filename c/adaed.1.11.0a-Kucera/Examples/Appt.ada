--This example doesn't seem to work completely.  But it is a start.
--It hits end-of-file when adding to an existing item.

--From pnc-a@minster.york.ac.uk Sat Feb 29 19:24:13 1992
--Return-Path: <pnc-a@minster.york.ac.uk>
--Received: from sun2.nsfnet-relay.ac.uk by beno.CSS.GOV (4.1/SMI-4.1)
--        id AA14724; Sat, 29 Feb 92 19:24:02 EST
--Received: from minster.york.ac.uk by sun2.nsfnet-relay.ac.uk via JANET
--          with NIFTP id <8562-0@sun2.nsfnet-relay.ac.uk>;
--          Sat, 29 Feb 1992 23:12:21 +0000
--Date: 29 Feb 1992 23:15:55 GMT
--From: pnc-a@minster.york.ac.uk
--To: black <@nsfnet-relay.ac.uk:black@beno.css.gov>
--Message-Id: <swordfish.699405359@minster.york.ac.uk>
--Status: R
--
--Here's some ADA code. It is not very complex, but you can play with
--it if you wish.
--This program sucks, as it is about the first program I did in ADA,
--but as I don't know the standards, it is more likely to push ADA
--over than a properly written program.
--(Oh, and I haven't got the mark back for it...)
--
--A datafile is needed (default is test.dat), in the format
--start_time/duration/day/month/year/comment
--time is in form 0000-2359, date is UK format.
--I could probably dredge up some docs for it if you wanted.
--
--
-- Author:      P.N.D. Corlett
--
-- Address:     Department of Computer Science,
--              University of York,
--              Heslington,
--              York, YO1 5DD,
--              ENGLAND.
--
-- EMail:       pnc-a@uk.ac.york.minster
--
-- Last Edit:   7 December 1991
--
-- Filename:    xmas.A
--
-- Requires:    TEXT_IO,INTEGER_IO
--
-- Summary:
--              (Hmm. I'll do this later... when I work out what it is doing...)
--
--
--
--

with TEXT_IO;
use TEXT_IO;

-- ****MAIN*************************************************************

procedure MAIN is

    package INT_IO is new INTEGER_IO(INTEGER);

    use INT_IO;
    STRLEN : constant := 100;

    -- change this to what you want
    -- Subtypes for the record structure

    subtype TIME_TYPE is INTEGER range 0 .. (60 * 24 - 1);

    -- I prefer to use minutes since midnight.

    subtype DAY_TYPE is INTEGER range 1 .. 31;
    subtype MONTH_TYPE is INTEGER range 1 .. 12;
    subtype YEAR_TYPE is INTEGER range 1901 .. 2099;

    -- fairly arbitrary....
    -- range for time checking

    subtype MINUTES is INTEGER range 0 .. 59;
    subtype HOURS is INTEGER range 0 .. 23;

    -- The record structure itself

    type DIARY_ENTRY is
        record
            START_TIME : TIME_TYPE;
            DURATION : TIME_TYPE;
            DAY : DAY_TYPE;
            MONTH : MONTH_TYPE;
            YEAR : YEAR_TYPE;
            APPOINTMENT : STRING(1 .. STRLEN);
        end record;

    -- Variable used in menu testing

    THE_END : BOOLEAN := FALSE;
    FILENAME : STRING(1 .. 50);

    -- ****EXPAND_STRING****************************************************
    -- This function returns the string 'string', but padded out to
    -- 'len' characters with NULs.

    function EXPAND_STRING(STR : STRING; LEN : INTEGER) return
        STRING is
        STR2 : STRING(1 .. LEN);
    begin

        -- There is probably an easier way of doing this (but it's 4am...)

        for N in 1 .. STR'LAST loop
            STR2(N) := STR(N);
        end loop;
        for N in STR'LAST + 1 .. LEN loop
            STR2(N) := ASCII.NUL;
        end loop;
        return (STR2);
    end EXPAND_STRING;

    -- ****PRINT_APPOINTMENT_TITLE******************************************
    -- Just prints the header for the info printed by
    -- print_appointment. 'head' says whether the header should be printed,
    -- or just the underline

    procedure PRINT_APPOINTMENT_TITLE(HEAD : BOOLEAN) is
    begin
        if HEAD then
            PUT("Time  Dur.  Date       Appointment");
            NEW_LINE;
        end if;
        PUT("----- ----- ---------- --------------------------------------------------------"
            );
        NEW_LINE;

        --   TT:TT TT:TT DD/MM/YYYY AAAAAA....

    end PRINT_APPOINTMENT_TITLE;

    -- ****INPUT_APPOINTMENT************************************************
    -- Fills in an appointment record with info from stdin

    procedure INPUT_APPOINTMENT(
        APPOINTMENT : out DIARY_ENTRY; ERROR : out BOOLEAN) is
        TEMP_CHAR : CHARACTER := '?';
        TEMP : INTEGER;

        -- misc work

        INDEX : INTEGER := 1;

        -- index in string array

        MINUTE, HOUR : INTEGER;

        -- temp work for time

    begin
        ERROR := FALSE;

        -- convert the start time from normal (0-2359) to internal format
        -- and check it while I'm at it...

        PUT("Start time [0000-2359] : ");
        GET(TEMP);
        MINUTE := TEMP mod 100;
        HOUR := TEMP / 100;
        if MINUTE not in MINUTES or else HOUR not in HOURS then
            ERROR := TRUE;
        else
            APPOINTMENT.START_TIME := HOUR * 60 + MINUTE;
        end if;

        -- convert the duration from normal (0-2359) to internal format
        -- and check it while I'm at it...

        PUT("Duration   [0000-2359] : ");
        GET(TEMP);
        MINUTE := TEMP mod 100;
        HOUR := TEMP / 100;
        if MINUTE not in MINUTES or else HOUR not in HOURS then
            ERROR := TRUE;
        else
            APPOINTMENT.DURATION := HOUR * 60 + MINUTE;
        end if;

        -- check date

        PUT("Date       [1-31]      : ");
        GET(TEMP);
        if TEMP not in DAY_TYPE then
            ERROR := TRUE;
        else
            APPOINTMENT.DAY := TEMP;
        end if;

        -- check month

        PUT("Month      [1-12]      : ");
        GET(TEMP);
        if TEMP not in MONTH_TYPE then
            ERROR := TRUE;
        else
            APPOINTMENT.MONTH := TEMP;
        end if;

        -- check year

        PUT("Year       [1901-2099] : ");
        GET(TEMP);
        if TEMP not in YEAR_TYPE then
            ERROR := TRUE;
        else
            APPOINTMENT.YEAR := TEMP;
        end if;
        PUT("Comments   : ");

        -- the rest of the line is the comment

        APPOINTMENT.APPOINTMENT := EXPAND_STRING("", STRLEN);

        -- makes string field empty

        loop
            GET(TEMP_CHAR);
            if INDEX <= STRLEN then

                -- trap lines longer than 'strlen' chars

                APPOINTMENT.APPOINTMENT(INDEX) := TEMP_CHAR;
            end if;
            INDEX := INDEX + 1;
            exit when END_OF_LINE;
        end loop;
    exception
        when DATA_ERROR =>
            ERROR := TRUE;
    end INPUT_APPOINTMENT;

    -- ****PRINT_APPOINTMENT************************************************
    -- this outputs the record data in a sensible format.

    procedure PRINT_APPOINTMENT(APPOINTMENT : in DIARY_ENTRY) is
    begin
        PUT(APPOINTMENT.START_TIME / 60, 2);
        PUT(':');
        if (APPOINTMENT.START_TIME mod 60) < 10 then
            PUT('0');
        end if;
        PUT(APPOINTMENT.START_TIME mod 60, 1);
        PUT(' ');
        PUT(APPOINTMENT.DURATION / 60, 2);
        PUT(':');
        if (APPOINTMENT.DURATION mod 60) < 10 then
            PUT('0');
        end if;
        PUT(APPOINTMENT.DURATION mod 60, 1);
        PUT(' ');
        PUT(APPOINTMENT.DAY, 2);
        PUT('/');
        PUT(APPOINTMENT.MONTH, 2);
        PUT('/');
        PUT(APPOINTMENT.YEAR, 4);
        PUT(' ');
        PUT(APPOINTMENT.APPOINTMENT);
        NEW_LINE;
    end PRINT_APPOINTMENT;

    -- ****READ_APPOINTMENT*************************************************
    -- Reads an appointment from the file 'handle' into the record
    -- 'appointment'. 'error' is true if the data is rubbish

    procedure READ_APPOINTMENT(HANDLE : FILE_TYPE;
                               APPOINTMENT : out DIARY_ENTRY;
                               ERROR : out BOOLEAN) is
        TEMP_CHAR : CHARACTER := '?';
        TEMP : INTEGER;

        -- misc work

        INDEX : INTEGER := 1;

        -- index in string array

        MINUTE, HOUR : INTEGER;

        -- temp work for time

    begin
        ERROR := FALSE;

        -- convert the start time from normal (0-2359) to internal format
        -- and check it while I'm at it...

        GET(HANDLE, TEMP);
        MINUTE := TEMP mod 100;
        HOUR := TEMP / 100;
        if MINUTE not in MINUTES or else HOUR not in HOURS then
            ERROR := TRUE;
        else
            APPOINTMENT.START_TIME := HOUR * 60 + MINUTE;
        end if;
        GET(HANDLE, TEMP_CHAR);

        -- Throw away '/'
        -- convert the duration from normal (0-2359) to internal format
        -- and check it while I'm at it...

        GET(HANDLE, TEMP);
        MINUTE := TEMP mod 100;
        HOUR := TEMP / 100;
        if MINUTE not in MINUTES or else HOUR not in HOURS then
            ERROR := TRUE;
        else
            APPOINTMENT.DURATION := HOUR * 60 + MINUTE;
        end if;
        GET(HANDLE, TEMP_CHAR);

        -- Throw away '/'
        -- check date

        GET(HANDLE, TEMP);
        if TEMP not in DAY_TYPE then
            ERROR := TRUE;
        else
            APPOINTMENT.DAY := TEMP;
        end if;
        GET(HANDLE, TEMP_CHAR);

        -- Throw away '/'
        -- check month

        GET(HANDLE, TEMP);
        if TEMP not in MONTH_TYPE then
            ERROR := TRUE;
        else
            APPOINTMENT.MONTH := TEMP;
        end if;
        GET(HANDLE, TEMP_CHAR);

        -- Throw away '/'
        -- check year

        GET(HANDLE, TEMP);
        if TEMP not in YEAR_TYPE then
            ERROR := TRUE;
        else
            APPOINTMENT.YEAR := TEMP;
        end if;
        GET(HANDLE, TEMP_CHAR);

        -- Throw away '/'
        -- the rest of the line is the comment

        APPOINTMENT.APPOINTMENT := EXPAND_STRING("", STRLEN);

        -- makes string field empty

        while not (END_OF_LINE(HANDLE)) loop
            if INDEX <= STRLEN then

                -- trap lines longer than 'strlen' chars

                GET(HANDLE, APPOINTMENT.APPOINTMENT(INDEX));
            else
                GET(HANDLE, TEMP_CHAR);

                -- throw away what we can't read.

            end if;
            INDEX := INDEX + 1;
        end loop;
    exception
        when DATA_ERROR =>
            ERROR := TRUE;
    end READ_APPOINTMENT;

    procedure WRITE_APPOINTMENT(
        HANDLE : FILE_TYPE; APPOINTMENT : DIARY_ENTRY) is
        INDEX : INTEGER := 1;
        TEMP : INTEGER;
    begin
        TEMP := APPOINTMENT.START_TIME;
        PUT(HANDLE, TEMP / 60 * 100 + TEMP mod 60, 0);

        -- expands internal format into file

        PUT(HANDLE, '/');
        TEMP := APPOINTMENT.DURATION;
        PUT(HANDLE, TEMP / 60 * 100 + TEMP mod 60, 0);

        -- expands internal format into file

        PUT(HANDLE, '/');
        PUT(HANDLE, APPOINTMENT.DAY, 0);
        PUT(HANDLE, '/');
        PUT(HANDLE, APPOINTMENT.MONTH, 0);
        PUT(HANDLE, '/');
        PUT(HANDLE, APPOINTMENT.YEAR, 0);
        PUT(HANDLE, '/');
        while APPOINTMENT.APPOINTMENT(INDEX) /= ASCII.NUL loop
            PUT(HANDLE, APPOINTMENT.APPOINTMENT(INDEX));
            INDEX := INDEX + 1;
        end loop;
        PUT(HANDLE, ASCII.LF);
    end WRITE_APPOINTMENT;

    procedure CLS is
    begin
--        PUT(ASCII.ESC & "[2J" & ASCII.ESC & "[0;0H" & ASCII.ESC
--            & "[0m");
        PUT( ASCII.FF );
    end CLS;

    function MENU(FILENAME : STRING) return CHARACTER is
        subtype MENU_RANGE is CHARACTER range '1' .. '5';
        OPTION : CHARACTER := ' ';
    begin
        while OPTION not in MENU_RANGE loop
            CLS;
            PUT_LINE(ASCII.ESC &
                     "#3      ITP Christmas Diary Program");
            PUT_LINE(ASCII.ESC &
                     "#4      ITP Christmas Diary Program");
            PUT_LINE(ASCII.ESC & "#6             Peter Corlett")
                ;
            NEW_LINE;
            PUT("Current diary is ");
            PUT_LINE(FILENAME);
            NEW_LINE;
            PUT_LINE("1) List diary");
            PUT_LINE("2) Add an entry");
            PUT_LINE("3) Search for an entry");
            PUT_LINE("4) Change diary");
            PUT_LINE("5) Quit");
            NEW_LINE;
            PUT("Choose an option : ");
            GET(OPTION);
        end loop;
        return (OPTION);
    end MENU;

    procedure WAIT_KEY is
        TEMP : INTEGER;
        TEMP2 : STRING(1 .. 10);
    begin
        PUT("Press RETURN to continue ");
        SKIP_LINE;

        -- there seems to be some stuff still in the buffer here..

        GET_LINE(TEMP2, TEMP);
    end WAIT_KEY;

    function APPOINTMENT_CLASH(
        APP1 : DIARY_ENTRY; APP2 : DIARY_ENTRY) return BOOLEAN
        is
    begin
        if APP1.YEAR /= APP2.YEAR then
            return (FALSE);
        end if;
        if APP1.MONTH /= APP2.MONTH then
            return (FALSE);
        end if;
        if APP1.DAY /= APP2.DAY then
            return (FALSE);
        end if;
        if APP1.START_TIME >= APP2.START_TIME + APP2.DURATION
            then
            return (FALSE);
        end if;
        if APP2.START_TIME >= APP1.START_TIME + APP1.DURATION
            then
            return (FALSE);
        end if;
        return (TRUE);
    end APPOINTMENT_CLASH;

    function APPOINTMENT_MATCH(APP : DIARY_ENTRY; DAY : INTEGER;
                               MONTH : INTEGER; YEAR : INTEGER)
        return BOOLEAN is
    begin
        if DAY /= 0 and APP.DAY /= DAY then
            return (FALSE);
        end if;
        if MONTH /= 0 and APP.MONTH /= MONTH then
            return (FALSE);
        end if;
        if YEAR /= 0 and APP.YEAR /= YEAR then
            return (FALSE);
        end if;
        return (TRUE);
    end APPOINTMENT_MATCH;

    procedure LIST_DIARY(FILENAME : STRING) is
        HANDLE : FILE_TYPE;
        ERROR : BOOLEAN;
        APP : DIARY_ENTRY;
    begin
        CLS;
        PRINT_APPOINTMENT_TITLE(TRUE);
        OPEN(HANDLE, IN_FILE, FILENAME);
        while not END_OF_FILE(HANDLE) loop
            READ_APPOINTMENT(HANDLE, APP, ERROR);
            PRINT_APPOINTMENT(APP);
        end loop;
        CLOSE(HANDLE);
        PRINT_APPOINTMENT_TITLE(FALSE);
        WAIT_KEY;
    end LIST_DIARY;

    procedure ADD_ENTRY(FILENAME : STRING) is
        HANDLE : FILE_TYPE;
        ERROR, CLASH : BOOLEAN := FALSE;
        APP, NEWAPP : DIARY_ENTRY;
    begin
        CLS;
        INPUT_APPOINTMENT(NEWAPP, ERROR);
        if ERROR then
            PUT_LINE("Sorry, there is something wrong there");
        else
            CLS;
            PRINT_APPOINTMENT_TITLE(TRUE);
            OPEN(HANDLE, IN_FILE, FILENAME);
            while not END_OF_FILE(HANDLE) loop
                READ_APPOINTMENT(HANDLE, APP, ERROR);
                if APPOINTMENT_CLASH(APP, NEWAPP) then
                    PUT(ASCII.ESC & "[5m");
                    PRINT_APPOINTMENT(APP);
                    PUT(ASCII.ESC & "[25m");
                    CLASH := TRUE;
                else
                    PRINT_APPOINTMENT(APP);
                end if;
            end loop;
            if CLASH then
                PUT(ASCII.ESC & "[5m" & ASCII.ESC & "[1m");
                PRINT_APPOINTMENT(NEWAPP);
                PUT(ASCII.ESC & "[0m");
                PRINT_APPOINTMENT_TITLE(FALSE);
                PUT_LINE("CLASH - New appointment not added");
            else
                PUT(ASCII.ESC & "[1m");
                PRINT_APPOINTMENT(NEWAPP);
                PUT(ASCII.ESC & "[0m");
                PRINT_APPOINTMENT_TITLE(FALSE);
                PUT_LINE("No clashes - Adding appointment");
                RESET(HANDLE, OUT_FILE);
                WRITE_APPOINTMENT(HANDLE, NEWAPP);
            end if;
        end if;
        WAIT_KEY;
        CLOSE(HANDLE);
    end ADD_ENTRY;

    procedure SEARCH_ENTRY(FILENAME : STRING) is
        HANDLE : FILE_TYPE;
        ERROR : BOOLEAN := FALSE;
        APP : DIARY_ENTRY;
        DAY, MONTH, YEAR : INTEGER := 42;

        -- nice dummy value that fails the test...

    begin
        CLS;
        PUT_LINE("Put 0 for 'any day/month/year'");
        while DAY not in 0 .. 31 loop
            PUT("Which day [1-31]       : ");
            GET(DAY);
        end loop;
        while MONTH not in 0 .. 12 loop
            PUT("Which month [1-12]     : ");
            GET(MONTH);
        end loop;
        while YEAR /= 0 and then YEAR not in YEAR_TYPE loop
            PUT("Which year [1901-2099] : ");
            GET(YEAR);
        end loop;
        CLS;
        PRINT_APPOINTMENT_TITLE(TRUE);
        OPEN(HANDLE, IN_FILE, FILENAME);
        while not END_OF_FILE(HANDLE) loop
            READ_APPOINTMENT(HANDLE, APP, ERROR);
            if APPOINTMENT_MATCH(APP, DAY, MONTH, YEAR) then
                PUT(ASCII.ESC & "[1m");
                PRINT_APPOINTMENT(APP);
                PUT(ASCII.ESC & "[0m");
            else
                PRINT_APPOINTMENT(APP);
            end if;
        end loop;
        PRINT_APPOINTMENT_TITLE(FALSE);
        WAIT_KEY;
        CLOSE(HANDLE);
    end SEARCH_ENTRY;

    procedure GET_FILENAME(FILENAME : in out STRING) is
        INDEX : INTEGER := 1;
    begin
        PUT("Old name : ");
        PUT(FILENAME);
        NEW_LINE;
        PUT("New name : ");
        FILENAME := EXPAND_STRING("", FILENAME'LENGTH);
        loop
            GET(FILENAME(INDEX));
            INDEX := INDEX + 1;
            exit when END_OF_LINE;
        end loop;
    end GET_FILENAME;

    -- ****MAIN procedure code *********************************************

begin
    FILENAME := EXPAND_STRING("Appt.dat", 50);

    -- default filename

    while not (THE_END) loop
        case MENU(FILENAME) is

            -- filename passed to menu so it can display it...

            when '1' =>
                LIST_DIARY(FILENAME);
            when '2' =>
                ADD_ENTRY(FILENAME);
            when '3' =>
                SEARCH_ENTRY(FILENAME);
            when '4' =>
                GET_FILENAME(FILENAME);
            when '5' =>
                THE_END := TRUE;
            when others =>
                null;

                -- user typed something silly

        end case;
    end loop;
exception
    when END_ERROR =>
        NEW_LINE;
        PUT_LINE("OK then, bye!");
end MAIN;



