 
with CALENDAR;
use  CALENDAR;
package body RANDOM is

-- Body of random number generator package.
-- Adapted from the Ada literature by
-- Michael B. Feldman, The George Washington University, November 1990.

    MODULUS            : constant := 9317;
 
    type    INT_16     is range -2 ** 15 .. 2 ** 15 - 1;
    type    INT_32     is range -2 ** 31 .. 2 ** 31 - 1;
    subtype SEED_RANGE is INT_16 range 0 .. (MODULUS - 1);
    SEED, DEFAULT_SEED : SEED_RANGE;
 
    procedure SET_SEED(N : POSITIVE) is separate;
    function UNIT_RANDOM return FLOAT is separate;
    function RANDOM_INT(N : POSITIVE) return POSITIVE is separate;
begin
    DEFAULT_SEED := INT_16(INT_32(SECONDS(CLOCK)) mod MODULUS);
    SEED := DEFAULT_SEED;
end RANDOM;
 
separate(RANDOM)
procedure SET_SEED(N : POSITIVE) is
begin
    SEED := SEED_RANGE(N);
end SET_SEED;
 
separate(RANDOM)
function UNIT_RANDOM return FLOAT is
    MULTIPLIER : constant := 421;
    INCREMENT  : constant := 2073;
    RESULT     : FLOAT;
begin
    SEED := (MULTIPLIER * SEED + INCREMENT) mod MODULUS;
    RESULT := FLOAT(SEED) / FLOAT(MODULUS);
    return RESULT;
exception
    when CONSTRAINT_ERROR | NUMERIC_ERROR =>
	SEED := INT_16((MULTIPLIER * INT_32(SEED) + INCREMENT) mod MODULUS);
	RESULT := FLOAT(SEED) / FLOAT(MODULUS);
	return RESULT;
end UNIT_RANDOM;
 
separate(RANDOM)
function RANDOM_INT(N : POSITIVE) return POSITIVE is
    RESULT : INTEGER range 1 .. N;
begin
    RESULT := INTEGER(FLOAT(N) * UNIT_RANDOM + 0.5);
    return RESULT;
exception
    when CONSTRAINT_ERROR | NUMERIC_ERROR =>
	return 1;
end RANDOM_INT;
