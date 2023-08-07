/* tree.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"
#include "tree.h"

static struct node* mk_node(NODEKIND tag) {
    struct node* p = (struct node*) malloc(sizeof(struct node));
    if ( !p ) {
	fprintf(stderr, "mk_node ran out of memory.");
	abort();
    } else {
	p->tag = tag;
	return p;
    }
}

struct node* mk_prim(NODEKIND tag) {
    return mk_node(tag);
}

struct node* mk_select(NODEKIND tag, int intval) {
    struct node* p = mk_node(tag);
    p->u_node.int_sy.intval = intval;
    return p;
}

struct node* mk_construct(struct node* car, struct node* cdr) {
    struct node* p = mk_node(CONSTRUCT);
    p->u_node.construct.car = car;
    p->u_node.construct.cdr = cdr;
    return p;
}

struct node* mk_userfcn(char* name) {
    struct node* p = mk_node(USERFCN);
    p->u_node.userfcn.name = malloc(strlen(name) + 1);
    strcpy(p->u_node.userfcn.name, name);
    return p;
}

struct node* mk_compose(struct node* car, struct node* cdr) {
    struct node* p = mk_node(COMPOSE);
    p->u_node.compose.car = car;
    p->u_node.compose.cdr = cdr;
    return p;
}

struct node* mk_condition(struct node* pred, struct node* true, struct node* false) {
    struct node* p = mk_node(CONDITION);
    p->u_node.condition.pred = pred;
    p->u_node.condition.true = true;
    p->u_node.condition.false = false;
    return p;
}

struct node* mk_apply_to_all(struct node* np) {
    struct node* p = mk_node(APPLY_TO_ALL);
    p->u_node.apply_to_all.f = np;
    return p;
}

struct node* mk_rins(struct node* np) {
    struct node* p = mk_node(RINS);
    p->u_node.rins.f = np;
    return p;
}

struct node* mk_constant(struct obj* op) {
    struct node* p = mk_node(CONSTANT);
    p->u_node.constant.objp = op;
    return p;
}

NODEKIND fcnkind(struct node* np) {
    return np->tag;
}

#define kid1(np) ((np)->u_node.backdoor.field1)
#define kid2(np) ((np)->u_node.backdoor.field2)
#define kid3(np) ((np)->u_node.backdoor.field3)

struct node* arg1(struct node* np) {
    return kid1(np);
}

struct node* arg2(struct node* np) {
    return kid2(np);
}

struct node* arg3(struct node* np) {
    return kid3(np);
}

struct obj* constant_object(struct node* np) {
    if ( np->tag == CONSTANT )
	return np->u_node.constant.objp;
    else {
	fprintf(stderr, "constant_object operating on non-object.");
	abort();
    }
}

char* userfcn_name(struct node* np) {
    return np->u_node.userfcn.name;
}

int int_sy_val(struct node* np) {
    return np->u_node.int_sy.intval;
}

void GEN(char* s) {
    (void)printf("(primitive %s ) ", s);
}

void LP(char* s) {
    (void)printf("( %s ", s);
}
void RP() {
    (void)printf(") ");
}

void print_tree(struct node* np) {
    if ( np == 0 )
	return;
    switch( np->tag ) {
      case COMPOSE:
	LP("compose");
	print_tree(kid1(np));
	print_tree(kid2(np));
	RP();
	break;
      case CONSTRUCT:
	LP("construct");
	for(; np->tag == CONSTRUCT; np = kid2(np))
	    print_tree(kid1(np));
	print_tree(np);
	RP();
	break;
      case CONDITION:
	LP("condition");
	print_tree(kid1(np));
	print_tree(kid2(np));
	print_tree(kid3(np));
	RP();
	break;
      case CONSTANT:
	LP("constant");
	printobj1(np->u_node.constant.objp);
	RP();
	break;
      case APPLY_TO_ALL:
	LP("apply_to_all");
	print_tree(kid1(np));
	RP();
	break;
      case RINS:
	LP("rins");
	print_tree(kid1(np));
	RP();
	break;
      case USERFCN:
	LP("userfcn");
	printf("%s", np->u_node.userfcn.name);
	RP();
	break;
      case ID_SY:
	GEN("id");
	break;
      case OUT_SY:
	GEN("out");
	break;
      case INT_SY:
	printf("%d", np->u_node.int_sy.intval);
	break;
      case TL_SY:
	GEN("tl");
	break;
      case NUL_SY:
	GEN("null");
	break;
      case REVERSE_SY:
	GEN("reverse");
	break;
      case SPLIT_SY:
	GEN("split");
	break;
      case PAIR_SY:
	GEN("pair");
	break;
      case TRANS_SY:
	GEN("trans");
	break;
      case LENGTH_SY:
	GEN("length");
	break;
      case EQL_SY:
	GEN("eql");
	break;
      case NE_SY:
	GEN("ne");
	break;
      case LT_SY:
	GEN("lt");
	break;
      case LE_SY:
	GEN("le");
	break;
      case GT_SY:
	GEN("gt");
	break;
      case GE_SY:
	GEN("ge");
	break;
      case ADD_SY:
	GEN("add");
	break;
      case SUBTRACT_SY:
	GEN("subtract");
	break;
      case MULTIPLY_SY:
	GEN("multiply");
	break;
      case DIVIDE_SY:
	GEN("divide");
	break;
      case AND_SY:
	GEN("and");
	break;
      case OR_SY:
	GEN("or");
	break;
      case NOT_SY:
	GEN("not");
	break;
      case ATOM_SY:
	GEN("atom");
	break;
      case DISTL_SY:
	GEN("distl");
	break;
      case DISTR_SY:
	GEN("distr");
	break;
      case APNDL_SY:
	GEN("apndl");
	break;
      case APNDR_SY:
	GEN("apndr");
	break;
      case ROTL_SY:
	GEN("rotl");
	break;
      case ROTR_SY:
	GEN("rotr");
	break;
      case CONCAT_SY:
	GEN("concat");
	break;
      default:
	fprintf(stderr, "print_tree: can't happen.");
	abort();
    }
}
