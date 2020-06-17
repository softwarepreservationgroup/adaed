/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

  */
/* Reduce: This is the reduce action for ada. When called, the array
   rh is created which contains pointers to the structures on the
   top of the parse stack (prs_stack). To do this we traverse the 
   stack. As this is happening, when we find a terminal on the stack,
   we free the token structure for this terminal. After the traversal,
   we then free all the structures on the top of the stack as indicated
   by the length of the right hand side of the rule except one, which is
   kept for reuse for the non-terminal being formed by the current reduction.
   When we free the stack structures and the token structures, we have not
   actually released the storage, but have put it back into a free pool
   for reuse. However in this function we never allocate a stack or token
   structure from our pool, so the data held in these structures remains
   intact. A special case is when the right hand side of the rule has zero
   symbols, when we instead allocate a new stack strucure and do not free
   anything. */

#include "adared.h"
#include "adalexprots.h"
#include "reduceprots.h"
#include "ppredefprots.h"
#include "pspansprots.h"
#include "prsutilprots.h"
#include "errsprots.h"
#include "adaredprots.h"


#define TBSL

struct prsstack *rh[MAX_RHS];
struct ast *id_node;

void reduce(int red)									/*;reduce*/
{
	int n = rhslen[red];
	struct prsstack *tmp, *top;
	struct ast *node;

	if (!n) {
		tmp = PRSALLOC();
		tmp->prev = prs_stack;
		prs_stack = tmp;
	}
	else {
		if (n == 1) {
			rh[0] = prs_stack;
		}
		else {
			top = prs_stack;
			while (--n > 1) {
				rh[n] = prs_stack;
				prs_stack = prs_stack->prev;
			}
			rh[1] = tmp = prs_stack;
			rh[0] = prs_stack = prs_stack->prev;
			PRSFREE(top,tmp);
		}
	}

	if (redopt)
		fprintf(errfile,"Rule %d [%d]\n",red + 1,rhslen[red]);
	switch (red + 1) {

		/* pragma ::= 
			PRAGMA identifier [(argument_association{,argument_association */
	case 1 :
#define id IND(1)
#define arg_assoc_list_node AST(2)
		{
			struct two_pool *arg_assoc_list;
			char *name_id;
			int pragma_type = 0;

			arg_assoc_list = arg_assoc_list_node->links.list;
			node = any_node;
			if (!strcmp(namelist(id),"LIST")) {
				if (arg_assoc_list == NULL
				  || arg_assoc_list->link != arg_assoc_list)
					prs_warning(LOC(0),LOC(3),
					  "Pragma LIST takes one argument: ON or OFF");
				else
				{
					struct ast *arg;

					arg = arg_assoc_list->link->val.node;
#define opt_id ((arg->links.subast)[0])
#define expression ((arg->links.subast)[1])
					if (opt_id != opt_node)
						prs_warning(LOC(0),LOC(3),
						  "Named argument is invalid for pragma LIST");
					else if (expression->kind != AS_NAME)
						prs_warning(LOC(0),LOC(3),
						  "Argument passed to pragma LIST is invalid");
					else {
						struct ast *name;

						name = (expression->links.subast)[0];
						if (name->kind != AS_SIMPLE_NAME)
							prs_warning(LOC(0),LOC(3),
							  "Name argument passed to pragma LIST is invalid");
						else {
							name_id = namelist(name->links.val);
							if (!strcmp(name_id,"ON"))
								pragma_type = PRAGMA_LIST_ON;
							else if (!strcmp(name_id,"OFF"))
								pragma_type = PRAGMA_LIST_OFF;
							else {
								char msg[100];

								sprintf(msg,
			    "Identifier %s is an invalid argument passed to pragma LIST",
								  name_id);
								prs_warning(LOC(0),LOC(3),msg);
							}
						}
					}
				}
				if (!pragma_type)
					write_pragma(PRAGMA_LIST_ERR,LOC(0),LOC(3));
			}
#undef opt_id
#undef expression
			else if (!strcmp(namelist(id),"PAGE"))
				pragma_type = PRAGMA_PAGE;
			else {
				struct two_pool *arg_assoc_list;
				struct ast *arg2_node,*name_node,*simple_name_node;

				if (!strcmp(namelist(id),"IO_INTERFACE")) {
					arg_assoc_list = arg_assoc_list_node->links.list;
					/* get second argument of pragma io_interface and change
					 * node from as_simple_name to as_line_no whose n_val
					 * contains the internal number of the io routine
					 * TBSL: this node kind should not be as_line_no, however
					 * this avoids adding a new node kind for the moment!
					 * The node we are changing is 2 levels down in the tree!
					 */
					/* 2nd as_arg node*/
					arg2_node=arg_assoc_list->link->link->val.node;
					/* N_AST2(arg2_node) */
					name_node = arg2_node->links.subast[1];
					/*N_AST1(name_node)*/
					simple_name_node = name_node->links.subast[0];
					simple_name_node->kind = AS_LINE_NO;
					simple_name_node->links.val = 
					  predef_code(namelist(simple_name_node->links.val));
				}
				NN(AS_PRAGMA);
				make_id(1);
				NAST2(id_node,arg_assoc_list_node);
			}
			if (pragma_type)
				write_pragma(pragma_type,LOC(0),LOC(3));
		}
#undef id
#undef arg_assoc_list_node

		/* argument_association ::= [argument_identifier=>]expression */
		/* case 2 : */

		/* basic_declaration ::= object_declaration */
		/* case 3 : */

		/* basic_declaration ::= number_declaration */
		/* case 4 : */

		/* basic_declaration ::= type_declaration */
		/* case 5 : */

		/* basic_declaration ::= subtype_declaration */
		/* case 6 : */

		/* basic_declaration ::= subprogram_declaration */
		/* case 7 : */

		/* basic_declaration ::= package_declaration */
		/* case 8 : */

		/* basic_declaration ::= task_declaration */
		/* case 9 : */

		/* basic_declaration ::= generic_declaration */
		/* case 10 : */

		/* basic_declaration ::= exception_declaration */
		/* case 11 : */

		/* basic_declaration ::= generic_instantiation */
		/* case 12 : */

		/* basic_declaration ::= renaming_declaration */
		/* case 13 : */

		break;
		/* object_declaration ::=
		     identifier_list : subtype_indication [:=expression */
	case 14 :
#define id_list AST(0)
#define subtype_indic AST(2)
#define opt_init AST(3)
		NN(AS_OBJ_DECL);
		NAST3(id_list,subtype_indic,opt_init);
#undef id_list
#undef subtype_indic
#undef opt_init

		break;
		/* object_declaration ::=
			identifier_list : CONSTANT subtype_indication [:=e */
	case 15 :
#define id_list AST(0)
#define subtype_indic AST(3)
#define opt_init AST(4)
		NN(AS_CONST_DECL);
		NAST3(id_list,subtype_indic,opt_init);
#undef id_list
#undef subtype_indic
#undef opt_init

		break;
		/* object_declaration ::=
			identifier_list : [CONSTANT] constrained_array_def */
	case 16 :
#define id_list AST(0)
#define constant AST(2)
#define array_def AST(3)
#define opt_init AST(4)
		NN((constant == opt_node) ? AS_OBJ_DECL : AS_CONST_DECL);
		NAST3(id_list,array_def,opt_init);
#undef id_list
#undef constant
#undef array_def
#undef opt_init

		break;
		/* number_declaration ::=
			identifier_list : CONSTANT := universal_static_exp */
	case 17 :
#define id_list AST(0)
#define expression AST(4)
		NN(AS_NUM_DECL);
		NAST2(id_list,expression);
#undef id_list
#undef expression

		break;
		/* identifier_list ::= identifier {,identifier} */
	case 18 :
		node = AST(1);
		make_id(0);
		prepend(id_node,node);

		/* type_declaration ::= full_type_declaration */
		/* case 19 : */

		/* type_declaration ::= incomplete_type_declaration */
		/* case 20 : */

		/* type_declaration ::= private_type_declaration */
		/* case 21 : */

		break;
		/* full_type_declaration ::=
			TYPE identifier [discriminant_part]IS type_defi */
	case 22 :
#define opt_discr AST(2)
#define type_def AST(3)
		make_id(1);
		NN(AS_TYPE_DECL);
		NAST3(id_node,opt_discr,type_def);
#undef opt_discr
#undef type_def

		/* type_definition ::= enumeration_type_definition */
		/* case 23 : */

		/* type_definition ::= integer_type_definition */
		/* case 24 : */

		/* type_definition ::= real_type_definition */
		/* case 25 : */

		/* type_definition ::= array_type_definition */
		/* case 26 : */

		/* type_definition ::= record_type_definition */
		/* case 27 : */

		/* type_definition ::= access_type_definition */
		/* case 28 : */

		/* type_definition ::= derived_type_definition */
		/* case 29 : */

		break;
		/* subtype_declaration ::= SUBTYPE identifier IS subtype_indication ; */
	case 30 :
#define subtype_indic AST(3)
		make_id(1);
		NN(AS_SUBTYPE_DECL);
		NAST2(id_node,subtype_indic);
#undef subtype_indic

		break;
		/* subtype_indication ::= type_mark [constraint] */
	case 31 :
#define type_mark AST(0)
#define opt_constraint AST(1)
		NN(AS_SUBTYPE_INDIC);
		NAST2(type_mark,opt_constraint);
#undef type_mark
#undef opt_constraint

		/* constraint ::= range_constraint */
		/* case 32 : */

		/* constraint ::= floating_point_constraint */
		/* case 33 : */

		/* constraint ::= fixed_point_constraint */
		/* case 34 : */

		break;
		/* constraint ::= general_aggregate */
	case 35 : {
			struct ast *element;
			struct two_pool *new_list = NULL;

			node = AST(0);
			node->kind = AS_CONSTRAINT;

			/* "Range" elements of the general_aggregate represent the
		 	* constraints of an indexed-constrained array.  The base types
		 	* of the constraints are left as optional in the AST and have to
		 	* be resolved semantically by looking at the definition of the
		 	* corresponding type_mark.  See the definition of
		 	* "subtype_indication".
		 	*/
			LLOOPTOP(node->links.list,tmp)
		    	if (tmp->val.node->kind == AS_RANGE) {
					element = new_node(AS_SUBTYPE);
					element->links.subast = new_ast2(opt_node,tmp->val.node);
				}
				else
					element = tmp->val.node;
				new_list = concatl(new_list,initlist(element));
			LLOOPBOTTOM(tmp)
			node->links.list = new_list;
		}

		break;
		/* derived_type_definition ::= NEW subtype_indication */
	case 36 :
#define subtype_indic AST(1)
		NN(AS_DERIVED_TYPE);
		NAST1(subtype_indic);
#undef subtype_indic

		break;
		/* range_constraint ::= RANGE range */
	case 37 :
		{
			struct tok_loc *save_span;

			node = AST(1);
			if (node->kind != AS_RANGE && node->kind != AS_RANGE_ATTRIBUTE) {
				syntax_err(SPAN(node),"Invalid range specification");
				save_span = get_left_span(node);
				node = opt_node;
				set_span(node, save_span);
			}
		}

		break;
		/* range ::= range_attribute */
	case 38 :
#define name (node->links.subast)[0]
		node = AST(0);
		if (node->kind == AS_NAME) {
			if (name->kind == AS_ATTRIBUTE) {
#define attr_desig (name->links.subast)[0]
				if (attr_desig->links.val == RANGE_SYM) {
#undef attr_desig
					struct ast *tmp;

					tmp = name;
					astfree(node->links.subast);
					nodefree(node);
					node = tmp;
					node->kind = AS_RANGE_ATTRIBUTE;
				}
			}
			else
				node->kind = AS_RANGE_EXPRESSION;
		}
#undef name

		break;
		/* range ::= simple_expression .. simple_expression */
	case 39 :
#define simple_expr1 AST(0)
#define simple_expr2 AST(2)
		NN(AS_RANGE);
		NAST2(simple_expr1,simple_expr2);
#undef simple_expr1
#undef simple_expr2

		break;
		/* enumeration_type_definition ::=
			( enumeration_literal_specification {,enu */
	case 40 :
#define enum_list AST(1)
		node = AST(2);
		prepend(enum_list,node);
		node->kind = AS_ENUM;
#undef enum_list

		/* enumeration_literal_specification ::= enumeration_literal */
		/* case 41 : */

		break;
		/* enumeration_literal ::= identifier */
	case 42 :
#define id IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = id;
		set_span(node,LOC(0));
#undef id

		break;
		/* enumeration_literal ::= character_literal */
	case 43 :
#define char_lit IND(0)
		NN(AS_CHARACTER_LITERAL);
		node->links.val = char_lit;
		set_span(node,LOC(0));
#undef char_lit

		break;
		/* integer_type_definition ::= range_constraint */
	case 44 :
#define range_constraint AST(0)
		NN(AS_INT_TYPE);
		NAST1(range_constraint);
#undef range_constraint

		break;
		/* real_type_definition ::= floating_point_constraint */
	case 45 :
#define floating_point_constraint AST(0)
		NN(AS_FLOAT_TYPE);
		NAST1(floating_point_constraint);
#undef floating_point_constraint

		break;
		/* real_type_definition ::= fixed_point_constraint */
	case 46 :
#define fixed_point_constraint AST(0)
		NN(AS_FIXED_TYPE);
		NAST1(fixed_point_constraint);
#undef fixed_point_constraint

		break;
		/* floating_point_constraint ::=
			floating_accuracy_definition [range_constra */
	case 47 :
#define opt_range_constraint AST(1)
		node = AST(0);
		(node->links.subast)[1] = opt_range_constraint;
#undef opt_range_constraint

		break;
		/* floating_accuracy_definition ::= DIGITS static_simple_expression */
	case 48 :
#define simple_expr AST(1)
		NN(AS_DIGITS);
		NAST2(simple_expr,any_node);
#undef simple_expr

		break;
		/* fixed_point_constraint ::=
			fixed_accuracy_definition [range_constraint] */
	case 49 :
#define opt_range_constraint AST(1)
		node = AST(0);
		(node->links.subast)[1] = opt_range_constraint;
#undef opt_range_constraint

		break;
		/* fixed_accuracy_definition ::= DELTA static_simple_expression */
	case 50 :
#define simple_expr AST(1)
		NN(AS_DELTA);
		NAST2(simple_expr,any_node);
#undef simple_expr

		/* array_type_definition ::= unconstrained_array_definition */
		/* case 51 : */

		/* array_type_definition ::= constrained_array_definition */
		/* case 52 : */

		break;
		/* unconstrained_array_definition ::=
			ARRAY ( index_subtype_definition {,ind */
	case 53 :
#define ndex AST(2)
#define index_node AST(3)
#define subtype_indic AST(6)
		prepend(ndex,index_node);
		NN(AS_ARRAY_TYPE);
		NAST2(index_node,subtype_indic);
#undef ndex
#undef index_node
#undef subtype_indic

		break;
		/* constrained_array_definition ::=
			ARRAY index_constraint OF component_subt */
	case 54 :
#define index_constraint AST(1)
#define subtype_indic AST(3)
		NN(AS_ARRAY_TYPE);
		NAST2(index_constraint,subtype_indic);
#undef index_constraint
#undef subtype_indic

		break;
		/* index_subtype_definition ::= name RANGE <> */
	case 55 :
#define name AST(0)
		if (!check_expanded_name(name))
			syntax_err(SPAN(name),
			    "Invalid type_mark used in index_subtype_definition");
		NN(AS_BOX);
		NAST1(name);
#undef name

		break;
		/* index_constraint ::= ( discrete_range {,discrete_range} ) */
	case 56 :
#define discrete_range AST(1)
		node = AST(2);
		prepend(discrete_range,node);
		LLOOPTOP(node->links.list,tmp)
		    check_discrete_range(tmp->val.node);
		LLOOPBOTTOM(tmp)
#undef discrete_range

		    break;
		/* discrete_range ::= name range_constraint */
	case 57 :
#define name AST(0)
#define range_constraint AST(1)
		if (!check_expanded_name(name))
			syntax_err(SPAN(name),
			  "Discrete_subtype_indication must be a type_mark");
		NN(AS_SUBTYPE);
		NAST2(name,range_constraint);
#undef name
#undef range_constraint

		break;
		/* discrete_range ::= range */
	case 58 :
#define ast_range AST(0)
		if (ast_range->kind == AS_RANGE) {
			NN(AS_SUBTYPE);
			NAST2(opt_node,ast_range);
		}
		else
			node = ast_range;
#undef ast_range

		break;
		/* record_type_definition ::= RECORD component_list END RECORD */
	case 59 :
#define component_list AST(1)
		NN(AS_RECORD);
		NAST1(component_list);
#undef component_list

		break;
		/* component_list ::=
			{pragma} {component_declaration} component_declaration */
	case 60 :
#define pragma_node1 AST(0)
#define comp_dec_node AST(1)
#define comp_dec AST(2)
#define pragma_node2 AST(3)
		check_pragmas(pragma_node1,null_pragmas);
		check_pragmas(pragma_node2,null_pragmas);
		comp_dec_node->links.list = concatl3(pragma_node1->links.list,
		  comp_dec_node->links.list,initlist(comp_dec));
		NN(AS_COMPONENT_LIST);
		NAST3(comp_dec_node,opt_node,pragma_node2);
		nodefree(pragma_node1);
#undef pragma_node1
#undef comp_dec_node
#undef comp_dec
#undef pragma_node2

		break;
		/* component_list ::=
			{pragma} {component_declaration} variant_part {pragma} */
	case 61 :
#define pragma_node1 AST(0)
#define comp_dec_node AST(1)
#define variant_part AST(2)
#define pragma_node2 AST(3)
		check_pragmas(pragma_node1,null_pragmas);
		check_pragmas(pragma_node2,null_pragmas);
		comp_dec_node->links.list = concatl(pragma_node1->links.list,
		  comp_dec_node->links.list);
		NN(AS_COMPONENT_LIST);
		NAST3(comp_dec_node,variant_part,pragma_node2);
		nodefree(pragma_node1);
#undef pragma_node1
#undef comp_dec_node
#undef variant_part
#undef pragma_node2

		break;
		/* component_list ::= {pragma} NULL ; {pragma} */
	case 62 :
#define pragma_node1 AST(0)
#define pragma_node2 AST(3)
		pragma_node1->links.list = concatl(pragma_node1->links.list,
		  pragma_node2->links.list);
		check_pragmas(pragma_node1,null_pragmas);
		NN(AS_COMPONENT_LIST);
		NAST3(opt_node,opt_node,pragma_node1);
		nodefree(pragma_node2);
#undef pragma_node1
#undef pragma_node2

		break;
		/* component_declaration ::=
			identifier_list : component_subtype_definition  */
	case 63 :
#define id_list AST(0)
#define subtype_indic AST(2)
#define opt_init AST(3)
		NN(AS_FIELD);
		NAST3(id_list,subtype_indic,opt_init);
#undef id_list
#undef subtype_indic
#undef opt_init

		break;
		/* discriminant_part ::=
			( discriminant_specification {;discriminant_specifi */
	case 64 :
#define discr_spec AST(1)
		node = AST(2);
		prepend(discr_spec,node);
#undef discr_spec

		break;
		/* discriminant_specification ::=
			identifier_list : type_mark [:=expression] */
	case 65 :
#define id_list AST(0)
#define type_mark AST(2)
#define opt_init AST(3)
		NN(AS_DISCR_SPEC);
		NAST3(id_list,type_mark,opt_init);
#undef id_list
#undef type_mark
#undef opt_init

		break;
		/* variant_part ::=
			CASE discriminant_simple_name IS {pragma} variant {varia */
	case 66 :
#define simple_name AST(1)
#define pragma_node AST(3)
#define variant AST(4)
#define variant_node AST(5)
		check_pragmas(pragma_node,null_pragmas);
		variant_node->links.list = concatl3(pragma_node->links.list,
		  initlist(variant),variant_node->links.list);
		check_choices(variant_node,"a variant_part");
		NN(AS_VARIANT_DECL);
		NAST2(simple_name,variant_node);
		nodefree(pragma_node);
#undef simple_name
#undef pragma_node
#undef variant
#undef variant_node

		break;
		/* variant ::= WHEN choice {|choice} => component_list */
	case 67 :
#define choice AST(1)
#define choice_node AST(2)
#define component_list AST(4)
		prepend(choice,choice_node);
		NN(AS_VARIANT_CHOICES);
		NAST2(choice_node,component_list);
#undef choice
#undef choice_node
#undef component_list

		break;
		/* choice ::= discrete_range */
	case 68 :
#define discrete_range AST(0)
		switch(discrete_range->kind) {
		case AS_SUBTYPE :
		case AS_RANGE_ATTRIBUTE :
			NN(AS_RANGE_CHOICE);
			NAST1(discrete_range);
			break;
		case AS_RANGE_EXPRESSION :
			node = discrete_range;
			node->kind = AS_CHOICE_UNRESOLVED;
			break;
		default :
			NN(AS_SIMPLE_CHOICE);
			NAST1(discrete_range);
			break;
		}
#undef discrete_range

		break;
		/* choice ::= OTHERS */
	case 69 :
		{
			struct ast *span_node;

			NN(AS_OTHERS_CHOICE);
			span_node = new_node(AS_SIMPLE_NAME);
			span_node->links.val = IND(0);
			set_span(span_node,LOC(0));
			NAST3(opt_node,opt_node,span_node);
		}

		break;
		/* access_type_definition ::= ACCESS subtype_indication */
	case 70 :
#define subtype_indic AST(1)
		NN(AS_ACCESS_TYPE);
		NAST1(subtype_indic);
#undef subtype_indic

		break;
		/* incomplete_type_declaration ::=
			TYPE identifier [discriminant_part]; */
	case 71 :
#define opt_discr_part AST(2)
		make_id(1);
		NN(AS_INCOMPLETE_DECL);
		NAST2(id_node,opt_discr_part);
#undef opt_discr_part

		break;
		/* declarative_part ::= {basic_declarative_item} */
	case 72 :
		{
			struct two_pool *dec_list,
			*prev,*bottom;
			struct ast *line_node;
			int list_all_done = 0 ;
			struct tok_loc *line_node_span;


			node = AST (0);
			dec_list = node -> links.list;


			if (node -> links.list != NULL) {

				dec_list = bottom = node -> links.list;
				do {
					prev = dec_list;
					dec_list = dec_list -> link;
					list_all_done = (dec_list == bottom) ;

					if (isbody_node[dec_list -> val.node -> kind])
						break;
					line_node = new_node (AS_LINE_NO);
					line_node_span = get_left_span(dec_list->val.node);
					line_node -> links.val = line_node_span->line;
					set_span(line_node,line_node_span);
					/* Insert a new node with the AS_LINE_NO between dec_list
						and its predecessor */
					prev -> link = initlist (line_node) ;
					prev->link->link = dec_list;
				} while (dec_list != bottom);

				/* If a body node was found, ensure that no simple decl follows
 				 * Since the above looped using dec_list, we can start from
				 * dec_list.
				 */
				if (!list_all_done) {
					do {
						dec_list = dec_list -> link;
						if(!islater_declarative_node[dec_list->val.node-> kind])
							syntax_err (SPAN (dec_list -> val.node),
							  "Misplaced basic_declarative_item");

					}                while (dec_list != bottom);
				}
			}
			node -> kind = AS_DECLARATIONS;
		}

		/* basic_declarative_item ::= basic_declaration */
		/* case 73 : */

		/* basic_declarative_item ::= representation_clause */
		/* case 74 : */

		/* basic_declarative_item ::= use_clause */
		/* case 75 : */

		/* basic_declarative_item ::= body */
		/* case 76 : */

		/* body ::= proper_body */
		/* case 77 : */

		/* body ::= body_stub */
		/* case 78 : */

		/* proper_body ::= subprogram_body */
		/* case 79 : */

		/* proper_body ::= package_body */
		/* case 80 : */

		/* proper_body ::= task_body */
		/* case 81 : */

		/* name ::= simple_name */
		/* case 82 : */

		break;
		/* name ::= character_literal */
	case 83 :
#define char_lit IND(0)
		NN(AS_CHARACTER_LITERAL);
		node->links.val = char_lit;
		set_span(node,LOC(0));
#undef char_list

		break;
		/* name ::= operator_symbol */
	case 84 :
		node = AST(0);
		node->kind = AS_STRING;

		/* name ::= indexed_component */
		/* case 85 : */

		/* name ::= selected_component */
		/* case 86 : */

		/* name ::= attribute */
		/* case 87 : */

		break;
		/* simple_name ::= identifier */
	case 88 :
#define id IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = id;
		set_span(node,LOC(0));
#undef id

		break;
		/* indexed_component ::= prefix general_aggregate */
	case 89 :
#define prefix AST(0)
#define general_aggregate AST(1)
		{
			int kind;

			/* second argument of attribute is N_AST[3] */
#define arg2  (prefix->links.subast)[2]

			if (prefix->kind == AS_ATTRIBUTE && arg2 == opt_node)
			/* do the following checks only if have not yet processed the node*/
			{
				node = prefix;
#define general_component (general_aggregate->links.list->link->val.node)
				if (general_aggregate->links.list == NULL
				  || general_aggregate->links.list->link != 
				   general_aggregate->links.list
				  || (kind = general_component->kind) == AS_CHOICE_LIST
				  || kind == AS_RANGE || kind == AS_SUBTYPE) {
					syntax_err(SPAN(general_aggregate),
					  "Illegal expression for argument of attribute");
					(node->links.subast)[2] = opt_node;
					free_everything(general_aggregate);
				}
				else {
					(node->links.subast)[2] = general_component;
					nodefree(general_aggregate);
				}
				(node->links.subast)[3] = NULL;
#undef general_component
#undef arg2
			}
			else {
				if (prefix->kind == AS_STRING && 
				  !isoverloadable_op(namelist(prefix->links.val))) {
					char msg[200];

					sprintf(msg,"\"%s\" is not a valid operator_symbol",
					  namelist(prefix->links.val));
					syntax_err(SPAN(prefix),msg);
				}
				NN(AS_CALL_UNRESOLVED);
				NAST2(prefix,general_aggregate);
			}
		}
#undef prefix
#undef general_aggregate

		break;
		/* selected_component ::= prefix . selector */
	case 90 :
#define prefix AST(0)
#define selector AST(2)
		if (selector->kind == AS_ALL) {
			node = selector;
			NAST1(prefix);
		}
		else {
			NN(AS_SELECTOR);
			NAST2(prefix,selector);
		}
#undef prefix
#undef selector

		/* selector ::= simple_name */
		/* case 91 : */

		break;
		/* selector ::= character_literal */
	case 92 :
#define lit IND(0)
		NN(AS_CHARACTER_LITERAL);
		node->links.val = lit;
		set_span(node,LOC(0));
#undef lit

		break;
		/* selector ::= operator_symbol */
	case 93 :
		{
			char tmp[200];

			node = AST(0);
			strcpy(tmp,namelist(node->links.val));
			convtolower(tmp);
			if (!isoverloadable_op(tmp)) {
				char msg[300];

				sprintf(msg,"\"%s\" is not a valid operator_symbol",tmp);
				syntax_err(get_left_span(node),get_right_span(node),msg);
			}
			node->links.val = namemap(tmp,strlen(tmp));
		}

		break;
		/* selector ::= ALL */
	case 94 :
		NN(AS_ALL);

		break;
		/* attribute ::= prefix ' attribute_designator */
	case 95 :
#define prefix AST(0)
#define attr_desig AST(2)
		NN(AS_ATTRIBUTE);
		NAST3(attr_desig,prefix,opt_node);
#undef prefix
#undef attr_desig

		/* attribute_designator ::= simple_name */
		/* case 96 : */

		break;
		/* attribute_designator ::= DIGITS */
	case 97 :

		/* attribute_designator ::= DELTA */
	case 98 :

		/* attribute_designator ::= RANGE */
	case 99 :
#define keyword IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = keyword;
		set_span(node,LOC(0));
#undef keyword

		break;
		/* aggregate ::= ( component_association {,component_association} ) */
	case 100 :
#define comp_assoc AST(1)
#define comp_assoc_node AST(2)
		if (comp_assoc_node->links.list == NULL
		  && comp_assoc->kind != AS_CHOICE_LIST) {
			NN(AS_PARENTHESIS);
			NAST1(comp_assoc);
			nodefree(comp_assoc_node);
		}
		else {
			node = comp_assoc_node;
			prepend(comp_assoc,node);
			node->kind = AS_AGGREGATE;
		}
#undef comp_assoc
#undef comp_assoc_node

		/* component_association ::= [choice{|choice}=>]expression */
		/* case 101 : */

		break;
		/* general_aggregate ::=
			( general_component_association {,general_component */
	case 102 :
#define gen_comp_assoc AST(1)
		node = AST(2);
		prepend(gen_comp_assoc,node);
#undef gen_comp_assoc

		/* general_component_association ::= component_association */
		/* case 103 : */

		break;
		/* general_component_association ::=
			simple_expression .. simple_expression */
	case 104 :
#define simple_expr1 AST(0)
#define simple_expr2 AST(2)
		NN(AS_RANGE);
		NAST2(simple_expr1,simple_expr2);
#undef simple_expr1
#undef simple_expr2

		break;
		/* general_component_association ::= name range_constraint */
	case 105 :
#define name AST(0)
#define range_constraint AST(1)
		if (!check_expanded_name(name))
			syntax_err(SPAN(name),"subtype_indicaiton must be a type_mark");
		NN(AS_SUBTYPE);
		NAST2(name,range_constraint);
#undef name
#undef range_constraint

		/* expression ::= relation */
		/* case 106 : */

		/* expression ::= relation{AND__relation} */
		/* case 107 : */

		/* expression ::= relation{OR__relation} */
		/* case 108 : */

		/* expression ::= relation{XOR__relation} */
		/* case 109 : */

		/* expression ::= relation{AND__THEN__relation} */
		/* case 110 : */

		/* expression ::= relation{OR__ELSE__relation} */
		/* case 111 : */

		break;
		/* relation ::=
			simple_expression [relational_operator__simple_expression] */
	case 112 :
#define simple_expr1 AST(0)
#define arg_list_node ((node->links.subast)[1])
		if ((node = AST(1)) == opt_node)
			node = simple_expr1;
		else
			arg_list_node->links.list->link->val.node = simple_expr1;
#undef simple_expr1
#undef arg_list_node

		break;
		/* relation ::= simple_expression [NOT] IN range */
	case 113 :
#define simple_expr AST(0)
#define opt_not AST(1)
#define optr IND(2)
#define optrloc LOC(2)
#define ast_range AST(3)
		{
			int kind, op_name;
			struct ast *arg_list_node, *simple_name;
			struct two_pool *tmp;
			struct tok_loc *old_span;

			if (opt_not != opt_node) {
				kind = AS_NOTIN;
				op_name = namemap("notin",5);
			}
			else if (!strcmp(namelist(optr),"any_op"))
				kind = AS_ANY_OP;
			else {
				kind = AS_IN;
				op_name = namemap("in",2);
			}
			switch (ast_range->kind) {
			case AS_RANGE_EXPRESSION :
#define name ((ast_range->links.subast)[0])
				if (!check_expanded_name(name))
					syntax_err(SPAN(ast_range),
					  "Invalid expression used as range specification");
				break;
#undef name
			case AS_RANGE :
			case AS_RANGE_ATTRIBUTE :
				break;
			default :
				syntax_err(SPAN(ast_range),"Invalid range specification");
				/* fix up to satisfy adasem */
				old_span = LOC(3);
				ast_range = new_node(AS_RANGE_EXPRESSION);
				ast_range->links.subast = new_ast1(new_node(AS_SIMPLE_NAME));
				(ast_range->links.subast[0])->links.val = namemap("any_id",6);
				set_span(ast_range->links.subast[0],old_span);
			}
			NN(kind);
			simple_name = new_node(AS_SIMPLE_NAME);
			set_span(simple_name,(opt_not!=opt_node ? &opt_not->span :optrloc));
			simple_name->links.val = op_name;
			arg_list_node = new_node(AS_LIST);
			(arg_list_node->links.list = TALLOC())->val.node = ast_range;
			(arg_list_node->links.list->link = tmp = TALLOC())->val.node =
			  simple_expr;
			tmp->link = arg_list_node->links.list;
			NAST2(simple_name,arg_list_node);
		}
#undef simple_expr
#undef opt_not
#undef optr
#undef optrloc
#undef ast_range

		/* simple_expression ::=
			[unary_adding_operator]term{binary_adding_operator_ */
		/* case 114 : */

		/* term ::= factor{multiplying_operator__factor} */
		/* case 115 : */

		break;
		/* factor ::= primary [**__primary] */
	case 116 :
#define primary1 AST(0)
#define arg_list_node ((node->links.subast)[1])
		if ((node = AST(1)) == opt_node)
			node = primary1;
		else
			arg_list_node->links.list->link->val.node = primary1;
#undef arg_list_node
#undef primary1

		break;
		/* factor ::= ABS primary */
	case 117 :
#define optrloc LOC(0)
#define primary AST(1)
		{
			struct ast *optr_node;

			optr_node = new_node(AS_SIMPLE_NAME);
			optr_node->links.val = namemap("abs",3);
			set_span(optr_node,optrloc);
			node = unary_operator(optr_node,primary);
		}
#undef optrloc
#undef primary

		break;
		/* factor ::= NOT primary */
	case 118 :
#define optrloc LOC(0)
#define primary AST(1)
		{
			struct ast *optr_node;

			optr_node = new_node(AS_SIMPLE_NAME);
			optr_node->links.val = namemap("not",3);
			set_span(optr_node,optrloc);
			node = unary_operator(optr_node,primary);
		}
#undef optrloc
#undef primary

		break;
		/* primary ::= numeric_literal */
	case 119 :
#define num_lit IND(0)
		NN((strchr(namelist(num_lit),'.')) ? AS_REAL_LITERAL :
		  AS_INT_LITERAL);
		node->links.val = num_lit;
		set_span(node,LOC(0));
#undef num_lit

		break;
		/* primary ::= NULL */
	case 120 :
		NN(AS_NULL);
		set_span(node,LOC(0));

		/* primary ::= aggregate */
		/* case 121 : */

		break;
		/* primary ::= name */
	case 122 :
#define name_node AST(0)
		if (name_node->kind == AS_STRING) {
			node = name_node;
			node->kind = AS_STRING_LITERAL;
		}
		else {
			NN(AS_NAME);
			NAST1(name_node);
		}
#undef name_node

		/* primary ::= allocator */
		/* case 123 : */

		/* primary ::= qualified_expression */
		/* case 124 : */

		break;
		/* relational_operator ::= = */
	case 125 :

		/* relational_operator ::= /= */
	case 126 :

		/* relational_operator ::= < */
	case 127 :

		/* relational_operator ::= <= */
	case 128 :

		/* relational_operator ::= > */
	case 129 :

		/* relational_operator ::= >= */
	case 130 :

		/* binary_adding_operator ::= + */
	case 131 :

		/* binary_adding_operator ::= - */
	case 132 :

		/* binary_adding_operator ::= & */
	case 133 :
#define optr IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = optr;
		set_span(node,LOC(0));
#undef optr

		break;
		/* unary_adding_operator ::= + */
	case 134 :

		/* unary_adding_operator ::= - */
	case 135 :
#define optr IND(0)
		{
			char str[6];

			NN(AS_SIMPLE_NAME);
			sprintf(str,"%s",namelist(optr));
			node->links.val = namemap(str,1);
			set_span(node,LOC(0));
		}
#undef optr

		break;
		/* multiplying_operator ::= * */
	case 136 :

		/* multiplying_operator ::= / */
	case 137 :
#define optr IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = optr;
		set_span(node,LOC(0));
#undef optr

		break;
		/* multiplying_operator ::= MOD */
	case 138 :
		NN(AS_SIMPLE_NAME);
		node->links.val = namemap("mod",3);
		set_span(node,LOC(0));

		break;
		/* multiplying_operator ::= REM */
	case 139 :
		NN(AS_SIMPLE_NAME);
		node->links.val =  namemap("rem",3);
		set_span(node,LOC(0));

		break;
		/* qualified_expression ::= name ' aggregate */
	case 140 :
#define name AST(0)
#define aggregate AST(2)
		if (!check_expanded_name(name))
			syntax_err(SPAN(name),
			  "Invalid type_mark found in qualified_expression");
		if (aggregate->kind == AS_PARENTHESIS) { /* remove parentheses */
			aggregate = aggregate->links.subast[0];
		}
		NN(AS_QUALIFY);
		NAST2(name,aggregate);
#undef name
#undef aggregate

		break;
		/* allocator ::= NEW type_mark */
	case 141 :
#define type_mark AST(1)
		NN(AS_NEW);
		NAST2(type_mark,opt_node);
#undef type_mark

		break;
		/* allocator ::= NEW type_mark general_aggregate */
	case 142 :
#define type_mark AST(1)
#define general_aggregate AST(2)
		NN(AS_NEW);
		NAST2(type_mark,general_aggregate);
#undef type_mark
#undef general_aggregate

		break;
		/* allocator ::= NEW type_mark ' aggregate */
	case 143 :
#define type_mark AST(1)
#define aggregate AST(3)
		NN(AS_NEW_INIT);
		NAST2(type_mark,aggregate);
#undef type_mark
#undef aggregate

		break;
		/* sequence_of_statements ::= {pragma} statement {statement} */
	case 144 :
#define pragma_node AST(0)
#define stmt AST(1)
#define stmt_node AST(2) 
		{
			struct ast *label_list_node, *line_node;
			struct two_pool *nodelabels = NULL, *lablistlabels = NULL;
			struct two_pool *prev;
			struct tok_loc *line_node_span;

			check_pragmas(pragma_node,null_pragmas);
			NN(AS_STATEMENTS);
			stmt_node->links.list = concatl3(pragma_node->links.list,
			  initlist(stmt),stmt_node->links.list);
			label_list_node = new_node(AS_LIST);
			prev = stmt_node->links.list;  /* bottom of list */
			LLOOPTOP(stmt_node->links.list,tmp)
			    nodelabels = concatl(nodelabels,
			      copylist(getlabels(tmp->val.node)));
				if (tmp->val.node->kind == AS_STATEMENT)
					lablistlabels = concatl(lablistlabels,
				      copylist((tmp->val.node->links.subast)[0]->links.list));
				if (tmp->val.node->kind != AS_PRAGMA) {
					/* insert AS_LINE_NO node before the current node (tmp) */
					line_node = new_node(AS_LINE_NO);
					line_node_span = get_left_span(tmp->val.node);
					line_node->links.val = line_node_span->line;
					set_span(line_node,line_node_span);
					prev->link = initlist(line_node);
					prev->link->link = tmp;
				}
				prev = tmp;
			LLOOPBOTTOM(tmp)
			/* add as_line_no node for next token (curtok) to end of stmt_list*/
			end_as_line_no(stmt_node,curtok);
			newlabels(node,nodelabels);
			label_list_node->links.list = lablistlabels;
			if (lablistlabels == NULL)
				set_span(label_list_node,&curtok->ptr.token->loc);
			NAST2(stmt_node,label_list_node);
			nodefree(pragma_node);
		}
#undef pragma_node
#undef stmt
#undef stmt_node

		break;
		/* statement ::= {label} simple_statement */
	case 145 :
#define labs_node AST(0)
#define stmt AST(1)
		if (labs_node->links.list != NULL) {
			NN(AS_STATEMENT);
			NAST2(labs_node,stmt);
			newlabels(node,copylist(labs_node->links.list));
		}
		else {
			node = stmt;
			nodefree(labs_node);
		}
#undef labs_node
#undef stmt

		break;
		/* statement ::= {label} compound_statement */
	case 146 :
#define labs_node AST(0)
#define stmt AST(1)
		if (labs_node->links.list != NULL) {
			NN(AS_STATEMENT);
			NAST2(labs_node,stmt);
			newlabels(node,concatl(copylist(labs_node->links.list),
			  copylist(getlabels(stmt))));
		}
		else {
			node = stmt;
			nodefree(labs_node);
		}
#undef labs_node
#undef stmt

		/* simple_statement ::= null_statement */
		/* case 147 : */

		/* simple_statement ::= assignment_statement */
		/* case 148 : */

		/* simple_statement ::= exit_statement */
		/* case 149 : */

		/* simple_statement ::= return_statement */
		/* case 150 : */

		/* simple_statement ::= goto_statement */
		/* case 151 : */

		/* simple_statement ::= delay_statement */
		/* case 152 : */

		/* simple_statement ::= abort_statement */
		/* case 153 : */

		/* simple_statement ::= raise_statement */
		/* case 154 : */

		/* simple_statement ::= code_statement */
		/* case 155 : */

		/* simple_statement ::= call_statement */
		/* case 156 : */

		/* compound_statement ::= if_statement */
		/* case 157 : */

		/* compound_statement ::= case_statement */
		/* case 158 : */

		/* compound_statement ::= loop_statement */
		/* case 159 : */

		/* compound_statement ::= block_statement */
		/* case 160 : */

		/* compound_statement ::= accept_statement */
		/* case 161 : */

		/* compound_statement ::= select_statement */
		/* case 162 : */

		break;
		/* label ::= << label_simple_name >> */
	case 163 :
		node = AST(1);

		break;
		/* null_statement ::= NULL ; */
	case 164 :
		NN(AS_NULL_S);
		set_span(node,LOC(0));

		break;
		/* assignment_statement ::= variable_name := expression ; */
	case 165 :
#define name AST(0)
#define expression AST(2)
		NN(AS_ASSIGNMENT);
		NAST2(name,expression);
#undef name
#undef expression

		break;
		/* if_statement ::=
			IF condition THEN sequence_of_statements {ELSIF__conditi */
	case 166 :
#define expression AST(1)
#define stmts AST(3)
#define if_list_node AST(4)
#define opt_else AST(5)
		{
			struct ast *if_node;
			struct two_pool *nodelabels = NULL;

			NN(AS_IF);
			if_node = new_node(AS_COND_STATEMENTS);
			if_node->links.subast = new_ast2(expression,stmts);
			prepend(if_node,if_list_node);
			LLOOPTOP(if_list_node->links.list,tmp)
			    nodelabels = concatl(nodelabels,
			      copylist(getlabels((tmp->val.node->links.subast)[1])));
			LLOOPBOTTOM(tmp)
			if (opt_else != opt_node)
				nodelabels = concatl(nodelabels,copylist(getlabels(opt_else)));
			newlabels(node,nodelabels);
			NAST2(if_list_node,opt_else);
		}
#undef expression
#undef stmts
#undef if_list_node
#undef opt_else

		break;
		/* condition ::= boolean_expression */
	case 167 :
#define boolean_expression AST(0)
		NN(AS_CONDITION);
		NAST1(boolean_expression);
#undef boolean_expression

		break;
		/* case_statement ::=
			CASE expression IS {pragma} case_statement_alternative */
	case 168 :
#define expression AST(1)
#define pragma_node AST(3)
#define alt AST(4)
#define alt_node AST(5)
		{
			struct two_pool *nodelabels = NULL;

			NN(AS_CASE);
			check_pragmas(pragma_node,null_pragmas);
			alt_node->links.list=concatl3(pragma_node->links.list,initlist(alt),
			  alt_node->links.list);
			check_choices(alt_node,"a case_statement");
			nodefree(pragma_node);
			LLOOPTOP(alt_node->links.list,tmp)
			    nodelabels = concatl(nodelabels,
			      copylist(getlabels((tmp->val.node->links.subast)[1])));
			LLOOPBOTTOM(tmp)
			    newlabels(node,nodelabels);
			NAST2(expression,alt_node);
		}
#undef expression
#undef pragma_node
#undef alt
#undef alt_node

		break;
		/* case_statement_alternative ::=
			WHEN choice {|choice} => sequence_of_state */
	case 169 :
#define choice AST(1)
#define choice_node AST(2)
#define stmts AST(4)
		prepend(choice,choice_node);
		NN(AS_CASE_STATEMENTS);
		NAST2(choice_node,stmts);
#undef choice
#undef choice_node
#undef stmts

		break;
		/* loop_statement ::=
			[loop_simple_name:] [iteration_scheme] LOOP sequence_o */
	case 170 :
#define iteration_scheme AST(1)
#define stmts AST(3)
#define simple_name2 AST(6)
		{
			struct ast *simple_name1;
			struct tok_loc *left_span, *right_span ;

			simple_name1 = AST(0);
			if (simple_name1 != opt_node)
				left_span = get_left_span(simple_name1);
			else if (iteration_scheme != opt_node)
				left_span = get_left_span(iteration_scheme);
			else left_span = LOC(2);
			if (simple_name2 != opt_node)
				right_span = get_right_span(simple_name2);
			else right_span = END_LOC(5);
			NN(AS_LOOP);
			if (simple_name1 != opt_node) {
				if (simple_name2 != opt_node) {
					if (simple_name1->links.val != simple_name2->links.val)
						match_error(simple_name1->links.val,
						  simple_name2->links.val, "loop_statement",
						  left_span,right_span);
				}
				else {
					char msg[200];

					sprintf(msg,
"%s at beginning of loop_statement does not match non-existent \
\"loop_simple_name\" at END LOOP", namelist(simple_name1->links.val));
					syntax_err(left_span,right_span,msg);
				}
			}
			else {
				char newlabel[15];

				if (simple_name2 != opt_node) {
					char msg[200];

					sprintf(msg,
"Non existent \"loop_simple_name:\" at beginning of loop_statement does \
not match %s", namelist(simple_name2->links.val));
					syntax_err(left_span,right_span,msg);
				}
				simple_name1 = new_node(AS_SIMPLE_NAME);
				sprintf(newlabel,"#%x",simple_name1);
				simple_name1->links.val = namemap(newlabel,strlen(newlabel));
				set_span(simple_name1,left_span);
			}
			newlabels(node,concatl(initlist(simple_name1),
			  copylist(getlabels(stmts))));
			NAST3(simple_name1,iteration_scheme,stmts);

			nodefree(simple_name2);
		}
#undef iteration_scheme
#undef stmts
#undef simple_name2

		break;
		/* iteration_scheme ::= WHILE condition */
	case 171 :
#define expression AST(1)
		NN(AS_WHILE);
		NAST1(expression);
#undef expression

		break;
		/* iteration_scheme ::= FOR loop_parameter_specification */
	case 172 :
		node = AST(1);

		break;
		/* loop_parameter_specification ::=
			identifier IN [REVERSE] discrete_range */
	case 173 :
#define opt_rev AST(2)
#define discrete_range AST(3)
		check_discrete_range(discrete_range);
		NN((opt_rev == opt_node) ? AS_FOR : AS_FORREV);
		make_id(0);
		NAST2(id_node,discrete_range);
#undef opt_rev
#undef discrete_range

		break;
		/* block_statement ::=
			[block_simple_name:] [DECLARE__declarative_part] BEGI */
	case 174 :
#define decl_part AST(1)
#define stmts AST(3)
#define opt_except_list AST(4)
#define simple_name2 AST(6)
		{
			struct ast *simple_name1, *labs_node;
			struct tok_loc *left_span, *right_span ;

			simple_name1 = AST(0);
			if (simple_name1 != opt_node)
				left_span = get_left_span(simple_name1);
			else if (decl_part != opt_node)
				left_span = get_left_span(decl_part);
			else left_span = LOC(2);
			if (simple_name2 != opt_node)
				right_span = get_right_span(simple_name2);
			else right_span = END_LOC(5);

			NN(AS_BLOCK);
			if (simple_name1 != opt_node) {
				if (simple_name2 != opt_node) {
					if (simple_name1->links.val != simple_name2->links.val)
						match_error(simple_name1->links.val,
						  simple_name2->links.val, "block_statement",
						  left_span,right_span);
				}
				else {
					char msg[200];

					sprintf(msg,
"%s at beginning of block_statement does not match non-existent \
\"block_simple_name\" at end of block", namelist(simple_name1->links.val));
					syntax_err(left_span,right_span,msg);
				}
			}
			else {
				char newlabel[15];

				if (simple_name2 != opt_node) {
					char msg[200];

					sprintf(msg,
"Non-existent \"block_simple_name:\" at beginning of block_statement does \
not match %s", namelist(simple_name2->links.val));
					syntax_err(get_left_span(simple_name2),
					  get_right_span(simple_name2),msg);
				}
				simple_name1 = new_node(AS_SIMPLE_NAME);
				sprintf(newlabel,"#%x",simple_name1);
				simple_name1->links.val = namemap(newlabel,strlen(newlabel));
				set_span(simple_name1,left_span);
			}

			/* The labels declared within a block are grouped together under
			 * a single node : labs_node.  This node is then passed upwards
			 * to help prevent duplicate declaration of labels within a program
			 * unit.  (see remove_duplicate_labels)
			 */

			labs_node = new_node(AS_LABELS);
			labs_node->links.list = remove_duplicate_labels(
			  concatl(copylist(getlabels(stmts)),
			  ((opt_except_list == opt_node) ? (struct two_pool *)0 : 
			  copylist(getlabels(opt_except_list)))));
			newlabels(node,concatl(initlist(simple_name1),initlist(labs_node)));
			append(decl_part,labs_node);
			NAST4(simple_name1,decl_part,stmts,opt_except_list);
			nodefree(simple_name2);
		}
#undef decl_part
#undef stmts
#undef opt_except_list
#undef simple_name2

		break;
		/* exit_statement ::= EXIT [loop_name] [WHEN__condition] ; */
	case 175 :
#define opt_name AST(1)
#define opt_expression AST(2)
		{
			struct ast *span_node;

			NN(AS_EXIT);
			span_node = new_node(AS_SIMPLE_NAME);
			span_node->links.val = IND(0);
			set_span(span_node,LOC(0));
			NAST4(opt_name,opt_expression,opt_node,span_node);
		}
#undef opt_name
#undef opt_expression

		break;
		/* return_statement ::= RETURN [expression] ; */
	case 176 :
#define opt_expression AST(1)
		{
			struct ast *span_node;

			NN(AS_RETURN);
			span_node = new_node(AS_SIMPLE_NAME);
			span_node->links.val = IND(0);
			set_span(span_node,LOC(0));
			NAST4(opt_expression,opt_node,opt_node,span_node);
		}
#undef opt_expression

		break;
		/* goto_statement ::= GOTO label_name ; */
	case 177 :
#define name AST(1)
		NN(AS_GOTO);
		NAST1(name);
#undef name

		break;
		/* subprogram_declaration ::= subprogram_specification ; */
	case 178 :
#define subprog_spec AST(0)
		NN(AS_SUBPROGRAM_DECL);
		NAST1(subprog_spec);
#undef subprog_spec

		break;
		/* subprogram_specification ::= PROCEDURE identifier [formal_part] */
	case 179 :
#define opt_formal AST(2)
		make_id(1);
		NN(AS_PROCEDURE);
		NAST3(id_node,opt_formal,opt_node);
#undef opt_formal

		break;
		/* subprogram_specification ::=
			FUNCTION designator [formal_part] RETURN typ */
	case 180 :
#define desig AST(1)
#define opt_formal AST(2)
#define type_mark AST(4)
		NN(AS_FUNCTION);
		NAST3(desig,opt_formal,type_mark);
#undef desig
#undef opt_formal
#undef type_mark

		break;
		/* designator ::= identifier */
	case 181 :
#define id IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = id;
		set_span(node,LOC(0));
#undef id

		break;
		/* designator ::= operator_symbol */
	case 182 :
		{
			char tmp[200];

			node = AST(0);
			strcpy(tmp,namelist(node->links.val));
			convtolower(tmp);
			if (!isoverloadable_op(tmp)) {
				char msg[300];

				sprintf(msg,"\"%s\" is not a valid operator_symbol",tmp);
				syntax_err(get_left_span(node),get_right_span(node),msg);
			}
			node->links.val = namemap(tmp,strlen(tmp));
		}

		break;
		/* operator_symbol ::= string_literal */
	case 183 :
#define lit IND(0)
		NN(AS_OPERATOR);
		node->links.val = lit;
		set_span(node,LOC(0));
#undef lit

		break;
		/* formal_part ::=
			( parameter_specification {;parameter_specification} ) */
	case 184 :
#define parm_spec AST(1)
		node = AST(2);
		prepend(parm_spec,node);
#undef parm_spec

		break;
		/* parameter_specification ::=
			identifier_list : mode type_mark [:=expressio */
	case 185 :
#define id_list AST(0)
#define ast_mode AST(2)
#define type_mark AST(3)
#define opt_init AST(4)
		NN(AS_FORMAL);
		NAST4(id_list,ast_mode,type_mark,opt_init);
#undef id_list
#undef ast_mode
#undef type_mark
#undef opt_init

		/* mode ::= [IN] */
		/* case 186 : */

		break;
		/* mode ::= IN OUT */
	case 187 :
		NN(AS_MODE);
		node->links.val = namemap("inout",5);
		set_span(node,LOC(0));

		break;
		/* mode ::= OUT */
	case 188 :
		NN(AS_MODE);
		node->links.val = namemap("out",3);
		set_span(node,LOC(0));

		break;
		/* subprogram_body ::=
			subprogram_specification IS declarative_part BEGIN se */
	case 189 :
#define sub_spec AST(0)
#define decl_part AST(2)
#define stmts AST(4)
#define opt_except_list AST(5)
#define opt_desig AST(7)
		{
			struct ast *labs_node;

#define desig1 (sub_spec->links.subast)[0]->links.val
#define desig2 opt_desig->links.val
			if (opt_desig != opt_node && desig1 != desig2)
				match_error(desig1,desig2,"subprogram_body",
				  get_left_span(sub_spec),get_right_span(opt_desig));
#undef desig1
#undef desig2
			nodefree(opt_desig);
			labs_node = new_node(AS_LABELS);
			labs_node->links.list = remove_duplicate_labels(
			  concatl(copylist(getlabels(stmts)),
			  ((opt_except_list == opt_node) ? (struct two_pool *)0 : 
			  copylist(getlabels(opt_except_list)))));
			append(decl_part,labs_node);
			NN(AS_SUBPROGRAM);
			NAST4(sub_spec,decl_part,stmts,opt_except_list);
		}
#undef sub_spec
#undef decl_part
#undef stmts
#undef opt_except_list
#undef opt_desig

		break;
		/* call_statement ::= name ; */
	case 190 :
#define proc_name AST(0)
		NN(AS_CALL);
		NAST1(proc_name);
#undef proc_name

		break;
		/* package_declaration ::= package_specification ; */
	case 191 :
		node = AST(0);

		break;
		/* package_specification ::=
			PACKAGE identifier IS {basic_declarative_item}  */
	case 192 :
#define id IND(1)
#define decl_node AST(3)
#define opt_private_part AST(4)
#define opt_simple_name AST(6)
		if (opt_simple_name != opt_node && id != opt_simple_name->links.val)
			match_error(id,opt_simple_name->links.val,"package_specification",
			  get_left_span(opt_simple_name),get_right_span(opt_simple_name));
		make_id(1);
		LLOOPTOP(decl_node->links.list,tmp)
		    if (isbody_node[tmp->val.node->kind])
			syntax_err(SPAN(tmp->val.node),
			  "Body declaration not allowed in package_specification");
		LLOOPBOTTOM(tmp)
		decl_node->kind = AS_DECLARATIONS;
		ins_as_line_no(decl_node);
		NN(AS_PACKAGE_SPEC);
		NAST3(id_node,decl_node,opt_private_part);
		nodefree(opt_simple_name);
#undef id
#undef decl_node
#undef opt_private_part
#undef opt_simple_name

		break;
		/* package_body ::=
			PACKAGE BODY package_simple_name IS declarative_part END */
	case 193 :
#define simple_name1 AST(2)
#define decl_part AST(4)
#define opt_simple_name2 AST(6)
		if (opt_simple_name2 != opt_node
		  && simple_name1->links.val != opt_simple_name2->links.val)
			match_error(simple_name1->links.val,opt_simple_name2->links.val,
			  "package_body",get_left_span(opt_simple_name2),
			  get_right_span(opt_simple_name2));
		NN(AS_PACKAGE_BODY);
		NAST4(simple_name1,decl_part,opt_node,opt_node);
		nodefree(opt_simple_name2);
#undef simple_name1
#undef decl_part
#undef opt_simple_name2

		break;
		/* package_body ::=
			PACKAGE BODY package_simple_name IS declarative_part BEG */
	case 194 :
#define simple_name1 AST(2)
#define decl_part AST(4)
#define stmts AST(6)
#define opt_except_list AST(7)
#define opt_simple_name2 AST(9)
		{
			struct ast *labs_node;

			if (opt_simple_name2 != opt_node
			  && simple_name1->links.val != opt_simple_name2->links.val)
				match_error(simple_name1->links.val,opt_simple_name2->links.val,
				  "package_body",get_left_span(opt_simple_name2),
				  get_right_span(opt_simple_name2));
			labs_node = new_node(AS_LABELS);
			labs_node->links.list = remove_duplicate_labels(
			  concatl(copylist(getlabels(stmts)),
			  ((opt_except_list == opt_node) ? (struct two_pool *)0 : 
			  copylist(getlabels(opt_except_list)))));
			append(decl_part,labs_node);
			NN(AS_PACKAGE_BODY);
			NAST4(simple_name1,decl_part,stmts,opt_except_list);
			nodefree(opt_simple_name2);
		}
#undef simple_name1
#undef decl_part
#undef stmts
#undef opt_except_list
#undef opt_simple_name2

		break;
		/* private_type_declaration ::=
			TYPE identifier [discriminant_part]IS [LIMIT */
	case 195 :
#define opt_discr_part AST(2)
#define opt_limited AST(3)
		{
			struct ast *kind_node;

			make_id(1);
			kind_node = new_node(AS_SIMPLE_NAME);
			kind_node->links.val =
			  (opt_limited == opt_node) ? namemap("private",7) :
			  namemap("limited_private",15);
			set_span(kind_node,
			  (opt_limited==opt_node ? LOC(4) : &opt_limited->span));
			NN(AS_PRIVATE_DECL);
			NAST3(id_node,opt_discr_part,kind_node);
		}
#undef opt_discr_part
#undef opt_limited

		break;
		/* use_clause ::= USE package_name {,package_name} ; */
	case 196 :
#define pack_name AST(1)
		node = AST(2);
		prepend(pack_name,node);
		node->kind = AS_USE;
#undef pack_name

		break;
		/* renaming_declaration ::= identifier:type_mark RENAMES object_name; */
	case 197 :
#define name AST(2)
		node = AST(0);
		(node->links.subast)[2] = name;
#undef name

		break;
		/* renaming_declaration ::=
			identifier:EXCEPTION RENAMES exception_name; */
	case 198 :
#define name AST(2)
		node = AST(0);
		(node->links.subast)[1] = name;
#undef name

		break;
		/* renaming_declaration ::= PACKAGE identifier RENAMES package_name ; */
	case 199 :
#define name AST(3)
		make_id(1);
		NN(AS_RENAME_PACK);
		NAST2(id_node,name);
#undef name

		break;
		/* renaming_declaration ::=
			subprogram_specification RENAMES subprogram_or_e */
	case 200 :
#define sub_spec AST(0)
#define name AST(2)
		NN(AS_RENAME_SUB);
		NAST2(sub_spec,name);
#undef sub_spec
#undef name

		break;
		/* task_declaration ::= task_specification ; */
	case 201 :
		node = AST(0);

		break;
		/* task_specification ::= TASK [TYPE] identifier */
	case 202 :
#define opt_type AST(1)
		{
			struct ast *entry_decl_list, *repr_clause_list;

			make_id(2);
			NN((opt_type == opt_node) ? AS_TASK_SPEC :
			  AS_TASK_TYPE_SPEC);
			entry_decl_list = new_node(AS_LIST);
			repr_clause_list = new_node(AS_LIST);
			entry_decl_list->links.list = NULL;
			repr_clause_list->links.list = NULL;
			set_span(entry_decl_list,&curtok->ptr.token->loc);
			set_span(repr_clause_list,&curtok->ptr.token->loc);
			NAST3(id_node,entry_decl_list,repr_clause_list);
		}
#undef opt_type

		break;
		/* task_specification ::=
			TASK [TYPE] identifier IS {entry_declaration} {rep */
	case 203 :
#define opt_type AST(1)
#define id IND(2)
#define entry_decl_list AST(4)
#define repr_clause_list AST(5)
#define opt_simple_name AST(7)
		if (opt_simple_name != opt_node && opt_simple_name->links.val != id)
			match_error(id,opt_simple_name->links.val,"task_specification",
			  get_left_span(opt_simple_name),get_right_span(opt_simple_name));
		make_id(2);
		NN((opt_type == opt_node) ? AS_TASK_SPEC : AS_TASK_TYPE_SPEC);
		NAST3(id_node,entry_decl_list,repr_clause_list);
		ins_as_line_no(entry_decl_list);
		nodefree(opt_simple_name);
#undef id
#undef entry_decl_list
#undef repr_clause_list
#undef opt_simple_name

		break;
		/* task_body ::=
			TASK BODY task_simple_name IS declarative_part BEGIN sequen */
	case 204 :
#define simple_name1 AST(2)
#define decl_part AST(4)
#define stmts AST(6)
#define opt_except_list AST(7)
#define opt_simple_name2 AST(9)
		{
			struct ast *labs_node;

			if (opt_simple_name2 != opt_node
			  && simple_name1->links.val != opt_simple_name2->links.val)
				match_error(simple_name1->links.val,opt_simple_name2->links.val,
				  "task_body",get_left_span(opt_simple_name2),
				  get_right_span(opt_simple_name2));
			labs_node = new_node(AS_LABELS);
			labs_node->links.list = remove_duplicate_labels(
			  concatl(copylist(getlabels(stmts)),
			  ((opt_except_list == opt_node) ? (struct two_pool *)0 : 
			  copylist(getlabels(opt_except_list)))));
			append(decl_part,labs_node);
			NN(AS_TASK);
			NAST4(simple_name1,decl_part,stmts,opt_except_list);
			nodefree(opt_simple_name2);
		}
#undef simple_name1
#undef decl_part
#undef stmts
#undef opt_except_list
#undef opt_simple_name2

		break;
		/* entry_declaration ::=
			ENTRY identifier [(discrete_range)][formal_part] ; */
	case 205 :
		node = AST(2);
		make_id(1);
		(node->links.subast)[0] = id_node;

		break;
		/* accept_statement ::=
			ACCEPT entry_simple_name [(entry_index)][formal_part */
	case 206 :
#define simple_name AST(1)
		node = AST(2);
		erase_labels(node);
		(node->links.subast)[0] = simple_name;
		(node->links.subast)[3] = opt_node;
#undef simple_name

		break;
		/* accept_statement ::=
			ACCEPT entry_simple_name [(entry_index)][formal_part */
	case 207 :
#define simple_name1 AST(1)
#define stmts AST(4)
#define opt_simple_name2 AST(6)
		node = AST(2);
		if (opt_simple_name2 != opt_node
		  && simple_name1->links.val != opt_simple_name2->links.val)
			match_error(simple_name1->links.val,opt_simple_name2->links.val,
			  "accept_statement",get_left_span(opt_simple_name2),
			  get_right_span(opt_simple_name2));
		newlabels(node,copylist(getlabels(stmts)));
		(node->links.subast)[0] = simple_name1;
		(node->links.subast)[3] = stmts;
		nodefree(opt_simple_name2);
#undef simple_name1
#undef stmts
#undef opt_simple_name2

		/* entry_index ::= expression */
		/* case 208 : */

		break;
		/* delay_statement ::= DELAY simple_expression ; */
	case 209 :
#define simple_expr AST(1)
		NN(AS_DELAY);
		NAST1(simple_expr);
#undef simple_expr

		/* select_statement ::= selective_wait */
		/* case 210 : */

		/* select_statement ::= conditional_entry_call */
		/* case 211 : */

		/* select_statement ::= timed_entry_call */
		/* case 212 : */

		break;
		/* selective_wait ::=
			SELECT {pragma} select_alternative {OR__select_alterna */
	case 213 :
#define pragma_node AST(1)
#define alt AST(2)
#define alt_node AST(3)
#define opt_stmts AST(4)
		{
			struct two_pool *nodelabels = NULL;
			int delay_index = 0, terminate_index = 0, has_accept = 0, i = 0;
			int terminate_ct = 0;
			struct ast *delay_ptr = NULL, *terminate_ptr = NULL, *tmp_alt;

			NN(AS_SELECTIVE_WAIT);
			check_pragmas(pragma_node,null_pragmas);
			alt_node->links.list = concatl3(pragma_node->links.list,
			  initlist(alt), alt_node->links.list);
			LLOOPTOP(alt_node->links.list,tmp)
			    nodelabels = concatl(nodelabels,
				  copylist(getlabels(tmp->val.node)));
			LLOOPBOTTOM(tmp)
			if (opt_stmts != opt_node)
			  nodelabels = concatl(nodelabels,copylist(getlabels(opt_stmts)));
			newlabels(node,nodelabels);
			NAST2(alt_node,opt_stmts);
			nodefree(pragma_node);

			LLOOPTOP(alt_node->links.list,tmp)
			    i++;
				if ((tmp_alt = tmp->val.node)->kind == AS_GUARD)
					tmp_alt = (tmp_alt->links.subast)[1];
				if (tmp_alt->kind == AS_DELAY_ALT) {
					delay_index = i;
					delay_ptr = tmp_alt;
				}
				else if (tmp_alt->kind == AS_TERMINATE_ALT) {
					terminate_index = i;
					terminate_ptr = tmp_alt;
					if (++terminate_ct > 1)
						syntax_err(SPAN(terminate_ptr),
			 "Only one terminate alternative can appear in a SELECT statement");
				}
				else
					has_accept = 1;
			LLOOPBOTTOM(tmp)
			if (delay_index && terminate_index) {
				tmp_alt = (delay_index > terminate_index) ? delay_ptr : 
				  terminate_ptr;
				syntax_err(SPAN(tmp_alt),
"Delay and terminate alternatives cannot appear in the same SELECT statement");
			}
			if ((delay_index || terminate_index) && opt_stmts != opt_node)
				syntax_err(SPAN(opt_stmts),
		"ELSE part cannot appear in SELECT statement if delay or terminate \
alternatives are present");

			/* A selective_wait must contain at least one accept_alternative */
			if (!has_accept)
				syntax_err(LOC(0),END_LOC(6),
				  "SELECT statement must have at least one ACCEPT alternative");
		}
#undef pragma_node
#undef alt
#undef alt_node
#undef opt_stmts

		break;
		/* select_alternative ::=
			[WHEN__condition=>] selective_wait_alternative */
	case 214 :
#define condition AST(0)
#define alt AST(1)
		if (condition == opt_node)
			node = alt;
		else {
			NN(AS_GUARD);
			newlabels(node,copylist(getlabels(alt)));
			NAST2(condition,alt);
		}
#undef condition
#undef alt

		/* selective_wait_alternative ::= accept_alternative */
		/* case 215 : */

		/* selective_wait_alternative ::= delay_alternative */
		/* case 216 : */

		/* selective_wait_alternative ::= terminate_alternative */
		/* case 217 : */

		break;
		/* accept_alternative ::= accept_statement [sequence_of_statements] */
	case 218 :
#define accept_stmt AST(0)
#define opt_stmts AST(1)
		NN(AS_ACCEPT_ALT);
		/*newlabels(node,concatl(copylist(getlabels(accept_stmt)),
        ((opt_stmts == opt_node) ? LASTARG : 
        copylist(getlabels(opt_stmts)))));*/
		if (opt_stmts!=opt_node) {
			newlabels(node,concatl(copylist(getlabels(accept_stmt)),
			  copylist(getlabels(opt_stmts))));
		}
		else {
			newlabels(node,copylist(getlabels(accept_stmt)));
		}
		NAST2(accept_stmt,opt_stmts);
#undef accept_stmt
#undef opt_stmts

		break;
		/* delay_alternative ::= delay_statement [sequence_of_statements] */
	case 219 :
#define delay_stmt AST(0)
#define opt_stmts AST(1)
		NN(AS_DELAY_ALT);
		if (opt_stmts != opt_node)
			newlabels(node,copylist(getlabels(opt_stmts)));
		NAST2(delay_stmt,opt_stmts);
#undef delay_stmt
#undef opt_stmts

		break;
		/* terminate_alternative ::= TERMINATE ; {pragma} */
	case 220 :
		node = AST(2);
		check_pragmas(node,null_pragmas);
		erase_labels(node);
		node->kind = AS_TERMINATE_ALT;

		break;
		/* conditional_entry_call ::=
			SELECT {pragma} call_statement [sequence_of_st */
	case 221 :
#define pragma_node AST(1)
#define call_stmt AST(2)
#define opt_stmts AST(3)
#define else_stmts AST(5)
		NN(AS_CONDITIONAL_ENTRY_CALL);
		check_pragmas(pragma_node,null_pragmas);
		pragmalist_warning(pragma_node);
		newlabels(node,concatl(((opt_stmts == opt_node) ? (struct two_pool *)0 :
		  copylist(getlabels(opt_stmts))),copylist(getlabels(else_stmts))));
		NAST3(call_stmt,opt_stmts,else_stmts);
		free_everything(pragma_node);
#undef pragma_node
#undef call_stmt
#undef opt_stmts
#undef else_stmts

		break;
		/* timed_entry_call ::=
			SELECT {pragma} call_statement [sequence_of_statemen */
	case 222 :
#define pragma_node1 AST(1)
#define call_stmt AST(2)
#define opt_stmts AST(3)
#define pragma_node2 AST(5)
#define delay_alt AST(6)
		NN(AS_TIMED_ENTRY_CALL);
		check_pragmas(pragma_node1,null_pragmas);
		check_pragmas(pragma_node2,null_pragmas);
		pragmalist_warning(pragma_node1);
		pragmalist_warning(pragma_node2);
		newlabels(node,concatl(((opt_stmts == opt_node) ? (struct two_pool *)0 :
		  copylist(getlabels(opt_stmts))),copylist(getlabels(delay_alt))));
		free_everything(pragma_node1);
		free_everything(pragma_node2);
		NAST3(call_stmt,opt_stmts,delay_alt);
#undef pragma_node1
#undef call_stmt
#undef opt_stmts
#undef pragma_node2
#undef delay_alt

		break;
		/* abort_statement ::= ABORT task_name {,task_name} ; */
	case 223 :
#define name AST(1)
		node = AST(2);
		prepend(name,node);
		node->kind = AS_ABORT;
#undef name

		/* compilation ::= {compilation_unit} */
		/* case 224 : */

		break;
		/* compilation_unit ::= context_clause library_unit */
	case 225 :
#define context_clause AST(0)
#define library_unit AST(1)
		NN(AS_UNIT);
		NAST2(context_clause,library_unit);
#undef context_clause
#undef library_unit

		break;
		/* compilation_unit ::= context_clause secondary_unit */
	case 226 :
#define context_clause AST(0)
#define secondary_unit AST(1)
		NN(AS_UNIT);
		NAST2(context_clause,secondary_unit);
#undef context_clause
#undef secondary_unit

		/* library_unit ::= subprogram_declaration */
		/* case 227 : */

		/* library_unit ::= package_declaration */
		/* case 228 : */

		/* library_unit ::= generic_declaration */
		/* case 229 : */

		/* library_unit ::= generic_instantiation */
		/* case 230 : */

		/* library_unit ::= subprogram_body */
		/* case 231 : */

		/* secondary_unit ::= library_unit_body */
		/* case 232 : */

		/* secondary_unit ::= subunit */
		/* case 233 : */

		/* library_unit_body ::= package_body */
		/* case 234 : */

		/* context_clause ::= {with_clause{use_clause}} */
		/* case 235 : */

		break;
		/* with_clause ::= WITH unit_simple_name {,unit_simple_name} ; */
	case 236 :
#define simple_name AST(1)
		node = AST(2);
		prepend(simple_name,node);
		node->kind = AS_WITH;
#undef simple_name

		break;
		/* body_stub ::= subprogram_specification IS SEPARATE ; */
	case 237 :
#define sub_spec AST(0)
		NN(AS_SUBPROGRAM_STUB);
		NAST1(sub_spec);
#undef sub_spec

		break;
		/* body_stub ::= PACKAGE BODY package_simple_name IS SEPARATE ; */
	case 238 :
		node = AST(2);
		node->kind = AS_PACKAGE_STUB;

		break;
		/* body_stub ::= TASK BODY task_simple_name IS SEPARATE ; */
	case 239 :
		node = AST(2);
		node->kind = AS_TASK_STUB;

		break;
		/* subunit ::= SEPARATE ( parent_unit_name ) proper_body */
	case 240 :
#define name AST(2)
#define body AST(4)
		NN(AS_SEPARATE);
		NAST2(name,body);
#undef name
#undef body

		break;
		/* exception_declaration ::= identifier_list : EXCEPTION ; */
	case 241 :
		node = AST(0);
		node->kind = AS_EXCEPT_DECL;

		break;
		/* exception_handler ::=
			WHEN exception_choice {|exception_choice} => sequen */
	case 242 :
#define choice AST(1)
#define choice_node AST(2)
#define stmts AST(4)
		NN(AS_HANDLER);
		prepend(choice,choice_node);
		newlabels(node,copylist(getlabels(stmts)));
		NAST2(choice_node,stmts);
#undef choice
#undef choice_node
#undef stmts

		/* exception_choice ::= exception_name */
		/* case 243 : */

		break;
		/* exception_choice ::= OTHERS */
	case 244 :
		NN(AS_OTHERS);
		set_span(node,LOC(0));

		break;
		/* raise_statement ::= RAISE [exception_name] ; */
	case 245 :
#define opt_name AST(1)
		{
			struct ast *span_node;

			NN(AS_RAISE);
			span_node = new_node(AS_SIMPLE_NAME);
			span_node->links.val = IND(0);
			set_span(span_node,LOC(0));
			NAST2(opt_name,span_node);
		}
#undef opt_name

		break;
		/* generic_declaration ::= generic_specification ; */
	case 246 :
		node = AST(0);

		break;
		/* generic_specification ::=
			generic_formal_part subprogram_specification */
	case 247 :
#define generic_part AST(0)
#define sub_spec AST(1)
#define id_node (sub_spec->links.subast)[0]
#define formals (sub_spec->links.subast)[1]
#define ret (sub_spec->links.subast)[2]
		if (sub_spec->kind == AS_FUNCTION) {
			if (id_node->kind == AS_OPERATOR)
				syntax_err(SPAN(id_node),
				  "Operator symbol invalid in Generic specification");
			NN(AS_GENERIC_FUNCTION);
			NAST4(id_node,generic_part,formals,ret);
		}
		else {
			NN(AS_GENERIC_PROCEDURE);
			NAST4(id_node,generic_part,formals,opt_node);
		}
		astfree(sub_spec->links.subast);
		nodefree(sub_spec);
#undef generic_part
#undef sub_spec
#undef id_node
#undef formals
#undef ret

		break;
		/* generic_specification ::= generic_formal_part package_specification*/
	case 248 :
#define generic_part AST(0)
#define pack_spec AST(1)
#define id_node (pack_spec->links.subast)[0]
#define decls (pack_spec->links.subast)[1]
#define opt_priv_part (pack_spec->links.subast)[2]
		NN(AS_GENERIC_PACKAGE);
		NAST4(id_node,generic_part,decls,opt_priv_part);
		astfree(pack_spec->links.subast);
		nodefree(pack_spec);
#undef generic_part
#undef pack_spec
#undef id_node
#undef decls
#undef opt_priv_part

		break;
		/* generic_formal_part ::= GENERIC {generic_parameter_declaration} */
	case 249 :
		node = AST(1);
		node->kind = AS_GENERIC_FORMALS;

		break;
		/* generic_parameter_declaration ::=
			identifier_list : [IN[OUT]] type_mark [ */
	case 250 :
#define id_list AST(0)
#define ast_mode AST(2)
#define type_mark AST(3)
#define opt_init AST(4)
		NN(AS_GENERIC_OBJ);
		NAST4(id_list,ast_mode,type_mark,opt_init);
#undef id_list
#undef ast_mode
#undef type_mark
#undef opt_init

		break;
		/* generic_parameter_declaration ::=
			TYPE identifier IS generic_type_definit */
	case 251 :
#define type_def AST(3)
		make_id(1);
		NN(AS_GENERIC_TYPE);
		NAST3(id_node,opt_node,type_def);
#undef type_def

		break;
		/* generic_parameter_declaration ::= private_type_declaration */
	case 252 :
		node = AST(0);
		node->kind = AS_GEN_PRIV_TYPE;

		break;
		/* generic_parameter_declaration ::=
			WITH subprogram_specification [IS__name */
	case 253 :
#define sub_spec AST(1)
#define opt_is_part AST(2)
		NN(AS_GENERIC_SUBP);
		NAST2(sub_spec,opt_is_part);
#undef sub_spec
#undef opt_is_part

		break;
		/* generic_type_definition ::= ( <> ) */
	case 254 :
		NN(AS_GENERIC);
		node->links.val = namemap("discrete_type",13);
		set_span(node,LOC(0));

		break;
		/* generic_type_definition ::= RANGE <> */
	case 255 :
		NN(AS_GENERIC);
		node->links.val = namemap("INTEGER",7);
		set_span(node,LOC(0));

		break;
		/* generic_type_definition ::= DIGITS <> */
	case 256 :
		NN(AS_GENERIC);
		node->links.val = namemap("FLOAT",5);
		set_span(node,LOC(0));

		break;
		/* generic_type_definition ::= DELTA <> */
	case 257 :
		NN(AS_GENERIC);
		node->links.val = namemap("$FIXED",6);
		set_span(node,LOC(0));

		/* generic_type_definition ::= array_type_definition */
		/* case 258 : */

		/* generic_type_definition ::= access_type_definition */
		/* case 259 : */

		break;
		/* generic_instantiation ::=
			PACKAGE identifier IS NEW generic_package_name  */
	case 260 :
#define pack_name AST(4)
#define actual_part AST(5)
		make_id(1);
		NN(AS_PACKAGE_INSTANCE);
		NAST3(id_node,pack_name,actual_part);
#undef pack_name
#undef actual_part

		break;
		/* generic_instantiation ::=
			FUNCTION designator IS NEW generic_function_nam */
	case 261 :
#define desig AST(1)
#define func_name AST(4)
#define actual_part AST(5)
		NN(AS_FUNCTION_INSTANCE);
		NAST3(desig,func_name,actual_part);
#undef desig
#undef func_name
#undef actual_part

		break;
		/* generic_instantiation ::=
			subprogram_specification IS NEW generic_procedu */
	case 262 :
#define sub_spec AST(0)
#define proc_name AST(3)
#define actual_part AST(4)
#define id (sub_spec->links.subast)[0]
#define opt_formal (sub_spec->links.subast)[1]
		if (sub_spec->kind != AS_PROCEDURE)
			syntax_err(SPAN(sub_spec),"Bad generic procedure instantiation");
		if (opt_formal != opt_node)
			syntax_err(SPAN(sub_spec),
			  "formal_part not allowed in procedure instantiation");
		NN(AS_PROCEDURE_INSTANCE);
		NAST3(id,proc_name,actual_part);
		FREEAST(sub_spec,1);
		astfree(sub_spec->links.subast);
		nodefree(sub_spec);
#undef sub_spec
#undef proc_name
#undef actual_part
#undef id
#undef opt_formal

		break;
		/* generic_actual_part ::=
			( generic_association {,generic_association} ) */
	case 263 :
#define assoc AST(1)
		node = AST(2);
		prepend(assoc,node);
#undef assoc

		/* generic_association ::=
			[generic_formal_parameter=>]generic_actual_parame */
		/* case 264 : */

		/* generic_formal_parameter ::= parameter_simple_name */
		/* case 265 : */

		break;
		/* generic_formal_parameter ::= operator_symbol */
	case 266 :
		{
			char tmp[MAXLINE + 1];

			node = AST(0);
			strcpy(tmp,namelist(node->links.val));
			convtolower(tmp);
			if (!isoverloadable_op(tmp)) {
				char msg[MAXLINE + 30];

				sprintf(msg,"\"%s\" is not a valid operator_symbol",tmp);
				syntax_err(get_left_span(node),get_right_span(node),msg);
			}
			node->links.val = namemap(tmp,strlen(tmp));
		}

		/* generic_actual_parameter ::= expression */
		/* case 267 : */

		break;
		/* representation_clause ::= type_representation_clause */
	case 268 :
		node = AST(0);
		break;
		/* representation_clause ::= address_clause */
	case 269 :
		node = AST(0);
		syntax_err(SPAN(node),"address_clause not supported");

		/* type_representation_clause ::= length_clause */
		/* case 270 : */

		/* type_representation_clause ::= enumeration_representation_clause */
		/* case 271 : */

		/* type_representation_clause ::= record_representation_clause */
		/* case 272 : */

		break;
		/* length_clause ::= FOR attribute USE simple_expression ; */
	case 273 :
#define attribute AST(1)
#define simple_expr AST(3)
		NN(AS_LENGTH_CLAUSE);
		NAST2(attribute,simple_expr);
#undef attribute
#undef simple_expr

		break;
		/* enumeration_representation_clause ::=
			FOR type_simple_name USE aggregate  */
	case 274 :
#define simple_name AST(1)
#define aggregate AST(3)
		NN(AS_ENUM_REP_CLAUSE);
		NAST2(simple_name,aggregate);
#undef simple_name
#undef aggregate

		break;
		/* record_representation_clause ::=
			FOR type_simple_name USE RECORD [alignme */
	case 275 :
#define simple_name AST(1)
#define opt_align_clause AST(4)
#define comp_clause_list AST(5)
		NN(AS_REC_REP_CLAUSE);
		NAST3(simple_name,opt_align_clause,comp_clause_list);
#undef simple_name
#undef opt_align_clause
#undef comp_clause_list

		break;
		/* alignment_clause ::= AT MOD static_simple_expression ; */
	case 276 :
		node = AST(2);

		break;
		/* component_clause ::=
			component_name AT static_simple_expression RANGE sta */
	case 277 :
#define name AST(0)
#define simple_expr AST(2)
#define ast_range AST(4)
		NN(AS_COMPON_CLAUSE);
		if(ast_range->kind != AS_RANGE && ast_range->kind != AS_RANGE_ATTRIBUTE)
			syntax_err(SPAN(ast_range),"Invalid range specification");
		NAST3(name,simple_expr,ast_range);
#undef name
#undef simple_expr
#undef ast_range

		break;
		/* address_clause ::= FOR simple_name USE AT simple_expression ; */
	case 278 :
#define simple_name AST(1)
#define simple_expr AST(4)
		NN(AS_ADDRESS_CLAUSE);
		NAST2(simple_name,simple_expr);
#undef simple_name
#undef simple_expr

		break;
		/* code_statement ::= name ' record_aggregate ; */
	case 279 :
#define name AST(0)
#define aggregate AST(2)
		if (!check_expanded_name(name))
			syntax_err(SPAN(name),"Invalid type_mark in code statement");
		NN(AS_CODE);
		NAST2(name,aggregate);
#undef name
#undef aggregate


		break;
		/* {PRAGMA} ::= empty */
	case 280 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {PRAGMA} ::= {pragma} pragma */
	case 281 :
#define pragma AST(1)
		node = AST(0);
		if (pragma != any_node)
			append(node,pragma);
#undef pragma


		break;
		/* [(argument_association{,argument_association})] ::= empty */
	case 282 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [(argument_association{,argument_association})] ::=
			( argument_associatio */
	case 283 :
		node = AST(1);

		break;
		/* argument_association_list ::= argument_association */
	case 284 :
#define arg_assoc AST(0)
		NN(AS_ARG_ASSOC_LIST);
		node->links.list = initlist(arg_assoc);
#undef arg_assoc

		break;
		/* argument_association_list ::=
			argument_association_list , argument_associ */
	case 285 :
#define arg_assoc AST(2)
		node = AST(0);
		append(node,arg_assoc);
#undef arg_assoc

		break;
		/* [argument_identifier=>]expression ::= expression */
	case 286 :
#define expression AST(0)
		NN(AS_ARG);
		NAST2(opt_node,expression);
#undef expression

		break;
		/* [argument_identifier=
			]expression ::= argument_identifier => expression */
	case 287 :

#define expression AST(2)
		make_id(0);
		NN(AS_ARG);
		NAST2(id_node,expression);
#undef expression


		break;
		/* [:=expression] ::= empty */
	case 288 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [:=expression] ::= := expression */
	case 289 :
		node = AST(1);


		break;
		/* [CONSTANT] ::= empty */
	case 290 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [CONSTANT] ::= CONSTANT */
	case 291 :
		node = any_node;


		break;
		/* {,identifier} ::= empty */
	case 292 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,identifier} ::= {,identifier} , identifier */
	case 293 :
		node = AST(0);
		make_id(2);
		append(node,id_node);


		break;
		/* [discriminant_part]IS ::= IS */
	case 294 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [discriminant_part]IS ::= discriminant_part IS */
	case 295 :
		node = AST(0);


		break;
		/* [constraint] ::= empty */
	case 296 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [constraint] ::= constraint */
		/* case 297 : */

		break;
		/* expanded_name ::= identifier */
	case 298 :
#define id IND(0)
		NN(AS_SIMPLE_NAME);
		node->links.val = id;
		set_span(node,LOC(0));
#undef id

		break;
		/* expanded_name ::= expanded_name . identifier */
	case 299 :
#define expanded_name AST(0)
		make_id(2);
		NN(AS_SELECTOR);
		NAST2(expanded_name,id_node);
#undef expanded_name


		break;
		/* {,enumeration_literal_specification} ::= empty */
	case 300 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,enumeration_literal_specification} ::=
			{,enumeration_literal_specificat */
	case 301 :
#define enum_lit AST(2)
		node = AST(0);
		append(node,enum_lit);
#undef enum_lit


		break;
		/* [range_constraint] ::= empty */
	case 302 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [range_constraint] ::= range_constraint */
		/* case 303 : */


		break;
		/* {,index_subtype_definition} ::= empty */
	case 304 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,index_subtype_definition} ::=
			{,index_subtype_definition} , index_subty */
	case 305 :
#define ndex AST(2)
		node = AST(0);
		append(node,ndex);
#undef ndex


		break;
		/* {,discrete_range} ::= empty */
	case 306 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,discrete_range} ::= {,discrete_range} , discrete_range */
	case 307 :
