/* scan.c */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "y.tab.h"

struct {
    char *txt;
    int token;
} keytab[] = {
    { "and",	AND	},
    { "apndl",	APNDL	},
    { "apndr",	APNDR	},
    { "atom",	ATOM	},
    { "concat",	CONCAT	},
    { "distl",	DISTL	},
    { "distr",	DISTR	},
    { "eq",	EQL	},
    { "ge",	GE	},
    { "gt",	GT	},
    { "id",	ID	},
    { "le",	LE	},
    { "length",	LENGTH	},
    { "lt",	LT	},
    { "ne",	NE	},
    { "not",	NOT	},
    { "null",	NUL	},
    { "or",	OR	},
    { "out",	OUT	},
    { "pair",	PAIR	},
    { "reverse",REVERSE	},
    { "rotl",	ROTL	},
    { "rotr",	ROTR	},
    { "split",	SPLIT	},
    { "tl",	TL	},
    { "trans",	TRANS	},
};
	
#define N (sizeof(keytab)/sizeof(keytab[0]))

int classify(char *s) {
    int low = 0, high = N - 1, mid, cmp;

    do {					/* binary search */
	mid = (low + high) / 2;
	if ( (cmp = strcmp(s, keytab[mid].txt)) > 0 )
	    low = mid + 1;
	else
	    high = mid - 1;
    } while( cmp != 0 && low <= high );

    if ( cmp == 0 )				/* a hit */
	return keytab[mid].token;
    else
	return IDENT;
}

int yylineno = 1;

int yylex() {
    char ch, look;
    char id[80];

    do {
	if ( (ch = getchar()) == EOF )
	    return EOF;
	if ( ch == '\n' )
	    yylineno++;
    } while( isspace(ch) );

    switch( ch ) {
      case '+':
	return ADD;
      case '-':
	if ( (look = getchar()) == EOF )	/* - EOF */
	    return SUBTRACT;
	else if ( look == '>' )			/* -> */
	    return ARROW;
	else if ( look == '-' ) {		/* -- */
	    while( (look = getchar()) != EOF && look != '\n' )
		;
	    yylineno++;
	    (void)ungetc(look, stdin);		/* no-op if EOF */
	    return yylex();
	} else {				/* -foo */
	    (void)ungetc(look, stdin);
	    return SUBTRACT;
	}
      case '*':
	return MULTIPLY;
      case '/':
	return DIVIDE;
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
	ungetc(ch, stdin);
	scanf("%d", &yylval.intval);
	return INT;
      default:
	if ( isalpha(ch) || ch == '_' || ch == '?' ) {
	    char *p = id;
	    do {
		*p++ = ch;
	    } while( (ch = getchar()) != EOF && (isalnum(ch) || ch == '_' || ch == '?') );
	    (void)ungetc(ch, stdin);
	    *p = '\0';
	    yylval.name = malloc(strlen(id)+1);
	    (void)strcpy(yylval.name, id);
	    return classify(id);
	} else
	    return ch;
    }
}
