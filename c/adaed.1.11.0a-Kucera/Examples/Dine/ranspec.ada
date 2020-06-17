package RANDOM is

-- Simple pseudo-random number generator package.
-- Adapated from the Ada literature by
-- Michael B. Feldman, The George Washington University, November 1990.

    procedure SET_SEED(N : POSITIVE);
    function UNIT_RANDOM return FLOAT;
    --returns a float >=0.0 and <1.0
    function RANDOM_INT(N : POSITIVE) return POSITIVE;
    --return a random integer in the range 1..N

end RANDOM;
 