#define discrete_range AST(2)
		node = AST(0);
		append(node,discrete_range);
#undef discrete_range


		break;
		/* {component_declaration} ::= empty */
	case 308 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {component_declaration} ::=
			{component_declaration} component_declaration */
	case 309 :
#define comp_dec AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,null_pragmas);
		node->links.list = concatl3(node->links.list,initlist(comp_dec),
		  pragma_node->links.list);
		nodefree(pragma_node);
#undef comp_dec
#undef pragma_node    


		break;
		/* {;discriminant_specification} ::= empty */
	case 310 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {;discriminant_specification} ::=
			{;discriminant_specification} ; discrim */
	case 311 :
#define discr_spec AST(2)
		node = AST(0);
		append(node,discr_spec);
#undef discr_spec


		break;
		/* {variant} ::= empty */
	case 312 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {variant} ::= {variant} variant */
	case 313 :
#define variant AST(1)
		node = AST(0);
		append(node,variant);
#undef variant

		break;
		/* {|choice} ::= empty */
	case 314 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {|choice} ::= {|choice} '|' choice */
	case 315 :
#define choice AST(2)
		node = AST(0);
		append(node,choice);
#undef choice


		break;
		/* [discriminant_part]; ::= ; */
	case 316 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [discriminant_part]; ::= discriminant_part ; */
	case 317 :
		node  = AST(0);

		break;
		/* {basic_declarative_item} ::= {pragma} */
	case 318 :
