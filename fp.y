%{
#include <stdio.h>
#include <string.h>
#include "obj.h"
#include "tree.h"
#include "symtab.h"
#include "apply.h"
extern int yylineno;
extern int yylex();
void yyerror(char* s);

%}

%token IDENT ARROW ID OUT INT TL NUL REVERSE SPLIT PAIR TRANS LENGTH
%token EQL NE LT LE GT GE ADD SUBTRACT MULTIPLY DIVIDE AND OR NOT ATOM
%token DISTL DISTR APNDL APNDR ROTL ROTR CONCAT

%left '@'
%left '&' '\''

%union {
    struct node* treep;			/* abstract syntax tree */
    struct obj* objp;			/* pointer to FP object */
    int intval;				/* integer value when token=INT */
    char* name;				/* string value when token=IDENT */
}

%type <treep>   fcn_list fcn prim_fcn
%type <objp>	obj_list obj
%type <intval>	INT
%type <name>	IDENT

%%

fp_defs
	: 
	| fp_app fp_defs
	| fp_def fp_defs
	;

fp_app
    	: fcn ':' obj
		{ printobj(apply($1, $3)); }    
	;

fp_def
	: '{' IDENT fcn '}'
		{ install($2, $3); }
	;

fcn_list
    	:
		{ $$ = 0; }
	| fcn
		{ $$ = mk_construct($1, 0); }
	| fcn ',' fcn_list
		{ $$ = mk_construct($1, $3); }
	;

fcn
	: IDENT
		{ $$ = mk_userfcn($1); }
	| prim_fcn
		{ $$ = $1; }
	| fcn '@' fcn
		{ $$ = mk_compose($1, $3); }
	| '[' fcn_list ']'
		{ $$ = $2; }
	| '(' fcn ARROW fcn ';' fcn ')'
		{ $$ = mk_condition($2, $4, $6); }
	| '&' fcn
		{ $$ = mk_apply_to_all($2); }
	| '%' obj
		{ $$ = mk_constant($2); }
	| '\'' fcn
		{ $$ = mk_rins($2); }
	;

prim_fcn
	: ID
		{ $$ = mk_prim(ID_SY); }
	| OUT
		{ $$ = mk_prim(OUT_SY); }
	| INT
		{ $$ = mk_select(INT_SY, $1); }
	| TL
		{ $$ = mk_prim(TL_SY); }
	| NUL
		{ $$ = mk_prim(NUL_SY); }
	| REVERSE
		{ $$ = mk_prim(REVERSE_SY); }
	| SPLIT
		{ $$ = mk_prim(SPLIT_SY); }
	| PAIR
		{ $$ = mk_prim(PAIR_SY); }
	| TRANS
		{ $$ = mk_prim(TRANS_SY); }
	| LENGTH
		{ $$ = mk_prim(LENGTH_SY); }
	| EQL
		{ $$ = mk_prim(EQL_SY); }
	| NE
		{ $$ = mk_prim(NE_SY); }
	| LT
		{ $$ = mk_prim(LT_SY); }
	| LE
		{ $$ = mk_prim(LE_SY); }
	| GT
		{ $$ = mk_prim(GT_SY); }
	| GE
		{ $$ = mk_prim(GE_SY); }
	| ADD
		{ $$ = mk_prim(ADD_SY); }
	| SUBTRACT
		{ $$ = mk_prim(SUBTRACT_SY); }
	| MULTIPLY
		{ $$ = mk_prim(MULTIPLY_SY); }
	| DIVIDE
		{ $$ = mk_prim(DIVIDE_SY); }
	| AND
		{ $$ = mk_prim(AND_SY); }
	| OR
		{ $$ = mk_prim(OR_SY); }
	| NOT
		{ $$ = mk_prim(NOT_SY); }
	| ATOM
		{ $$ = mk_prim(ATOM_SY); }
	| DISTL
		{ $$ = mk_prim(DISTL_SY); }
	| DISTR
		{ $$ = mk_prim(DISTR_SY); }
	| APNDL
		{ $$ = mk_prim(APNDL_SY); }
	| APNDR
		{ $$ = mk_prim(APNDR_SY); }
	| ROTL
		{ $$ = mk_prim(ROTL_SY); }
	| ROTR
		{ $$ = mk_prim(ROTR_SY); }
	| CONCAT
		{ $$ = mk_prim(CONCAT_SY); }

obj_list
	:
		{ $$ = 0; }
	| obj
		{ $$ = cons($1, 0); }
	| obj ',' obj_list
		{ $$ = cons($1, $3); }
	;

obj
	: '!'
		{ $$ = bottom; }
	| IDENT
		{ if ( !strcmp($1, "T") )
		      $$ = true;
		  else if ( !strcmp($1, "F") )
		      $$ = false;
		  else 
		      $$ = mk_name($1); }
	| INT
		{ $$ = mk_int($1); }
	| '<' obj_list '>'
		{ $$ = $2; }
	;

%%

void yyerror(char* s) {
    fprintf(stderr, "%s: at line %d\n", s, yylineno);
}
