/* obj.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "obj.h"

struct obj* bottom;    
struct obj* true;
struct obj* false;

void init_obj() {
    bottom = (struct obj*) malloc(sizeof(struct obj));
    OBJTAG(bottom) = BOTTOM;
    true = (struct obj*) malloc(sizeof(struct obj));
    OBJTAG(true) = TRUE;
    false = (struct obj*) malloc(sizeof(struct obj));
    OBJTAG(false) = FALSE;
}

struct obj* mk_name(char* name) {
    struct obj* p = (struct obj*) malloc(sizeof(struct obj));
    OBJTAG(p) = NAME;
    OBJNAME(p) = name;
    return p;
}

struct obj* mk_int(int n) {
    struct obj* p = (struct obj*) malloc(sizeof(struct obj));
    OBJTAG(p) = INTEGER;
    OBJINT(p) = n;
    return p;
}

/*===============================
 *     Sequence manipulation
 *===============================
 */

struct obj* cons(struct obj* item, struct obj* seq) {
    struct obj* box;

    if ( item == bottom || seq == bottom )
	return bottom;	/* no proper sequence has bottom as an element */

    box = (struct obj*) malloc(sizeof(struct obj));
    if ( !box ) {
	fprintf(stderr, "cons: ran out of memory.\n");
	abort();
    }
    OBJTAG(box) = SEQ;
    OBJCAR(box) = item;
    OBJCDR(box) = seq;
    return box;
}

struct obj* car(struct obj* seq) {
    return OBJTAG(seq) == SEQ ? OBJCAR(seq) : bottom;
}

struct obj* cdr(struct obj* seq) {
    return OBJTAG(seq) == SEQ ? OBJCDR(seq) : bottom;
}

struct obj* cadr(struct obj* seq) {
    return car(cdr(seq));
}

struct obj* cddr(struct obj* seq) {
    return cdr(cdr(seq));
}

void setcdr(struct obj* seq, struct obj* value) {
    OBJCDR(seq) = value;
}

struct obj* map(struct obj* (*f)(struct obj*), struct obj* seq) {
    return (seq == 0) ? 0 : cons((*f)(car(seq)), map(f, cdr(seq)));
}

struct obj* list2(struct obj* x, struct obj* y) {
    return cons(x, cons(y, 0));
}

int seqp(struct obj* objp) {
    return objp == 0 || OBJTAG(objp) == SEQ;
}

int cmpobj(struct obj* arg1, struct obj* arg2) {
    if ( arg1 == 0 )		/* <> */
	return arg2 == 0;
    if ( OBJTAG(arg1) != OBJTAG(arg2) )
	return 0;
    switch( OBJTAG(arg1) ) {
      case NAME:
	return !strcmp(OBJNAME(arg1), OBJNAME(arg2));
      case INTEGER:
	return OBJINT(arg1) == OBJINT(arg2);
      case BOTTOM:
      case TRUE:
      case FALSE:
	return arg1 == arg2;
      case SEQ:
	return cmpobj(car(arg1), car(arg2)) && cmpobj(cdr(arg1), cdr(arg2));
      default:
	fprintf(stderr, "cmpobj: can't happen.\n");
	abort();
    }
}

void printobj1(struct obj* objp) {
    if ( objp == 0 ) { /* <> is a SEQ but is represented with the 0-pointer. */
	printf("<>");
	return;
    }
    switch( OBJTAG(objp) ) {
      case BOTTOM:
	printf("!");
	break;
      case TRUE:
	printf("T");
	break;
      case FALSE:
	printf("F");
	break;
      case INTEGER:
	printf("%d", OBJINT(objp));
	break;
      case NAME:
	printf("%s", OBJNAME(objp));
	break;
      case SEQ:
	printseq(objp,0);
	break;
      default:
	fprintf(stderr, "can't happen -- printobj1\n");
	abort();
    }
}
	
void printseq(struct obj* objp, int dotted) {
    /* dotted=1 means invoker is printing dotted pair */
    if ( !dotted )		/* rule 1: (x .(y z)) => <x y z> */
	printf("<");
    if ( objp ) {
	printobj1(car(objp));
	if ( cdr(objp) != 0 ) {	/* rule 2: (x . NIL)  => <x> */
	    printf(",");		/* (dotted pairs should not show up) */
	    printseq(cdr(objp),1);
	}
    }
    if ( !dotted )
	printf(">");		/* rule 1 again */
}
	
void printobj(struct obj* objp) {
    printobj1(objp);
    printf("\n");
}