#define pragma_node AST(0)
		check_pragmas(pragma_node,immediate_decl_pragmas);
		node = pragma_node;
#undef pragma_node

		break;
		/* {basic_declarative_item} ::=
			{basic_declarative_item} basic_declarative_i */
	case 319 :
#define basic_decl AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,immediate_decl_pragmas);
		node->links.list = concatl3(node->links.list,initlist(basic_decl),
		  pragma_node->links.list);
		nodefree(pragma_node);
#undef basic_decl
#undef pragma_node


		break;
		/* {,component_association} ::= empty */
	case 320 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,component_association} ::=
			{,component_association} , component_associa */
	case 321 :
#define comp_assoc AST(2)
		node = AST(0);
		append(node,comp_assoc);
#undef comp_assoc

		/* [choice{|choice}=>]expression ::= expression */
		/* case 322 : */

		break;
		/* [choice{|choice}=>]expression ::= choice {|choice} => expression */
	case 323 :
#define choice AST(0)
#define choice_node AST(1)
#define expression AST(3)
		prepend(choice,choice_node);
		NN(AS_CHOICE_LIST);
		NAST2(choice_node,expression);
#undef choice
#undef choice_node
#undef expression


		break;
		/* {,general_component_association} ::= empty */
	case 324 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,general_component_association} ::=
			{,general_component_association} , g */
	case 325 :
