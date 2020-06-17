/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
#include "ada.h"

int N_DEFINED[] = {
    N_D_AST1 | N_D_AST2,                        /*   0 pragma */
    N_D_AST1 | N_D_AST2,                        /*   1 arg */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*   2 obj_decl */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*   3 const_decl */
    N_D_AST1 | N_D_AST2,                        /*   4 num_decl */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_TYPE,  /*   5 type_decl */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /*   6 subtype_decl */
    N_D_AST1 | N_D_AST2 | N_D_UNQ,              /*   7 subtype_indic */
    N_D_AST1,                                   /*   8 derived_type */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*   9 range */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_TYPE,  /*  10 range_attribute */
    N_D_AST1 | N_D_LIST,                        /*  11 constraint */
    N_D_AST1 | N_D_LIST,                        /*  12 enum */
    N_D_AST1,                                   /*  13 int_type */
    N_D_AST1,                                   /*  14 float_type */
    N_D_AST1,                                   /*  15 fixed_type */
    N_D_AST1 | N_D_AST2,                        /*  16 digits */
    N_D_AST1 | N_D_AST2,                        /*  17 delta */
    N_D_AST1 | N_D_AST2 | N_D_UNQ,              /*  18 array_type */
    N_D_AST1 | N_D_UNQ,                         /*  19 box */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /*  20 subtype */
    N_D_AST1,                                   /*  21 record */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  22 component_list */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  23 field */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  24 discr_spec */
    N_D_AST1 | N_D_AST2,                        /*  25 variant_decl */
    N_D_AST1 | N_D_AST2,                        /*  26 variant_choices */
    N_D_AST1 | N_D_VAL,                         /*  27 string */
    N_D_AST1 | N_D_AST2,                        /*  28 simple_choice */
    N_D_AST1,                                   /*  29 range_choice */
    N_D_AST1,                                   /*  30 choice_unresolved */
    N_D_AST1 | N_D_AST2,                        /*  31 others_choice */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  32 access_type */
    N_D_AST1,                                   /*  33 incomplete_decl */
    N_D_AST1 | N_D_LIST,                        /*  34 declarations */
    N_D_AST1 | N_D_LIST,                        /*  35 labels */
    N_D_AST1 | N_D_VAL | N_D_TYPE,              /*  36 character_literal */
    N_D_AST1 | N_D_VAL | N_D_UNQ | N_D_TYPE,    /*  37 simple_name */
    N_D_AST1 | N_D_AST2,                        /*  38 call_unresolved */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  39 selector */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /*  40 all */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_TYPE,  /*  41 attribute */
    N_D_AST1 | N_D_LIST | N_D_TYPE,             /*  42 aggregate */
    N_D_AST1 | N_D_TYPE,                        /*  43 parenthesis */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  44 choice_list */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /*  45 op */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  46 in */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  47 notin */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /*  48 un_op */
    N_D_AST1 | N_D_VAL | N_D_TYPE,              /*  49 int_literal */
    N_D_AST1 | N_D_VAL | N_D_TYPE,              /*  50 real_literal */
    N_D_AST1 | N_D_VAL | N_D_TYPE,              /*  51 string_literal */
    N_D_AST1 | N_D_TYPE,                        /*  52 null */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /*  53 name */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  54 qualify */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  55 new_init */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  56 new */
    N_D_AST1 | N_D_AST2,                        /*  57 statements */
    N_D_AST1 | N_D_AST2,                        /*  58 statement */
    N_D_AST1,                                   /*  59 null_s */
    N_D_AST1 | N_D_AST2,                        /*  60 assignment */
    N_D_AST1 | N_D_AST2,                        /*  61 if */
    N_D_AST1 | N_D_AST2,                        /*  62 cond_statements */
    N_D_AST1,                                   /*  63 condition */
    N_D_AST1 | N_D_AST2,                        /*  64 case */
    N_D_AST1 | N_D_AST2,                        /*  65 case_statements */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  66 loop */
    N_D_AST1,                                   /*  67 while */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  68 for */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  69 forrev */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /*  70 block */
    N_D_AST1 | N_D_AST2 | N_D_UNQ,              /*  71 exit */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  72 return */
    N_D_AST1,                                   /*  73 goto */
    N_D_AST1,                                   /*  74 subprogram_decl */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  75 procedure */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  76 function */
    N_D_AST1 | N_D_VAL | N_D_UNQ | N_D_TYPE,    /*  77 operator */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /*  78 formal */
    N_D_AST1 | N_D_VAL,                         /*  79 mode */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /*  80 subprogram */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  81 call */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  82 package_spec */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /*  83 package_body */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  84 private_decl */
    N_D_AST1 | N_D_LIST,                        /*  85 use */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /*  86 rename_obj */
    N_D_AST1 | N_D_AST2,                        /*  87 rename_ex */
    N_D_AST1 | N_D_AST2,                        /*  88 rename_pack */
    N_D_AST1 | N_D_AST2,                        /*  89 rename_sub */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /*  90 task_spec */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_TYPE,  /*  91 task_type_spec */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /*  92 task */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /*  93 entry */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_TYPE,  /*  94 entry_family */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /*  95 accept */
    N_D_AST1,                                   /*  96 delay */
    N_D_AST1 | N_D_AST2,                        /*  97 selective_wait */
    N_D_AST1 | N_D_AST2,                        /*  98 guard */
    N_D_AST1 | N_D_AST2,                        /*  99 accept_alt */
    N_D_AST1 | N_D_AST2,                        /* 100 delay_alt */
    N_D_AST1 | N_D_VAL,                         /* 101 terminate_alt */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 102 conditional_entry_call */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 103 timed_entry_call */
    N_D_AST1 | N_D_LIST,                        /* 104 abort */
    N_D_AST1 | N_D_AST2,                        /* 105 unit */
    N_D_AST1 | N_D_LIST,                        /* 106 with_use_list */
    N_D_AST1 | N_D_LIST,                        /* 107 with */
    N_D_AST1 | N_D_VAL,                         /* 108 subprogram_stub */
    N_D_AST1 | N_D_VAL | N_D_UNQ,               /* 109 package_stub */
    N_D_AST1 | N_D_VAL | N_D_UNQ,               /* 110 task_stub */
    N_D_AST1 | N_D_AST2,                        /* 111 separate */
    N_D_AST1 | N_D_LIST,                        /* 112 exception */
    N_D_AST1 | N_D_LIST,                        /* 113 except_decl */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 114 handler */
    N_D_AST1,                                   /* 115 others */
    N_D_AST1 | N_D_TYPE,                        /* 116 raise */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 117 generic_function */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 118 generic_procedure */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 119 generic_package */
    N_D_AST1 | N_D_LIST,                        /* 120 generic_formals */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 121 generic_obj */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 122 generic_type */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 123 gen_priv_type */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 124 generic_subp */
    N_D_AST1,                                   /* 125 generic */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 126 package_instance */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 127 function_instance */
    N_D_AST1 | N_D_AST2 | N_D_AST3 | N_D_AST4,  /* 128 procedure_instance */
    N_D_AST1 | N_D_AST2,                        /* 129 instance */
    N_D_AST1 | N_D_AST2,                        /* 130 length_clause */
    N_D_AST1 | N_D_AST2,                        /* 131 enum_rep_clause */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 132 rec_rep_clause */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 133 compon_clause */
    N_D_AST1,                                   /* 134 address_clause */
    N_D_AST1,                                   /* 135 any_op */
    N_D_AST1,                                   /* 136 opt */
    N_D_AST1 | N_D_LIST,                        /* 137 list */
    N_D_AST1 | N_D_UNQ,                         /* 138 range_expression */
    N_D_AST1 | N_D_LIST,                        /* 139 arg_assoc_list */
    N_D_AST1,                                   /* 140 private */
    N_D_AST1,                                   /* 141 limited_private */
    N_D_AST1,                                   /* 142 code */
    N_D_AST1 | N_D_VAL,                         /* 143 line_no */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /* 144 index */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /* 145 slice */
    N_D_AST1 | N_D_VAL,                         /* 146 number */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /* 147 convert */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 148 entry_name */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /* 149 array_aggregate */
    N_D_AST1 | N_D_AST2 | N_D_UNQ | N_D_TYPE,   /* 150 record_aggregate */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /* 151 ecall */
    N_D_AST1 | N_D_AST2 | N_D_TYPE,             /* 152 call_or_index */
    N_D_AST1 | N_D_VAL | N_D_TYPE,              /* 153 ivalue */
    N_D_AST1 | N_D_TYPE,                        /* 154 qual_range */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /* 155 qual_index */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /* 156 qual_discr */
    N_D_AST1,                                   /* 157 qual_arange */
    N_D_AST1,                                   /* 158 qual_alength */
    N_D_AST1 | N_D_TYPE,                        /* 159 qual_adiscr */
    N_D_AST1 | N_D_TYPE,                        /* 160 qual_aindex */
    N_D_AST1 | N_D_AST2,                        /* 161 check_bounds */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /* 162 discr_ref */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /* 163 row */
    N_D_AST1 | N_D_UNQ,                         /* 164 current_task */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 165 check_discr */
    N_D_AST1,                                   /* 166 end */
    N_D_AST1 | N_D_VAL,                         /* 167 terminate */
    N_D_AST1,                                   /* 168 exception_accept */
    N_D_AST1,                                   /* 169 test_exception */
    N_D_AST1 | N_D_TYPE,                        /* 170 create_task */
    N_D_AST1 | N_D_AST2 | N_D_UNQ,              /* 171 predef */
    N_D_AST1,                                   /* 172 deleted */
    N_D_AST1 | N_D_LIST | N_D_TYPE,             /* 173 insert */
    N_D_AST1,                                   /* 174 arg_convert */
    N_D_AST1 | N_D_VAL,                         /* 175 end_activation */
    N_D_AST1,                                   /* 176 activate_spec */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 177 delayed_type */
    N_D_AST1 | N_D_UNQ | N_D_TYPE,              /* 178 qual_sub */
    N_D_AST1 | N_D_AST2,                        /* 179 static_comp */
    N_D_AST1 | N_D_AST2,                        /* 180 array_ivalue */
    N_D_AST1 | N_D_AST2,                        /* 181 record_ivalue */
    N_D_AST1,                                   /* 182 expanded */
    N_D_AST1,                                   /* 183 choices */
    N_D_AST1 | N_D_AST2,                        /* 184 init_call */
    N_D_AST1 | N_D_AST2,                        /* 185 type_and_value */
    N_D_AST1,                                   /* 186 discard */
    N_D_AST1,                                   /* 187 unread */
    N_D_AST1 | N_D_VAL | N_D_TYPE,              /* 188 string_ivalue */
    N_D_AST1 | N_D_VAL,                         /* 189 instance_tuple */
    N_D_AST1 | N_D_AST2 | N_D_AST3,             /* 190 entry_family_name */
    N_D_AST1,                                   /* 191 astend */
    N_D_AST1,                                   /* 192 astnull */
    N_D_AST1 | N_D_AST2,                        /* 193 aggregate_list */
    N_D_AST1 | N_D_UNQ,                         /* 194 interfaced */
0};
