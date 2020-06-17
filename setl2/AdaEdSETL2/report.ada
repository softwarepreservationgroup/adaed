-- REPSPEC.ADA

-- PURPOSE:
--      THIS REPORT PACKAGE PROVIDES THE MECHANISM FOR REPORTING THE
--      PASS/FAIL/NOT-APPLICABLE RESULTS OF EXECUTABLE (CLASSES A, C,
--      D, E, AND L) TESTS.

--      IT ALSO PROVIDES THE MECHANISM FOR GUARANTEEING THAT CERTAIN
--      VALUES BECOME DYNAMIC (NOT KNOWN AT COMPILE-TIME).

-- HISTORY:
--      JRK 12/13/79
--      JRK 06/10/80
--      JRK 08/06/81
--      JRK 10/27/82
--      JRK 06/01/84
--      PWB 07/30/87  ADDED PROCEDURE SPECIAL_ACTION.
--      TBN 08/20/87  ADDED FUNCTION LEGAL_FILE_NAME.
--      BCB 05/17/90  ADDED FUNCTION TIME_STAMP.

PACKAGE REPORT IS

     SUBTYPE FILE_NUM IS INTEGER RANGE 1..3;

  -- THE REPORT ROUTINES.

     PROCEDURE TEST           -- THIS ROUTINE MUST BE INVOKED AT THE
                              -- START OF A TEST, BEFORE ANY OF THE
                              -- OTHER REPORT ROUTINES ARE INVOKED.
                              -- IT SAVES THE TEST NAME AND OUTPUTS THE
                              -- NAME AND DESCRIPTION.
        ( NAME : STRING;      -- TEST NAME, E.G., "C23001A-AB".
          DESCR : STRING      -- BRIEF DESCRIPTION OF TEST, E.G.,
                              -- "UPPER/LOWER CASE EQUIVALENCE IN " &
                              -- "IDENTIFIERS".
        );

     PROCEDURE FAILED         -- OUTPUT A FAILURE MESSAGE.  SHOULD BE
                              -- INVOKED SEPARATELY TO REPORT THE
                              -- FAILURE OF EACH SUBTEST WITHIN A TEST.
        ( DESCR : STRING      -- BRIEF DESCRIPTION OF WHAT FAILED.
                              -- SHOULD BE PHRASED AS:
                              -- "(FAILED BECAUSE) ...REASON...".
        );

     PROCEDURE NOT_APPLICABLE -- OUTPUT A NOT-APPLICABLE MESSAGE.
                              -- SHOULD BE INVOKED SEPARATELY TO REPORT
                              -- THE NON-APPLICABILITY OF EACH SUBTEST
                              -- WITHIN A TEST.
        ( DESCR : STRING      -- BRIEF DESCRIPTION OF WHAT IS
                              -- NOT-APPLICABLE. SHOULD BE PHRASED AS:
                              -- "(NOT-APPLICABLE BECAUSE)...REASON...".
        );

     PROCEDURE SPECIAL_ACTION -- OUTPUT A MESSAGE DESCRIBING SPECIAL
                              -- ACTIONS TO BE TAKEN.
                              -- SHOULD BE INVOKED SEPARATELY TO GIVE
                              -- EACH SPECIAL ACTION.
        ( DESCR : STRING      -- BRIEF DESCRIPTION OF ACTION TO BE
                              -- TAKEN.
        );

     PROCEDURE COMMENT        -- OUTPUT A COMMENT MESSAGE.
        ( DESCR : STRING      -- THE MESSAGE.
        );

     PROCEDURE RESULT;        -- THIS ROUTINE MUST BE INVOKED AT THE
                              -- END OF A TEST.  IT OUTPUTS A MESSAGE
                              -- INDICATING WHETHER THE TEST AS A
                              -- WHOLE HAS PASSED, FAILED, IS
                              -- NOT-APPLICABLE, OR HAS TENTATIVELY
                              -- PASSED PENDING SPECIAL ACTIONS.

  -- THE DYNAMIC VALUE ROUTINES.

     -- EVEN WITH STATIC ARGUMENTS, THESE FUNCTIONS WILL HAVE DYNAMIC
     -- RESULTS.

     FUNCTION IDENT_INT       -- AN IDENTITY FUNCTION FOR TYPE INTEGER.
        ( X : INTEGER         -- THE ARGUMENT.
        ) RETURN INTEGER;     -- X.

     FUNCTION IDENT_CHAR      -- AN IDENTITY FUNCTION FOR TYPE
                              -- CHARACTER.
        ( X : CHARACTER       -- THE ARGUMENT.
        ) RETURN CHARACTER;   -- X.

     FUNCTION IDENT_BOOL      -- AN IDENTITY FUNCTION FOR TYPE BOOLEAN.
        ( X : BOOLEAN         -- THE ARGUMENT.
        ) RETURN BOOLEAN;     -- X.

     FUNCTION IDENT_STR       -- AN IDENTITY FUNCTION FOR TYPE STRING.
        ( X : STRING          -- THE ARGUMENT.
        ) RETURN STRING;      -- X.

     FUNCTION EQUAL           -- A RECURSIVE EQUALITY FUNCTION FOR TYPE
                              -- INTEGER.
        ( X, Y : INTEGER      -- THE ARGUMENTS.
        ) RETURN BOOLEAN;     -- X = Y.