#define gen_comp_assoc AST(2)
		node = AST(0);
		append(node,gen_comp_assoc);
#undef gen_comp_assoc

		break;
		/* relation{AND__relation} ::= relation AND relation */
	case 326 :

		/* relation{AND__relation} ::= relation{AND__relation} AND relation */
	case 327 :

		/* relation{OR__relation} ::= relation OR relation */
	case 328 :

		/* relation{OR__relation} ::= relation{OR__relation} OR relation */
	case 329 :

		/* relation{XOR__relation} ::= relation XOR relation */
	case 330 :

		/* relation{XOR__relation} ::= relation{XOR__relation} XOR relation */
	case 331 :
#define relation1 AST(0)
#define relation2 AST(2)
		make_id(1);
		node = binary_operator(id_node,relation1,relation2);
#undef relation1
#undef relation2

		break;
		/* relation{AND__THEN__relation} ::= relation AND THEN relation */
	case 332 :

		/* relation{AND__THEN__relation} ::=
			relation{AND__THEN__relation} AND THEN  */
	case 333 :
#define relation1 AST(0)
#define relation2 AST(3)
		{
			struct ast *optr_node;

			optr_node = new_node(AS_SIMPLE_NAME);
			optr_node->links.val = namemap("andthen",7);
			set_span(optr_node,LOC(1));
			node = binary_operator(optr_node,relation1,relation2);
		}
