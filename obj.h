/* obj.h */

/* tags for objects */
#define BOTTOM   00
#define TRUE     01
#define FALSE    02
#define NAME     03
#define INTEGER  04
#define SEQ      05
    
struct obj {
    int tag;
    union {
	int intval;		/* tag=INTEGER */
	char *name;		/* tag=NAME */
	struct objlist {	/* tag=SEQ */
	    struct obj *car;
	    struct obj *cdr;
	} seqp;
    } u_obj;
};

#define OBJTAG(p)    ((p)->tag)
#define OBJINT(p)    ((p)->u_obj.intval)
#define OBJNAME(p)   ((p)->u_obj.name)
#define OBJCAR(p)    ((p)->u_obj.seqp.car)
#define OBJCDR(p)    ((p)->u_obj.seqp.cdr)

extern struct obj* bottom;
extern struct obj* true;
extern struct obj* false;

extern void init_obj();
extern struct obj* mk_name(char*);
extern struct obj* mk_int(int);

extern struct obj* cons(struct obj*, struct obj*);
extern struct obj* car(struct obj*);
extern struct obj* cdr(struct obj*);
extern struct obj* cadr(struct obj*);
extern struct obj* cddr(struct obj*);
extern void setcdr(struct obj*, struct obj*);
extern struct obj* map(struct obj* (*)(struct obj*), struct obj*);
extern struct obj* list2(struct obj*, struct obj*);

extern int seqp(struct obj*);
extern int cmpobj(struct obj*, struct obj*);

extern void printobj1(struct obj*);
extern void printseq(struct obj*, int);
extern void printobj(struct obj*);
