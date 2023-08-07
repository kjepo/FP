/* primitives.c */

#include <stdio.h>
#include <stdlib.h>
#include "obj.h"
#include "tree.h"
#include "primitives.h"

struct obj* copy_seq(struct obj* seq) {
    return (seq == 0) ? 0 : cons(car(seq), copy_seq(cdr(seq)));
}

struct obj* merge(struct obj* seq1, struct obj* seq2) {
    struct obj* copy;
    struct obj* p;

    if ( seq1 == 0 )
	return seq2;
    copy = copy_seq(seq1);
    for(p = copy; cdr(p); p = cdr(p))
	;
    setcdr(p, seq2);
    return copy;
}


struct obj* select(struct obj* seq, int index) {
    /* index == 1 gives first element */
    int i;
    struct obj* p = seq;

    if ( index < 1 )
	return bottom;
    for(i = 1; i < index; i++)
	if ( (p = cdr(p)) == 0 )
	    return bottom;
    return car(p);
}

struct obj* tl(struct obj* seq) {
    return seq == 0 ? bottom : cdr(seq);
}

struct obj* nul(struct obj* seq) {
    return seq == 0 ? true : false;
}

struct obj* reverse_1(struct obj* seq, struct obj* acc) {
    return seq == 0 ? acc : reverse_1(cdr(seq), cons(car(seq), acc));
}

struct obj* reverse(struct obj* seq) {
    return reverse_1(seq, 0);
}

struct obj* split1(struct obj* l, struct obj* r, int ln, int rn) {
    /* length of l */
    /* length of r */
    return (ln >= rn)
	? list2(reverse(l), r)
	: split1(cons(car(r), l), cdr(r), ln+1, rn-1);
}

struct obj* split(struct obj* seq) {
    int i;
    struct obj* p;

    if ( seq == 0 )
	return bottom;
    for(i = 0, p = seq; p; i++)
	p = cdr(p);
    return split1(0, seq, 0, i);
}

struct obj* pair(struct obj* seq) {
    return (seq == 0 || cdr(seq) == 0)
	? seq
	: cons(list2(car(seq), cadr(seq)), pair(cddr(seq)));
}

struct obj* trans(struct obj* seq) {
    if ( seq == 0 || !seqp(car(seq)) )
	return bottom;
    else if ( car(seq) == 0 )
	return 0;
    else
	return cons(map(car, seq), trans(map(cdr, seq)));
}

struct obj* length(struct obj* seq) {
    int i = 0;
    for(i = 0; seq; i++)
	if ( (seq = cdr(seq)) == bottom )
	    return bottom;
    return mk_int(i);
}

struct obj* eql(struct obj* seq) {
    struct obj* arg1;
    struct obj* rest;

    if ( seq == 0 )
	return true;	/* =:<> ==> T */

    arg1 = car(seq);
    for(rest = cdr(seq); rest; rest = cdr(rest))
	if ( !cmpobj(car(rest), arg1) )
	    return false;
    return true;
}

struct obj* ne(struct obj* seq) {
    if ( seq == 0 || cdr(seq) == 0 )
	return bottom;
    return (OBJINT(car(seq)) != OBJINT(cadr(seq))) ? true : false;
}

struct obj* gt(struct obj* seq) {
    if ( seq == 0 || cdr(seq) == 0 )
	return bottom;
    return (OBJINT(car(seq)) > OBJINT(cadr(seq))) ? true : false;
}

struct obj* ge(struct obj* seq) {
    if ( seq == 0 || cdr(seq) == 0 )
	return bottom;
    return (OBJINT(car(seq)) >= OBJINT(cadr(seq))) ? true : false;
}

struct obj* lt(struct obj* seq) {
    if ( seq == 0 || cdr(seq) == 0 )
	return bottom;
    return (OBJINT(car(seq)) < OBJINT(cadr(seq))) ? true : false;
}

struct obj* le(struct obj* seq) {
    if ( seq == 0 || cdr(seq) == 0 )
	return bottom;
    return (OBJINT(car(seq)) <= OBJINT(cadr(seq))) ? true : false;
}

struct obj* add(struct obj* seq) {
    int res;
    struct obj* rest;

    for(res = 0, rest = seq; rest; rest = cdr(rest))
	if ( car(rest) == bottom )
	    return bottom;
	else
	    res += OBJINT(car(rest));
    return mk_int(res);
}

struct obj* subtract(struct obj* seq) {
    int res;
    int sign = 1;
    struct obj* rest;

    for(res = 0, rest = seq; rest; rest = cdr(rest)) {
	if ( car(rest) == bottom )
	    return bottom;
	else
	    res += sign * OBJINT(car(rest));
	sign = -1;
    }
    return mk_int(res);
}

struct obj* multiply(struct obj* seq) {
    int res;
    struct obj* rest;

    for(res = 1, rest = seq; rest; rest = cdr(rest))
	if (car( rest) == bottom )
	    return bottom;
	else
	    res *= OBJINT(car(rest));
    return mk_int(res);
}

