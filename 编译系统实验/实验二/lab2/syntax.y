%{
    #include "lex.yy.c"
    int hasError = 0;
    void yyerror(char* msg);
    extern Tree root;
%}

%locations
%union
{
    struct TNode* node;
}

%token <node> INT FLOAT ID
%token <node> SEMI COMMA
%token <node> ASSIGNOP RELOP PLUS MINUS STAR DIV
%token <node> AND OR NOT
%token <node> DOT TYPE
%token <node> LP RP LB RB LC RC
%token <node> STRUCT RETURN IF ELSE WHILE

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def Dec DecList
%type <node> Exp Args 

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%%

Program: ExtDefList {
    $$ = createNonTerminal("Program");
    insertNode($$, 1, $1);
    root = $$;
}
    ;
ExtDefList: ExtDef ExtDefList {
    $$ = createNonTerminal("ExtDefList");
    insertNode($$, 2, $2, $1);
}
    | { $$ = NULL; }
    ;
ExtDef: Specifier ExtDecList SEMI {
    $$ = createNonTerminal("ExtDef");
    insertNode($$, 3, $3, $2, $1);
}
    | Specifier SEMI {
    $$ = createNonTerminal("ExtDef");
    insertNode($$, 2, $2, $1);
}
    | Specifier FunDec CompSt {
    $$ = createNonTerminal("ExtDef");
    insertNode($$, 3, $3, $2, $1);
}
    | error SEMI { hasError = 1; }
    ;
ExtDecList: VarDec {
    $$ = createNonTerminal("ExtDecList");
    insertNode($$, 1, $1);
}
    | VarDec COMMA ExtDecList {
    $$ = createNonTerminal("ExtDecList");
    insertNode($$, 3, $3, $2, $1);
}
    ;


Specifier: TYPE {
    $$ = createNonTerminal("Specifier");
    insertNode($$, 1, $1);
}
    | StructSpecifier {
    $$ = createNonTerminal("Specifier");
    insertNode($$, 1, $1);
}
    ;
StructSpecifier: STRUCT OptTag LC DefList RC {
    $$ = createNonTerminal("StructSpecifier");
    insertNode($$, 5, $5, $4, $3, $2, $1);
}
    | STRUCT Tag {
    $$ = createNonTerminal("StructSpecifier");
    insertNode($$, 2, $2, $1);
}
    | error RC { hasError = 1; }
    ;
OptTag: ID {
    $$ = createNonTerminal("OptTag");
    insertNode($$, 1, $1);
}
    | { $$ = NULL; }
    ;
Tag: ID {
    $$ = createNonTerminal("Tag");
    insertNode($$, 1, $1);
}
    ;


VarDec: ID {
    $$ = createNonTerminal("VarDec");
    insertNode($$, 1, $1);
}
    | VarDec LB INT RB {
    $$ = createNonTerminal("VarDec");
    insertNode($$, 4, $4, $3, $2, $1);
}
    | error RB { hasError = 1; }
    ;
FunDec: ID LP VarList RP {
    $$ = createNonTerminal("FunDec");
    insertNode($$, 4, $4, $3, $2, $1);
}
    | ID LP RP {
    $$ = createNonTerminal("FunDec");
    insertNode($$, 3, $3, $2, $1);
}
    | error RP { hasError = 1; }
    ;
VarList: ParamDec COMMA VarList {
    $$ = createNonTerminal("VarList");
    insertNode($$, 3, $3, $2, $1);
}
    | ParamDec {
    $$ = createNonTerminal("VarList");
    insertNode($$, 1, $1);
}
    ;
ParamDec: Specifier VarDec {
    $$ = createNonTerminal("ParamDec");
    insertNode($$, 2, $2, $1);
}
    ;


CompSt: LC DefList StmtList RC {
    $$ = createNonTerminal("CompSt");
    insertNode($$, 4, $4, $3, $2, $1);
}
    | error RC { hasError = 1; }
    ;
StmtList: Stmt StmtList {
    $$ = createNonTerminal("StmtList");
    insertNode($$, 2, $2, $1);
}
    | { $$ = NULL; }
    ;
Stmt: Exp SEMI {
    $$ = createNonTerminal("Stmt");
    insertNode($$, 2, $2, $1);
}
    | CompSt {
    $$ = createNonTerminal("Stmt");
    insertNode($$, 1, $1);
}
    | RETURN Exp SEMI {
    $$ = createNonTerminal("Stmt");
    insertNode($$, 3, $3, $2, $1);
}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
    $$ = createNonTerminal("Stmt");
    insertNode($$, 5, $5, $4, $3, $2, $1);
}
    | IF LP Exp RP Stmt ELSE Stmt {
    $$ = createNonTerminal("Stmt");
    insertNode($$, 7, $7, $6, $5, $4, $3, $2, $1);
}
    | WHILE LP Exp RP Stmt {
    $$ = createNonTerminal("Stmt");
    insertNode($$, 5, $5, $4, $3, $2, $1);
}
    | error SEMI { hasError = 1; }
    ;


DefList: Def DefList {
    $$ = createNonTerminal("DefList");
    insertNode($$, 2, $2, $1);
}
    | { $$ = NULL; }
    ;
Def: Specifier DecList SEMI {
    $$ = createNonTerminal("Def");
    insertNode($$, 3, $3, $2, $1);
}
    | error SEMI { hasError = 1; }
    ;
DecList: Dec {
    $$ = createNonTerminal("DecList");
    insertNode($$, 1, $1);
}
    | Dec COMMA DecList {
    $$ = createNonTerminal("DecList");
    insertNode($$, 3, $3, $2, $1);
}
    ;
Dec: VarDec {
    $$ = createNonTerminal("Dec");
    insertNode($$, 1, $1);
}
    | VarDec ASSIGNOP Exp {
    $$ = createNonTerminal("Dec");
    insertNode($$, 3, $3, $2, $1);
}
    ;


Exp: Exp ASSIGNOP Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp AND Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp OR Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp RELOP Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp PLUS Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp MINUS Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp STAR Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp DIV Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | LP Exp RP {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | MINUS Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 2, $2, $1);
}
    | NOT Exp {
    $$ = createNonTerminal("Exp");
    insertNode($$, 2, $2, $1);
}
    | ID LP Args RP {
    $$ = createNonTerminal("Exp");
    insertNode($$, 4, $4, $3, $2, $1);
}
    | ID LP RP {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp LB Exp RB {
    $$ = createNonTerminal("Exp");
    insertNode($$, 4, $4, $3, $2, $1);
}
    | Exp DOT ID {
    $$ = createNonTerminal("Exp");
    insertNode($$, 3, $3, $2, $1);
}
    | ID {
    $$ = createNonTerminal("Exp");
    insertNode($$, 1, $1);
}
    | INT {
    $$ = createNonTerminal("Exp");
    insertNode($$, 1, $1);
}
    | FLOAT {
    $$ = createNonTerminal("Exp");
    insertNode($$, 1, $1);
}
    | error RP { hasError = 1; }
    | error RB { hasError = 1; }
    ;
Args: Exp COMMA Args {
    $$ = createNonTerminal("Args");
    insertNode($$, 3, $3, $2, $1);
}
    | Exp {
    $$ = createNonTerminal("Args");
    insertNode($$, 1, $1);
}
    ;

%%

void yyerror(char* msg)
{
    fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}