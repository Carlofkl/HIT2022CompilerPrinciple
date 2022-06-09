objs = lex.yy.c scanner syntax.yy.c\
	syntax.tab.c syntax.tab.h parser\
	syntax.output

cc: main.c lex.yy.c syntax.tab.c semantic.c
	gcc main.c syntax.tab.c semantic.c -lfl -ly -o cc

scanner: main.c lex.yy.c
	gcc main.c lex.yy.c -lfl -o scanner

lex.yy.c: lexical.l
	flex lexical.l

syntax.tab.c: syntax.y
	bison -d -v syntax.y

syntax.tab.h: syntax.y
	bison -d syntax.y

.PHONY: clean
clean:
	rm -rf $(objs)