#undef relation1
#undef relation2

		break;
		/* relation{OR__ELSE__relation} ::= relation OR ELSE relation */
	case 334 :

		/* relation{OR__ELSE__relation} ::=
			relation{OR__ELSE__relation} OR ELSE rel */
	case 335 :
#define relation1 AST(0)
#define relation2 AST(3)
		{
			struct ast *optr_node;

			optr_node = new_node(AS_SIMPLE_NAME);
			optr_node->links.val = namemap("orelse",6);
			set_span(optr_node,LOC(1));
			node = binary_operator(optr_node,relation1,relation2);
		}
#undef relation1
#undef relation2


		break;
		/* [relational_operator__simple_expression] ::= empty */
	case 336 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [relational_operator__simple_expression] ::=
			relational_operator simple_e */
	case 337 :
#define optr AST(0)
#define simple_expr AST(1)
		node = binary_operator(optr,any_node,simple_expr);
#undef optr
#undef simple_expr


		break;
		/* [NOT] ::= empty */
	case 338 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [NOT] ::= NOT */
	case 339 :
		node = any_node;

		/* [unary_adding_operator]term{binary_adding_operator__term} ::= term */
		/* case 340 : */

		break;
		/* [unary_adding_operator]term{binary_adding_operator__term} ::=
			unary_addin */
	case 341 :
