--  Filename: MATHSTUB.ADA
  package MATHSTUB is
    function  SIN(X: in FLOAT) return FLOAT;
    function  COS(X: in FLOAT) return FLOAT;
    function ATAN(X: in FLOAT) return FLOAT;
    function  LOG(X: in FLOAT) return FLOAT;
    function SQRT(X: in FLOAT) return FLOAT;
    function  EXP(X: in FLOAT) return FLOAT;
  end MATHSTUB;
  package body MATHSTUB is
    function  SIN (X: in FLOAT) return FLOAT is
                  begin return 1.0; end SIN;
    function  COS (X: in FLOAT) return FLOAT is
                  begin return 1.0; end COS;
    function ATAN (X: in FLOAT) return FLOAT is
                  begin return 1.0; end ATAN;
    function  LOG (X: in FLOAT) return FLOAT is
                  begin return 1.0; end LOG;
    function SQRT (X: in FLOAT) return FLOAT is 
                  begin return 1.0; end SQRT;
    function  EXP (X: in FLOAT) return FLOAT is
                  begin return 1.0; end EXP;
  end MATHSTUB;

