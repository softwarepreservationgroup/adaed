;This batch file likes to crash now and again, it seems (not that I've
;run it that many times, but...) If it does, check for the last
;.o file created, and comment everything before it (best to recompile the
;last one it did for good measure).  Don't give up hope on if it croaks once!
delete errors
echo MAKE SURE STACK IS HIGH ENOUGH (200000)
gcc -c 0a.c -o 0a.o >>errors
gcc -c 0b.c -o 0b.o >>errors
gcc -c 10.c -o 10.o >>errors
gcc -c 11.c -o 11.o >>errors
gcc -c 12a.c -o 12a.o >>errors
gcc -c 12b.c -o 12b.o >>errors
gcc -c 12c.c -o 12c.o >>errors
gcc -c 13.c -o 13.o >>errors
gcc -c 14.c -o 14.o >>errors
gcc -c 2.c -o 2.o >>errors
gcc -c 3a.c -o 3a.o >>errors
gcc -c 3b.c -o 3b.o >>errors
gcc -c 4a.c -o 4a.o >>errors
gcc -c 4b.c -o 4b.o >>errors
gcc -c 4c.c -o 4c.o >>errors
gcc -c 5.c -o 5.o >>errors
gcc -c 6.c -o 6.o >>errors
gcc -c 7.c -o 7.o >>errors
gcc -c 8.c -o 8.o >>errors
gcc -c 9.c -o 9.o >>errors
gcc -c action.c -o action.o >>errors
gcc -c acttoks.c -o acttoks.o >>errors
gcc -c -DAMIGA adacomp.c -o adacomp.o >>errors
gcc -c adalex.c -o adalex.o >>errors
gcc -c adalib.c -o adalib.o >>errors
gcc -c adamrg.c -o adamrg.o >>errors
gcc -c adaprs.c -o adaprs.o >>errors
gcc -c adared.c -o adared.o >>errors
gcc -c aggr.c -o aggr.o >>errors
gcc -c arith.c -o arith.o >>errors
gcc -c astread.c -o astread.o >>errors
gcc -c axqr.c -o axqr.o >>errors
gcc -c axqw.c -o axqw.o >>errors
gcc -c blib.c -o blib.o >>errors
gcc -c bmain.c -o bmain.o >>errors
gcc -c dbg.c -o dbg.o >>errors
gcc -c dbx.c -o dbx.o >>errors
gcc -c dclmap.c -o dclmap.o >>errors
gcc -c debug.c -o debug.o >>errors
gcc -c decl.c -o decl.o >>errors
gcc -c errmsg.c -o errmsg.o >>errors
gcc -c errs.c -o errs.o >>errors
gcc -c eval.c -o eval.o >>errors
gcc -c expand.c -o expand.o >>errors
gcc -c expand2.c -o expand2.o >>errors
gcc -c expr.c -o expr.o >>errors
gcc -c farith.c -o farith.o >>errors
gcc -c follow.c -o follow.o >>errors
gcc -c g0a.c -o g0a.o >>errors
gcc -c gen.c -o gen.o >>errors
gcc -c ginter.c -o ginter.o >>errors
gcc -c glib.c -o glib.o >>errors
gcc -c gmain.c -o gmain.o >>errors
gcc -c gmisc.c -o gmisc.o >>errors
gcc -c gmisc.c -DBINDER -o bmisc.o >>errors
gcc -c gnodes.c -o gnodes.o >>errors
gcc -c gnodes.c -DBINDER -o bnodes.o >>errors
gcc -c gpredef.c -o gpredef.o >>errors
gcc -c gutil.c -o gutil.o >>errors
gcc -c ilist.c -o ilist.o >>errors
gcc -c imain.c -o imain.o >>errors
gcc -c imisc.c -o imisc.o >>errors
gcc -c init.c -o init.o >>errors
gcc -c initobj.c -o initobj.o >>errors
gcc -c inta.c -o inta.o >>errors
gcc -c -DINTERFACE inta.c -o inta_interface.o >>errors
gcc -c intb.c -o intb.o >>errors
gcc -c intc.c -o intc.o >>errors
gcc -c ipar.c -o ipar.o >>errors
gcc -c lang.c -o lang.o >>errors
gcc -c lib.c -o lib.o >>errors
gcc -c libf.c -o libf.o >>errors
gcc -c libr.c -o libr.o >>errors
gcc -c libw.c -o libw.o >>errors
gcc -c load.c -o load.o >>errors
gcc -c -DINTERFACE load.c -o load_interface.o >>errors
gcc -c lookup.c -o lookup.o >>errors
gcc -c machine.c -o machine.o >>errors
gcc -c main.c -o main.o >>errors
gcc -c maincase.c -o maincase.o >>errors
gcc -c makecorr.c -o makecorr.o >>errors
gcc -DSYSTEM_V -DLIBDIR="ADA:lib" -c misc.c -o misc.o >>errors
gcc -c nam.c -o nam.o >>errors
gcc -c nodes.c -o nodes.o >>errors
gcc -c opdesc.c -o opdesc.o >>errors
gcc -c opname.c -o opname.o >>errors
gcc -c pack.c -o pack.o >>errors
gcc -c peep.c -o peep.o >>errors
gcc -c pnodes.c -o pnodes.o >>errors
gcc -c ppredef.c -o ppredef.o >>errors
gcc -c predef1.c -o predef1.o >>errors
gcc -c predef2.c -o predef2.o >>errors
gcc -c predef3.c -o predef3.o >>errors
gcc -c predef4.c -o predef4.o >>errors
gcc -c predef5.c -o predef5.o >>errors
gcc -c predef6.c -o predef6.o >>errors
gcc -c proc.c -o proc.o >>errors
gcc -c prserr.c -o prserr.o >>errors
;The following steps move T: to the hard drive, since even 5 meg ram wasn't
;enough to compile prsinit.c
if NOT exists dh0:T
makedir dh0:T
endif
assign T: dh0:T
gcc -c prsinit.c -o prsinit.o >>errors
assign T: ram:T
gcc -c prsutil.c -o prsutil.o >>errors
gcc -c pspans.c -o pspans.o >>errors
gcc -c read.c -o read.o >>errors
gcc -c recover.c -o recover.o >>errors
gcc -c reduce.c -o reduce.o >>errors
gcc -c segment.c -o segment.o >>errors
gcc -c sep.c -o sep.o >>errors
gcc -c set.c -o set.o >>errors
gcc -c shift.c -o shift.o >>errors
gcc -c smisc.c -o smisc.o >>errors
gcc -c sspans.c -o sspans.o >>errors
gcc -c stat.c -o stat.o >>errors
gcc -c tasking.c -o tasking.o >>errors
gcc -c type.c -o type.o >>errors
gcc -c units.c -o units.o >>errors
gcc -c util.c -o util.o >>errors
gcc -c vars.c -o vars.o >>errors
gcc -c gvars.c -o gvars.o >>errors
gcc -c ivars.c -o ivars.o >>errors