#define optr AST(0)
#define ast_term AST(1)
		node = unary_operator(optr,ast_term);
#undef optr
#undef ast_term

		break;
		/* [unary_adding_operator]term{binary_adding_operator__term} ::=
			[unary_addi */
	case 342 :
#define expression AST(0)
#define optr AST(1)
#define ast_term AST(2)
		node = binary_operator(optr,expression,ast_term);
#undef expression
#undef optr
#undef ast_term

		/* factor{multiplying_operator__factor} ::= factor */
		/* case 343 : */

		break;
		/* factor{multiplying_operator__factor} ::=
			factor{multiplying_operator__fac */
	case 344 :
#define ast_term AST(0)
#define optr AST(1)
#define factor AST(2)
		node = binary_operator(optr,ast_term,factor);
#undef ast_term
#undef optr
#undef factor


		break;
		/* [**__primary] ::= empty */
	case 345 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [**__primary] ::= ** primary */
	case 346 :
#define primary AST(1)
		make_id(0);
		node = binary_operator(id_node,any_node,primary);
#undef primary

		break;
		/* {statement} ::= {pragma} */
	case 347 :
#define pragma_node AST(0)
		check_pragmas(pragma_node,null_pragmas);
		node = pragma_node;
#undef pragma_node

		break;
		/* {statement} ::= {statement} statement {pragma} */
	case 348 :