struct obj* divide(struct obj* seq) {
    int res;
    int first = 1;
    struct obj* rest;

    for(res = 1, rest = seq; rest; rest = cdr(rest)) {
	if ( car(rest) == bottom )
	    return bottom;
	else if ( first )
	    res = OBJINT(car(rest));
	else if ( OBJINT(car(rest)) )
	    res /= OBJINT(car(rest));
	else
	    return bottom;	/* 1/0 ==> bottom */
	first = 0;
    }
    return mk_int(res);
}

struct obj* and(struct obj* seq) {
    struct obj* rest;

    for(rest = seq; rest; rest = cdr(rest))
	if ( car(rest) == bottom )
	    return bottom;
	else if ( car(rest) == false )
	    return false;
	else if ( car(rest) != true )
	    return bottom;
    return true;
}

struct obj* or(struct obj* seq) {
    struct obj* rest;

    for(rest = seq; rest; rest = cdr(rest))
	if ( car(rest) == bottom )
	    return bottom;
	else if ( car(rest) == true )
	    return true;
	else if ( car(rest) != false )
	    return bottom;
    return false;
}

struct obj* not(struct obj* seq) {
    if ( seq == false )
	return true;
    else if ( seq == true )
	return false;
    else return
	bottom;
}

struct obj* atom(struct obj* seq) {
    if ( seq == 0 )
	return true;		/* empty list is both an atom and a sequence */

    switch( OBJTAG(seq) ) {
      case BOTTOM:		/* caught by apply, per se */
      case TRUE:
      case FALSE:
      case NAME:
      case INTEGER:
	return true;
      case SEQ:
	return false;
      default:
	fprintf(stderr, "atom: can't happen.\n");
	abort();
    }
}

/* distl:<y,<>> ==> <> */
/* distl:<y,<z1,...,zN>> ==> <<y,z1>,<y,z2>,...,<y,zN>> */

struct obj* distl1(struct obj* y, struct obj* zs) {
    return (zs == 0) ? 0 : cons(list2(y, car(zs)), distl1(y, cdr(zs)));
}

struct obj* distl(struct obj* seq) {
    /* seq must be a pair, the 2nd component being a sequence */
    if ( seq == 0 || cdr(seq) == 0 || !seqp(cadr(seq)) )
	return bottom;
    return distl1(car(seq), cadr(seq));
}

/* distr:<<>,z> ==> <> */
/* distr:<<y1,...,yN>,z> ==> <<y1,z>,<y2,z>,...,<yN,z>> */

struct obj* distr1(struct obj* ys, struct obj* z) {
    return (ys == 0) ? 0 : cons(list2(car(ys), z), distr1(cdr(ys), z));
}

struct obj* distr(struct obj* seq) {
    /* seq must be a pair, the 1st component being a sequence */
    return (seq == 0 || cdr(seq) == 0 || !seqp(car(seq)))
	? bottom
        : distr1(car(seq), cadr(seq));
}

/* apndl:<y,<>> ==> <y> */
/* apndl:<y,<z1,...,zN>> ==> <y,z1,...,zN> */

struct obj* apndl(struct obj* seq) {
    /* seq must be a pair, the 2nd component being a sequence */
    if ( seq == 0 || cdr(seq) == 0 || !seqp(cadr(seq)) )
	return bottom;
    else
	return cons(car(seq), cadr(seq));
}

/* apndr:<<>,z> ==> <z> */
/* apndr:<<y1,...,yN>,z> ==> <y1,...,yN,z> */

struct obj* apndr(struct obj* seq) {
    /* seq must be a pair, the 1st component being a sequence */
    if ( seq == 0 || cdr(seq) == 0 || !seqp(car(seq)) )
	return bottom;
    return reverse(apndl(list2(cadr(seq), reverse(car(seq)))));
}

/* rotl:<> ==> <>
   rotl:<x> ==> <x>
   rotl:<x1,x2,...,xN> ==> <x2,...,xN,x1> */

struct obj* rotl(struct obj* seq) {
    if ( seq == 0 || cdr(seq) == 0 )
	return seq;
    else
	return apndr(list2(cdr(seq), car(seq)));
}

/* rotr:<> ==> <>
   rotr:<x> ==> <x>
   rotr:<x1,x2,...,xN> ==> <xN,x1,...,xN-1> */

struct obj* rotr(struct obj* seq) {
    struct obj* r;

    if ( seq == 0 || cdr(seq) == 0 )
	return seq;
    else {
	r = reverse(seq);	/* <xN,...,x1> */
	return cons(car(r), reverse(cdr(r)));
    }
}

/* concat:<<a1,...,aN>,<b1,...,bN>,...,<c1,...,cN>>
      ==> <a1,...,aN,b1,...,bN,c1,...,cN>
 */
  
struct obj* concat(struct obj* seq) {
    if ( seq == 0 )
	return 0;
    else if ( !seqp(car(seq)) )
	return bottom;
    else
	return merge(car(seq), concat(cdr(seq)));
}
