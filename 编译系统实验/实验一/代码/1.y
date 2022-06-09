%{
# include <unistd.h>
# include <stdio.h>   
# include "1.h"
%}

%union{
    treeNode type_node;
}

/*终结符*/
%token <type_node> INT FLOAT ID
%token <type_node> SEMI COMMA ASSIGNOP RELOP ADD SUB MUL DIV  
%token <type_node> AND OR DOT NOT TYPE LP RP LB RB LC RC
%token <type_node> STRUCT RETURN IF ELSE WHILE
%token <type_node> COMMENT SPACE AERROR NUM_ERROR ID_ERROR

/*非终结符*/
%type <type_node> Program ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag 
%type <type_node> VarDec FunDec VarList ParamDec CompSt
%type <type_node> StmtList Stmt DefList Def DecList Dec Exp Args

/*优先级*/
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left ADD SUB
%left MUL DIV
%right NOT
%left LP RP RB LB DOT
%nonassoc ELSE



%%

/*High-level Definitions*/
Program : ExtDefList 					{treeNode childList[1] = {$1}; $$= newNode("Program", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++; printTree();}
        ;
ExtDefList : ExtDef ExtDefList 			{treeNode childList[2] = {$1, $2}; $$= newNode("ExtDefList", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
           | 						{$$= newNode("ExtDefList", 0, -1, NULL);nodeList[nodeNum]=$$;nodeNum++;}
           ;
ExtDef : Specifier ExtDecList SEMI 			{treeNode childList[3] = {$1, $2, $3}; $$= newNode("ExtDef", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
       | Specifier SEMI 				{treeNode childList[2] = {$1, $2}; $$= newNode("ExtDefList", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
       | Specifier FunDec CompSt 			{treeNode childList[3] = {$1, $2, $3}; $$= newNode("ExtDef", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
       | error SEMI 					{  }
       ;
ExtDecList : VarDec 					{treeNode childList[1] = {$1}; $$= newNode("ExtDecList", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
           | VarDec COMMA ExtDecList 			{treeNode childList[3] = {$1, $2, $3}; $$= newNode("ExtDecList", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
           ;

/*Specifiers*/
Specifier : TYPE 					{treeNode childList[1] = {$1}; $$= newNode("Specifier", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
          | StructSpecifier 					{treeNode childList[1] = {$1}; $$= newNode("Specifier", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
          ;
StructSpecifier : STRUCT OptTag LC DefList RC 	{treeNode childList[5] = {$1, $2, $3, $4, $5}; $$= newNode("StructSpecifier", 5, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
                | STRUCT Tag 					{treeNode childList[2] = {$1, $2}; $$= newNode("StructSpecifier", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;} 
                ;
OptTag : ID 						{treeNode childList[1] = {$1}; $$= newNode("OptTag", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
       |  						{$$= newNode("OptTag", 0, -1, NULL);nodeList[nodeNum]=$$;nodeNum++;}
       ;
Tag : ID 						{treeNode childList[1] = {$1}; $$= newNode("Tag", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;

/*Declarators*/
VarDec : ID 						{treeNode childList[1] = {$1}; $$= newNode("VarDec", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | VarDec LB INT RB 				{treeNode childList[4] = {$1, $2, $3, $4}; $$= newNode("VarDec", 4, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;
FunDec : ID LP VarList RP 				{treeNode childList[4] = {$1, $2, $3, $4}; $$= newNode("FunDec", 4, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | ID LP RP  					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("FunDec", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;
VarList : ParamDec COMMA VarList  			{treeNode childList[3] = {$1, $2, $3}; $$= newNode("VarList", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | ParamDec 					{treeNode childList[1] = {$1}; $$= newNode("VarList", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;
ParamDec : Specifier VarDec 				{treeNode childList[2] = {$1, $2}; $$= newNode("ParamDec", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;

/* Statements*/
CompSt : LC DefList StmtList RC 			{treeNode childList[4] = {$1, $2, $3, $4}; $$= newNode("CompSt", 4, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
       //| LC DefList RC					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("CompSt", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
       ;
StmtList : Stmt StmtList 				{treeNode childList[2] = {$1, $2}; $$= newNode("StmtList", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
         | 						{$$= newNode("StmtList", 0, -1, NULL);nodeList[nodeNum]=$$;nodeNum++;}
         ;	
Stmt : Exp SEMI 					{treeNode childList[2] = {$1, $2}; $$= newNode("Stmt", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
     | CompSt 						{treeNode childList[1] = {$1}; $$= newNode("Stmt", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
     | RETURN Exp SEMI  				{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Stmt", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
     | IF LP Exp RP Stmt 				{treeNode childList[5] = {$1, $2, $3, $4, $5}; $$= newNode("Stmt", 5, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
     | IF LP Exp RP Stmt ELSE Stmt 			{treeNode childList[7] = {$1, $2, $3, $4, $5, $6, $7}; $$= newNode("Stmt", 7, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
     | WHILE LP Exp RP Stmt 				{treeNode childList[5] = {$1, $2, $3, $4, $5}; $$= newNode("Stmt", 5, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
     | error SEMI 					{ }
     ;

/* Local Definitions*/
DefList : Def DefList 					{treeNode childList[2] = {$1, $2}; $$= newNode("DefList", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
        | 					   	{$$= newNode("DefList", 0, -1, NULL);nodeList[nodeNum]=$$;nodeNum++;}
        ;
Def : Specifier DecList SEMI 				{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Def", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;
DecList : Dec 						{treeNode childList[1] = {$1}; $$= newNode("DecList", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
        | Dec COMMA DecList 				{treeNode childList[3] = {$1, $2, $3}; $$= newNode("DecList", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
        ;
Dec : VarDec 						{treeNode childList[1] = {$1}; $$= newNode("Dec", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | VarDec ASSIGNOP Exp 				{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Dec", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;

/*Expressions*/
Exp : Exp ASSIGNOP Exp 				{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp AND Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp OR Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp RELOP Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;} 
    | Exp ADD Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp SUB Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp MUL Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp DIV Exp 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | LP Exp RP 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | SUB Exp 						{treeNode childList[2] = {$1, $2}; $$= newNode("Exp", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | NOT Exp 						{treeNode childList[2] = {$1, $2}; $$= newNode("Exp", 2, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | ID LP Args RP 					{treeNode childList[4] = {$1, $2, $3, $4}; $$= newNode("Exp", 4, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | ID LP RP 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp LB Exp RB 					{treeNode childList[4] = {$1, $2, $3, $4}; $$= newNode("Exp", 4, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp DOT ID 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Exp", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | ID 						{treeNode childList[1] = {$1}; $$= newNode("Exp", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | INT 						{treeNode childList[1] = {$1}; $$= newNode("Exp", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | FLOAT 						{treeNode childList[1] = {$1}; $$= newNode("Exp", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;
Args : Exp COMMA Args 					{treeNode childList[3] = {$1, $2, $3}; $$= newNode("Args", 3, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    | Exp 						{treeNode childList[1] = {$1}; $$= newNode("Args", 1, yylineno, childList);nodeList[nodeNum]=$$;nodeNum++;}
    ;

/*ERRORS*/
/*Exp : Exp error LB Exp RB {}
    ;
*/

%% 
