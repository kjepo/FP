/* main.c */

#include <stdio.h>
#include "y.tab.h"
#include "obj.h"    
#include "tree.h"
extern int yyparse();

main() {
    init_obj();
    (void) yyparse();	/* all functions are installed in the symbol table */
}
