/*
 * Copyright (C) 1985-1992  New York University
 * 
 * This file is part of the Ada/Ed-C system.  See the Ada/Ed README file for
 * warranty (none) and distribution info and also the GNU General Public
 * License for more details.

 */
/* This file contains various functions needed for reduce actions */

#include "adared.h"
#include "prsutilprots.h"
#include "errsprots.h"
#include "adalexprots.h"
#include "pspansprots.h"
#include "reduceprots.h"

static void pragma_warning(struct ast *);
static void remove_link(struct ast *, struct two_pool **, struct two_pool *);
static int in_label_set(struct ast *, struct two_pool *);
static int is_pragma(int);

/* define PDEBUG to trace pragma procedures */


void free_everything(struct ast *node)				/*;free_everything*/
{
	/* Recursively free all nodes in the tree beginning at node, and
	 * free all list structures and AST structures where they exist.
	 */

	struct two_pool *listptr;
	int i;

#ifdef FDEBUG
	if (trcopt)
		fprintf(errfile,"Freeing node 0x%x, kind = %d\n",node,node->kind);
#endif
	/* check to see if the node has already been freed */
	if (node->kind == AS_FREE)
		return;
	if (islist_node[node->kind]) {
#ifdef FDEBUG
		if (trcopt)
			fprintf(errfile,"Traversing through list\n");
#endif
		if (node->links.list != NULL) {
			listptr = node->links.list;
			do {
				listptr = listptr->link;
				free_everything(listptr->val.node);
			}	    while (listptr != node->links.list);
			TFREE(node->links.list->link,node->links.list);
		}
	}
	else if (isast_node[node->kind]) {
#ifdef FDEBUG
		if (trcopt)
			fprintf(errfile,"Traversing through tree\n");
#endif
		for (i = 0; i < MAX_AST && (node->links.subast)[i] != NULL; i++)
			if ((node->links.subast)[i] != opt_node)
				free_everything((node->links.subast)[i]);
		astfree(node->links.subast);
	}
	nodefree(node);
#ifdef FDEBUG
	if (trcopt)
		fprintf(errfile,"Returning\n");
#endif
}

void set_span(struct ast *node, struct tok_loc *span)		/*;set_span*/
{
	/* Set the spans field of node to match the given spans. */
	node->span.line = span->line;
	node->span.col = span->col;
}

struct two_pool *initlist(struct ast *node)				/*;initlist*/
{
	/* Allocate a single list structure (struct two_pool), set its data to
	 * be a pointer to the node given, and set its link field to point
	 * to itself, since tree node lists are circular.
	 */
	struct two_pool *tmp;

	tmp = TALLOC();
	tmp->val.node = node;
	tmp->link = tmp;
	return(tmp);
}

struct ast **new_ast1(struct ast *a1)					/*;new_ast1*/
{
	/* Allocate an array to hold pointers to the children of a node
	 * in the AST, and set elements of the array to the values given. This
	 * function may be called with fewer than the 4 parameters below, in which
	 * case the parameters given will be used properly, and undefined (garbage)
	 * values will be put in the latter part of the array.
	 */

	struct ast **tmp;

	tmp = astalloc();
	tmp[0] = a1;
	tmp[1] = 0;
	tmp[2] = 0;
	tmp[3] = 0;
	return(tmp);
}

struct ast **new_ast2(struct ast *a1, struct ast *a2)			/*;new_ast2*/
{
	struct ast **tmp;

	tmp = astalloc();
	tmp[0] = a1;
	tmp[1] = a2;
	tmp[2] = 0;
	tmp[3] = 0;
	return(tmp);
}

struct ast **new_ast3(struct ast *a1, struct ast *a2, struct ast *a3)
																/*;new_ast3*/
{
	struct ast **tmp;

	tmp = astalloc();
	tmp[0] = a1;
	tmp[1] = a2;
	tmp[2] = a3;
	tmp[3] = 0;
	return(tmp);
}

