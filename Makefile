# Makefile for FP
OBJS=obj.o tree.o y.tab.o scanner.o main.o apply.o primitives.o symtab.o
CC=gcc
#CFLAGS=-ansi

fp:	$(OBJS)
	$(CC) -o fp $(CFLAGS) $(OBJS) -ll

symtab.o:	symtab.h
main.o:		obj.h tree.h y.tab.h
primitives.o:	obj.h tree.h primitives.h
apply.o:	obj.h tree.h symtab.h apply.h
tree.o:		tree.h obj.h
y.tab.o:	obj.h tree.h
scanner.o:	y.tab.h
obj.o:		obj.h


y.tab.c y.tab.h:	fp.y
	yacc -d fp.y

clean:
	rm -f fp $(OBJS) y.tab.c y.tab.h y.output a.out core
