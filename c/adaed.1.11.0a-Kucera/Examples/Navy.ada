--I haven't the slightest idea what this really does, but at least
--you can test some of the error handling with it.

--From schweige@taurus.cs.nps.navy.mil Sun Mar  1 21:06:35 1992
--Return-Path: <schweige@taurus.cs.nps.navy.mil>
--Date: Sun, 1 Mar 92 18:06:03 PST
--From: schweige@taurus.cs.nps.navy.mil (Jeffrey M. Schweiger)
--To: black@beno.CSS.GOV
--Subject: Ada program
--Status: R
--
--Mike -
--
--The following Ada program does not have any graceful error handling
--built in, so improper data entry will cause it to crash.
--
--Jeff Schweiger
with TEXT_IO;
use TEXT_IO;

procedure MOE is

    type V_ARRAY is array (1 .. 5) of FLOAT;
    S, M :  V_ARRAY;
    ST, MT : FLOAT;

    package INT_IO is new INTEGER_IO(INTEGER);
    package FLOAT_INOUT is new FLOAT_IO(FLOAT);
    use INT_IO, FLOAT_INOUT;


    procedure GET_INPUT (S, M : out V_ARRAY) is


    begin

        for I in 1 .. 5 loop
            PUT("Enter S[");
            PUT(I, width => 1);
            PUT("]:  ");
            GET(S(I));
            SKIP_LINE;
            NEW_LINE;
        end loop;

            NEW_LINE(2);

        for I in 1 .. 5 loop
            PUT("Enter M[");
            PUT(I, width => 1);
            PUT("]:  ");
            GET(M(I));
            SKIP_LINE;
            NEW_LINE;
        end loop;

    end GET_INPUT;

begin

   GET_INPUT(S, M);
   ST := (S(1)* 0.2) + (S(2)* 0.15) + (S(3)* 0.1) + (S(4)* 0.25) + (S(5)* 0.3);
   MT := (M(1)* 0.25) + (M(2)* 0.1) + (M(3)* 0.15) + (M(4)* 0.2) + (M(5)* 0.3);
   PUT("Tactical MOE is ");
   PUT((ST/MT), FORE => 2, AFT =>4, EXP => 0);
   NEW_LINE(2);
   PUT("Normalized Tactical MOE is ");
   PUT((ST/(ST + MT)), FORE => 2, AFT =>4, EXP => 0);
   NEW_LINE(2);


end MOE;

