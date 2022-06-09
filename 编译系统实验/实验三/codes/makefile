CC = gcc
FLEX = flex
BISON = bison

parser: main.c lex.yy.c syntax.tab.c 
	$(CC) main.c syntax.tab.c -lfl -o parser -g

lex.yy.c: lexical.l
	$(FLEX) lexical.l

syntax.tab.c: syntax.y
	$(BISON) -d -t syntax.y

.PHONY: clean test
test:
	./parser test1.c
	./parser test2.c
	./parser test3.c
	./parser test4.c
	./parser test5.c
	./parser test6.c
	./parser test7.c
	./parser test8.c
	./parser test9.c
	./parser test10.c
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