-- OTHER UTILITY ROUTINES.

     FUNCTION LEGAL_FILE_NAME -- A FUNCTION TO GENERATE LEGAL EXTERNAL
                              -- FILE NAMES.
        ( X : FILE_NUM := 1;  -- DETERMINES FIRST CHARACTER OF NAME.
          NAM : STRING := ""  -- DETERMINES REST OF NAME.
        ) RETURN STRING;      -- THE GENERATED NAME.

     FUNCTION TIME_STAMP      -- A FUNCTION TO GENERATE THE TIME AND
                              -- DATE TO PLACE IN THE OUTPUT OF AN ACVC
                              -- TEST.
          RETURN STRING;      -- THE TIME AND DATE.

END REPORT;

-- REPBODY.ADA

-- HISTORY:
--      DCB 04/27/80
--      JRK 6/10/80
--      JRK 11/12/80
--      JRK 8/6/81
--      JRK 10/27/82
--      JRK 6/1/84
--      JRK 11/18/85  ADDED PRAGMA ELABORATE.
--      PWB 07/29/87  ADDED STATUS ACTION_REQUIRED AND
--                    PROCEDURE SPECIAL_ACTION.
--      TBN 08/20/87  ADDED FUNCTION LEGAL_FILE_NAME.
--      BCB 05/17/90  MODIFIED TO ALLOW OUTPUT TO DIRECT_IO FILE.
--                    ADDED TIME-STAMP.
--      LDC 05/17/90  REMOVED OUTPUT TO DIRECT_IO FILE.

WITH TEXT_IO, CALENDAR;
USE TEXT_IO, CALENDAR;
PRAGMA ELABORATE (TEXT_IO, CALENDAR);