#define stmt AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,null_pragmas);
		node->links.list = concatl3(node->links.list,initlist(stmt),
		  pragma_node->links.list);
		nodefree(pragma_node);
#undef stmt
#undef pragma_node


		break;
		/* {label} ::= empty */
	case 349 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {label} ::= {label} label */
	case 350 :
#define label AST(1)
		node = AST(0);
		append(node,label);
#undef label


		break;
		/* {ELSIF__condition__THEN__sequence_of_statements} ::= empty */
	case 351 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {ELSIF__condition__THEN__sequence_of_statements} ::=
			{ELSIF__condition__T */
	case 352 :
#define expression AST(2)
#define stmts AST(4)
		{
			struct ast *if_node;

			node = AST(0);
			if_node = new_node(AS_COND_STATEMENTS);
			if_node->links.subast = new_ast2(expression,stmts);
			append(node,if_node);
		}
#undef expression
#undef stmts


		break;
		/* [ELSE__sequence_of_statements] ::= empty */
	case 353 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [ELSE__sequence_of_statements] ::= ELSE sequence_of_statements */
	case 354 :
		node = AST(1);


		break;
		/* {case_statement_alternative} ::= empty */
	case 355 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {case_statement_alternative} ::=
			{case_statement_alternative} case_statem */
	case 356 :
#define alt AST(1)
		node = AST(0);
		append(node,alt);
#undef alt


		break;
		/* [simple_name:] ::= empty */
	case 357 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [simple_name:] ::= simple_name : */
	case 358 :
		node = AST(0);


		break;
		/* [simple_name] ::= empty */
	case 359 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [simple_name] ::= simple_name */
		/* case 360 : */


		break;
		/* [iteration_scheme] ::= empty */
	case 361 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [iteration_scheme] ::= iteration_scheme */
		/* case 362 : */


		break;
		/* [REVERSE] ::= empty */
	case 363 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [REVERSE] ::= REVERSE */
	case 364 :
		node = any_node;

		break;
		/* [DECLARE__declarative_part] ::= empty */
	case 365 :
		NN(AS_DECLARATIONS);
		node->links.list = NULL;

		break;
		/* [DECLARE__declarative_part] ::= DECLARE declarative_part */
	case 366 :
		node = AST(1);


		break;
		/* [EXCEPTION__exception_handler{exception_handler}] ::= empty */
	case 367 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [EXCEPTION__exception_handler{exception_handler}] ::=
			EXCEPTION {pragma}  */
	case 368 :
#define pragma_node AST(1)
		{
			struct two_pool *nodelabels = NULL;

			node = AST(2);
			check_pragmas(pragma_node,null_pragmas);
			LLOOPTOP(node->links.list,tmp)
			    nodelabels = concatl(nodelabels,
				  copylist(getlabels(tmp->val.node)));
			LLOOPBOTTOM(tmp)
			    newlabels(node,nodelabels);
			node->kind = AS_EXCEPTION;
			node->links.list =concatl(pragma_node->links.list,node->links.list);
			check_choices(node,"an exception_handler list");
			nodefree(pragma_node);
		}
#undef pragma_node

		break;
		/* exception_handler_list ::= exception_handler */
	case 369 :
#define except AST(0)
		NN(AS_LIST);
		node->links.list = initlist(except);
#undef except

		break;
		/* exception_handler_list ::= exception_handler_list exception_handler*/
	case 370 :
#define except AST(1)
		node = AST(0);
		append(node,except);
#undef except


		break;
		/* [expanded_name] ::= empty */
	case 371 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [expanded_name] ::= expanded_name */
		/* case 372 : */


		break;
		/* [WHEN__condition] ::= empty */
	case 373 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [WHEN__condition] ::= WHEN condition */
	case 374 :
		node = AST(1);


		break;
		/* [expression] ::= empty */
	case 375 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [expression] ::= expression */
		/* case 376 : */


		break;
		/* [formal_part] ::= empty */
	case 377 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [formal_part] ::= formal_part */
		/* case 378 : */


		break;
		/* {;parameter_specification} ::= empty */
	case 379 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {;parameter_specification} ::=
			{;parameter_specification} ; parameter_spe */
	case 380 :
#define parm_spec AST(2)
		node = AST(0);
		append(node,parm_spec);
#undef parm_spec

		break;
		/* [IN] ::= empty */
	case 381 :
		NN(AS_MODE);
		node->links.val = namemap("",0);
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [IN] ::= IN */
	case 382 :
		NN(AS_MODE);
		node->links.val = namemap("in",2);
		set_span(node,LOC(0));


		break;
		/* [designator] ::= empty */
	case 383 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [designator] ::= designator */
		/* case 384 : */


		break;
		/* [PRIVATE{basic_declarative_item}] ::= empty */
	case 385 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [PRIVATE{basic_declarative_item}] ::=
			PRIVATE {basic_declarative_item} */
	case 386 :
		node = AST(1);
		LLOOPTOP(node->links.list,tmp)
		    if (isbody_node[tmp->val.node->kind])
				syntax_err(SPAN(tmp->val.node),
	 "Body declaration not allowed in private part of package_specification");
		LLOOPBOTTOM(tmp)
		node->kind = AS_DECLARATIONS;
		ins_as_line_no(node);


		break;
		/* [LIMITED] ::= empty */
	case 387 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [LIMITED] ::= LIMITED */
	case 388 :
		node = any_node;
		set_span(node,LOC(0));


		break;
		/* {,package_name} ::= empty */
	case 389 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,package_name} ::= {,package_name} , package_name */
	case 390 :
