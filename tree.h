/* tree.h */

typedef enum {
    COMPOSE, CONSTRUCT, CONDITION, CONSTANT, APPLY_TO_ALL, USERFCN, RINS,
    /* primitive operators */
    ID_SY, OUT_SY, INT_SY, TL_SY, NUL_SY, REVERSE_SY, SPLIT_SY,
    PAIR_SY, TRANS_SY, LENGTH_SY, EQL_SY, NE_SY, LT_SY, LE_SY, GT_SY,GE_SY,
    ADD_SY, SUBTRACT_SY, MULTIPLY_SY, DIVIDE_SY, AND_SY, OR_SY, NOT_SY,
    ATOM_SY, DISTL_SY, DISTR_SY, APNDL_SY, APNDR_SY, ROTL_SY, ROTR_SY,
    CONCAT_SY
} NODEKIND;

struct node {
    NODEKIND tag;
    union {
	/* INT_SY (selector function) */
	struct {
	    int intval;
	} int_sy;
	/* USERFCN */
	struct {
	    char* name;
	} userfcn;
	/* COMPOSE */
	struct {
	    struct node* car;
	    struct node* cdr;
	} compose;
	/* CONSTRUCT */
	struct {
	    struct node* car;
	    struct node* cdr;
	} construct;
	/* CONDITION */
	struct {
	    struct node* pred;
	    struct node* true;
	    struct node* false;
	} condition;
	/* CONSTANT */
	struct {
	    struct obj* objp;
	} constant;
	/* APPLY_TO_ALL */
	struct {
	    struct node* f;
	} apply_to_all;
	/* RINS */
	struct {
	    struct node* f;
	} rins;
	struct {
	    struct node* field1;
	    struct node* field2;
	    struct node* field3;
	} backdoor;
    } u_node;
};

extern struct node* mk_prim(NODEKIND);
extern struct node* mk_select(NODEKIND, int);
extern struct node* mk_construct(struct node*, struct node*);
extern struct node* mk_userfcn(char*);
extern struct node* mk_compose(struct node*, struct node*);
extern struct node* mk_condition(struct node*, struct node*, struct node*);
extern struct node* mk_apply_to_all(struct node*);
extern struct node* mk_rins(struct node*);
extern struct node* mk_constant(struct obj*);
extern NODEKIND fcnkind(struct node*);
extern struct node* arg1(struct node*);
extern struct node* arg2(struct node*);
extern struct node* arg3(struct node*);
extern struct obj* constant_object(struct node*);
extern char* userfcn_name(struct node*);
extern int int_sy_val(struct node*);
extern void print_tree(struct node*);