struct ast **new_ast4(struct ast *a1, struct ast *a2, struct ast *a3,
  struct ast *a4)												/*;new_ast4*/
{
	struct ast **tmp;

	tmp = astalloc();
	tmp[0] = a1;
	tmp[1] = a2;
	tmp[2] = a3;
	tmp[3] = a4;
	return(tmp);
}

struct two_pool *concatl(struct two_pool *bottom,struct two_pool *nextlist)
															/*;concatl*/
{
	/* Concatenate circular linked lists of structs of type two_pool
	 * together, returning the new end pointer. There are two such
	 * functions: concatl catenates two lists, concatl3 catenates three.
	 */
	struct two_pool *tmp;

	if (bottom==(struct two_pool *)0) bottom = nextlist;
	if (nextlist!=(struct two_pool *)0) {
		tmp = bottom->link;
		bottom->link = nextlist->link;
		nextlist->link = tmp;
		bottom = nextlist;
	}
	return(bottom);
}

struct two_pool *concatl3(struct two_pool *bottom, struct two_pool *nextlist,
  struct two_pool *list3)									/*;concatl3*/
{
	return concatl(concatl(bottom,nextlist),list3);
}

void append(struct ast *orignode, struct ast *node)			/*;append*/
{
	/* Given two nodes from the AST, orignode with a list of children,
	 * and a new node node, allocate a new list structure for node and append
	 * it to the list of children, and set the spans of orignode so they remain
	 * correct.
	 */

	struct two_pool *tmp;

	tmp = TALLOC();
	tmp->val.node = node;
	if (orignode->links.list == (struct two_pool *)0)
		tmp->link = tmp;
	else {
		tmp->link = orignode->links.list->link;
		orignode->links.list->link = tmp;
	}
	orignode->links.list = tmp;
}

void prepend(struct ast *node, struct ast *orignode)		/*;prepend*/
{
	/* Given two nodes from the AST, orignode with a list of children,
	 * and a new node node, allocate a new list structure for node and
	 * prepend it to the list of children, and set the spans of orignode
	 * so they remain correct.
	 */

	struct two_pool *tmp;

	tmp = TALLOC();
	tmp->val.node = node;
	if (orignode->links.list == (struct two_pool *)0)
		orignode->links.list = tmp->link = tmp;
	else {
		tmp->link = orignode->links.list->link;
		orignode->links.list->link = tmp;
	}
}

struct ast *binary_operator(struct ast *optr, struct ast *expr1,
  struct ast *expr2)									/*;binary_operator*/
{
	/* Set up the AST node for a binary operator. */

	struct ast *node, *arg_list_node;
	struct two_pool *tmp;

	NN(AS_OP);
	arg_list_node = new_node(AS_LIST);
	(arg_list_node->links.list = TALLOC())->val.node = expr2;
	(arg_list_node->links.list->link = tmp = TALLOC())->val.node = expr1;
	tmp->link = arg_list_node->links.list;
	NAST2(optr,arg_list_node);
	return(node);
}

struct ast *unary_operator(struct ast *optr, struct ast *expr)
														/*;unary_operator*/
{
	/* Set up the AST node for a unary operator. */
	struct ast *node, *arg_list_node;

	node = new_node(AS_UN_OP);
	arg_list_node = new_node(AS_LIST);
	arg_list_node->links.list = initlist(expr);
	NAST2(optr,arg_list_node);
	return(node);
}

int check_expanded_name(struct ast *name)			/*;check_expanded_name*/
{
	/* Make sure an expanded name node is valid. */

#define sub_expanded_name ((name->links.subast)[0])
	return((name->kind == AS_SELECTOR) ? 
	  check_expanded_name(sub_expanded_name) : (name->kind == AS_SIMPLE_NAME));
#undef sub_expanded_name
}