PACKAGE BODY REPORT IS

     TYPE STATUS IS (PASS, FAIL, DOES_NOT_APPLY, ACTION_REQUIRED, 
                     UNKNOWN);

     TYPE TIME_INTEGER IS RANGE 0 .. 86_400;

     TEST_STATUS : STATUS := FAIL;

     MAX_NAME_LEN : CONSTANT := 15;     -- MAXIMUM TEST NAME LENGTH.
     TEST_NAME : STRING (1..MAX_NAME_LEN);

     NO_NAME : CONSTANT STRING (1..7) := "NO_NAME";
     TEST_NAME_LEN : INTEGER RANGE 0..MAX_NAME_LEN := 0;

     ACVC_VERSION : CONSTANT STRING := "1.12";  -- VERSION OF ACVC BEING
                                                -- RUN (X.XX).

     PROCEDURE PUT_MSG (MSG : STRING) IS
          -- WRITE MESSAGE.  LONG MESSAGES ARE FOLDED (AND INDENTED).
          MAX_LEN : CONSTANT INTEGER RANGE 50..150 := 72;  -- MAXIMUM
                                        -- OUTPUT LINE LENGTH.
          INDENT : CONSTANT INTEGER := TEST_NAME_LEN + 9;  -- AMOUNT TO
                                        -- INDENT CONTINUATION LINES.
          I : INTEGER := 0;             -- CURRENT INDENTATION.
          M : INTEGER := MSG'FIRST;     -- START OF MESSAGE SLICE.
          N : INTEGER;                  -- END OF MESSAGE SLICE.
     BEGIN
          LOOP
               IF I + (MSG'LAST-M+1) > MAX_LEN THEN
                    N := M + (MAX_LEN-I) - 1;
                    IF MSG (N) /= ' ' THEN
                         WHILE N >= M AND THEN MSG (N+1) /= ' ' LOOP
                              N := N - 1;
                         END LOOP;
                         IF N < M THEN
                              N := M + (MAX_LEN-I) - 1;
                         END IF;
                    END IF;
               ELSE N := MSG'LAST;
               END IF;
               SET_COL (STANDARD_OUTPUT, TEXT_IO.COUNT (I+1));
               PUT_LINE (STANDARD_OUTPUT, MSG (M..N));
               I := INDENT;
               M := N + 1;
               WHILE M <= MSG'LAST AND THEN MSG (M) = ' ' LOOP
                    M := M + 1;
               END LOOP;
               EXIT WHEN M > MSG'LAST;
          END LOOP;
     END PUT_MSG;

     FUNCTION TIME_STAMP RETURN STRING IS
          TIME_NOW : CALENDAR.TIME;
          YEAR,
          MONTH,
          DAY,
          HOUR,
          MINUTE,
          SECOND : TIME_INTEGER := 1;

          FUNCTION CONVERT (NUMBER : TIME_INTEGER) RETURN STRING IS
               STR : STRING (1..2) := (OTHERS => '0');
               DEC_DIGIT : CONSTANT STRING := "0123456789";
               NUM : TIME_INTEGER := NUMBER;
          BEGIN
               IF NUM = 0 THEN
                    RETURN STR;
               ELSE
                    NUM := NUM MOD 100;
                    STR (2) := DEC_DIGIT (INTEGER (NUM MOD 10 + 1));
                    NUM := NUM / 10;
                    STR (1) := DEC_DIGIT (INTEGER (NUM + 1));
                    RETURN STR;
               END IF;
          END CONVERT;
     BEGIN
          TIME_NOW := CALENDAR.CLOCK;
          SPLIT (TIME_NOW, YEAR_NUMBER (YEAR), MONTH_NUMBER (MONTH),
                  DAY_NUMBER (DAY), DAY_DURATION (SECOND));
          HOUR := SECOND / 3600;
          SECOND := SECOND MOD 3600;
          MINUTE := SECOND / 60;
          SECOND := SECOND MOD 60;
          RETURN (CONVERT (TIME_INTEGER (YEAR)) & "-" &
                  CONVERT (TIME_INTEGER (MONTH)) & "-" &
                  CONVERT (TIME_INTEGER (DAY)) & " " &
                  CONVERT (TIME_INTEGER (HOUR)) & ":" &
                  CONVERT (TIME_INTEGER (MINUTE)) & ":" &
                  CONVERT (TIME_INTEGER (SECOND)));
     END TIME_STAMP;

     PROCEDURE TEST (NAME : STRING; DESCR : STRING) IS
     BEGIN
          TEST_STATUS := PASS;
          IF NAME'LENGTH <= MAX_NAME_LEN THEN
               TEST_NAME_LEN := NAME'LENGTH;
          ELSE TEST_NAME_LEN := MAX_NAME_LEN;
          END IF;
          TEST_NAME (1..TEST_NAME_LEN) :=
                    NAME (NAME'FIRST .. NAME'FIRST+TEST_NAME_LEN-1);

          PUT_MSG ("");
          PUT_MSG (",.,. " & TEST_NAME (1..TEST_NAME_LEN) & " " &
                   "ACVC " & ACVC_VERSION & " " & TIME_STAMP);
          PUT_MSG ("---- " & TEST_NAME (1..TEST_NAME_LEN) & " " &
                   DESCR & ".");
     END TEST;

     PROCEDURE COMMENT (DESCR : STRING) IS
     BEGIN
          PUT_MSG ("   - " & TEST_NAME (1..TEST_NAME_LEN) & " " &
                   DESCR & ".");
     END COMMENT;

     PROCEDURE FAILED (DESCR : STRING) IS
     BEGIN
          TEST_STATUS := FAIL;
          PUT_MSG ("   * " & TEST_NAME (1..TEST_NAME_LEN) & " " &
                   DESCR & ".");
     END FAILED;

     PROCEDURE NOT_APPLICABLE (DESCR : STRING) IS
     BEGIN
          IF TEST_STATUS = PASS OR TEST_STATUS = ACTION_REQUIRED THEN
               TEST_STATUS := DOES_NOT_APPLY;
          END IF;
          PUT_MSG ("   + " & TEST_NAME (1..TEST_NAME_LEN) & " " &
                   DESCR & ".");
     END NOT_APPLICABLE;

     PROCEDURE SPECIAL_ACTION (DESCR : STRING) IS
     BEGIN
          IF TEST_STATUS = PASS THEN
               TEST_STATUS := ACTION_REQUIRED;
          END IF;
          PUT_MSG ("   ! " & TEST_NAME (1..TEST_NAME_LEN) & " " &
                   DESCR & ".");
     END SPECIAL_ACTION;

     PROCEDURE RESULT IS
     BEGIN
          CASE TEST_STATUS IS
          WHEN PASS =>
               PUT_MSG ("==== " & TEST_NAME (1..TEST_NAME_LEN) &
                        " PASSED ============================.");
          WHEN DOES_NOT_APPLY =>
               PUT_MSG ("++++ " & TEST_NAME (1..TEST_NAME_LEN) &
                        " NOT-APPLICABLE ++++++++++++++++++++.");
          WHEN ACTION_REQUIRED =>
               PUT_MSG ("!!!! " & TEST_NAME (1..TEST_NAME_LEN) &
                        " TENTATIVELY PASSED !!!!!!!!!!!!!!!!.");
               --PUT_MSG ("!!!! " & (1..TEST_NAME_LEN => ' ') &
                        --" SEE '!' COMMENTS FOR SPECIAL NOTES!!");
          WHEN OTHERS =>
               PUT_MSG ("**** " & TEST_NAME (1..TEST_NAME_LEN) &
                        " FAILED ****************************.");
          END CASE;
          TEST_STATUS := FAIL;
          TEST_NAME_LEN := NO_NAME'LENGTH;
          TEST_NAME (1..TEST_NAME_LEN) := NO_NAME;
     END RESULT;

FUNCTION IDENT_INT (X : INTEGER) RETURN INTEGER IS
     BEGIN
          RETURN X;
     END IDENT_INT;

     FUNCTION IDENT_CHAR (X : CHARACTER) RETURN CHARACTER IS
     BEGIN
          RETURN X;
     END IDENT_CHAR;

     FUNCTION IDENT_BOOL (X : BOOLEAN) RETURN BOOLEAN IS
     BEGIN
          RETURN X;
     END IDENT_BOOL;

     FUNCTION IDENT_STR (X : STRING) RETURN STRING IS
     BEGIN
          RETURN X;
     END IDENT_STR;

     FUNCTION EQUAL (X, Y : INTEGER) RETURN BOOLEAN IS
     BEGIN
          RETURN X = Y;
     END EQUAL;

     FUNCTION LEGAL_FILE_NAME (X : FILE_NUM := 1;
                               NAM : STRING := "")
                              RETURN STRING IS
          SUFFIX : STRING (2..6);
     BEGIN
          IF NAM = "" THEN
               SUFFIX := TEST_NAME(3..7);
          ELSE
               SUFFIX := NAM(3..7);
          END IF;

          CASE X IS
               WHEN 1 => RETURN ('X' & SUFFIX);
               WHEN 2 => RETURN ('Y' & SUFFIX);
               WHEN 3 => RETURN ('Z' & SUFFIX);
          END CASE;
     END LEGAL_FILE_NAME;

BEGIN

     TEST_NAME_LEN := NO_NAME'LENGTH;
     TEST_NAME (1..TEST_NAME_LEN) := NO_NAME;

END REPORT;
