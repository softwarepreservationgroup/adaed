adacomp -v  -nl demolib  list
adacomp -v  -l demolib  topsort 
adabind -l demolib
adaexec -l demolib <topsort.dat

adacomp -v  -l demolib  concord 
adabind -m concordance -l demolib
adaexec -m concordance -l demolib <concord.dat
