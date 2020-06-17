/*
    GWAda Development Environment for 386/486 PCs   
    Copyright (C) 1993, Arthur Vargas Lopes  & Michael Bliss Feldman
                        vlopes@vortex.ufrgs.br mfeldman@seas.gwu.edu
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.    

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* Bsearch.c */



int IS_KEYWORD(char *s)
{

	typedef struct {
		int  code;
		char *key;
	} KEY_WORD;

	static KEY_WORD keywords[NO_KEYWORDS] = {
		 {XABORT               ,"ABORT"}
		,{XABS                 ,"ABS"}
		,{XACCEPT              ,"ACCEPT"}
		,{XACCESS              ,"ACCESS"}
		,{XALL                 ,"ALL"}
		,{XAND                 ,"AND"}
		,{XARRAY               ,"ARRAY"}
		,{XAT                  ,"AT"}
		,{XBEGIN               ,"BEGIN"}
		,{XBODY                ,"BODY"}
		,{XCASE                ,"CASE"}
		,{XCONSTANT            ,"CONSTANT"}
		,{XDECLARE             ,"DECLARE"}
		,{XDELAY               ,"DELAY"}
		,{XDELTA               ,"DELTA"}
		,{XDIGITS              ,"DIGITS"}
		,{XDO                  ,"DO"}
		,{XELSE                ,"ELSE"}
		,{XELSIF               ,"ELSIF"}
		,{XEND                 ,"END"}
		,{XENTRY               ,"ENTRY"}
		,{XEXCEPTION           ,"EXCEPTION"}
		,{XEXIT                ,"EXIT"}
		,{XFOR                 ,"FOR"}
		,{XFUNCTION            ,"FUNCTION"}
		,{XGENERIC             ,"GENERIC"}
		,{XGOTO                ,"GOTO"}
		,{XIF                  ,"IF"}
		,{XIN                  ,"IN"}
		,{XIS                  ,"IS"}
		,{XLIMITED             ,"LIMITED"}
		,{XLOOP                ,"LOOP"}
		,{XMOD                 ,"MOD"}
		,{XNEW                 ,"NEW"}
		,{XNOT                 ,"NOT"}
		,{XNULL                ,"NULL"}
		,{XOF                  ,"OF"}
		,{XOR                  ,"OR"}
		,{XOTHERS              ,"OTHERS"}
		,{XOUT                 ,"OUT"}
		,{XPACKAGE             ,"PACKAGE"}
		,{XPRAGMA              ,"PRAGMA"}
		,{XPRIVATE             ,"PRIVATE"}
		,{XPROCEDURE           ,"PROCEDURE"}
		,{XRAISE               ,"RAISE"}
		,{XRANGE               ,"RANGE"}
		,{XRECORD              ,"RECORD"}
		,{XREM                 ,"REM"}
		,{XRENAMES             ,"RENAMES"}
		,{XRETURN              ,"RETURN"}
		,{XREVERSE             ,"REVERSE"}
		,{XSELECT              ,"SELECT"}
		,{XSEPARATE            ,"SEPARATE"}
		,{XSUBTYPE             ,"SUBTYPE"}
		,{XTASK                ,"TASK"}
		,{XTERMINATE           ,"TERMINATE"}
		,{XTHEN                ,"THEN"}
		,{XTYPE                ,"TYPE"}
		,{XUSE                 ,"USE"}
		,{XWHEN                ,"WHEN"}
		,{XWHILE               ,"WHILE"}
		,{XWITH                ,"WITH"}
		,{XXOR                 ,"XOR"}
		};

	int left, high, med, r;
	left = 0;
	high = NO_KEYWORDS - 1;
	while (left <= high)  {
		med = (high + left) / 2;
		if (!(r = strcmp(s,keywords[med].key)))
			return keywords[med].code;
		if (r < 0)
			high = med - 1;
		else
			left = med + 1;
		}
	return 0;
}


int IS_ATT(char *s)
{
	typedef struct {
		int  code;
		char *key;
	} KEY_ATT;

	int left, high, med, r;

	static KEY_ATT keyatts[40] = {
		 {XA_ADDRESS             ,"ADDRESS"}
		,{XA_AFT                 ,"AFT"}
		,{XA_BASE                ,"BASE"}
		,{XA_CALLABLE            ,"CALLABLE"}
		,{XA_CONSTRAINED         ,"CONSTRAINED"}
		,{XA_COUNT               ,"COUNT"}
		,{XA_DELTA               ,"DELTA"}
		,{XA_DIGITS              ,"DIGITS"}
		,{XA_EMAX                ,"EMAX"}
		,{XA_EPSILON             ,"EPSILON"}
		,{XA_FIRST               ,"FIRST"}
		,{XA_FIRST_BIT           ,"FIRST_BIT"}
		,{XA_FORE                ,"FORE"}
		,{XA_IMAGE               ,"IMAGE"}
		,{XA_LARGE               ,"LARGE"}
		,{XA_LAST                ,"LAST"}
		,{XA_LAST_BIT            ,"LAST_BIT"}
		,{XA_LENGTH              ,"LENGTH"}
		,{XA_MACHINE_EMAX        ,"MACHINE_EMAX"}
		,{XA_MACHINE_EMIN        ,"MACHINE_EMIN"}
		,{XA_MACHINE_MANTISSA    ,"MACHINE_MANTISSA"}
		,{XA_MACHINE_OVERFLOWS   ,"MACHINE_OVERFLOWS"}
		,{XA_MACHINE_RADIX       ,"MACHINE_RADIX"}
		,{XA_MACHINE_ROUNDS      ,"MACHINE_ROUNDS"}
		,{XA_MANTISSA            ,"MANTISSA"}
		,{XA_POS                 ,"POS"}
		,{XA_POSITION            ,"POSITION"}
		,{XA_PRED                ,"PRED"}
		,{XA_RANGE               ,"RANGE"}
		,{XA_SAFE_EMAX           ,"SAFE_EMAX"}
		,{XA_SAFE_LARGE          ,"SAFE_LARGE"}
		,{XA_SAFE_SMALL          ,"SAFE_SMALL"}
		,{XA_SIZE                ,"SIZE"}
		,{XA_SMALL               ,"SMALL"}
		,{XA_STORAGE_SIZE        ,"STORAGE_SIZE"}
		,{XA_SUCC                ,"SUCC"}
		,{XA_TERMINATED          ,"TERMINATED"}
		,{XA_VAL                 ,"VAL"}
		,{XA_VALUE               ,"VALUE"}
		,{XA_WIDTH               ,"WIDTH"}
		};

	left = 0;
	high = NO_ATTS - 1;

	while (left <= high)  {
		med = (high + left) / 2;
		if (!(r = strcmp(s,keyatts[med].key)))
			return keyatts[med].code;
		if (r < 0)
			high = med - 1;
		else
			left = med + 1;
		}
	return 0;
}

