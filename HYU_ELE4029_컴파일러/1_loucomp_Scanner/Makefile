# Makefile for C-Minus Scanner
# ./lex/tiny.l --> ./cminus.l

CC = gcc 

CFLAGS = -W -Wall

OBJS = main.o util.o scan.o 
OBJS_LEX = main.o util.o lex.yy.o

.PHONY: all clean
all: cminus_cimpl cminus_lex

clean:
	-rm -vf cminus_cimpl cminus_lex *.o lex.yy.c

cminus_cimpl: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) 

cminus_lex: $(OBJS_LEX)
	$(CC) $(CFLAGS) -o $@ $(OBJS_LEX) -lfl

main.o: main.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c -o $@ $<

scan.o: scan.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c -o $@ $<

util.o: util.c globals.h util.h
	$(CC) $(CFLAGS) -c -o $@ $<

lex.yy.o: lex.yy.c globals.h util.h scan.h
	$(CC) $(CFLAGS) -c -o $@ $<

lex.yy.c: cminus.l
	flex -o $@ $<

