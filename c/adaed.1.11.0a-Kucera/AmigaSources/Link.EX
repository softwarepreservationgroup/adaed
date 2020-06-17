gcc -o adalib adalib.o misc.o libf.o -lm >lib.lm
gcc -o adacomp adacomp.o adamrg.o misc.o -lm >comp.lm
gcc -o adaprs action.o acttoks.o adalex.o adaprs.o adared.o debug.o errs.o follow.o libf.o lookup.o makecorr.o misc.o pnodes.o ppredef.o prserr.o prsinit.o prsutil.o pspans.o recover.o reduce.o shift.o -lm >prs.lm
gcc -o adasem 0a.o 0b.o 2.o 3a.o 3b.o 4a.o 4b.o 4c.o 5.o 6.o 7.o 8.o 9.o 10.o 11.o 12a.o 12b.o 12c.o 13.o 14.o arith.o astread.o dbx.o dclmap.o errmsg.o eval.o lib.o libf.o libr.o libw.o machine.o main.o misc.o nodes.o set.o smisc.o sspans.o units.o util.o vars.o -lm >sem.lm
gcc -o adagen 12b.o aggr.o arith.o axqr.o axqw.o dbg.o dbx.o dclmap.o decl.o expand.o expand2.o expr.o g0a.o gen.o glib.o  gmain.o gmisc.o gnodes.o gpredef.o gutil.o gvars.o init.o initobj.o lang.o lib.o libf.o libr.o libw.o maincase.o misc.o nam.o opdesc.o pack.o peep.o proc.o read.o segment.o sep.o set.o smisc.o stat.o type.o util.o vars.o -lm >gen.lm
gcc -o adabind 12b.o arith.o axqr.o axqw.o blib.o bmisc.o bmain.o bnodes.o dbg.o dbx.o dclmap.o g0a.o gen.o ginter.o glib.o gpredef.o gutil.o gvars.o init.o lang.o lib.o libf.o libr.o libw.o misc.o opdesc.o peep.o read.o segment.o sep.o set.o smisc.o util.o vars.o -lm >bind.lm
gcc -o adaexec axqr.o farith.o ilist.o imain.o imisc.o intb.o intc.o ipar.o ivars.o libf.o machine.o  misc.o opname.o predef1.o predef2.o predef3.o predef4.o predef5.o predef6.o tasking.o inta.o load.o -lm >int.lm
gcc:compilers/amiga/2.3.3/ld -r -x load_interface.o axqr.o farith.o ilist.o imain.o imisc.o intb.o intc.o ipar.o ivars.o libf.o machine.o  misc.o opname.o predef1.o predef2.o predef3.o predef4.o predef5.o predef6.o tasking.o inta_interface.o
delete adaint
rename a.out adaint
