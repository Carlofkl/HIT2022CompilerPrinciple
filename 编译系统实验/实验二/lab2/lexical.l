%{
    #include "syntax.tab.h"
    #include "Tree.c"
    extern int hasError;

    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

%option yylineno

INT 0|[1-9][0-9]*
FLOAT {INT}(\.{INT})?
ID [_a-zA-Z][_a-zA-Z0-9]*
SEMI ;
COMMA ,
ASSIGNOP =
RELOP >|<|>=|<=|==|!=
PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
DOT \.
NOT !
TYPE int|float
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while

%%

{INT} { yylval.node = createTerminal(yylineno, "INT", yytext); return INT; }
{FLOAT} { yylval.node = createTerminal(yylineno, "FLOAT", yytext); return FLOAT; }
{SEMI} { yylval.node = createTerminal(yylineno, "SEMI", NULL); return SEMI; }
{COMMA} { yylval.node = createTerminal(yylineno, "COMMA", NULL); return COMMA; }
{ASSIGNOP} { yylval.node = createTerminal(yylineno, "ASSIGNOP", NULL); return ASSIGNOP; }
{RELOP} { yylval.node = createTerminal(yylineno, "RELOP", NULL); return RELOP; }
{PLUS} { yylval.node = createTerminal(yylineno, "PLUS", NULL); return PLUS; }
{MINUS} { yylval.node = createTerminal(yylineno, "MINUS", NULL); return MINUS; }
{STAR} { yylval.node = createTerminal(yylineno, "STAR", NULL); return STAR; }
{DIV} { yylval.node = createTerminal(yylineno, "DIV", NULL); return DIV; }
{AND} { yylval.node = createTerminal(yylineno, "AND", NULL); return AND; }
{OR} { yylval.node = createTerminal(yylineno, "OR", NULL); return OR; }
{DOT} { yylval.node = createTerminal(yylineno, "DOT", NULL); return DOT; }
{NOT} { yylval.node = createTerminal(yylineno, "NOT", NULL); return NOT; }
{TYPE} { yylval.node = createTerminal(yylineno, "TYPE", yytext); return TYPE; }
{LP} { yylval.node = createTerminal(yylineno, "LP", NULL); return LP; }
{RP} { yylval.node = createTerminal(yylineno, "RP", NULL); return RP; }
{LB} { yylval.node = createTerminal(yylineno, "LB", NULL); return LB; }
{RB} { yylval.node = createTerminal(yylineno, "RB", NULL); return RB; }
{LC} { yylval.node = createTerminal(yylineno, "LC", NULL); return LC; }
{RC} { yylval.node = createTerminal(yylineno, "RC", NULL); return RC; }
{STRUCT} { yylval.node = createTerminal(yylineno, "STRUCT", NULL); return STRUCT; }
{RETURN} { yylval.node = createTerminal(yylineno, "RETURN", NULL); return RETURN; }
{IF} { yylval.node = createTerminal(yylineno, "IF", NULL); return IF; }
{ELSE} { yylval.node = createTerminal(yylineno, "ELSE", NULL); return ELSE; }
{WHILE} { yylval.node = createTerminal(yylineno, "WHILE", NULL); return WHILE; }
{ID} { yylval.node = createTerminal(yylineno, "ID", yytext); return ID; }
[ \t] { }
\n { yycolumn = 1; }
. { hasError = 1; printf("Error type A at Line %d: Mysteriout characters \'%s\'\n", yylineno, yytext); }

%%