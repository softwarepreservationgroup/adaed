/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */

#include "opnamep.h"

char *op_name(unsigned int opnum)							/*;op_name*/
{
	static char *opnames[] = {
		"nop",                              /* 0 nop */
		"abort",                            /* 1 abort */
		"abs_b",                            /* 2 abs_b */
		"abs_w",                            /* 3 abs_w */
		"abs_l",                            /* 4 abs_l */
		"abs_x",                            /* 5 abs_x */
		"activate",                         /* 6 activate */
		"activate_new_g",                   /* 7 activate_new_g */
		"activate_new_l",                   /* 8 activate_new_l */
		"add_b",                            /* 9 add_b */
		"add_w",                            /* 10 add_w */
		"add_l",                            /* 11 add_l */
		"add_x",                            /* 12 add_x */
		"add_immediate_b",                  /* 13 add_immediate_b */
		"add_immediate_w",                  /* 14 add_immediate_w */
		"add_immediate_l",                  /* 15 add_immediate_l */
		"add_immediate_x",                  /* 16 add_immediate_x */
		"and",                              /* 17 and */
		"allocate",                         /* 18 allocate */
		"allocate_copy_g",                  /* 19 allocate_copy_g */
		"allocate_copy_l",                  /* 20 allocate_copy_l */
		"array_and",                        /* 21 array_and */
		"array_catenate",                   /* 22 array_catenate */
		"array_move",                       /* 23 array_move */
		"array_not",                        /* 24 array_not */
		"array_or",                         /* 25 array_or */
		"array_slice",                      /* 26 array_slice */
		"array_xor",                        /* 27 array_xor */
		"attribute",                        /* 28 attribute */
		"call_g",                           /* 29 call_g */
		"call_l",                           /* 30 call_l */
		"call_predef",                      /* 31 call_predef */
		"case_b",                           /* 32 case_b */
		"case_w",                           /* 33 case_w */
		"case_l",                           /* 34 case_l */
		"compare_b",                        /* 35 compare_b */
		"compare_w",                        /* 36 compare_w */
		"compare_a",                        /* 37 compare_a */
		"compare_l",                        /* 38 compare_l */
		"compare_d",                        /* 39 compare_d */
		"compare_x",                        /* 40 compare_x */
		"compare_struc",                    /* 41 compare_struc */
		"convert_to_g",                     /* 42 convert_to_g */
		"convert_to_l",                     /* 43 convert_to_l */
		"create_b",                         /* 44 create_b */
		"create_w",                         /* 45 create_w */
		"create_a",                         /* 46 create_a */
		"create_l",                         /* 47 create_l */
		"create_d",                         /* 48 create_d */
		"create_x",                         /* 49 create_x */
		"create_copy_b",                    /* 50 create_copy_b */
		"create_copy_w",                    /* 51 create_copy_w */
		"create_copy_a",                    /* 52 create_copy_a */
		"create_copy_l",                    /* 53 create_copy_l */
		"create_copy_d",                    /* 54 create_copy_d */
		"create_copy_x",                    /* 55 create_copy_x */
		"create_copy_struc",                /* 56 create_copy_struc */
		"create_task_g",                    /* 57 create_task_g */
		"create_task_l",                    /* 58 create_task_l */
		"create_struc",                     /* 59 create_struc */
		"current_task",                     /* 60 current_task */
		"deallocate",                       /* 61 deallocate */
		"declare_b",                        /* 62 declare_b */
		"declare_w",                        /* 63 declare_w */
		"declare_a",                        /* 64 declare_a */
		"declare_l",                        /* 65 declare_l */
		"declare_d",                        /* 66 declare_d */
		"declare_x",                        /* 67 declare_x */
		"deref_b",                          /* 68 deref_b */
		"deref_w",                          /* 69 deref_w */
		"deref_a",                          /* 70 deref_a */
		"deref_l",                          /* 71 deref_l */
		"deref_d",                          /* 72 deref_d */
		"deref_x",                          /* 73 deref_x */
		"discard_addr",                     /* 74 discard_addr */
		"div_b",                            /* 75 div_b */
		"div_w",                            /* 76 div_w */
		"div_l",                            /* 77 div_l */
		"duplicate_b",                      /* 78 duplicate_b */
		"duplicate_w",                      /* 79 duplicate_w */
		"duplicate_a",                      /* 80 duplicate_a */
		"duplicate_l",                      /* 81 duplicate_l */
		"duplicate_d",                      /* 82 duplicate_d */
		"duplicate_x",                      /* 83 duplicate_x */
		"end_activation",                   /* 84 end_activation */
		"end_for_loop_b",                   /* 85 end_for_loop_b */
		"end_for_loop_w",                   /* 86 end_for_loop_w */
		"end_for_loop_l",                   /* 87 end_for_loop_l */
		"end_forrev_loop_b",                /* 88 end_forrev_loop_b */
		"end_forrev_loop_w",                /* 89 end_forrev_loop_w */
		"end_forrev_loop_l",                /* 90 end_forrev_loop_l */
		"end_rendezvous",                   /* 91 end_rendezvous */
		"enter_block",                      /* 92 enter_block */
		"entry_call",                       /* 93 entry_call */
		"fix_mul",                          /* 94 fix_mul */
		"fix_div",                          /* 95 fix_div */
		"float_add_l",                      /* 96 float_add_l */
		"float_add_x",                      /* 97 float_add_x */
		"float_compare_l",                  /* 98 float_compare_l */
		"float_compare_x",                  /* 99 float_compare_x */
		"float_div_l",                      /* 100 float_div_l */
		"float_div_x",                      /* 101 float_div_x */
		"float_mul_l",                      /* 102 float_mul_l */
		"float_mul_x",                      /* 103 float_mul_x */
		"float_neg_l",                      /* 104 float_neg_l */
		"float_neg_x",                      /* 105 float_neg_x */
		"float_pow_l",                      /* 106 float_pow_l */
		"float_pow_x",                      /* 107 float_pow_x */
		"float_sub_l",                      /* 108 float_sub_l */
		"float_sub_x",                      /* 109 float_sub_x */
		"float_abs_l",                      /* 110 float_abs_l */
		"float_abs_x",                      /* 111 float_abs_x */
		"indirect_move_b",                  /* 112 indirect_move_b */
		"indirect_move_w",                  /* 113 indirect_move_w */
		"indirect_move_a",                  /* 114 indirect_move_a */
		"indirect_move_l",                  /* 115 indirect_move_l */
		"indirect_move_d",                  /* 116 indirect_move_d */
		"indirect_move_x",                  /* 117 indirect_move_x */
		"indirect_pop_b_g",                 /* 118 indirect_pop_b_g */
		"indirect_pop_b_l",                 /* 119 indirect_pop_b_l */
		"indirect_pop_w_g",                 /* 120 indirect_pop_w_g */
		"indirect_pop_w_l",                 /* 121 indirect_pop_w_l */
		"indirect_pop_a_g",                 /* 122 indirect_pop_a_g */
		"indirect_pop_a_l",                 /* 123 indirect_pop_a_l */
		"indirect_pop_l_g",                 /* 124 indirect_pop_l_g */
		"indirect_pop_l_l",                 /* 125 indirect_pop_l_l */
		"indirect_pop_d_g",                 /* 126 indirect_pop_d_g */
		"indirect_pop_d_l",                 /* 127 indirect_pop_d_l */
		"indirect_pop_x_g",                 /* 128 indirect_pop_x_g */
		"indirect_pop_x_l",                 /* 129 indirect_pop_x_l */
		"install_handler",                  /* 130 install_handler */
		"is_equal",                         /* 131 is_equal */
		"is_less",                          /* 132 is_less */
		"is_greater",                       /* 133 is_greater */
		"is_less_or_equal",                 /* 134 is_less_or_equal */
		"is_greater_or_equal",              /* 135 is_greater_or_equal */
		"jump",                             /* 136 jump */
		"jump_if_false",                    /* 137 jump_if_false */
		"jump_if_greater",                  /* 138 jump_if_greater */
		"jump_if_greater_or_equal",         /* 139 jump_if_greater_or_equal */
		"jump_if_less",                     /* 140 jump_if_less */
		"jump_if_less_or_equal",            /* 141 jump_if_less_or_equal */
		"jump_if_true",                     /* 142 jump_if_true */
		"leave_block",                      /* 143 leave_block */
		"link_tasks_declared",              /* 144 link_tasks_declared */
		"load_exception_register",          /* 145 load_exception_register */
		"membership",                       /* 146 membership */
		"mod_b",                            /* 147 mod_b */
		"mod_w",                            /* 148 mod_w */
		"mod_l",                            /* 149 mod_l */
		"move_b",                           /* 150 move_b */
		"move_w",                           /* 151 move_w */
		"move_a",                           /* 152 move_a */
		"move_l",                           /* 153 move_l */
		"move_d",                           /* 154 move_d */
		"move_x",                           /* 155 move_x */
		"mul_b",                            /* 156 mul_b */
		"mul_w",                            /* 157 mul_w */
		"mul_l",                            /* 158 mul_l */
		"neg_b",                            /* 159 neg_b */
		"neg_w",                            /* 160 neg_w */
		"neg_l",                            /* 161 neg_l */
		"neg_x",                            /* 162 neg_x */
		"not",                              /* 163 not */
		"or",                               /* 164 or */
		"pop_b_g",                          /* 165 pop_b_g */
		"pop_b_l",                          /* 166 pop_b_l */
		"pop_w_g",                          /* 167 pop_w_g */
		"pop_w_l",                          /* 168 pop_w_l */
		"pop_a_g",                          /* 169 pop_a_g */
		"pop_a_l",                          /* 170 pop_a_l */
		"pop_l_g",                          /* 171 pop_l_g */
		"pop_l_l",                          /* 172 pop_l_l */
		"pop_d_g",                          /* 173 pop_d_g */
		"pop_d_l",                          /* 174 pop_d_l */
		"pop_x_g",                          /* 175 pop_x_g */
		"pop_x_l",                          /* 176 pop_x_l */
		"pop_tasks_declared_g",             /* 177 pop_tasks_declared_g */
		"pop_tasks_declared_l",             /* 178 pop_tasks_declared_l */
		"pow_b",                            /* 179 pow_b */
		"pow_w",                            /* 180 pow_w */
		"pow_l",                            /* 181 pow_l */
		"push_b_g",                         /* 182 push_b_g */
		"push_b_l",                         /* 183 push_b_l */
		"push_w_g",                         /* 184 push_w_g */
		"push_w_l",                         /* 185 push_w_l */
		"push_a_g",                         /* 186 push_a_g */
		"push_a_l",                         /* 187 push_a_l */
		"push_l_g",                         /* 188 push_l_g */
		"push_l_l",                         /* 189 push_l_l */
		"push_d_g",                         /* 190 push_d_g */
		"push_d_l",                         /* 191 push_d_l */
		"push_x_g",                         /* 192 push_x_g */
		"push_x_l",                         /* 193 push_x_l */
		"push_effective_address_g",         /* 194 push_effective_address_g */
		"push_effective_address_l",         /* 195 push_effective_address_l */
		"push_immediate_b",                 /* 196 push_immediate_b */
		"push_immediate_w",                 /* 197 push_immediate_w */
		"push_immediate_a",                 /* 198 push_immediate_a */
		"push_immediate_l",                 /* 199 push_immediate_l */
		"push_immediate_d",                 /* 200 push_immediate_d */
		"push_immediate_x",                 /* 201 push_immediate_x */
		"qual_discr_g",                     /* 202 qual_discr_g */
		"qual_discr_l",                     /* 203 qual_discr_l */
		"qual_index_g",                     /* 204 qual_index_g */
		"qual_index_l",                     /* 205 qual_index_l */
		"qual_range_g",                     /* 206 qual_range_g */
		"qual_range_l",                     /* 207 qual_range_l */
		"qual_sub_g",                       /* 208 qual_sub_g */
		"qual_sub_l",                       /* 209 qual_sub_l */
		"raise",                            /* 210 raise */
		"raise_in_caller",                  /* 211 raise_in_caller */
		"record_move_g",                    /* 212 record_move_g */
		"record_move_l",                    /* 213 record_move_l */
		"rem_b",                            /* 214 rem_b */
		"rem_w",                            /* 215 rem_w */
		"rem_l",                            /* 216 rem_l */
		"restore_stack_pointer",            /* 217 restore_stack_pointer */
		"return_b",                         /* 218 return_b */
		"return_w",                         /* 219 return_w */
		"return_a",                         /* 220 return_a */
		"return_l",                         /* 221 return_l */
		"return_d",                         /* 222 return_d */
		"return_x",                         /* 223 return_x */
		"return_struc",                     /* 224 return_struc */
		"save_stack_pointer",               /* 225 save_stack_pointer */
		"select",                           /* 226 select */
		"selective_wait",                   /* 227 selective_wait */
		"compare_array",                    /* 228 compare_array */
		"stmt",                             /* 229 stmt */
		"sub_b",                            /* 230 sub_b */
		"sub_w",                            /* 231 sub_w */
		"sub_l",                            /* 232 sub_l */
		"sub_x",                            /* 233 sub_x */
		"subprogram",                       /* 234 subprogram */
		"subscript",                        /* 235 subscript */
		"terminate",                        /* 236 terminate */
		"test_exception_register",          /* 237 test_exception_register */
		"timed_entry_call",                 /* 238 timed_entry_call */
		"type_global",                      /* 239 type_global */
		"type_local",                       /* 240 type_local */
		"uncreate",                         /* 241 uncreate */
		"update",                           /* 242 update */
		"update_and_discard",               /* 243 update_and_discard */
		"wait",                             /* 244 wait */
		"xor",                              /* 245 xor */
		"exit_block",                       /* 246 exit_block */
		"interface",                        /* 247 interface */
		"check_rec_subtype",		/* 248 check_rec_subtype */
	};
	if (opnum < 249) return opnames[opnum];
	else return "illegal";
}