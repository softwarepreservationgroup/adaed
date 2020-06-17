/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#include "hdr.h"
#include "vars.h"
#include "miscprots.h"
#include "chapprots.h"
#include "smiscprots.h"
#include "errmsgprots.h"

static char *strings[5];

static char *insert(char *, int);

#ifdef TBSN
	unit := FIND_UNIT(node);	
	 find unit in which node was created.
	[u_name, file] := unit;
	if (file ? AISFILE) /= AISFILE then	 $ node from previous compunit
		header := "in " + u_name(1) + " " + u_name(2);
		header +:= "", in file "" + file + " :";
		ERR_LIST([p_s, 12, msg+header]);
	else	$ node in current compilation unit
		ERR_LIST([p_s, 12, msg]);
	end if;
#endif

void errmsg_id(char *msg, Symbol name, char *lrm, Node node)	/*;errmsg_id*/
{
	strings[0] = ORIG_NAME(name);
	errmsg(insert(msg, 1), lrm, node);
}

void errmsg_str(char *msg, char *str, char *lrm, Node node)		/*;errmsg_str*/
{
	strings[0] = str;
	errmsg(insert(msg, 1), lrm, node);
}

void errmsg_nat(char *msg, Symbol sym, char *lrm, Node node)	/*;errmsg_nat*/
{
	strings[0] = nature_str(NATURE(sym));
	errmsg(insert(msg, 1), lrm, node);
}

void errmsg_type(char *msg, Symbol type, char *lrm, Node node)	/*;errmsg_type*/
{
	strings[0] = full_type_name(type);
	errmsg(insert(msg, 1), lrm, node);
}

void errmsg_nval(char *msg, Node name, char *lrm, Node node)	/*;errmsg_nval*/
{
	strings[0] = N_VAL(name);
	errmsg(insert(msg, 1), lrm, node);
}

void errmsg_id_id(char *msg, Symbol name1, Symbol name2, char *lrm, Node node)
															/*;errmsg_id_id*/
{
	strings[0]     = ORIG_NAME(name1);
	strings[1] = ORIG_NAME(name2);
	errmsg(insert(msg, 2), lrm, node);
}

void errmsg_id_type(char *msg, Symbol name, Symbol type, char *lrm, Node node)
															/*;errmsg_id_type*/
{
	strings[0]     = ORIG_NAME(name);
	strings[1] = full_type_name(type);
	errmsg(insert(msg, 2), lrm, node);
}

void errmsg_nat_id_str(char *msg, Symbol sym, Symbol name, char *str, char *lrm,
  Node node)											/*;errmsg_nat_id_str*/
{
	char *name_str;

	strings[0] = nature_str(NATURE(sym));
	name_str = ORIG_NAME(name);
	if (name_str[0] == '#') name_str = "#BLOCK";
	strings[1] = name_str;
	strings[2] = str;

	errmsg(insert(msg, 3), lrm, node);
}

void errmsg_str_id(char *msg, char *str, Symbol name, char *lrm, Node node)
															/*;errmsg_str_id*/
{
	strings[0]     = str;
	strings[1] = ORIG_NAME(name);
	errmsg(insert(msg, 2), lrm, node);
}

void errmsg_str_num(char *msg, char *str, int i, char *lrm, Node node)
														/*;errmsg_str_num*/
{
	char numstr[5];

	strings[0] = str;
	sprintf(numstr, "%d", i);
	strings[1] = numstr;

	errmsg(insert(msg, 2), lrm, node);
}

static char *insert(char *in_format, int nstrings)				/*;insert*/
{
	/*  -in_format- is a character string containing an error message, and 1 or
	 *  more "substitution" (%) characters to be replaced by the character
	 *  strings pointed to by the array -strings-
	 *  -tmp_format- is a working copy to be used by this procedure (to avoid
	 *  "clobbering" string constants).
	 */

	char *msg, *p;
	char *tmp_format;
	int	i;

	/* copy input format string */
	tmp_format = emalloct((unsigned) strlen(in_format)+1, "errmsg-tmp");
	strcpy(tmp_format, in_format);

	/* initialize msg to empty string */
	msg = emalloct(1, "errmsg-1"); 
	*msg = '\0';

	for (i = 0; i < nstrings; i++) {
		p = strchr(tmp_format, '%');
		if (p == 0) break;
		*p = '\0';
		if (p != tmp_format)
			msg = strjoin(msg, tmp_format);
		msg = strjoin(msg, strings[i]);
		tmp_format = ++p;
	}
	if (tmp_format != '\0' )
		msg = strjoin(msg, tmp_format);
	if (p == 0) {
		printf("error in proc insert, too few %c's\n", '%');
#ifdef TBSN
		while (*strings != DUMMY_STR)
			msg = strjoin(msg, *strings++);
#endif
	}
	return(msg);
}

