WITH Calendar;USE  Calendar;PACKAGE BODY Random IS-- Body of random number generator package.-- Adapted from the Ada literature by-- Michael B. Feldman, The George Washington University, November 1990.  Modulus      : CONSTANT := 9317;  TYPE Int_16 IS RANGE - 2 ** 15 .. 2 ** 15 - 1;  TYPE Int_32 IS RANGE - 2 ** 31 .. 2 ** 31 - 1;  SUBTYPE Seed_Range IS Int_16 RANGE 0 .. (Modulus - 1);  Seed,  Default_Seed : Seed_Range;  PROCEDURE Set_Seed (N : Positive) IS  BEGIN    Seed := Seed_Range (N);  END Set_Seed;  FUNCTION  Unit_Random RETURN Float IS    Multiplier : CONSTANT := 421;    Increment  : CONSTANT := 2073;    Result     : Float;  BEGIN    Seed := (Multiplier * Seed + Increment) MOD Modulus;    Result := Float (Seed) / Float (Modulus);    RETURN Result;  EXCEPTION    WHEN Constraint_Error | Numeric_Error =>      Seed := Int_16 ((Multiplier * Int_32 (Seed) + Increment) MOD Modulus);      Result := Float (Seed) / Float (Modulus);      RETURN Result;  END Unit_Random;  FUNCTION  Random_Int (N : Positive) RETURN Positive IS    Result : Integer RANGE 1 .. N;  BEGIN    Result := Integer (Float (N) * Unit_Random + 0.5);    RETURN Result;  EXCEPTION    WHEN Constraint_Error | Numeric_Error =>      RETURN 1;  END Random_Int;BEGIN  Default_Seed := Int_16 (Int_32 (Seconds (Clock)) MOD Modulus);  Seed := Default_Seed;END Random;