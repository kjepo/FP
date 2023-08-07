/* symtab.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "obj.h"
#include "tree.h"
#include "symtab.h"

#define TABSIZE 1001		/* size of hash table */

struct {
    int			len;			/* len != 0 => occupied */
    char*		str;
    struct node*	fcn;
} htab[TABSIZE];

/* hash -- return hash value in the range 0 .. TABSIZE-1 from s */
int hash(char* s) {
    unsigned int r;

    for(r = 0; *s; s++) {
	r = (r << 4) + *s;
	if (r > 0x0fffffff) {
	    r ^= (r >> 24) & 0xf0;
	    r &= 0x0fffffff;
	}
    }
    return r % TABSIZE;
}

void install(char* s, struct node* fcn) {
    static int count = TABSIZE - 1;
    int len = strlen(s);
    int id = hash(s);

    while( htab[id].len && strcmp(htab[id].str, s) )
	id = (id + 1) % TABSIZE;

    if ( htab[id].len == 0 ) {	/* empty */
	htab[id].str = malloc(len + 1);
	strcpy(htab[id].str, s);
	htab[id].fcn = fcn;
	htab[id].len = len;
    } else			/* replace it */
	htab[id].fcn = fcn;

    if ( --count <= 0 ) {
	fprintf(stderr, "install: hashtable overflow.\n");
	abort();
    }
}

struct node* retrieve(char* s) {
    int id = hash(s);

    while( htab[id].len && strcmp(htab[id].str, s) )
	id = (id + 1) % TABSIZE;

    if ( htab[id].len == 0 ) {
	fprintf(stderr, "function %s is not defined.\n", s);
	exit(1);		/* not very user friendly... */
    } else
	return htab[id].fcn;
}
