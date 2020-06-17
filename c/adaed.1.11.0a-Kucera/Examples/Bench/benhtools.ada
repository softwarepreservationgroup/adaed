with text_io; use text_io;
with system;
with direct_io;
package help_tools is

  procedure time_filename (file_name: out string);

  procedure read_environ (outfile_ptr: in file_type);

  procedure read_envfile (outfile_ptr: in file_type);

end help_tools;

-------------------------------------------------------------------------------
with calendar; use calendar;
with text_io; use text_io;
with system;
with direct_io;

package body help_tools is
--------------------------------------------------------------------------------
--
--  procedure time_filename
--
--  purpose: to create a string which can be used as a file name which
--           has the date encoded in the title
--
--------------------------------------------------------------------------------
procedure time_filename (file_name: out string) is

subtype month_name is string(1..2);

MON_START_POS :  CONSTANT := 2;         -- start and end indices for month
MON_END_POS:  CONSTANT := 3;
DAY_START_POS:  CONSTANT := 4;          -- start and end indices for day
DAY_END_POS:  CONSTANT := 5;
HOUR_START_POS: CONSTANT := 6;          -- start and end indices for hour
HOUR_END_POS: CONSTANT := 7;
MIN_START_POS:  CONSTANT := 8;          -- start and end indices for minutes
MIN_END_POS:    CONSTANT := 9;
SMAX: constant := 2;                    -- maximum length for a numeric string

date : time;                            -- date
cur_year: year_number;                  -- current year
cur_mon:  month_number;                 -- current month
cur_day:  day_number;                   -- current day

ipass,                                  -- total_seconds/max_int
itemp,                                  -- temporary integer variable
hours,                                  -- number of hours
minutes: integer;                       -- number of minutes

total_sec: day_duration;                -- total number of seconds

int_seconds: integer;                   -- integer variable for seconds

hour_string,                            -- ascii string for hours
min_string,                             -- ascii string for minutes
day_string: string (1..2);              -- ascii string for days

dtotal_sec,                             -- floating point var for seconds
dtemp_result,                           -- results
dtemp_num,                              -- temp variable for floating pt numbers
dtemp_remainder:  float;                -- remainder of total seconds

month_code : array (month_number) of month_name :=
                ("ja","fe","mr","ap","my","jn",
                 "jl","ag","sp","oc","nv","dc");


function int_to_string (I: integer) return string is

-- converts integers to strings

   S: string (1..smax);
   SPOS : integer := s'last;
   HOLD : integer := I;
   DIGIT : integer;
--
begin
   S := (others=>'0');
   while HOLD /= 0 loop
     DIGIT := HOLD rem 10;
     S(SPOS) := character'val(abs(DIGIT) + character'pos('0'));
     SPOS := SPOS - 1;
     HOLD := HOLD/10;
   end loop;
   return S;
end INT_TO_STRING;

begin
--
--  initialize the filename to Wmmddhhmm.DAT
--  get the informations for the current date and time
--
    file_name := "Wmmddmmmm.DAT";
    date := clock;
    cur_year := year (date);
    cur_mon  := month (date);
    cur_day  := day(date);
    total_sec := seconds(date);
--
--  get the largest integer available to the system
--  float the values for both max_int and the total number of seconds
--
    itemp := integer'last;
    dtotal_sec := float (total_sec);
    dtemp_num := float (itemp);
    dtemp_result := dtotal_sec - dtemp_num;
