# Makefile for C-Minus
#
# ./lex/tiny.l        --> ./cminus.l (from Project 1)
# ./yacc/tiny.y       --> ./cminus.y (from Project 2)
# ./yacc/globals.h    --> ./globals.h (from Project 2)

CC = gcc

CFLAGS = -W -Wall -g

OBJS = main.o util.o lex.yy.o y.tab.o symtab.o analyze.o

.PHONY: all clean
all: cminus_semantic

clean:
	rm -vf cminus_semantic *.o lex.yy.c y.tab.c y.tab.h y.output

cminus_semantic: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ -lfl

main.o: main.c globals.h util.h scan.h parse.h y.tab.h analyze.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h y.tab.h
	$(CC) $(CFLAGS) -c util.c

lex.yy.o: lex.yy.c scan.h globals.h y.tab.h util.h
	$(CC) $(CFLAGS) -c lex.yy.c

lex.yy.c: cminus.l
	flex cminus.l

y.tab.h: y.tab.c

y.tab.o: y.tab.c parse.h
	$(CC) $(CFLAGS) -c y.tab.c

y.tab.c: cminus.y
	yacc -d -v cminus.y

analyze.o: analyze.c analyze.h globals.h y.tab.h symtab.h util.h
	$(CC) $(CFLAGS) -c analyze.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c
