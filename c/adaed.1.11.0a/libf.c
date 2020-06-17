/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

/* libf - auxiliary procedures for reading in IFILE format files.
 * This is subset of procedures needed to read files generated in
 * library format without the need all the library primitives.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "ifile.h"
#include "miscprots.h"
#include "libfprots.h"
#ifndef TRUE
#define TRUE 1
#endif

#ifdef DEBUG
#define IOT
#endif

/* iot_opt_number is set to give file numbers for traces*/
static int iot_opt_number = TRUE;
static int iot_opt_desc = TRUE;
static int iot_off_trace = TRUE; /* set to trace file positions */

void iot_off_info(int lev)									/*;iot_off_info*/
{
	iot_off_trace = lev;
}

int getint(IFILE *ifile, char *desc)							/*;getint*/
{
	/* read int  from input file */
	int si;
	int n = 0;
	int	nr;

#ifdef IOT
	if (ifile->fh_trace == 2) iot_info(ifile, desc);
#endif
#ifdef HI_LEVEL_IO
	nr = fread((char *) &si, sizeof(int), 1, ifile->fh_file);
	if (nr != 1) {
#else
	nr = read(ifile->fh_file, (char *) &si, sizeof(int));
	if (nr != sizeof(int)) {
#endif
		chaos("libr.c: read_ais - unable to read int value");
	}
	n = si;
#ifdef IOT
	if (ifile->fh_trace == 2) printf(" (int) %ld\n", n);
#endif
	return n;
}

int getnum(IFILE *ifile, char *desc)								/*;getnum*/
{
	/* read integer (only 2 bytes) from input file */

	short si;
	int n = 0;

#ifdef IOT
	if (ifile->fh_trace == 2) iot_info(ifile, desc);
#endif
#ifdef HI_LEVEL_IO
	fread((char *) &si, sizeof(short), 1, ifile->fh_file);
#else
	read(ifile->fh_file, (char *) &si, sizeof(short));
#endif
	n = si;
#ifdef IOT
	if (ifile->fh_trace == 2) printf(" %d\n", n);
#endif
	return n;
}

int getchr(IFILE *ifile, char *desc)								/*;getchr*/
{
	/* This is variant of getnum used when reading character values */
	/* read integer (only 2 bytes) from input file */

	short si;
	int n = 0;

#ifdef IOT
	if (ifile->fh_trace == 2) iot_info(ifile, desc);
#endif
#ifdef HI_LEVEL_IO
	fread((char *) &si, sizeof(short), 1, ifile->fh_file);
#else
	read(ifile->fh_file, (char *) &si, sizeof(short));
#endif
	n = si;
#ifdef IOT
	if (ifile->fh_trace == 2) printf(" %d %c\n", n, (char)n);
#endif
	return n;
}

long getlong(IFILE *ifile, char *desc)							/*;getlong*/
{
	/* read long  from input file */

	long si;
	long n = 0;
	int	nr;

#ifdef IOT
	if (ifile->fh_trace == 2) iot_info(ifile, desc);
#endif

#ifdef HI_LEVEL_IO
	nr = fread((char *) &si, sizeof(long), 1, ifile->fh_file);
	if (nr != 1) {
#else
	nr = read(ifile->fh_file, (char *) &si, sizeof(long));
	if (nr != sizeof(long)) {
#endif
		chaos("libr.c: read_ais - unable to read long value");
	}
	n = si;
#ifdef IOT
	if (ifile->fh_trace == 2) printf(" (long) %ld\n", n);
#endif
	return n;
}

char *getstr(IFILE *ifile, char *desc)							/*;getstr*/
{
	char	*s, *p;
	int		n, i;

#ifdef IOT
	if (ifile->fh_trace == 1) printf("getstr(ifile, )\n");
	if (ifile->fh_trace == 2) iot_info(ifile, "str");
#endif
	n = getnum(ifile, "");
	if (n == 0) return (char *)0;
	s = (char *) smalloc((unsigned) n);
	p = s;
	for (i = 1; i < n; i++) {
#ifdef HI_LEVEL_IO
		*p++ = getc(ifile->fh_file);
#else
		read(ifile->fh_file, p++, 1);
#endif
	}
	*p = '\0'; /* set end of string*/

#ifdef IOT
	if (ifile->fh_trace == 2) printf("%s\n", s);
#endif
	return s;
}

#ifdef IOT
void iot_info(IFILE *ifile, char *s)							/*;iot_info*/
{
	long pos;

	if (ifile->fh_trace<2) return;
	if (strlen(s) == 0) return; /* skip if null string */
	pos = iftell(ifile);
	if (iot_opt_desc) printf("%s ", s);
	if (iot_opt_number) printf("%d", ifile->fh_number);
	printf("%c", ifile->fh_type);
	if (iot_off_trace) printf("=%ld", pos);
	printf(" ");
}
#endif

long read_init(IFILE *ifile)								/*;read_init*/
{
	/* initialize read, position at start of first record, return
	 * offset of next record. return 0 if no first first record.
	 * read first word in file that may have offset to slot info.
	 */

	long  pos, start;
	int   nr;

	if (ifile->fh_trace>1) start = iftell(ifile);
#ifdef HI_LEVEL_IO
	nr = fread((char *) &pos, sizeof(long), 1, ifile->fh_file);
	if (nr != 1) {
#else
	nr = read(ifile->fh_file, (char *) &pos, sizeof(long));
	if (nr != sizeof(long)) {
#endif
		/* temporary fix: permit read_init to fail for stub files ds 11-15*/
		if (ifile->fh_type == 's') return 0;
		chaos("read_init read failed ");
		return 0;
	}
#ifdef IOT
	if (ifile->fh_trace>1) printf("read_init at %ld got %ld\n", start, pos);
#endif
	return pos;
}

long read_next(IFILE *ifile, long p)						/*;read_next*/
{
	int	 nr;
	long  pos;

	ifseek(ifile, "next-unit", p, 0);
	if (p == ifile->fh_units_end) {
#ifdef IOT
		if (ifile->fh_trace) printf("read_next units_end reached %ld\n", p);
#endif
		return 0; /* if at end */
	}
#ifdef HI_LEVEL_IO
	nr = fread((char *) &pos, sizeof(long), 1, ifile->fh_file);
	if (nr != 1) {
#else
	nr = read(ifile->fh_file, (char *) &pos, sizeof(long));
	if (nr != sizeof(long)) {
#endif
		chaos("read_next read failure");
		return 0;
	}
	return pos;
}

void iot_set_opt_number(int val)					/*;iot_set_opt_number*/
{
	/* set status of fnumber flag that when on causes internal file number
	 * to be include in trace output
	 */
	iot_opt_number = val;
}

void iot_set_opt_desc(int val)						/*;iot_set_opt_desc*/
{
	/* set status of fnumber flag that when on causes internal file number
	 * to be include in trace output
	 */
	iot_opt_desc = val;
}

void putnum(IFILE *ofile, char *desc, int n)						/*;putnum*/
{
	/* write integer (as a short) to output file */

	short s;
	s = n;

#ifdef IOT
	if (ofile->fh_trace > 1) {
		iot_info(ofile, desc);
		printf(" %d\n", n);
	}
#endif
#ifdef HI_LEVEL_IO
	fwrite((char *) &s, sizeof(short), 1, ofile->fh_file);
#else
	write(ofile->fh_file, (char *) &s, sizeof(short));
#endif
}

void putpos(IFILE *ofile, char *desc, int n)					/*;putpos*/
{
	/* like putnum, but verifies that argument positive */
	/* write integer (as a short) to output file */

	if (n < 0) chaos("putpos: negative argument");
	putnum(ofile, desc, n);
}

void putstr(IFILE *ofile, char *desc, char *s)					/*;putstr*/
{
	if (s == (char *)0) {
#ifdef IOT
		if (ofile->fh_trace>1) iot_info(ofile, desc);
#endif
		putnum(ofile, "", 0);
	}
	else {
#ifdef IOT
		if (ofile->fh_trace>1) iot_info(ofile, desc);
#endif
		putnum(ofile, "", strlen(s)+1);
#ifdef IOT
		if (ofile->fh_trace>1) printf("%s\n", s);
#endif
#ifdef HI_LEVEL_IO
		fputs(s, ofile->fh_file);
#else
		write(ofile->fh_file, s, strlen(s));
#endif
	}
}

void putchr(IFILE *ofile, char *desc, int n)					/*;putchr*/
{
	/* variant of putnum used when writing character value */
	/* write integer (as a short) to output file */

	short s = n;

#ifdef IOT
	if (ofile->fh_trace>1) {
		iot_info(ofile, desc);
		printf(" %d %c\n", n, n);
	}
#endif
#ifdef HI_LEVEL_IO
	fwrite((char *) &s, sizeof(short), 1, ofile->fh_file);
#else
	write(ofile->fh_file, (char *) &s, sizeof(short));
#endif
}