/*
 * Following are variations of pass1_error that call the appropriate
 * errmsg_ routine.
 * The original (simple case) pass1_error is still in 4c.c
 */

void pass1_error_id(char *msg, Symbol name, char *lrm_sec, Node node)
														/*;pass1_error_id */
{
	/* This procedure is invoked when a type error which requires a special
	 * message is encountered in resolve1.
	 */

	if (cdebug2 > 3) TO_ERRFILE("AT PROC :  pass1_error_id");

	if (!noop_error) errmsg_id(msg, name, lrm_sec, node);
	noop_error = TRUE;	/* To avoid cascaded errors.*/
}

void pass1_error_str(char *msg, char *str, char *lrm_sec, Node node)
														/*;pass1_error_str */
{
	/* This procedure is invoked when a type error which requires a special
	 * message is encountered in resolve1.
	 */

	if (cdebug2 > 3) TO_ERRFILE("AT PROC :  pass1_error");

	if (!noop_error) errmsg_str(msg, str, lrm_sec, node);
	noop_error = TRUE;	/* To avoid cascaded errors.*/
}

void pass1_error_l(char *msg1, char *msg2, char *lrm_sec, Node node)
														/*;pass1_error_l */
{
	/* This procedure is invoked when a type error which requires a special
	 * message is encountered in resolve1.
	 */

	if (cdebug2 > 3) TO_ERRFILE("AT PROC :  pass1_error_l");

	if (!noop_error) errmsg_l(msg1, msg2, lrm_sec, node);
	noop_error = TRUE;	/* To avoid cascaded errors.*/
}

char *build_full_names(Set symbols)					/*;build_full_names */
{
	/* builds a string containing the full names (scope.name) of all Symbols
	 * in the set 'symbols`
	 */

	Symbol sym;
	Forset fs;
	char   *name, *name_string;

	/* TBSL: this should be improved to free extra storage !! */

	name_string = strjoin("","");
	if (symbols == (Set)0) return(name_string);
	FORSET(sym = (Symbol), symbols, fs);
		name = ORIG_NAME(SCOPE_OF(sym));
		/* skip internally generated block names */
		if (name[0] == '#')
			name = "#BLOCK.";
		else
			name = strjoin(name, ".");
		name = strjoin(name, ORIG_NAME(sym));
		name = strjoin(name, " ");
		name_string = strjoin(name_string, name);
	ENDFORSET(fs);
	return(name_string);
}

#ifdef ERRNUM