void check_discrete_range(struct ast *discrete_range) /*;check_discrete_range*/
{
	/* Check whether a discrete range node is valid. */

	switch (discrete_range->kind)
	{
	case AS_RANGE_EXPRESSION :
#define name (discrete_range->links.subast)[0]
		if (!check_expanded_name(name))
			syntax_err(SPAN(discrete_range),
			  "Invalid discrete_range specification");
		else
			discrete_range->kind = AS_NAME;
		break;
#undef name
	case AS_RANGE_ATTRIBUTE :
	case AS_SUBTYPE :
		break;
	default :
		syntax_err(SPAN(discrete_range),
		  "Invalid discrete_range specification");
	}
}

static void pragma_warning(struct ast *pragma_node)			/*;pragma_warning*/
{
	/* Give a warning that a pragma is ignored. */

	char msg[MAXLINE + 30];

#define id (pragma_node->links.subast)[0]
	sprintf(msg,"Pragma %s is ignored", namelist(id->links.val));
	prs_warning(SPAN(pragma_node),msg);
#undef id
}

void pragmalist_warning(struct ast *list_node)		/*;pragmalist_warning*/
{
	/* For all nodes in the list of list_node give a warning the the pragma
	 * is invalid.
	 */

	LLOOPTOP(list_node->links.list,tmp)
	    pragma_warning(tmp->val.node);
	LLOOPBOTTOM(tmp)
}

static void remove_link(struct ast *pragma_node, struct two_pool **current,
  struct two_pool *prev)								/*;remove_link*/
{
	/* Remove a node (pragma_node) from a circular linked list of nodes. */

	if ((*current)->link != *current)
		prev->link = (*current)->link;
	else
		pragma_node->links.list = (struct two_pool *) 0;
	nodefree((*current)->val.node);
	TFREE(*current,*current);
	*current = prev;
}

void check_pragmas(struct ast *pragma_node, int (*allowed_test)(int))
													/*;check_pragmas*/
{
	/* Check that a pragma is valid. */

	struct two_pool *old_list, *prev, *current;
	int id;
	int last;

#ifdef PDEBUG
	printf("enter check_pragmas\n");
#endif
	if ((old_list = prev = current = pragma_node->links.list) != 
	  (struct two_pool *)0)
	{
		do {
			last = (current = current->link) == old_list;
			id = (current->val.node->links.subast)[0]->links.val;
#ifdef PDEBUG
			printf("check_pragma id %d\n",id);
#endif
			if (is_pragma(id) && (*allowed_test)(id - MIN_PRAGMA)) {
				if (strcmp(namelist(id),"PRIORITY")
				  && strcmp(namelist(id),"ELABORATE")
				  && strcmp(namelist(id),"INTERFACE"))
				{
					pragma_warning(current->val.node);
					remove_link(pragma_node,&current,prev);
				}
			}
			else if (is_pragma(id) && ispredef_pragma[id - MIN_PRAGMA])
			{
				char msg[200];

				sprintf(msg,"Pragma %s is not valid in this context",
				  namelist(id));
				prs_warning(SPAN(current->val.node),msg);
				remove_link(pragma_node,&current,prev);
			}
			else if (!(is_pragma(id) && isimpldef_pragma[id - MIN_PRAGMA])
			  && strcmp(namelist(id),"OPTIMIZE"))
			{
				pragma_warning(current->val.node);
				remove_link(pragma_node,&current,prev);
			}
			prev = current;
		}	while (!last);
	}
}

int isoverloadable_op(char *str)				/*;isoverloadable_op*/
{
	/* Check whether a string represnts an overloadable operator by
	 * comparing against all overloadable operators.
	 */

	char tmp[MAXLINE + 1];
	int i;

	strcpy(tmp,str);
	convtolower(tmp);
	for (i = 0; i < NUMOVERLOADOPS; i++)
		if (!strcmp(tmp,overloadable_operators[i]))
			return(1);
	return(0);
}

