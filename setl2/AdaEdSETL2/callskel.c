/*\
 *  Skeleton Callout
 *  ================
 *
 *  This is a skeleton of a callout handler in C.  There is just enough
 *  here for me to test the callout facility.
\*/

#define BSD

/* standard C header files */

#if MIPS_VARARGS
#include <varargs.h>                   /* variable argument macros          */
#else
#include <stdarg.h>                    /* variable argument macros          */
#endif
#include <stdio.h>                     /* standard I/O library              */

/* SETL2 provides these functions */

#if MIPS_VARARGS
char *setl2_callback();                /* call back to SETL2                */
void abend();                          /* abort program                     */
#else
char *setl2_callback(char *, ...);     /* call back to SETL2                */
void abend(char *, ...);               /* abort program                     */
#endif

#ifdef BSD
#include <sys/time.h>
#include <sys/param.h>
#else
#include <time.h>
#endif

char converted_string[20];
char pathname[500];

char *setl2_callout(
   int service,                        /* service selector                  */
   int argc,                           /* length of argument vector         */
   char **argv)                        /* argument vector                   */

{
char *_convstring(long);
char *getdirname();
long cumtime();

   switch (service) {

      case 1:

         /* set the clock for the cumtime function */

	    reset_clock();
            setl2_callback("reset_clock", NULL);
            return "done with reset clock";

      case 2:

         /* get the cumulative time in milleseconds since execution*/

            setl2_callback("cumtime", _convstring(cumtime()), NULL);
            return "done with cumtime";

      case 3:

         /* get the full pathname of the current working directoty*/

            setl2_callback("getdirname", getdirname(), NULL);
            return "done with getdirname";
      default:

         abend("Invalid service to callout => %d\n",service);

   }
}
char * _convstring(num)
long num;
{
sprintf(converted_string, "%ld", num);
return converted_string;
}

/* Procedures reset_clock and cumtime are used to maintain the 'clock'.
 * reset_clock resets the clock, cumtime returns the number of milliseconds
 * since the clock was reset. 
 */

long start_time;	/* starting time (units vary according to version) */
long time_now;

/* set TIME_KIND to determine which cumtime and reset_clock to use */

#define TIME_DEFAULT

#ifdef BSD
#undef TIME_DEFAULT
#define TIME_BSD
/* start_time is in seconds for BSD */
static int tzpa[2];
static long tpa[2];
#endif

reset_clock()
{
/* set start_time as described above. */
#ifdef TIME_BSD
    struct timeval *tp;
    tp = (struct timeval *)tpa;
    gettimeofday(tp,tzpa);
    start_time = tp->tv_sec;
#endif
#ifdef TIME_DEFAULT
    start_time = time(0);
#endif
}

long cumtime()
{
    /* find elapsed seconds, convert to milliseconds */
    long  itim;
#ifdef TIME_DEFAULT
    return (time(0) - start_time) * 1000;
#endif

#ifdef TIME_BSD
    struct timeval *tp;
#endif
#ifdef TIME_BSD
    tp = (struct timeval *)tpa;
    gettimeofday(tp,tzpa);
    itim = ((tp->tv_sec - start_time)*1000L + tp->tv_usec/1000L);
#endif
    return itim ;
}

char *getdirname()
{
  char *getwd();
  getwd(pathname);
  return pathname;
}
