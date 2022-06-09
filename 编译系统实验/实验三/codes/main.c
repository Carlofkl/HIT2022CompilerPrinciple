#include<stdio.h>
#include<stdlib.h>
#include<string.h>
extern int syn_error;
extern int lex_error;
FILE* out;
int main(int argc, char** argv) {
        //yydebug = 1;
        FILE* fp;
	if (argc <= 1) 
	{
		return 1;
	}
	FILE* f = fopen(argv[1],"r");
	if (!f) 
	{
		perror(argv[1]);
		return 1;
	}
	if(argc > 2)
	{
		out = fopen(argv[2],"w");
	}
	yyrestart(f);
	yyparse();	
	return 0;
}