#define pack_name AST(2)
		node = AST(0);
		append(node,pack_name);
#undef pack_name

		break;
		/* identifier:type_mark ::= identifier_list : type_mark */
	case 391 :
#define id_list_node AST(0)
#define type_mark AST(2)
		{
			struct two_pool *tmp;

			tmp = id_list_node->links.list->link;
			id_node = tmp->val.node;
			if (tmp != id_list_node->links.list) {
				syntax_err(get_left_span(id_list_node),
				  get_right_span(type_mark),
				  "Only one identifier is allowed in this context");
				id_list_node->links.list->link = tmp->link;
			}
			else
				id_list_node->links.list = NULL;
			TFREE(tmp,tmp);
			free_everything(id_list_node);
			NN(AS_RENAME_OBJ);
			NAST3(id_node,type_mark,any_node);
		}
#undef id_list_node
#undef type_mark

		break;
		/* identifier:EXCEPTION ::= identifier_list : EXCEPTION */
	case 392 :
#define id_list_node AST(0)
		{
			struct two_pool *tmp;

			tmp = id_list_node->links.list->link;
			id_node = tmp->val.node;
			if (tmp != id_list_node->links.list) {
				syntax_err(get_left_span(id_list_node),END_LOC(2),
				  "Only one identifier is allowed in this context");
				id_list_node->links.list->link = tmp->link;
			}
			else
				id_list_node->links.list = NULL;
			TFREE(tmp,tmp);
			free_everything(id_list_node);
			NN(AS_RENAME_EX);
			NAST2(id_node,any_node);
		}
#undef id_list_node


		break;
		/* [TYPE] ::= empty */
	case 393 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [TYPE] ::= TYPE */
	case 394 :
		node = any_node;

		break;
		/* {entry_declaration} ::= {pragma} */
	case 395 :
		node = AST(0);
		check_pragmas(node,task_pragmas);

		break;
		/* {entry_declaration} ::=
			{entry_declaration} entry_declaration {pragma} */
	case 396 :
#define entry_decl AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,task_pragmas);
		node->links.list = concatl3(node->links.list,initlist(entry_decl),
		  pragma_node->links.list);
		nodefree(pragma_node);
#undef entry_decl
#undef pragma_node


		break;
		/* {representation_clause} ::= empty */
	case 397 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {representation_clause} ::=
			{representation_clause} representation_clause */
	case 398 :
#define repr_clause AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,task_repr_pragmas);
		node->links.list = concatl3(node->links.list,initlist(repr_clause),
		  pragma_node->links.list);
		nodefree(pragma_node);
#undef repr_clause
#undef pragma_node

		break;
		/* [(discrete_range)][formal_part] ::= [formal_part] */
	case 399 :
#define formal_part AST(0)
		NN(AS_ENTRY);
		NAST2(any_node,formal_part);
		set_span(any_node,get_left_span(formal_part)); /* kludge for errors */
#undef formal_part

		break;
		/* [(discrete_range)][formal_part] ::= (discrete_range) [formal_part] */
	case 400 :
#define discrete_range AST(1)
#define formal_part AST(3)
		check_discrete_range(discrete_range);
		NN(AS_ENTRY_FAMILY);
		NAST3(any_node,discrete_range,formal_part);
#undef discrete_range
#undef formal_part

		break;
		/* [(entry_index)][formal_part] ::= [formal_part] */
	case 401 :
#define formal_part AST(0)
		NN(AS_ACCEPT);
		NAST4(any_node,opt_node,formal_part,any_node);
#undef formal_part

		break;
		/* [(entry_index)][formal_part] ::= ( entry_index ) [formal_part] */
	case 402 :
#define entry_index AST(1)
#define formal_part AST(3)
		NN(AS_ACCEPT);
		NAST4(any_node,entry_index,formal_part,any_node);
#undef entry_index
#undef formal_part


		break;
		/* {OR__select_alternative} ::= empty */
	case 403 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {OR__select_alternative} ::=
			{OR__select_alternative} OR {pragma} select_ */
	case 404 :
#define pragma_node AST(2)
#define alt AST(3)
		node = AST(0);
		check_pragmas(pragma_node,null_pragmas);
		node->links.list = concatl3(node->links.list,pragma_node->links.list,
		  initlist(alt));
		nodefree(pragma_node);
#undef pragma_node
#undef alt


		break;
		/* [WHEN__condition=>] ::= empty */
	case 405 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [WHEN__condition=>] ::= WHEN condition => {pragma} */
	case 406 :
#define pragma_node AST(3)
		node = AST(1);
		check_pragmas(pragma_node,null_pragmas);
		pragmalist_warning(pragma_node);
		free_everything(pragma_node);
#undef pragma_node

		break;
		/* [sequence_of_statements] ::= {pragma} */
	case 407 :
#define pragma_node AST(0)
		check_pragmas(pragma_node,null_pragmas);
		if (pragma_node->links.list != NULL) {
			struct ast *label_list_node;

			NN(AS_STATEMENTS);
			label_list_node = new_node(AS_LIST);
			label_list_node->links.list = NULL;
			set_span(label_list_node,&curtok->ptr.token->loc);
			NAST2(pragma_node,label_list_node);
		}
		else
			node = opt_node;
		set_span(node,&curtok->ptr.token->loc);
#undef pragma_node

		/* [sequence_of_statements] ::= sequence_of_statements */
		/* case 408 : */


		break;
		/* {,task_name} ::= empty */
	case 409 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,task_name} ::= {,task_name} , task_name */
	case 410 :
#define task_name AST(2)
		node = AST(0);
		append(node,task_name);
#undef task_name

		break;
		/* {compilation_unit} ::= {pragma} */
	case 411 :
#define pragma_node AST(0)
		check_pragmas(pragma_node,compilation_pragmas);
		TBSL;
		if (astopt)
			print_tree(pragma_node);
		if (curtok->symbol != EOFT_SYM) {
			free_everything(pragma_node);
			free_labels();
		}
		prs_stack->symbol = lhs[red];
		return;
#undef pragma_node

		/* {compilation_unit} ::= {compilation_unit} compilation_unit {pragma}*/
	case 412 :
#define comp_unit AST(1)
#define pragma_node AST(2)
		{
			node = AST(0);
			check_pragmas(pragma_node,after_libunit_pragmas);
			pragmalist_warning(pragma_node);
			TBSL;
			if (astopt) {
				print_tree(comp_unit);
				/*        print_tree(pragma_node); */
			}
			if (curtok->symbol != EOFT_SYM)
			{
				free_everything(comp_unit);
				free_everything(pragma_node);
				free_labels();
			}
		}
#undef comp_unit
#undef pragma_node


		break;
		/* {with_clause{use_clause}} ::= empty */
	case 413 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {with_clause{use_clause}} ::=
			{with_clause{use_clause}} with_clause use_c */
	case 414 :
#define with_clause AST(1)
#define use_clause_node AST(2)
		node = AST(0);
		use_clause_node->kind = AS_WITH_USE_LIST;
		prepend(with_clause,use_clause_node);
		append(node,use_clause_node);
#undef with_clause
#undef use_clause_node

		break;
		/* use_clause_list ::= {pragma} */
	case 415 :
		node = AST(0);
		check_pragmas(node,context_pragmas);

		break;
		/* use_clause_list ::= use_clause_list use_clause {pragma} */
	case 416 :
#define use_clause AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,context_pragmas);
		node->links.list = concatl3(node->links.list,initlist(use_clause),
		  pragma_node->links.list);
		nodefree(pragma_node);
#undef use_clause
#undef pragma_node


		break;
		/* {,unit_simple_name} ::= empty */
	case 417 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,unit_simple_name} ::= {,unit_simple_name} , unit_simple_name */
	case 418 :
#define simple_name AST(2)
		node = AST(0);
		append(node,simple_name);
#undef simple_name


		break;
		/* {|exception_choice} ::= empty */
	case 419 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {|exception_choice} ::= {|exception_choice} '|' exception_choice */
	case 420 :
#define choice AST(2)
		node = AST(0);
		append(node,choice);
#undef choice


		break;
		/* {generic_parameter_declaration} ::= empty */
	case 421 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {generic_parameter_declaration} ::=
			{generic_parameter_declaration} gener */
	case 422 :
#define parm_decl AST(1)
		node = AST(0);
		append(node,parm_decl);
#undef parm_decl

		/* [IN[OUT]] ::= [IN] */
		/* case 423 : */

		break;
		/* [IN[OUT]] ::= IN OUT */
	case 424 :
		NN(AS_MODE);
		node->links.val = namemap("inout",5);
		set_span(node,LOC(0));


		break;
		/* [IS__name__or__<>] ::= empty */
	case 425 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* [IS__name__or__<>] ::= IS name */
	case 426 :
		node = AST(1);

		break;
		/* [IS__name__or__<>] ::= IS <> */
	case 427 :
		NN(AS_SIMPLE_NAME);
		node->links.val = namemap("box",3);
		set_span(node,LOC(1));


		break;
		/* [generic_actual_part] ::= empty */
	case 428 :
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);

		/* [generic_actual_part] ::= generic_actual_part */
		/* case 429 : */


		break;
		/* {,generic_association} ::= empty */
	case 430 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {,generic_association} ::=
			{,generic_association} , generic_association */
	case 431 :
#define assoc AST(2)
		node = AST(0);
		append(node,assoc);
#undef assoc

		break;
		/* [generic_formal_parameter=>]generic_actual_parameter ::=
			generic_actual_p */
	case 432 :
#define actual AST(0)
		NN(AS_INSTANCE);
		NAST2(opt_node,actual);
#undef actual

		break;
		/* [generic_formal_parameter=>]generic_actual_parameter ::=
			generic_formal_p */
	case 433 :
#define formal AST(0)
#define actual AST(2)
		NN(AS_INSTANCE);
		NAST2(formal,actual);
#undef formal
#undef actual

		break;
		/* [alignment_clause] ::= {pragma} */
	case 434 :
#define pragma_node AST(0)
		check_pragmas(pragma_node,null_pragmas);
		pragmalist_warning(pragma_node);
		node = opt_node;
		set_span(node,&curtok->ptr.token->loc);
		free_everything(pragma_node);
#undef pragma_node

		break;
		/* [alignment_clause] ::= {pragma} alignment_clause {pragma} */
	case 435 :
#define pragma_node1 AST(0)
#define pragma_node2 AST(2)
		node = AST(1);
		check_pragmas(pragma_node1,null_pragmas);
		check_pragmas(pragma_node2,null_pragmas);
		pragmalist_warning(pragma_node1);
		pragmalist_warning(pragma_node2);
		free_everything(pragma_node1);
		free_everything(pragma_node2);
#undef pragma_node1
#undef pramga_node2


		break;
		/* {component_clause} ::= empty */
	case 436 :
		NN(AS_LIST);
		node->links.list = NULL;
		set_span(node,&curtok->ptr.token->loc);

		break;
		/* {component_clause} ::= {component_clause} component_clause {pragma}*/
	case 437 :
#define compon_clause AST(1)
#define pragma_node AST(2)
		node = AST(0);
		check_pragmas(pragma_node,null_pragmas);
		pragmalist_warning(pragma_node);
		append(node,compon_clause);
		free_everything(pragma_node);
#undef compon_clause
#undef pragma_node
		break;
	default :
		prs_stack->symbol = lhs[red];
		return;
	}
	prs_stack->symbol = lhs[red];
	prs_stack->ptr.ast = node;
	for (n = rhslen[red]; n--;)
		if (ISTOKEN(rh[n]))
			TOKFREE(rh[n]->ptr.token);
}


