/* apply.c */

#include <stdio.h>
#include <stdlib.h>
#include "obj.h"
#include "tree.h"
#include "apply.h"
#include "primitives.h"
#include "symtab.h"
	  
struct obj* do_construct(struct node* flist, struct obj* obj) {
    return (flist == 0)
	? 0
	: cons(apply(arg1(flist), obj), do_construct(arg2(flist), obj));
}    
	   
struct obj* do_apply_to_all(struct node* f, struct obj* obj) {
    if ( !seqp(obj) )
	return bottom;
    else if ( obj == 0 )
	return 0;
    else
	return cons(apply(f, car(obj)), do_apply_to_all(f, cdr(obj)));
}

/* 'f:<x> ==> x
   'f:<x1,...,xN> ==> f:<x1,'f:<x2,...,xN>>
 */

struct obj* rins(struct node* f, struct obj* seq) {
    if ( seq == 0 )
	return 0;
    else if ( cdr(seq) == 0 )
	return car(seq);
    else
	return apply(f, list2(car(seq), rins(f, cdr(seq))));
}

struct obj* apply(struct node* f, struct obj* obj) {
    if ( obj == bottom )
	return obj;		/* f is strict (bottom-preserving) */
    switch( fcnkind(f) ) {
      case COMPOSE:
	return apply(arg1(f), apply(arg2(f), obj));
      case CONSTRUCT:		/* [f1,f2,...,fN]:<a1,...,aK> */
	return do_construct(f, obj);
      case CONDITION:
	{
	    struct obj* t = apply(arg1(f), obj);
	    if ( t == true )
		return apply(arg2(f), obj);
	    else if ( t == false )
		return apply(arg3(f), obj);
	    else
		return bottom;
	}
      case CONSTANT:
	return constant_object(f);
      case APPLY_TO_ALL:
	return do_apply_to_all(arg1(f), obj);
      case RINS:
	return rins(arg1(f), obj);
      case USERFCN:
	return apply(retrieve(userfcn_name(f)), obj);
      case ID_SY:
	return obj;
      case OUT_SY:
	printobj(obj);
	return obj;
      case INT_SY:
	return select(obj, int_sy_val(f));
      case TL_SY:
	return tl(obj);
      case NUL_SY:
	return nul(obj);
      case REVERSE_SY:
	return reverse(obj);
      case SPLIT_SY:
	return split(obj);
      case PAIR_SY:
	return pair(obj);
      case TRANS_SY:
	return trans(obj);
      case LENGTH_SY:
	return length(obj);
      case EQL_SY:
	return eql(obj);
      case NE_SY:
	return ne(obj);
      case LT_SY:
	return lt(obj);
      case LE_SY:
	return le(obj);
      case GT_SY:
	return gt(obj);
      case GE_SY:
	return ge(obj);
      case ADD_SY:
	return add(obj);
      case SUBTRACT_SY:
	return subtract(obj);
      case MULTIPLY_SY:
	return multiply(obj);
      case DIVIDE_SY:
	return divide(obj);
      case AND_SY:
	return and(obj);
      case OR_SY:
	return or(obj);
      case NOT_SY:
	return not(obj);
      case ATOM_SY:
	return atom(obj);
      case DISTL_SY:
	return distl(obj);
      case DISTR_SY:
	return distr(obj);
      case APNDL_SY:
	return apndl(obj);
      case APNDR_SY:
	return apndr(obj);
      case ROTL_SY:
	return rotl(obj);
      case ROTR_SY:
	return rotr(obj);
      case CONCAT_SY:
	return concat(obj);
      default:
	fprintf(stderr, "apply: function not implemented.\n");
	abort();
    }
}
