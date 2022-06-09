%{
	#include <stdio.h>
	#include "lex.yy.c"
	#include "multitree.h"
        #include <stdlib.h>
        #include "semantic.h"
        #include "intercodegenerate.h"
	int syn_error = 0;
	extern FILE* out;
	extern tablenode* chainheader;
%}

%union {
	node* type_node;
}

%token  <type_node> INT
%token  <type_node> FLOAT
%token  <type_node> ID
%token  <type_node> SEMI
%token  <type_node> COMMA
%token  <type_node> ASSIGNOP
%token  <type_node> RELOP
%token	<type_node> PLUS
%token	<type_node> MINUS
%token	<type_node> STAR
%token	<type_node> DIV
%token	<type_node> AND
%token	<type_node> OR
%token	<type_node> DOT
%token	<type_node> NOT
%token	<type_node> TYPE
%token	<type_node> LP
%token	<type_node> RP
%token	<type_node> LB
%token	<type_node> RB
%token	<type_node> LC
%token	<type_node> RC
%token	<type_node> STRUCT
%token	<type_node> RETURN
%token	<type_node> IF
%token	<type_node> ELSE
%token	<type_node> WHILE
%type <type_node> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier OptTag Tag
%type <type_node> VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args
%right ASSIGNOP
%left  OR
%left  AND
%left  RELOP
%left  PLUS MINUS
%left  STAR DIV
%right NOT
%left  DOT LP RP LB RB
%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%%
Program : ExtDefList {	
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Program" , 1 , temp , $1->linenum);
		node* root = $$;
		if(syn_error == 0 && lex_error == 0)
		{
			Analysis (root);
			intergenerate(root, chainheader, out);
		}	
	}
	;
	
ExtDefList : ExtDef ExtDefList {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "ExtDefList" , 2 , temp , $1->linenum);
	}
	| { $$ = NULL;}
	;
	
ExtDef : Specifier ExtDecList SEMI {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "ExtDef" , 3 , temp , $1->linenum);
	}
	| Specifier SEMI {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "ExtDef" , 2 , temp , $1->linenum);
	}
	| Specifier FunDec CompSt {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "ExtDef" , 3 , temp , $1->linenum);
	}
	;
	
ExtDecList : VarDec {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "ExtDecList" , 1 , temp , $1->linenum);
	}
	| VarDec COMMA ExtDecList {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "ExtDecList" , 3 , temp , $1->linenum);
	}
	//| error ExtDecList { syn_error = 1; }
	;
	
Specifier : TYPE {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Specifier" , 1 , temp , $1->linenum);
	}
	| StructSpecifier {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Specifier" , 1 , temp , $1->linenum);
	}
	| error { syn_error = 1; }
	;
	
StructSpecifier : STRUCT OptTag LC DefList RC {
		node** temp = (node**)malloc(sizeof(node*)*5);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		temp[4] = $5;
		$$ = Create_new_node( "StructSpecifier" , 5 , temp , $1->linenum);
	}
	| STRUCT Tag {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "StructSpecifier" , 2 , temp , $1->linenum);
	}
	;
	
OptTag : ID {	node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "OptTag" , 1 , temp , $1->linenum);
	}
	| { $$ = NULL;}
	;
	
Tag : ID {	node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Tag" , 1 , temp , $1->linenum);
	}
	;
	
VarDec : ID {	node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "VarDec" , 1 , temp , $1->linenum);
	}
	| VarDec LB INT RB {
		node** temp = (node**)malloc(sizeof(node*)*4);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		$$ = Create_new_node( "VarDec" , 4 , temp , $1->linenum);
	}
	;
	
FunDec : ID LP VarList RP {
		node** temp = (node**)malloc(sizeof(node*)*4);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		$$ = Create_new_node( "FunDec" , 4 , temp , $1->linenum);
	}
	| ID LP RP {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "FunDec" , 3 , temp , $1->linenum);
	}
	| error RP { syn_error = 1; }
	| error { syn_error = 1; }
	;
	
VarList : ParamDec COMMA VarList {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "VarList" , 3 , temp , $1->linenum);
	}
	| error VarList { syn_error = 1; }
	| ParamDec {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "VarList" , 1 , temp , $1->linenum);
	}
	;
	
