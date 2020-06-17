       	       	       	     
all : stlx	       	     

callskel.o : callskel.c	     
	gcc -o callskel.o -c -ansi -O -finline-functions callskel.c
		       	     
stlx : callskel.o libstlx.a        			 
	gcc -o stlx callskel.o libstlx.a -lm 