char *phrases[] = {
	"",						/*   0 */
	"9.16",						/*   1 */
	"label hidden by inner declaration",		/*   2 */
	"5.1",						/*   3 */
	"missing full declaration for %",		/*   4 */
	"3.8.1",					/*   5 */
	"Name of separately compiled unit cannot be ",	/*   6 */
	"an operator designator",			/*   7 */
	"10.1",						/*   8 */
	"System error: invalid node %",			/*   9 */
	"none",						/*  10 */
	"Invalid compilation unit",			/*  11 */
	"% does not appear in previous with clause",	/*  12 */
	"10.1.1",					/*  13 */
	"Unknown unit in with clause: %",		/*  14 */
	"specification and stub for % are in different scopes", /*  15 */
	"7.1, 9.1",					/*  16 */
	"Matching specification not found for stub %",	/*  17 */
	"Subunit identifier not unique",		/*  18 */
	"10.2",						/*  19 */
	"stubs can only appear in the outermost scope of a ", /*	 20 */
	"compilation unit",				/*  21 */
	"cannot find stub for subunit %",		/*  22 */
	"% is not an exception",			/*  23 */
	"11.1",						/*  24 */
	"Duplicate exception name in handler",		/*  25 */
	"11.2",						/*  26 */
	"Duplicate OTHERS in exception part",		/*  27 */
	"RAISE statement not directly in exception handler", /*	28 */
	"11.3",						/*  29 */
	"Invalid exception name",			/*  30 */
	" parameter not allowed for functions",		/*  31 */
	"6.5",						/*  32 */
	"Invalid use of limited type % for out parameter ", /*  33 */
	"7.4.4",					/*  34 */
	"Separately compiled generics not supported",	/*  35 */
	"Premature use of incomplete or private type %", /*  36 */
	"7.4.1",					/*  37 */
	"Type of a generic formal object of mode IN must not", /*  38 */
	" be a limited type",				/*  39 */
	"12.1.1",					/*  40 */
	"Deferred constant cannot be default expression", /*  41 */
	" for a generic parameter",			/*  42 */
	"7.4.3",					/*  43 */
	"Initialization not allowed for IN OUT generic parameters", /*  44 */
	"OUT generic formals objects not allowed",	/*  45 */
	"Premature usage of type % before its full declaration", /*  46 */
	"generic private type cannot have defaults for discriminants", /*  47 */
	"12.1.2",					/*  48 */
	"invalid reference to %",			/*  49 */
	"8.3(16)",					/*  50 */
	"not a generic ",				/*  51 */
	"12.1, 12.3",					/*  52 */
	"Initializations not allowed for operators",	/*  53 */
	"6.7",						/*  54 */
	"not a generic package",			/*  55 */
	"12.1",						/*  56 */
	"Recursive instantiation not allowed",		/*  57 */
	"12.3",						/*  58 */
	"Invalid recursive instantiation",		/*  59 */
	"Too many actuals in generic instantiation",	/*  60 */
	"Positional association after named one",	/*  61 */
	"named associations not allowed for overloaded names", /*  62 */
	"12.3(3)",					/*  63 */
	"Missing instantiation for generic parameter %", /*  64 */
	"duplicate or erroneous named associations in instantiation", /*	 65 */
	"% is not an operator designator",		/*  66 */
	"4.5",						/*  67 */
	"Instantiation of a generic in parameter cannot be a ", /*  68 */
	" deferred constant",				/*  69 */
	"Instantiation of generic in out parameter must be a variable", /*  70 */
	"12.1.1, 12.3.1",				/*  71 */
	"Instantiation of generic in out parameter ",	/*  72 */
	"cannot be a conversion",			/*  73 */
	"12.3.1",					/*  74 */
	"must be a variable",				/*  75 */
	"instantiation of generic in out parameter % depends on a ", /*	76 */
	"discriminant",					/*  77 */
	"invalid expression for instantiation of %",	/*  78 */
	"Invalid type for instantiation of %",		/*  79 */
	"12.3.2 - 12.3.5",				/*  80 */
	"Invalid use of incomplete type in instantiation of %", /*  81 */
	"Invalid use of private type in instantiation of %", /*	82 */
	"Expect non-limited type to instantiate %",	/*  83 */
	"12.3.2",					/*  84 */
	"discriminant mismatch in instantiation of %",	/*  85 */
	"Instantiation of % must be unconstrained",	/*  86 */
	"Usage of private type % requires instantiation with", /*  87 */
	" constrained type",				/*  88 */
	"expect access to % to instantiate %",		/*  89 */
	"12.3.3",					/*  90 */
	"formal and actual designated types must be both ", /*  91 */
	"constrained or unconstrained",			/*  92 */
	"Expect access type to instantiate %",		/*  93 */
	"12.3.5",					/*  94 */
	"Expect array type to instantiate %",		/*  95 */
	"12.3.4",					/*  96 */
	"Expect constrained array type to instantiate %", /*  97 */
	"Expect unconstrained array type to instantiate %", /*  98 */
	"Dimensions of actual type do not match those of %", /*	99 */
	"index or component type mismatch in instantiation", /* 100 */
	" of array type %",				/* 101 */
	"formal and actual array component type must be both ", /* 102 */
	"invalid match for generic subprogram %",	/* 103 */
	"12.3.6",					/* 104 */
	"Expression in size spec is not static",	/* 105 */
	"13.2",						/* 106 */
	"Prefix of attribute is not type or first named subtype", /* 107 */
	"Prefix of attribute is not task type or access type", /* 108 */
	"expect fixed type in representation clause for SMALL", /* 109 */
	"13.2(11)",					/* 110 */
	"expression for SMALL must be static",		/* 111 */
	"Identifier is not an enumeration type",	/* 112 */
	"13.3",						/* 113 */
	"Integer code is not distinct or violates ",	/* 114 */
	"predefined ordering relation of type",		/* 115 */
	"Component of aggregate in enumeration representation clause", /* 116 */
	"is not static",				/* 117 */
	"Identifier is not a record type",		/* 118 */
	"13.4",						/* 119 */
	"Alignment clause must contain a static expression", /* 120 */
	"Component % does not appear in record type",	/* 121 */
	"Component % already occurs in clause",		/* 122 */
	"Expression for component % must be static",	/* 123 */
	"Range for component % must be static",		/* 124 */
	"Invalid expression for range constraint",	/* 125 */
	"3.3",						/* 126 */
	"RANGE attribute has wrong type for constraint", /* 127 */
	"invalid use of 'RANGE in expression",		/* 128 */
	"Format error in pragma",			/* 129 */
	"Appendices B,F",				/* 130 */
	"Unconstrained % in object declaration",	/* 131 */
	"3.6.1, 3.7.2",					/* 132 */
	"Missing initialization in constant declaration", /* 133 */
	"3.2",						/* 134 */
	"Wrong scope for type of deferred constant",	/* 135 */
	"7.4",						/* 136 */
	"Invalid context for deferred constant",	/* 137 */
	"3.2, 7.4",					/* 138 */
	"constants of a generic type cannot be deferred", /* 139 */
	"a deferred constant must be defined with a type mark", /* 140 */
	"% is not a deferred constant",			/* 141 */
	"Invalid redeclaration of %",			/* 142 */
	"8.3",						/* 143 */
	"incorrect type in redeclaration of %",		/* 144 */
	"7.4, 7.4.1",					/* 145 */
	"Missing initialization in redeclaration of %", /* 146 */
	"Initialization not available for entities of limited type", /* 147 */
	"Expect literal expression in number declaration", /* 148 */
	"Invalid use of discriminants",			/* 149 */
	"3.7.1",					/* 150 */
	"missing discriminants in full type declaration", /* 151 */
	"3.8",						/* 152 */
	"Incomplete type definition must be completed ", /* 153 */
	" in the same scope in which it first appears", /* 154 */
	"Invalid context for redeclaration of private type", /* 155 */
	"Generic private type % cannot have declaration ", /* 156 */
	"in private part",				/* 157 */
	"Invalid use of type % before its full declaration", /* 158 */
	"invalid use of type % within its definition or body", /* 159 */
	"3.3,9.1",					/* 160 */
	"Invalid constraint on access type",		/* 161 */
	"Invalid subtype indication: type is already constrained", /* 162 */
	"Invalid type mark in subtype indication: %",	/* 163 */
	"3.3, 3.6.1",					/* 164 */
	"premature derivation of derived or private type %", /* 165 */
	"3.4, 7.4.1",					/* 166 */
	"discriminant mismatch in declaration",		/* 167 */
	"cannot obtain derived type from %",		/* 168 */
	"3.4",						/* 169 */
	"Bounds in an integer type definition must be static", /* 170 */
	"3.5.4",					/* 171 */
	"Bounds in an integer type definition must be of some ", /* 172 */
	"integer type",					/* 173 */
	"Expect static expression for digits",		/* 174 */
	"3.5.7",					/* 175 */
	"Expect integer expression for DIGITS",		/* 176 */
	"Invalid digits value in real type declaration", /* 177 */
	"Precision not supported by implementation",	/* 178 */
	"Expect static expression for delta",		/* 179 */
	"Expression for delta must be of some real type", /* 180 */
	"3.5.9",					/* 181 */
	"Missing range in Fixed type declaration",	/* 182 */
	"Bound in range constraint of type definition must be static", /* 183 */
	"3.5.7, 3.5.9",					/* 184 */
	"Invalid RANGE constraint for type",		/* 185 */
	"Invalid constraint for type",			/* 186 */
	"accurracy constraint cannot depend on a generic type", /* 187 */
	"value for DIGITS must be positive",		/* 188 */
	"value of DELTA must be positive",		/* 189 */
	"expect static expression for DIGITS or DELTA", /* 190 */
	"3.5.7,3.5.9",					/* 191 */
	"Invalid constraint for scalar type",		/* 192 */
	"3.3.2",					/* 193 */
	"Constraints apply to all indices or none",	/* 194 */
	"3.6.1",					/* 195 */
	"Unconstrained element type in array declaration", /* 196 */
	"Array type is already constrained",		/* 197 */
	"Incorrect no. of index constraints for type %", /* 198 */
	"Invalid expression for index definition",	/* 199 */
	"expect discrete type in discrete range",	/* 200 */
	"Invalid index constraint for %",		/* 201 */
	"Incomplete specification of default values for discriminants", /* 202 */
	"Discriminant must have discrete type",		/* 203 */
	"non conformance to previous declaration",	/* 204 */
	"6.3.1",					/* 205 */
	"Invalid self-reference in definition of %",	/* 206 */
	"3.1",						/* 207 */
	"Unconstrained % in component declaration",	/* 208 */
	"Invalid type for constraint",			/* 209 */
	"3.3, 3.7.2",					/* 210 */
	"Invalid constraint: Record type has no discriminant", /* 211 */
	"3.7.1, 3.7.2",					/* 212 */
	"Positional associations after named ones",	/* 213 */
	"3.7.2",					/* 214 */
	"Too many constraints for record type",		/* 215 */
	"Expect discriminant names only in discriminant", /* 216 */
	" constraint",					/* 217 */
	"Invalid discriminant name in discriminant constraint", /* 218 */
	"3.7. 3.7.2",					/* 219 */
	"Duplicate constraint for discriminant %",	/* 220 */
	"discriminants in named association must have same type", /* 221 */
	"3.7.2(4)",					/* 222 */
	"Missing constraints for discriminants",	/* 223 */
	"a discriminant appearing in a subtype indication ", /* 224 */
	"must appear by itself",			/* 225 */
	"Invalid discriminant name in variant part",	/* 226 */
	"3.7.1, 3.7.3",					/* 227 */
	"invalid redeclaration of % in private part",	/* 228 */
	"8.2(b)",					/* 229 */
	"invalid redeclaration of %",			/* 230 */
	"3.8, 8.2",					/* 231 */
	"Invalid argument for attribute SIZE",		/* 232 */
	"Annex A",					/* 233 */
	"bounds of range for membership op must be scalar", /* 234 */
	"4.4",						/* 235 */
	"Invalid use of attribute BASE",		/* 236 */
	"ambiguous entry name for attribute",		/* 237 */
	"9.9",						/* 238 */
	"% has incorrect type. Expect %",		/* 239 */
	"invalid reading of out parameter %",		/* 240 */
	"6.2",						/* 241 */
	"premature use of deferred constant before its", /* 242 */
	"full declaration",				/* 243 */
	"invalid context for integer literal",		/* 244 */
	"4.6(15)",					/* 245 */
	"invalid context for real literal",		/* 246 */
	"Incorrect type for string literal. Expect %",	/* 247 */
	"Invalid context for NULL",			/* 248 */
	"3.8.2",					/* 249 */
	"No aggregate available for type %",		/* 250 */
	"4.2",						/* 251 */
	"Context of allocator must be an access type",	/* 252 */
	"4.8, 3.8",					/* 253 */
	"Invalid type for allocator. Expect %",		/* 254 */
	"3.8, 4.8",					/* 255 */
	"ambiguous expression for conversion",		/* 256 */
	"4.6",						/* 257 */
	"Invalid array conversion",			/* 258 */
	"cannot convert to %",				/* 259 */
	"Invalid universal expression in",		/* 260 */
	" discrete range",				/* 261 */
	"Incorrect type for expression. Expect %",	/* 262 */
	"Second argument of VAL must be of some integer type", /* 263 */
	"ambiguous argument for attribute VAL",		/* 264 */
	"attribute must apply to selected component",	/* 265 */
	"13.7.2",					/* 266 */
	"Missing explicit conversion from universal fixed value", /* 267 */
	"3.5.9, 4.5.5",					/* 268 */
	"system error: strange op type %",		/* 269 */
	"Missing explicit conversion from universal_real value ", /* 270 */
	"4.5.6",					/* 271 */
	"Invalid context for mixed mode operation",	/* 272 */
	"4.5.5, 4.10",					/* 273 */
	"Missing explicit conversion from ",		/* 274 */
	"universal_fixed value ",			/* 275 */
	"4.5.5",					/* 276 */
	"% not available on a limited type",		/* 277 */
	"7.4.2",					/* 278 */
	"No positional arguments can appear after named ones", /* 279 */
	"6.4",						/* 280 */
	"Invalid format for argument association",	/* 281 */
	"% actual parameter no. % in call is not a variable", /* 282 */
	"6.4.1",					/* 283 */
	"A null range in array aggregate must be the only choice", /* 284 */
	"4.3.2.(3)",					/* 285 */
	"Component is not allowed to be specified more than once", /* 286 */
	"4.3.(6)",					/* 287 */
	"Missing association in array aggregate",	/* 288 */
	"aggregates not available for limited type %",	/* 289 */
	"Invalid context for aggregate",		/* 290 */
	"OTHERS choice not allowed in this context",	/* 291 */
	"4.3.2",					/* 292 */
	"invalid type mark in array aggregate",		/* 293 */
	"4.3",						/* 294 */
	"Non static choice in array aggregate must be the only choice", /* 295 */
	"OTHERS must be the last aggregate component",	/* 296 */
	"In a positional aggregate only named association ", /* 297 */
	"allowed is OTHERS",				/* 298 */
	"Invalid context for OTHERS and named associations", /* 299 */
	"4.3.2(6)",					/* 300 */
	"Invalid use of literal in aggregate",		/* 301 */
	"Expect aggregate for component of multidimensional aggregate", /* 302 */
	"characters in a string literal must be directly visible", /* 303 */
	"4.2(3)",					/* 304 */
	"Component type of context is not a character type", /* 305 */
	"OTHERS must appear alone and last in a choice list", /* 306 */
	"No value supplied for discriminant %",		/* 307 */
	"4.3.1",					/* 308 */
	"Value for discriminant % must be static",	/* 309 */
	"Undefined component name",			/* 310 */
	"Duplicate value for component in aggregate",	/* 311 */
	"choice in record aggregate must be selector name", /* 312 */
	"Range choice not allowed in record aggregate", /* 313 */
	"OTHERS choice must represent at least ",	/* 314 */
	"one component",				/* 315 */
	"OTHERS expression incompatible with %",	/* 316 */
	"components on a choice list must have same type", /* 317 */
	"Too many components for record aggregate",	/* 318 */
	"No value supplied for component %",		/* 319 */
	"expect task name ",				/* 320 */
	"9.5",						/* 321 */
	"invalid use of task type outside of its own body", /* 322 */
	"9.1",						/* 323 */
	"use of SYSTEM.ADDRESS requires presence of package SYSTEM", /* 324 */
	"13.7.2, Annex A",				/* 325 */
	"Undefined attribute: %",			/* 326 */
	"Ambiguous call to one of %",			/* 327 */
	"6.6, 8.3",					/* 328 */
	"Ambiguous operands for %",			/* 329 */
	"6.7, 8.3",					/* 330 */
	"Ambiguous literal: %",				/* 331 */
	"3.5.1, 4.7, 8.3",				/* 332 */
	"ambiguous expression",				/* 333 */
	"8.2, 8.3",					/* 334 */
	"TEXT_IO not instantiated nor defined for type", /* 335 */
	"8.4, 14.4",					/* 336 */
	"invalid types for %",				/* 337 */
	"invalid argument list for %",			/* 338 */
	"no instance of % has type %",			/* 339 */
	"3.5.1",					/* 340 */
	"Expect expression to yield type %",		/* 341 */
	"Unexpected range in slice",			/* 342 */
	"",						/* 343 */
	"assignment not available on a limited type",	/* 344 */
	"incompatible types for assignment",		/* 345 */
	"5.2",						/* 346 */
	"ambiguous types for assigment",		/* 347 */
	"left-hand side in assignment is not a variable", /* 348 */
	"Case expression not of discrete type",		/* 349 */
	"3.7.3, 5.4",					/* 350 */
	"Case expression cannot be of a generic type",	/* 351 */
	"5.4",						/* 352 */
	"Choice must have type %",			/* 353 */
	"Case choice not static",			/* 354 */
	"choice value(s) not in range of static ",	/* 355 */
	"subtype of case expression",			/* 356 */
	"Duplicate choice value(s)",			/* 357 */
	"Missing OTHERS choice",			/* 358 */
	"EXIT statement not in loop",			/* 359 */
	"5.7",						/* 360 */
	"Invalid loop label in EXIT: %",		/* 361 */
	"5.5, 5.7",					/* 362 */
	"attempt to exit from %",			/* 363 */
	"invalid context for RETURN statement",		/* 364 */
	"5.8",						/* 365 */
	"Procedure cannot return value",		/* 366 */
	"Function must return value",			/* 367 */
	"Duplicate identifier for label",		/* 368 */
	"target of goto is not a label",		/* 369 */
	"5.9",						/* 370 */
	"target of goto is not a reachable label",	/* 371 */
	"attempt to jump out of %",			/* 372 */
	"Incorrect no. of arguments for operator %",	/* 373 */
	"Invalid argument profile for \"=\"",		/* 374 */
	" /=	 cannot be given an explicit definition",  /* 375 */
	"library subprograms cannot be overloaded",	/* 376 */
	"10.1(10)",					/* 377 */
	"Missing RETURN statement in function body",	/* 378 */
	"Invalid use of incomplete type %",		/* 379 */
	"default initialization only allowed for IN parameters", /* 380 */
	"6.1",						/* 381 */
	"functions cannot have % parameters ",		/* 382 */
	"Declaration does not match previous specification", /* 383 */
	"Invalid statement: not procedure or entry call", /* 384 */
	"Invalid procedure or entry call",		/* 385 */
	"6.5, 9.5",					/* 386 */
	"Invalid statement",				/* 387 */
	"Invalid call",					/* 388 */
	"Redeclaration of identifier %",		/* 389 */
	"8.3, 8.4",					/* 390 */
	"invalid declaration of homograph %",		/* 391 */
	"8.3(17)",					/* 392 */
	"= can only be defined for limited types",	/* 393 */
	"Specification and body are in different scopes", /* 394 */
	"Matching specification not found for body %",	/* 395 */
	"Invalid context for private declaration",	/* 396 */
	"7.4, 12.1.2",					/* 397 */
	"Invalid redeclaration ",			/* 398 */
	"8.2",						/* 399 */
	"invalid use of type % before its full declaration", /* 400 */
	"3.8.1, 7.4.1",					/* 401 */
	"Private type requires full declaration with non limited type", /* 402 */
	"Private type cannot be fully declared as an unconstrained", /* 403 */
	" array type",					/* 404 */
	"Private type without discriminants cannot be given ", /* 405 */
	"full declaration with discriminants",		/* 406 */
	"A private type with discriminants must be given ", /* 407 */
	"full declaration with a discriminated type",	/* 408 */
	"Use of type for an OUT parameter requires full ", /* 409 */
	"declaration  with non limited type",		/* 410 */
	"recursive definition of private type %",	/* 411 */
	"7.2",						/* 412 */
	"Missing full declaration in private part for %", /* 413 */
	"Redeclaration of % in private part",		/* 414 */
	"7.2 , 7.4.1",					/* 415 */
	"Missing body for % %.%",			/* 416 */
	"7.3",						/* 417 */
	"Missing full type declaration for incomplete type %", /* 418 */
	"Cannot find package specification for %",	/* 419 */
	"% is not supported in current implementation", /* 420 */
	"Invalid type mark ",				/* 421 */
	"identifier undeclared or not visible %",	/* 422 */
	"Invalid reference to %",			/* 423 */
	"Ambiguous identifier. Could be one of: %",	/* 424 */
	"premature usage of %",				/* 425 */
	"% not declared in %",				/* 426 */
	"4.1.3, 8.3",					/* 427 */
	"Invalid prefix in qualified name",		/* 428 */
	"4.1.3",					/* 429 */
	"Ambiguous name in selected component",		/* 430 */
	"duplicate identifier: %",			/* 431 */
	"Expect identifier in renaming",		/* 432 */
	"8.5",						/* 433 */
	"not an exception",				/* 434 */
	"not a package",				/* 435 */
	"renaming with = can only rename an equality operator", /* 436 */
	"invalid renaming",				/* 437 */
	"function spec. does not match attribute",	/* 438 */
	"8.5,12.3.6",					/* 439 */
	"attribute cannot be renamed as function",	/* 440 */
	"existence of object % depends on a discriminant ", /* 441 */
	"ambiguous subprogram name: %",			/* 442 */
	"No match for subprogam specification ",	/* 443 */
	"parameter modes do not match",			/* 444 */
	"8.5(8)",					/* 445 */
	"ambiguous or invalid entry name in renaming",	/* 446 */
	"invalid index on entry name (not entry family)", /* 447 */
	"Cannot rename entry family as a whole",	/* 448 */
	"Renamed entity must be an object",		/* 449 */
	"Duplicate declaration of %",			/* 450 */
	"undeclared package name %",			/* 451 */
	"8.4, 10.1",					/* 452 */
	"% is not the name of a USEable package",	/* 453 */
	"8.4",						/* 454 */
	"Accept statements can only appear in tasks",	/* 455 */
	"Undefined entry name in ACCEPT ",		/* 456 */
	"invalid index on entry (not entry family)",	/* 457 */
	"Entry name in ACCEPT statement does not match any entry", /* 458 */
	"Missing index for entry family.",		/* 459 */
	"Invalid entry name in ACCEPT",			/* 460 */
	"An accept_statement cannot appear within an ACCEPT for", /* 461 */
	" the same entry",				/* 462 */
	"Call to member of entry family requires one index", /* 463 */
	"9.5, 3.6.1",					/* 464 */
	"invalid index. % is not an entry family",	/* 465 */
	"context requires entry name ",			/* 466 */
	"9.7.2, 7.3",					/* 467 */
	"Invalid entry name",				/* 468 */
	"Missing index in name of member of entry family", /* 469 */
	"Member of entry family requires a single index ", /* 470 */
	"Undefined entry name in task : ",		/* 471 */
	"ambiguous entry family name: %",		/* 472 */
	"Invalid context for TERMINATE alternative",	/* 473 */
	"9.7.1",					/* 474 */
	" expect task name is ABORT statement",		/* 475 */
	"9.10",						/* 476 */
	"Invalid task type in ABORT statement",		/* 477 */
	"attribute cannot be applied to unconstrained array type", /* 478 */
	"3.6.2",					/* 479 */
	""};