ParamDec : Specifier VarDec {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "ParamDec" , 2 , temp , $1->linenum);
	}
	;
	
CompSt : LC DefList StmtList RC {
		node** temp = (node**)malloc(sizeof(node*)*4);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		$$ = Create_new_node( "CompSt" , 4 , temp , $1->linenum);
	}
	//| error DefList StmtList RC { syn_error = 1; }
	//| error { syn_error = 1; }
	;
	
StmtList : Stmt StmtList {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "StmtList" , 2 , temp , $1->linenum);
	}
	| { $$ = NULL;}
	;
	
Stmt : Exp SEMI {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "Stmt" , 2 , temp , $1->linenum);
	}
	| CompSt {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Stmt" , 1 , temp , $1->linenum);
	}
	| RETURN Exp SEMI {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Stmt" , 3 , temp , $1->linenum);
	}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
		node** temp = (node**)malloc(sizeof(node*)*5);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		temp[4] = $5;
		$$ = Create_new_node( "Stmt" , 5 , temp , $1->linenum);
	}
	| IF LP Exp RP Stmt ELSE Stmt {
		node** temp = (node**)malloc(sizeof(node*)*7);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		temp[4] = $5;
		temp[5] = $6;
		temp[6] = $7;
		$$ = Create_new_node( "Stmt" , 7 , temp , $1->linenum);
	}
	| WHILE LP Exp RP Stmt {
		node** temp = (node**)malloc(sizeof(node*)*5);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		temp[4] = $5;
		$$ = Create_new_node( "Stmt" , 5 , temp , $1->linenum);
	}
	| error SEMI { syn_error = 1; }
	;
	
DefList : Def DefList {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "DefList" , 2 , temp , $1->linenum);
	}
	| { $$ = NULL;}
	;
	
Def : Specifier DecList SEMI {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Def" , 3 , temp , $1->linenum);
	}
	| error SEMI {syn_error = 1;}
	;
	
DecList : Dec {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "DecList" , 1 , temp , $1->linenum);
	}
	| Dec COMMA DecList {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "DecList" , 3 , temp , $1->linenum);
	}
	;
	
Dec : VarDec {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Dec" , 1 , temp , $1->linenum);
	}
	| VarDec ASSIGNOP Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Dec" , 3 , temp , $1->linenum);
	}
	;
	
Exp : Exp ASSIGNOP Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp AND Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp OR Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp RELOP Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp PLUS Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp MINUS Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp STAR Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp DIV Exp {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| LP Exp RP {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| MINUS Exp {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "Exp" , 2 , temp , $1->linenum);
	}
	| NOT Exp {
		node** temp = (node**)malloc(sizeof(node*)*2);
		temp[0] = $1;
		temp[1] = $2;
		$$ = Create_new_node( "Exp" , 2 , temp , $1->linenum);
	}
	| ID LP Args RP {
		node** temp = (node**)malloc(sizeof(node*)*4);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		$$ = Create_new_node( "Exp" , 4 , temp , $1->linenum);
	}
	| ID LP RP {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| Exp LB Exp RB {
		node** temp = (node**)malloc(sizeof(node*)*4);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		temp[3] = $4;
		$$ = Create_new_node( "Exp" , 4 , temp , $1->linenum);
	}
	| Exp DOT ID {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Exp" , 3 , temp , $1->linenum);
	}
	| ID {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Exp" , 1 , temp , $1->linenum);
	}
	| INT {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Exp" , 1 , temp , $1->linenum);
	}
	| FLOAT {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Exp" , 1 , temp , $1->linenum);
	}
	;
Args : Exp COMMA Args {
		node** temp = (node**)malloc(sizeof(node*)*3);
		temp[0] = $1;
		temp[1] = $2;
		temp[2] = $3;
		$$ = Create_new_node( "Args" , 3 , temp , $1->linenum);
	}
	| Exp {
		node** temp = (node**)malloc(sizeof(node*)*1);
		temp[0] = $1;
		$$ = Create_new_node( "Args" , 1 , temp , $1->linenum);
	}
	;
%%
 int yyerror(char* msg)
{
	syn_error = 1;
	fprintf(stderr, "Error type B at Line %d: %s\n", yylineno, msg);
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	













































	
















