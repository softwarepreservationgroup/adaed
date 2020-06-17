-- UNIT: procedure MORTGAGE
-- FILES: mortgage.a
-- COMPILE: ada mortgage.a
-- LINK: a.ld -o mortgage mortgage
-- PURPOSE:Ishow use of MATH package (floating point exponentiation)
-- in VERDIXLIB library, and the linking of additional C libraries (-lm)
-- DESCRIPTION: calculates monthly mortgage payments.
--              Usage: mortgage
-- .......................................................................... --

with Text_Io;
use Text_Io;

procedure Mortgage is

    package Fio is new Float_Io (Float);
    use Fio;
    package Iio is new Integer_Io (Integer);
    use Iio;

    Pmt, I, A, V, N, Remp : Float;
    K, Sumi, Sump, Pmti, Pmtp : Float;
    Toti, Totp : Float;
    M, Year : Integer;
    Str : String (1 .. 80);
    Last : Natural;

    function F (I, M : Integer) return Integer is
        Ii : Integer;
    begin
        Ii := (I + M - 1) mod 12;
        if Ii = 0 then
            Ii := 12;
        end if;
        return Ii;
    end F;

    function Make_Float (Str : String) return Float is
        Ff : Float;
        Last : Positive;
    begin
        Get (Str, Ff, Last);
        return Ff;
    exception
        when Data_Error =>
            Get (Str & ".0", Ff, Last);
            return Ff;
    end Make_Float;


begin
    Put ("Enter Amount (f):" & Ascii.Ht);
    Get_Line (Str, Last);
    A := Make_Float (Str (1 .. Last));
    New_Line;
    Put ("Enter rate (f):" & Ascii.Ht);
    Get_Line (Str, Last);
    I := Make_Float (Str (1 .. Last));
    New_Line;
    Put ("Enter # of years (f):" & Ascii.Ht);
    Get_Line (Str, Last);
    N := Make_Float (Str (1 .. Last));
    New_Line;
    Put ("Month to start (i):" & Ascii.Ht);
    Get (M);
    New_Line;
    Put ("Year to start (i):" & Ascii.Ht);
    Get (Year);
    New_Line;
    N := N * 12.0;
    I := I / 1200.0;
    V := 1.0 / (1.0 + I);
    Pmt := I * A / (1.0 - V ** Integer (N));
    New_Line (2);
    Put ("loan amount        = ");
    Put (A, 9, 2, 0);
    New_Line;
    Put ("interest rate      = ");
    Put (I * 1200.0, 9, 2, 0);
    New_Line;
    Put ("number of payments = ");
    Put (Integer (N), 9);
    New_Line;
    Put ("monthly payment    = ");
    Put (Pmt, 9, 2, 0);
    New_Line (3);
    Put ("amortization schedule");
    New_Line (2);
    Put ("   payment    remaining    amount    amount");
    New_Line;
    Put ("  #     date    balance   interest  principal");
    New_Line (2);
    Totp := 0.0;
    Toti := 0.0;
    Sump := 0.0;
    Sumi := 0.0;
    Remp := A;
    for Pmtn in 1 .. Integer (N) loop
        K := Float (Pmtn);
        Pmti := Remp * I;
        Pmtp := Pmt - Pmti;
        Remp := Remp - Pmtp;
        Sumi := Sumi + Pmti;
        Sump := Sump + Pmtp;
        Totp := Totp + Pmtp;
        Toti := Toti + Pmti;
        Put (Pmtn, 3);
        Put (F (Pmtn, M), 4);
        Put ("/");
        Put (Year, 4);
        Put (Remp, 8, 2, 0);
        Put (Pmti, 8, 2, 0);
        Put (Pmtp, 8, 2, 0);
        New_Line;
        if (Pmtn + M) mod 12 = 1 then
            New_Line;
            Put ("year");
            Put (Year, 8);
            Put (Ascii.Ht);
            Put (Sumi, 15, 2, 0);
            Put (Sump, 8, 2, 0);
            New_Line;
            Put ("average");
            Put (Ascii.Ht & Ascii.Ht);
            Put (Sumi / 12.0, 15, 2, 0);
            Put (Sump / 12.0, 8, 2, 0);
            New_Line (2);
            Year := Year + 1;
            Sump := 0.0;
            Sumi := 0.0;
        end if;
    end loop;
    if Sump /= 0.0 then
        New_Line;
        Put ("year" & Ascii.Ht);
        Put (Year, 8);
        Put (Sumi, 8, 2, 0);
        Put (Sump, 8, 2, 0);
        New_Line;
    end if;
    New_Line;
    Put ("total loan payments" & Ascii.Ht);
    Put (Toti, 7, 2, 0);
    Put (Totp, 8, 2, 0);
    New_Line;

end Mortgage;

-- .......................................................................... --
--
-- DISTRIBUTION AND COPYRIGHT:
--
-- This software is released to the Public Domain (note:
--   software released to the Public Domain is not subject
--   to copyright protection).
-- Restrictions on use or distribution:  NONE
--
-- DISCLAIMER:
--
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