/* errmsg(text, text, node) -> errmsgn(enum, enum, node) */
void errmsgn(int en, int lrm, Node node)						/*;errmsgn*/
{
	errmsg(phrases[en], phrases[lrm], node);
}

void id_errmsgn(int msg, Symbol name, int lrm, Node node)		/*;id_errmsgn*/
{
	errmsg_id(phrases[msg], name, phrases[lrm], node);
}

void id_id_errmsgn(int msg, Symbol name1, Symbol name2, int lrm, Node node)
															/*;id_id_errmsgn*/
{
	errmsg_id_id(phrases[msg], name1, name2, phrases[lrm], node);
}

void id_type_errmsgn(int msg, Symbol name, Symbol type, int lrm, Node node)
														/*;id_type_errmsgn*/
{
	errmsg_id_type(phrases[msg], name, type, phrases[lrm], node);
}

void l_errmsgn(int msg1, int msg2, int lrm, Node node)			/*;l_errmsgn*/
{
	char *strjoin();
	errmsg(strjoin(phrases[msg1], phrases[msg2]), phrases[lrm], node);
}

void l1_errmsgn(char *msg1, int msg2, int lrm, Node node)		/*;l1_errmsgn*/
{
	char *strjoin();
	errmsg(strjoin(msg1, phrases[msg2]), phrases[lrm], node);
}