void make_id(int n)											/*;make_id*/
{
	/* Allocate a node for a simple name whose value and span are* known by
	 * looking at the nth symbol in the right hand side of the reduction,
	 * and set these fields.
	 */
	id_node = new_node(AS_SIMPLE_NAME);
	id_node->links.val = IND(n);
	set_span(id_node,LOC(n));
}


/* The following functions are for passing to check_pragmas */

int immediate_decl_pragmas(int p)				/*;immediate_decl_pragmas*/
{
#ifdef PDEBUG
	printf("enter immediate_decl pragmas %d %d\n",p,isimmediate_decl_pragma[p]);
#endif
	return(isimmediate_decl_pragma[p]);
}

int compilation_pragmas(int p)					/*;compilation_pragmas*/
{
#ifdef PDEBUG
	printf("enter compilation pragmas %d %d\n",p,iscompilation_pragma[p]);
#endif
	return(iscompilation_pragma[p]);
}

int after_libunit_pragmas(int p)				/*;after_libunit_pragmas*/
{
#ifdef PDEBUG
	printf("enter after_libunit pragmas %d %d\n",p,isafter_libunit_pragma[p]);
#endif
	return(isafter_libunit_pragma[p]);
}

int task_pragmas(int p)							/*;task_pragmas*/
{
#ifdef PDEBUG
	printf("enter istask pragmas %d %d\n",p,istask_pragma[p]);
#endif
	return(istask_pragma[p]);
}

int task_repr_pragmas(int p)					/*;task_repr_pragmas*/
{
#ifdef PDEBUG
	int r;
	r =(istask_pragma[p] || isrepr_pragma[p]);
	printf("enter task_repr pragmas %d %d\n",p,r);
#endif
	return(istask_pragma[p] || isrepr_pragma[p]);
}

int context_pragmas(int p)						/*;context_pragmas*/
{
#ifdef PDEBUG
	printf("enter context pragmas %d %d\n",p,iscontext_pragma[p]);
#endif
	return(iscontext_pragma[p]);
}

int null_pragmas(int i)									/*;null_pragmas*/
{
#ifdef PDEBUG
	printf("enter null pragmas \n");
#endif
	return(i = 0);
}


void check_choices(struct ast *alt_node, char *source)	/*;check_choices*/
{
	struct two_pool *others_indices = (struct two_pool *)0, 
	*choice_list, *last_alt_list;
	struct ast *last_alt = (struct ast *) 0;
	int choice_flag = 0;

	LLOOPTOP(alt_node->links.list,tmp)
	    if (tmp->val.node->kind != AS_PRAGMA) {
			choice_list = (tmp->val.node->links.subast)[0]->links.list;
			if (choice_list->link != choice_list)   /* cannot be NULL */
				LLOOPTOP(choice_list,tmp2)
			    	if (tmp2->val.node->kind == AS_OTHERS
					  || tmp2->val.node->kind == AS_OTHERS_CHOICE)
					{
						char msg[90];

						sprintf(msg,"The choice OTHERS must appear alone in %s",
						  source);
						syntax_err(SPAN(tmp2->val.node),msg);
						choice_flag = 1;
						break;
					}
				LLOOPBOTTOM(tmp2)
		   	if (!choice_flag) {
				if (choice_list->link->val.node->kind == AS_OTHERS
				  || choice_list->link->val.node->kind == AS_OTHERS_CHOICE)
					others_indices = concatl(others_indices,
					  initlist(tmp->val.node));
			}
			else
				choice_flag = 0;
			last_alt_list = tmp;
		}
	LLOOPBOTTOM(tmp)
	last_alt = last_alt_list->val.node;
	LLOOPTOP(others_indices,tmp)
	    {
		struct ast *choice;
		char msg[90];

		if (tmp->val.node == last_alt)
			continue;
		choice = (tmp->val.node->links.subast)[0]->links.list->link->val.node;
		sprintf(msg,"The choice OTHERS must appear last in %s",source);
		syntax_err(SPAN(choice),msg);
	}
	LLOOPBOTTOM(tmp)
	if (others_indices != (struct two_pool *)0 )
		TFREE(others_indices->link,others_indices);
}

