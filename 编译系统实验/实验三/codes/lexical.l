%{
	#include "multitree.h"
	#include <stdio.h>
       #include <stdlib.h>
       #include "syntax.tab.h"
	int lex_error = 0;
%}

digit [0-9]
letter [_a-zA-Z]
%option yylineno

%%
"while" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "WHILE";
		yylval.type_node->linenum=yylineno;
		return WHILE;
	}
"else" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "ELSE";
		yylval.type_node->linenum=yylineno;
		return ELSE;
	}
"if" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "IF";
		yylval.type_node->linenum=yylineno;
		return IF;
	}
"return" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RETURN";
		yylval.type_node->linenum=yylineno;
		return RETURN;
	}
"struct" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "STRUCT";
		yylval.type_node->linenum=yylineno;
		return STRUCT;
	}
"}" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RC";
		yylval.type_node->linenum=yylineno;
		return RC;
	}
"{" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "LC";
		yylval.type_node->linenum=yylineno;
		return LC;
	}
"]" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RB";
		yylval.type_node->linenum=yylineno;
		return RB;
	}
"[" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "LB";
		yylval.type_node->linenum=yylineno;
		return LB;
	}
")" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RP";
		yylval.type_node->linenum=yylineno;
		return RP;
	}
"(" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "LP";
		yylval.type_node->linenum=yylineno;
		return LP;
	}	
"int"|"float" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "TYPE";
		yylval.type_node->typevalue = strdup(yytext);
		yylval.type_node->linenum=yylineno;
		return TYPE;
	}
"!" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "NOT";
		yylval.type_node->linenum=yylineno;
		return NOT;
	}
"." {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "DOT";
		yylval.type_node->linenum=yylineno;
		return DOT;
	}
"||" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "OR";
		yylval.type_node->linenum=yylineno;
		return OR;
	}
"&&" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "AND";
		yylval.type_node->linenum=yylineno;
		return AND;
	}
"/" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "DIV";
		yylval.type_node->linenum=yylineno;
		return DIV;
	}
"*" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "STAR";
		yylval.type_node->linenum=yylineno;
		return STAR;
	}
"-" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "MINUS";
		yylval.type_node->linenum=yylineno;
		return MINUS;
	}
"+" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "PLUS";
		yylval.type_node->linenum=yylineno;
		return PLUS;
	}
">" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RELOP";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->relopvalue = 1;
		return RELOP;
	}
"<" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RELOP";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->relopvalue = 2;
		return RELOP;
	}
">=" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RELOP";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->relopvalue = 3;
		return RELOP;
	}
"<=" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RELOP";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->relopvalue = 4;
		return RELOP;
	}
"==" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RELOP";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->relopvalue = 5;
		return RELOP;
	}
"!=" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "RELOP";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->relopvalue = 6;
		return RELOP;
	}
"=" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "ASSIGNOP";
		yylval.type_node->linenum=yylineno;
		return ASSIGNOP;
	}
"," {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "COMMA";
		yylval.type_node->linenum=yylineno;
		return COMMA;
	}
";" {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "SEMI";
		yylval.type_node->linenum=yylineno;
		return SEMI;
	}
0|[1-9]{digit}*  {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "INT";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->intvalue = atoi(yytext);
		return INT;
	}
0[0-7]+ {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "INT";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->intvalue = (int)strtol(yytext,NULL,0);
		return INT;
	}
(0x|0X)[A-Fa-f0-9]+ {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "INT";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->intvalue = (int)strtol(yytext,NULL,0);
		return INT;
	}
{digit}+\.{digit}+ {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "FLOAT";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->floatvalue = atof(yytext);
		return FLOAT;
	}
{digit}*\.{digit}+(E|e)(\+|\-)?{digit}+	{
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "FLOAT";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->floatvalue = atof(yytext);
		return FLOAT;
	}
{letter}({letter}|{digit})* {
		yylval.type_node = (node*)malloc(sizeof(node));
		yylval.type_node->numchild = 0;
		yylval.type_node->name = "ID";
		yylval.type_node->linenum=yylineno;
		yylval.type_node->IDvalue = strdup(yytext);
		return ID;
	}
"//" { char c = input(); while (c != '\n') c = input(); }
"/*" { char c = input();  TAB: while (c != '*') c = input(); c = input(); if (c != '/') goto TAB;}
\n|\t|" " { }
. {
	lex_error = 1;
	printf("Error type A at Line %d: Mysterious characters \"%s\"\n", yylineno,yytext);
	}
%%