void l2_errmsgn(int msg1, char *msg2, int lrm, Node node)		/*;l2_errmsgn*/
{
	char *strjoin();
	errmsg(strjoin(phrases[msg1], msg2), phrases[lrm], node);
}

void l_id_errmsgn(int msg1, int msg2, Symbol name, int lrm, Node node)
															/*;l_id_errmsgn*/
{
	char *strjoin();
	errmsg_id(strjoin(phrases[msg1], phrases[msg2]), name, phrases[lrm], node);
}

void l_str_errmsgn(int msg1, int msg2, char *str, int lrm, Node node)
															/*;l_str_errmsgn*/
{
	errmsg_str(strjoin(phrases[msg1], phrases[msg2]), str, phrases[lrm], node);
}

void nat_errmsgn(int msg, Symbol sym, int lrm, Node node)		/*;nat_errmsgn*/
{
	errmsg_nat(phrases[msg], sym, phrases[lrm], node);
}

void nat_id_str_errmsgn(int msg, Symbol sym, Symbol name, char *str, int lrm,
  Node node)											/*;nat_id_str_errmsgn*/
{
	errmsg_nat_id_str(phrases[msg], sym, name, str, phrases[lrm], node);
}

void nval_errmsgn(int msg, Node name, int lrm, Node node)	/*;nval_errmsgn*/
{
	errmsg_nval(phrases[msg], name, phrases[lrm], node);
}

void str_errmsgn(int msg, char *str, int lrm, Node node)		/*;str_errmsgn*/
{
	errmsg_str(phrases[msg], str, phrases[lrm], node);
}

void str_id_errmsgn(int msg, char *str, Symbol name, int lrm, Node node)
															/*;str_id_errmsgn*/
{
	errmsg_str_id(phrases[msg], str, name, phrases[lrm], node);
}

void str_num_errmsgn(int msg, char *str, int i, int lrm, Node node)
															/*;str_num_errmsgn*/
{
	errmsg_str_num(phrases[msg], str, i, phrases[lrm], node);
}

void type_errmsgn(int msg, Symbol type, int lrm, Node node)	/*;type_errmsgn*/
{
	errmsg_type(phrases[msg], type, phrases[lrm], node);
}

void type1_errmsgn(char *msg, Symbol type, int lrm, Node node)
															/*;type1_errmsgn*/
{
	errmsg_type(msg, type, phrases[lrm], node);
}
#endif