struct two_pool *remove_duplicate_labels(struct two_pool *label_list)
											/*;remove_duplicate_labels*/
{
	struct two_pool *new_label_list = (struct two_pool *)0,
	*label_id_set = (struct two_pool *)0;
	struct ast *node, *label;

	LLOOPTOP(label_list,tmp)
	    if ((node = tmp->val.node)->kind == AS_SIMPLE_NAME) {
			if (in_label_set(node,label_id_set))
				syntax_err(SPAN(node),"Duplicate label name");
			else {
				/* new_label_list = concatl(new_label_list,initlist(node)); */
				label_id_set = concatl(label_id_set,initlist(node));
			}
			new_label_list = concatl(new_label_list,initlist(node));
		}
		else
		LLOOPTOP(node->links.list,tmp2)
		    label = tmp2->val.node;
		if (in_label_set(label,label_id_set))
			syntax_err(SPAN(label),"Duplicate label name");
		else
			label_id_set = concatl(label_id_set,initlist(label));
		LLOOPBOTTOM(tmp2)
	LLOOPBOTTOM(tmp)
	if (label_id_set != (struct two_pool *)0)
		TFREE(label_id_set->link,label_id_set);
	if (label_list != (struct two_pool *)0)
		TFREE(label_list->link,label_list);
	return(new_label_list);
}

static int in_label_set(struct ast *label, struct two_pool *label_set)
														/*;in_label_set*/
{
	int val = label->links.val;

	LLOOPTOP(label_set,tmp)
	    if (tmp->val.node->links.val == val)
			return(1);
	LLOOPBOTTOM(tmp)
	return(0);
}

void ins_as_line_no(struct ast *node)				/*;ins_as_line_no*/
{
	/* insert as_line_no nodes before each item in declarative/stmt list */
	struct two_pool *dec_list,
	*prev,*bottom;
	struct ast *line_node;
	struct tok_loc *line_node_span;


	if (node -> links.list != (struct two_pool *)0) {

		dec_list = bottom = node -> links.list;
		do {
			prev = dec_list;
			dec_list = dec_list -> link;

			line_node = new_node (AS_LINE_NO);
			line_node_span = get_left_span(dec_list->val.node);
			line_node -> links.val = line_node_span->line;
			set_span(line_node,line_node_span);
			/* Insert a new node with the AS_LINE_NO between dec_list and its 
					predecessor */
			prev -> link = initlist (line_node) ;
			prev->link->link = dec_list;
		} while (dec_list != bottom);
	}
}

void end_as_line_no(struct ast *list_node, struct prsstack *next_token)
													/*;end_as_line_no*/
{
	/* add an as_line_no node to end of statement list this is the line
	 * number of the token following the sequence of statements
	 */

	struct two_pool * last, * first;
	struct ast	    * line_node;

	if ( (last = list_node->links.list) != (struct two_pool *)0) {
		first = last->link;
		line_node = new_node (AS_LINE_NO);
		line_node->links.val = next_token->ptr.token->loc.line ;
		set_span(line_node,make_span(line_node->links.val,
		  next_token->ptr.token->loc.col));
		last->link = initlist(line_node);
		last->link->link = first;
		list_node->links.list = last->link;
	}
}

#define LABELSMAPSIZE 50

struct labelsmap {
	struct ast *node;
	struct two_pool *list;
	struct labelsmap *link;
};

struct labelsmap *nodetolabelstable[LABELSMAPSIZE]; /* Table for Labels map */
/* List of free label structures */
static struct labelsmap *deadlabels = (struct labelsmap *)0;

unsigned long labelshash(struct ast *node)			/*;labelshash*/
{
	/* The hash function from nodes to integers */
	return( ((unsigned long) node) % LABELSMAPSIZE);
}