--
--  if the total number of seconds is greater than max_int
--     ( a case which is true for doing 16 bit arithmetic)
--     determine how many times max_int is a divisor for total number
--          of seconds
--     for the number of passes (max_int divides) do
--         increment the hours accordingly
--         increment the minutes accordingly
--     end for
--     calculate the values of minutes
--

    IPASS := 0;  -- GTE FORGOT TO INITIALIZE THIS OBJECT!!!!

    if (dtemp_result > 0.0) then

       while (dtemp_result > 0.0) loop
          ipass := ipass + 1;
          dtemp_result := dtemp_result - dtemp_num;
       end loop;
       hours := 0; minutes := 0;
       dtemp_remainder := dtemp_num + dtemp_result;
       for iter in 1..ipass loop
           hours := hours + 9;
           minutes :=  minutes + 6;
       end loop;
       itemp := integer(dtemp_remainder);
       hours := hours + itemp/3600;
       itemp := itemp rem 3600;
       minutes := minutes + itemp/60;
--
--  else
--     calculate hours directly from total seconds
--     calculate minutes directly from total seconds
--  endif
--
    else

       int_seconds :=  integer(total_sec);
       hours := int_seconds/3600;
       minutes := (int_seconds - (hours*3600))/60;

    end if;
--
--  translate the numbers to acsii representations and
--      build the filename
--
    day_string := int_to_string (CUR_DAY);
    hour_string:=  int_to_string(hours);
    min_string  := int_to_string (minutes);

    file_name (mon_START_POS..mon_END_POS) := month_code(cur_mon);
    file_name (DAY_START_POS..DAY_END_POS) := day_STRING;
    file_name (HOUR_START_POS..HOUR_END_POS) := hour_string;
    file_name (MIN_START_POS..MIN_END_POS) := min_string;

end time_filename;
--------------------------------------------------------------------------------
--
--  procedure read_environ
--
--  purpose: writes system specific data in to the data file
--
--------------------------------------------------------------------------------
procedure read_environ (outfile_ptr: in file_type) is

--package int_io is new integer_io(integer);
package INT_IO is new INTEGER_IO(INTEGER);
use int_io;

package flt_io is new float_io(float);
use flt_io;

package dur_io is new fixed_io(duration);
--above statement replaced because TeleSoft 3.2 defines type "duration"
--  as 32-bit integer.
--package DUR_IO is new INTEGER_IO(DURATION);
use dur_io;

package dirinf_io is new direct_io(integer);

temp_dvar:  day_duration;       -- temporary variable for day duration;

temp_fvar:  float;              -- temporary variable for floating constants

--temp_ivar,                            -- temporary variable to ge values from
TEMP_IVAR : INTEGER;  --TELESOFT 3.2 PACKAGE "SYSTEM" OBJECTS ARE LONG_INTEGER

                                --   package system
last_char: integer;             -- pointer to last character in line

blank_line,
line : string (1..80) := (others => ' ');

