/* primitives.h */

extern struct obj* select(struct obj*, int);
extern struct obj* tl(struct obj*);
extern struct obj* nul(struct obj*);
extern struct obj* reverse(struct obj*);
extern struct obj* split(struct obj*);
extern struct obj* pair(struct obj*);
extern struct obj* trans(struct obj*);
extern struct obj* length(struct obj*);
extern struct obj* eql(struct obj*);
extern struct obj* ne(struct obj*);
extern struct obj* gt(struct obj*);
extern struct obj* ge(struct obj*);
extern struct obj* lt(struct obj*);
extern struct obj* le(struct obj*);
extern struct obj* add(struct obj*);
extern struct obj* subtract(struct obj*);
extern struct obj* multiply(struct obj*);
extern struct obj* divide(struct obj*);
extern struct obj* and(struct obj*);
extern struct obj* or(struct obj*);
extern struct obj* not(struct obj*);
extern struct obj* atom(struct obj*);
extern struct obj* distl(struct obj*);
extern struct obj* distr(struct obj*);
extern struct obj* apndl(struct obj*);
extern struct obj* apndr(struct obj*);
extern struct obj* rotl(struct obj*);
extern struct obj* rotr(struct obj*);
extern struct obj* concat(struct obj*);