void newlabels(struct ast *node, struct two_pool *list)		/*;newlabels*/
{
	/* Add node to the map, and initialize its labels list to list.
	 * Storage allocation is done using malloc/free structure list.
	 */

	int pos;
	struct labelsmap *labelnode;

	pos = (int)labelshash(node);
	if (deadlabels == (struct labelsmap *)0)
		labelnode = (struct labelsmap *)malloc(sizeof(struct labelsmap));
	else {
		labelnode = deadlabels;
		deadlabels = deadlabels->link;
	}
	labelnode->link = nodetolabelstable[pos];
	nodetolabelstable[pos] = labelnode;
	labelnode->node = node;
	labelnode->list = list;
}

struct two_pool *getlabels(struct ast *node)				/*;getlabels*/
{
	/* Return the list of labels corresponding to a given node. If
	 * The map is not defined for a node, NULL is returned.
	 */

	struct labelsmap *tmp;

	for (tmp = nodetolabelstable[labelshash(node)];
	  tmp != (struct labelsmap *)0 && tmp->node != node; tmp = tmp->link);
	return((tmp == (struct labelsmap *)0) ? (struct two_pool *)0 : tmp->list);
}

void erase_labels(struct ast *node)						/*;erase_labels*/
{
	/* Remove a node from the labels map, freeing the structure used for
	 * that node's labels.
	 */

	struct labelsmap *tmp, *last;
	int pos;

	pos = (int)labelshash(node);
	for (tmp = nodetolabelstable[pos], last = (struct labelsmap *)0; 
	  tmp != (struct labelsmap *)0 && tmp->node != node;
	  last = tmp, tmp = tmp->link);
	if (tmp == (struct labelsmap *)0)
		return;
	if (last == (struct labelsmap *)0)
		nodetolabelstable[pos] = tmp->link;
	else
		last->link = tmp->link;
	tmp->link = deadlabels;
	deadlabels = tmp;
	if (tmp->list != (struct two_pool *)0)
		TFREE(tmp->list->link,tmp->list);
}

struct two_pool *copylist(struct two_pool *list)			/*;copylist*/
{
	/* Copy a circular linked list of structs of type two_pool leaving
	 * the data of the nodes intact.
	 */
	struct two_pool *oldcurr, *newcurr, *top;

	if (list == (struct two_pool *)0)
		return((struct two_pool *)0);
	newcurr = top = TALLOC();
	oldcurr = list->link;
	top->val.node = oldcurr->val.node;
	while (oldcurr != list) {
		oldcurr = oldcurr->link;
		newcurr->link = TALLOC();
		newcurr = newcurr->link;
		newcurr->val.node = oldcurr->val.node;
	}
	newcurr->link = top;
	return(newcurr);
}

void free_labels()											/*;free_labels*/
{
	/* Remove all entries in the labels map. */
	int i;
	struct labelsmap *curr;

	for (i = 0; i < LABELSMAPSIZE; i++)
		if (nodetolabelstable[i] != (struct labelsmap *)0) {
			for (curr = nodetolabelstable[i]; curr->link!=NULL; curr=curr->link)
				if (curr->list != NULL)
					TFREE(curr->list->link,curr->list);
			curr->link = deadlabels;
			deadlabels = nodetolabelstable[i];
			nodetolabelstable[i] = NULL;
		}
}

#ifdef DEBUG
/* for debugging use */
void dump_labels(struct ast *node)							/*;dump_labels*/
{
	struct labelsmap *tmp;
	void zpnlist(struct two_pool *);

	for (tmp = nodetolabelstable[labelshash(node)];
	  tmp != NULL && tmp->node != node; tmp = tmp->link);
	zpnlist(tmp->list);
}
#endif 

static int is_pragma(int n)				 				/*;is_pragma*/
{
	/* Metaware miscompiles if:
	return (MIN_PRAGMA <= (n) && (n) <= MAX_PRAGMA);
     * so reorder first test until MetaWare compiler bug fixed
     */
	return ((n)>=MIN_PRAGMA  && (n) <= MAX_PRAGMA);
}