begin
--
--  get data from package system
--

    Read_Envfile (outfile_ptr);

    new_line(outfile_ptr);
    put_line (outfile_ptr," DATA FROM PACKAGE SYSTEM: ");

    temp_ivar := system.storage_unit;
    put (outfile_ptr,"SYSTEM.STORAGE_UNIT = ");
    int_io.put (outfile_ptr,temp_ivar);

    new_line(outfile_ptr);
    temp_ivar := system.memory_size;
    put (outfile_ptr,"SYSTEM.MEMORY_SIZE  = ");
    int_io.put (outfile_ptr,temp_ivar);

    new_line(outfile_ptr);
    temp_ivar := system.min_int;
    put (outfile_ptr,"SYSTEM.MIN_INT      = ");
    int_io.put (outfile_ptr,temp_ivar);

    new_line(outfile_ptr);
    temp_ivar := system.max_int;
    put (outfile_ptr,"SYSTEM.MAX_INT      = ");
    int_io.put (outfile_ptr,temp_ivar);

    new_line(outfile_ptr);
    temp_ivar := system.max_digits;
    put (outfile_ptr,"SYSTEM.MAX_DIGITS   = ");
    int_io.put (outfile_ptr,temp_ivar);

    new_line(outfile_ptr);
    temp_ivar := system.max_mantissa;
    put (outfile_ptr,"SYSTEM.MAX_MANTISSA = ");
    int_io.put (outfile_ptr,temp_ivar);


    new_line(outfile_ptr);
    temp_fvar := system.fine_delta;
    put (outfile_ptr,"SYSTEM.FINE_DELTA   = ");
    put (outfile_ptr,temp_fvar);

    new_line(outfile_ptr);
    temp_fvar := system.tick;
    put (outfile_ptr,"SYSTEM.TICK         = ");
    put (outfile_ptr,temp_fvar);

     new_line(outfile_ptr); new_line(outfile_ptr);
     put (outfile_ptr," DATA FROM PACKAGES FROM I-O SYSTEMS: ");

     new_line(outfile_ptr);
      --temp_ivar := integer(text_io.count'last);
      TEMP_IVAR := INTEGER(TEXT_IO.COUNT'LAST);
     put (outfile_ptr,"text_io.count'last   = ");
     put (outfile_ptr,temp_ivar);

     new_line(outfile_ptr);
      --temp_ivar := integer(dirinf_io.count'last);
      TEMP_IVAR := INTEGER(DIRINF_IO.COUNT'LAST);
     put (outfile_ptr,"direct_io.count'last = ");
     put (outfile_ptr,temp_ivar);


     new_line (outfile_ptr); new_line(outfile_ptr);
     put_line (outfile_ptr,"DATA FROM PACKAGE STANDARD: ");

    --temp_ivar := integer'first;
    TEMP_IVAR := INTEGER'FIRST;
     put (outfile_ptr,"Integer range is ");
     put(outfile_ptr,temp_ivar);
    --temp_ivar := integer'last;
    TEMP_IVAR := INTEGER'LAST;
     put (outfile_ptr," .. "); put(outfile_ptr,temp_ivar);


     new_line (outfile_ptr);
     temp_fvar := duration'delta;
     put (outfile_ptr,"DURATION is delta ");
     put (outfile_ptr,temp_fvar);


     new_line (outfile_ptr);
     temp_fvar := duration'small;
     put (outfile_ptr,"DURATION'small = ");
     put (outfile_ptr,temp_fvar);

     new_line (outfile_ptr); new_line(outfile_ptr);
     put (outfile_ptr,"Calendar package values: ");
     new_line (outfile_ptr);
     temp_dvar := calendar.day_duration'first;
     put (outfile_ptr,"DAY DURATION range is : ");
     put (outfile_ptr,temp_dvar);put (outfile_ptr," to ");
     temp_dvar := calendar.day_duration'last;
     put (outfile_ptr,temp_dvar);

     new_line (outfile_ptr); new_line (outfile_ptr);
     put (outfile_ptr,"End of looking at system parameters");
     new_line (outfile_ptr); new_line (outfile_ptr);

end read_environ;
-------------------------------------------------------------------------------
--
--   procedure read_envfile
--
--   purpose:  to copy the data from a file called ENVIRON.INF into the
--               file specified by the outfile_ptr
--
--   exception: if the file ENVIRON.INF does not exit, an exception
--               handler will write a message in file specified by outfile_ptr
--               that ENVIRON.INF has existence problems
--
-------------------------------------------------------------------------------
procedure read_envfile (outfile_ptr: in file_type) is
--
indata: file_type;              -- internal filename for actual data file

file_name : string (1..11) := ("environ.inf");

blank_line,
line : string (1..120) := (others => ' ');

last_char: integer;             -- position of last character in line read

--
--  get data out of environment file ENVIRON.INF
--

begin

    open (indata,in_file,file_name);
    while not end_of_file (indata) loop
        line := blank_line;
        get_line(indata,line,last_char);
        put_line(outfile_ptr,line(1..last_char));
    end loop;
    close (indata);

  exception
    when others =>
        put_line (outfile_ptr,
                  "Something is wrong with the existence of file ENVIRON.INF");
        new_line (outfile_ptr);

end read_envfile;

end help_tools;

