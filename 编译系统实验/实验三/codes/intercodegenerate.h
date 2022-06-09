#ifndef INTERM
#define INTERM
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "semantic.h"
typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;

extern FILE* out;
int tmpnum = 1;
int labelnum = 1;
struct Operand_ {
	enum { VARIABLE, CONSTANT, ADDRESS, LABEL_OP, RELOP1, DER } kind;
	union {
		char* name;
		int value;
	} u;
};

struct InterCode_
{
	enum { LABEL_IR, FUNCTION, ASSIGN, ADD1, SUB1, MUL1, DIV1, GOTO, IF_GOTO, RETURN1, DEC, ARG, CALL, PARAM, READ, WRITE } kind;
	union {
		struct { Operand op; } oneop;
		struct { Operand right, left; } assign;
		struct { Operand result, op1, op2; } binop;
		struct { Operand x, relop, y, z; } ifgoto;
		struct { Operand op;  int size; } dec;	
	} u;
};

struct InterCodes_ { InterCode code; struct InterCodes_ *prev, *next; };

void translate_Exp(node* Exp, tablenode* sym_table, Operand place);
void translate_Args(node* Args, tablenode* sym_table, Operand* arg_list, int num);
void translate_Cond(node* Exp, Operand label_true, Operand label_false, tablenode* sym_table);
void translate_Stmt(node* Stmt, tablenode* sym_table);
void translate_StmtList(node* StmtList, tablenode* sym_table);
void translate_VarDec(node* VarDec, tablenode* sym_table);
void translate_Dec(node* Dec, tablenode* sym_table);
void translate_DecList(node* DecList, tablenode* sym_table);
void translate_Def(node* Def, tablenode* sym_table);
void translate_DefList(node* DefList, tablenode* sym_table);
void translate_CompSt(node* CompSt, tablenode* sym_table);
void translate_ParamDec(node* ParamDec, tablenode* sym_table);
void translate_VarList(node* VarList, tablenode* sym_table);
void translate_FunDec(node* FunDec, tablenode* sym_table);
void translate_ExtDecList(node* ExtDecList, tablenode* sym_table);
void translate_ExtDef(node* ExtDef, tablenode* sym_table);
void translate_ExtDefList(node* ExtDefList, tablenode* sym_table);
void translate_Program(node* Program, tablenode* sym_table);

tablenode* lookup(tablenode* chainheader, node* ID)
{
	tablenode* tmp = chainheader;
	while(tmp!=NULL)
	{
		if(strcmp(tmp->symname, ID->IDvalue) == 0)
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}

Operand new_temp()
{
	Operand tmp = (Operand)malloc(sizeof(struct Operand_));
	tmp->kind = 0;
	char* str2 = (char*)malloc(sizeof(char)*20);
	sprintf(str2, "t%d", tmpnum);
	tmp->u.name = str2;
	tmpnum++;
	return tmp;
}

Operand new_label() 
{
    	Operand tmp = (Operand)malloc(sizeof(struct Operand_));
    	tmp->kind = 3;
    	char* str2 = (char*)malloc(sizeof(char)*20);
    	sprintf(str2, "label%d", labelnum);
	tmp->u.name = str2;
	labelnum++;
	return tmp;	
}

InterCodes geninterCodeList()
{
	InterCodes first = (InterCodes)malloc(sizeof(struct InterCodes_));
	first->code = (InterCode)malloc(sizeof(struct InterCode_));
	first->code = NULL;
	first->prev = NULL;
	first->next = NULL;
	return first;
}

InterCodes last;
void gen_intercode0(char* kind, Operand op1, Operand op2, Operand op3, Operand op4)
{
	InterCodes newcode = (InterCodes)malloc(sizeof(struct InterCodes_));
	newcode->code = (InterCode)malloc(sizeof(struct InterCode_));
	if(strcmp(kind, "IF_GOTO") == 0)
	{
		newcode->code->kind = 8;
		newcode->code->u.ifgoto.x = op1;
		newcode->code->u.ifgoto.relop = op2;
		newcode->code->u.ifgoto.y = op3;
		newcode->code->u.ifgoto.z = op4;
	}
	last->next = newcode;
	newcode->prev = last;
	newcode->next = NULL;
	last = newcode;
}

void gen_intercode1(char* kind, Operand place, Operand t1, Operand t2)
{
	InterCodes newcode = (InterCodes)malloc(sizeof(struct InterCodes_));
	newcode->code = (InterCode)malloc(sizeof(struct InterCode_));
	if(strcmp(kind, "ADD1") == 0)
	{
		newcode->code->kind = 3;
		newcode->code->u.binop.result = place;
		newcode->code->u.binop.op1 = t1;
		newcode->code->u.binop.op2 = t2;
	}
	if(strcmp(kind, "SUB1") == 0)
	{
		newcode->code->kind = 4;
		newcode->code->u.binop.result = place;
		newcode->code->u.binop.op1 = t1;
		newcode->code->u.binop.op2 = t2;
	}
	if(strcmp(kind, "MUL1") == 0)
	{
		newcode->code->kind = 5;
		newcode->code->u.binop.result = place;
		newcode->code->u.binop.op1 = t1;
		newcode->code->u.binop.op2 = t2;
	}
	if(strcmp(kind, "DIV1") == 0)
	{
		newcode->code->kind = 6;
		newcode->code->u.binop.result = place;
		newcode->code->u.binop.op1 = t1;
		newcode->code->u.binop.op2 = t2;
	}
	last->next = newcode;
	newcode->prev = last;
	newcode->next = NULL;
	last = newcode;
}

void gen_intercode2(char* kind, Operand op1, Operand op2)
{
	InterCodes newcode = (InterCodes)malloc(sizeof(struct InterCodes_));
	newcode->code = (InterCode)malloc(sizeof(struct InterCode_));
	if(strcmp(kind, "ASSIGN") == 0)
	{
		newcode->code->kind = 2;
		newcode->code->u.assign.left = op1;
		newcode->code->u.assign.right = op2;
	}
	if(strcmp(kind, "CALL") == 0)
	{
		newcode->code->kind = 12;
		newcode->code->u.assign.left = op1;
		newcode->code->u.assign.right = op2;
	}
	if(strcmp(kind, "DEC") == 0)
	{
		newcode->code->kind = 10;
		newcode->code->u.dec.op = op1;
		newcode->code->u.dec.size = op2->u.value;
	}
	last->next = newcode;
	newcode->prev = last;
	newcode->next = NULL;
	last = newcode;
}

void gen_intercode3(char* kind, Operand op1)
{
	InterCodes newcode = (InterCodes)malloc(sizeof(struct InterCodes_));
	newcode->code = (InterCode)malloc(sizeof(struct InterCode_));
	if(strcmp(kind, "LABEL_IR") == 0)
	{
		newcode->code->kind = 0;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "READ") == 0)
	{
		newcode->code->kind = 14;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "WRITE") == 0)
	{
		newcode->code->kind = 15;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "GOTO") == 0)
	{
		newcode->code->kind = 7;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "RETURN1") == 0)
	{
		newcode->code->kind = 9;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "ARG") == 0)
	{
		newcode->code->kind = 11;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "PARAM") == 0)
	{
		newcode->code->kind = 13;
		newcode->code->u.oneop.op = op1;
	}
	if(strcmp(kind, "FUNCTION") == 0)
	{
		newcode->code->kind = 1;
		newcode->code->u.oneop.op = op1;
	}
	last->next = newcode;
	newcode->prev = last;
	newcode->next = NULL;
	last = newcode;
}


void translate_Exp(node* Exp, tablenode* sym_table, Operand place)
{
	if(Exp->numchild == 3)
	{
		if(strcmp(Exp->children[1]->name, "PLUS") == 0)
		{
			Operand t1 = new_temp();
			Operand t2 = new_temp();
			translate_Exp(Exp->children[0], sym_table, t1);
			translate_Exp(Exp->children[2], sym_table, t2);
			gen_intercode1("ADD1", place, t1, t2);
		}
		if(strcmp(Exp->children[1]->name, "MINUS") == 0)
		{
			Operand t1 = new_temp();
			Operand t2 = new_temp();
			translate_Exp(Exp->children[0], sym_table, t1);
			translate_Exp(Exp->children[2], sym_table, t2);
			gen_intercode1("SUB1", place, t1, t2);
		}
		if(strcmp(Exp->children[1]->name, "STAR") == 0)
		{
			Operand t1 = new_temp();
			Operand t2 = new_temp();
			translate_Exp(Exp->children[0], sym_table, t1);
			translate_Exp(Exp->children[2], sym_table, t2);
			gen_intercode1("MUL1", place, t1, t2);
		}
		if(strcmp(Exp->children[1]->name, "DIV1") == 0)
		{
			Operand t1 = new_temp();
			Operand t2 = new_temp();
			translate_Exp(Exp->children[0], sym_table, t1);
			translate_Exp(Exp->children[2], sym_table, t2);
			gen_intercode1("DIV1", place, t1, t2);
		}
		if(strcmp(Exp->children[1]->name, "AND") == 0)
		{
			Operand label1 = new_label();
                	Operand label2 = new_label();
                	Operand val1 = (Operand)malloc(sizeof(struct Operand_));
                	val1->kind = CONSTANT;
                	val1->u.value = 0;
                	gen_intercode2("ASSIGN", place, val1);
                	translate_Cond(Exp, label1, label2, sym_table);
                	gen_intercode3("LABEL_IR", label1);
                	Operand val2 = (Operand)malloc(sizeof(struct Operand_));
                	val2->kind = CONSTANT;
                	val2->u.value = 1;
                	gen_intercode2("ASSIGN", place, val2);
                	gen_intercode3("LABEL_IR", label2);
		}
		if(strcmp(Exp->children[1]->name, "OR") == 0)
		{
			Operand label1 = new_label();
                	Operand label2 = new_label();
                	Operand val1 = (Operand)malloc(sizeof(struct Operand_));
                	val1->kind = CONSTANT;
                	val1->u.value = 0;
                	gen_intercode2("ASSIGN", place, val1);
                	translate_Cond(Exp, label1, label2, sym_table);
                	gen_intercode3("LABEL_IR", label1);
                	Operand val2 = (Operand)malloc(sizeof(struct Operand_));
                	val2->kind = CONSTANT;
                	val2->u.value = 1;
                	gen_intercode2("ASSIGN", place, val2);
                	gen_intercode3("LABEL_IR", label2);
		}
		if(strcmp(Exp->children[1]->name, "RELOP") == 0)
		{
			Operand label1 = new_label();
                	Operand label2 = new_label();
                	Operand val1 = (Operand)malloc(sizeof(struct Operand_));
                	val1->kind = CONSTANT;
                	val1->u.value = 0;
                	gen_intercode2("ASSIGN", place, val1);
                	translate_Cond(Exp, label1, label2, sym_table);
                	gen_intercode3("LABEL_IR", label1);
                	Operand val2 = (Operand)malloc(sizeof(struct Operand_));
                	val2->kind = CONSTANT;
                	val2->u.value = 1;
                	gen_intercode2("ASSIGN", place, val2);
                	gen_intercode3("LABEL_IR", label2);
		}
		if(strcmp(Exp->children[1]->name, "ASSIGNOP") == 0)
		{
			//tablenode* variable = lookup(sym_table, Exp->children[0]->children[0]); 
			Operand t1 = new_temp();
			translate_Exp(Exp->children[2], sym_table, t1);
			Operand val = (Operand)malloc(sizeof(struct Operand_));
			val->kind = VARIABLE;
			val->u.name = Exp->children[0]->children[0]->IDvalue;
			gen_intercode2("ASSIGN", val, t1);
			if(place!=NULL)
			{
				gen_intercode2("ASSIGN", place, val);
			}	
		}
		if(strcmp(Exp->children[1]->name, "Exp") == 0)
		{
			translate_Exp(Exp->children[1], sym_table, place);
		}
		if(strcmp(Exp->children[0]->name, "ID") == 0)
		{
			//tablenode* function = lookup(sym_table, Exp->children[0]);
			if(strcmp(Exp->children[0]->IDvalue, "read") == 0)
			{
				gen_intercode3("READ", place);
			}
			else
			{
				Operand val = (Operand)malloc(sizeof(struct Operand_));
                		val->kind = VARIABLE;
                		val->u.name = Exp->children[0]->IDvalue;
				gen_intercode2("CALL", place, val);
			}
		}
	}
	if(Exp->numchild == 2)
	{
		if(strcmp(Exp->children[0]->name, "MINUS") == 0)
		{
			Operand t1 = new_temp();
			translate_Exp(Exp->children[1], sym_table, t1);
			Operand val = (Operand)malloc(sizeof(struct Operand_));
                	val->kind = CONSTANT;
                	val->u.value = 0;
                	gen_intercode1("SUB1", place, val, t1);
		}
		if(strcmp(Exp->children[0]->name, "NOT") == 0)
		{
			Operand label1 = new_label();
                	Operand label2 = new_label();
                	Operand val1 = (Operand)malloc(sizeof(struct Operand_));
                	val1->kind = CONSTANT;
                	val1->u.value = 0;
                	gen_intercode2("ASSIGN", place, val1);
                	translate_Cond(Exp, label1, label2, sym_table);
                	gen_intercode3("LABEL_IR", label1);
                	Operand val2 = (Operand)malloc(sizeof(struct Operand_));
                	val2->kind = CONSTANT;
                	val2->u.value = 1;
                	gen_intercode2("ASSIGN", place, val2);
                	gen_intercode3("LABEL_IR", label2);
		}
	}
	if(Exp->numchild == 1)
	{
		if(strcmp(Exp->children[0]->name, "ID") == 0)
		{
			//tablenode* variable = lookup(sym_table, Exp->children[0]);
			Operand val = (Operand)malloc(sizeof(struct Operand_));
			val->kind = VARIABLE;
			val->u.name = Exp->children[0]->IDvalue;
			gen_intercode2("ASSIGN", place, val);
		}
		if(strcmp(Exp->children[0]->name, "INT") == 0)
		{
			Operand val = (Operand)malloc(sizeof(struct Operand_));
			val->kind = CONSTANT;
			val->u.value = Exp->children[0]->intvalue;
			gen_intercode2("ASSIGN", place, val);
		}
	}
	if(Exp->numchild == 4)
	{
		if(strcmp(Exp->children[2]->name, "Args") == 0)
		{
			//tablenode* function = lookup(sym_table, Exp->children[0]);   
			Operand* arg_list = (Operand*)malloc(sizeof(Operand)*10);
			memset(arg_list,0,10*sizeof(Operand));
			if(strcmp(Exp->children[0]->IDvalue, "write") == 0)
			{
				int num = 0;
				translate_Args(Exp->children[2], sym_table, arg_list, num);
				gen_intercode3("WRITE", arg_list[0]);
			}
			else
			{
				int num = 0;
				translate_Args(Exp->children[2], sym_table, arg_list, num);
				int i = 0 ;
				//while(arg_list[i] != NULL)
				//{
				//	gen_intercode3("ARG", arg_list[i]);
				//	i++;
				//}
				for(int j = i; j >= 0; j--)
				{
					gen_intercode3("ARG", arg_list[j]);
				}
				Operand val = (Operand)malloc(sizeof(struct Operand_));
                		val->kind = VARIABLE;
                		val->u.name = Exp->children[0]->IDvalue;
				gen_intercode2("CALL", place, val);
			}
		}
		if(strcmp(Exp->children[2]->name, "Exp") == 0)
		{
			if(strcmp(Exp->children[2]->children[0]->name, "ID") == 0)
			{
				Operand t1 = new_temp();
				Operand t2 = new_temp();
				Operand val1 = (Operand)malloc(sizeof(struct Operand_));
				val1->kind = CONSTANT;
				val1->u.value = 4;
                		Operand val2 = (Operand)malloc(sizeof(struct Operand_));
				val2->kind = VARIABLE;
				val2->u.name = Exp->children[2]->children[0]->IDvalue;
                		gen_intercode1("MUL1", t1, val1, val2);
				Operand val3 = (Operand)malloc(sizeof(struct Operand_));
				val3->kind = ADDRESS;
				val3->u.name = Exp->children[0]->children[0]->IDvalue;
				gen_intercode1("ADD1", t2, val3, t1);
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = DER;
				t->u.name = t2->u.name;
				gen_intercode2("ASSIGN", place, t);			
			}
			if(strcmp(Exp->children[2]->children[0]->name, "INT") == 0)
			{
				Operand t1 = new_temp();
				Operand t2 = new_temp();
				Operand val1 = (Operand)malloc(sizeof(struct Operand_));
				val1->kind = CONSTANT;
				val1->u.value = 4;
				Operand val2 = (Operand)malloc(sizeof(struct Operand_));
				val2->kind = CONSTANT;
				val2->u.value = Exp->children[2]->children[0]->intvalue;
                		gen_intercode1("MUL1", t1, val1, val2);
                		Operand val3 = (Operand)malloc(sizeof(struct Operand_));
				val3->kind = ADDRESS;
				val3->u.name = Exp->children[0]->children[0]->IDvalue;
				gen_intercode1("ADD1", t2, val3, t1);
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = DER;
				t->u.name = t2->u.name;
				gen_intercode2("ASSIGN", place, t);
			}
			if(strcmp(Exp->children[2]->children[0]->name, "Exp") == 0)
			{
				Operand t1 = new_temp();
				Operand t2 = new_temp();
				Operand t3 = new_temp();
				Operand val1 = (Operand)malloc(sizeof(struct Operand_));
				val1->kind = CONSTANT;
				val1->u.value = 4;
				translate_Exp(Exp->children[2], sym_table, t2);
                		gen_intercode1("MUL1", t1, val1, t2);
                		Operand val3 = (Operand)malloc(sizeof(struct Operand_));
				val3->kind = ADDRESS;
				val3->u.name = Exp->children[0]->children[0]->IDvalue;
				gen_intercode1("ADD1", t3, val3, t1);
				Operand t = (Operand)malloc(sizeof(struct Operand_));
				t->kind = DER;
				t->u.name = t3->u.name;
				gen_intercode2("ASSIGN", place, t);
			}
		}
	}
}

void translate_Args(node* Args, tablenode* sym_table, Operand* arg_list, int num)
{
	if(Args->numchild == 1)
	{
		Operand t1 = new_temp();
		translate_Exp(Args->children[0], sym_table, t1);
		arg_list[num] = t1;
		num++;
	}
	if(Args->numchild == 3)
	{
		Operand t1 = new_temp();
		translate_Exp(Args->children[0], sym_table, t1);
		arg_list[num] = t1;
		num++;
		translate_Args(Args->children[2], sym_table, arg_list, num);
	}
}

void translate_Cond(node* Exp, Operand label_true, Operand label_false, tablenode* sym_table)
{
	if(strcmp(Exp->children[1]->name, "RELOP") == 0)
	{
		Operand t1 = new_temp();
		Operand t2 = new_temp();
		translate_Exp(Exp->children[0], sym_table, t1);
		translate_Exp(Exp->children[2], sym_table, t2);
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = RELOP1;
		op->u.value = Exp->children[1]->relopvalue;
		gen_intercode0("IF_GOTO", t1, op, t2, label_true);
		gen_intercode3("GOTO", label_false);
	}
	else if(strcmp(Exp->children[1]->name, "AND") == 0)
	{
		Operand label1 = new_label();
		translate_Cond(Exp->children[0], label1, label_false, sym_table);
		gen_intercode3("LABEL_IR", label1);
		translate_Cond(Exp->children[2], label_true, label_false, sym_table);
	}
	else if(strcmp(Exp->children[1]->name, "OR") == 0)
	{
		Operand label1 = new_label();
		translate_Cond(Exp->children[0], label_true, label1, sym_table);
		gen_intercode3("LABEL_IR", label1);
		translate_Cond(Exp->children[2], label_true, label_false, sym_table);
	}
	else if(strcmp(Exp->children[1]->name, "Exp") == 0)
	{
		translate_Cond(Exp->children[1], label_false, label_true, sym_table);
	}
	else
	{
		Operand t1 = new_temp();
		translate_Exp(Exp, sym_table, t1);
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = RELOP1;
		op->u.value = 6;
		Operand val = (Operand)malloc(sizeof(struct Operand_));
		val->kind = CONSTANT;
		val->u.value = 0;
		gen_intercode0("IF_GOTO", t1, op, val, label_true);
		gen_intercode3("GOTO", label_false);
	}
}

void translate_Stmt(node* Stmt, tablenode* sym_table)
{
	if(Stmt->numchild == 2)
	{
		translate_Exp(Stmt->children[0], sym_table, NULL);
	}
	if(Stmt->numchild == 1)
	{
		translate_CompSt(Stmt->children[0], sym_table);
	}
	if(Stmt->numchild == 3)
	{
		Operand t1 = new_temp();
		translate_Exp(Stmt->children[1], sym_table, t1);
		gen_intercode3("RETURN1", t1);
	}
	if(Stmt->numchild == 5)
	{
		if(strcmp(Stmt->children[0]->name, "IF") == 0)
		{
			Operand label1 = new_label();
			Operand label2 = new_label();
			translate_Cond(Stmt->children[2], label1, label2, sym_table);
			gen_intercode3("LABEL_IR", label1);
			translate_Stmt(Stmt->children[4], sym_table);
			gen_intercode3("LABEL_IR", label2);
		}
		if(strcmp(Stmt->children[0]->name, "WHILE") == 0)
		{
			Operand label1 = new_label();
			Operand label2 = new_label();
			Operand label3 = new_label();
			gen_intercode3("LABEL_IR", label1);
			translate_Cond(Stmt->children[2], label2, label3, sym_table);
			gen_intercode3("LABEL_IR", label2);
			translate_Stmt(Stmt->children[4], sym_table);
			gen_intercode3("GOTO", label1);
			gen_intercode3("LABEL_IR", label3);
		}
	}
	if(Stmt->numchild == 7)
	{
		Operand label1 = new_label();
		Operand label2 = new_label();
		Operand label3 = new_label();
		translate_Cond(Stmt->children[2], label1, label2, sym_table);
		gen_intercode3("LABEL_IR", label1);
		translate_Stmt(Stmt->children[4], sym_table);
		gen_intercode3("GOTO", label3);
		gen_intercode3("LABEL_IR", label2);
		translate_Stmt(Stmt->children[6], sym_table);
		gen_intercode3("LABEL_IR", label3);
	}
}

void translate_StmtList(node* StmtList, tablenode* sym_table) 
{
    if (StmtList == NULL) 
    {
    	return;
    }
    else
    {
    	translate_Stmt(StmtList->children[0], sym_table);
    	translate_StmtList(StmtList->children[1], sym_table);
    }
}

void translate_VarDec(node* VarDec, tablenode* sym_table)
{
	if(VarDec->numchild == 1)
	{
		return;
	}
	else
	{
		Operand size = (Operand)malloc(sizeof(struct Operand_));
		size->kind = CONSTANT;
		size->u.value = 4*(VarDec->children[2]->intvalue);
		Operand op = (Operand)malloc(sizeof(struct Operand_));
		op->kind = VARIABLE;
		op->u.name = VarDec->children[0]->children[0]->IDvalue;
		gen_intercode2("DEC", op, size);
	}
}

void translate_Dec(node* Dec, tablenode* sym_table)
{
	if(Dec->numchild == 1)
	{
		translate_VarDec(Dec->children[0], sym_table);
	}
	else
	{
		Operand t1 = new_temp();
		translate_Exp(Dec->children[2], sym_table, t1);
		translate_VarDec(Dec->children[0], sym_table);
		Operand val = (Operand)malloc(sizeof(struct Operand_));
		val->kind = VARIABLE;
		val->u.name = Dec->children[0]->children[0]->IDvalue;
		gen_intercode2("ASSIGN", val, t1);
	}
}

void translate_DecList(node* DecList, tablenode* sym_table)
{
	if(DecList->numchild == 1)
	{
		translate_Dec(DecList->children[0], sym_table);
	}
	else
	{
		translate_Dec(DecList->children[0], sym_table);
		translate_DecList(DecList->children[2], sym_table);
	}
}

void translate_Def(node* Def, tablenode* sym_table)
{
	translate_DecList(Def->children[1], sym_table);
}

void translate_DefList(node* DefList, tablenode* sym_table)
{
    	if (DefList == NULL) 
    	{
    		return;
    	}
    	else
    	{
    		translate_Def(DefList->children[0], sym_table);
    		translate_DefList(DefList->children[1], sym_table);
    	}
}

void translate_CompSt(node* CompSt, tablenode* sym_table)
{
	translate_DefList(CompSt->children[1], sym_table);
	translate_StmtList(CompSt->children[2], sym_table);
}

void translate_ParamDec(node* ParamDec, tablenode* sym_table)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = VARIABLE;
	op->u.name = ParamDec->children[1]->children[0]->IDvalue;
	gen_intercode3("PARAM", op);
}

void translate_VarList(node* VarList, tablenode* sym_table)
{
	if(VarList->numchild == 1)
	{
		translate_ParamDec(VarList->children[0], sym_table);
	}
	else
	{
		translate_ParamDec(VarList->children[0], sym_table);
		translate_VarList(VarList->children[2], sym_table);
	}
}

void translate_FunDec(node* FunDec, tablenode* sym_table)
{
	if(FunDec->numchild == 3)
	{
		Operand func = (Operand)malloc(sizeof(struct Operand_));
		func->kind = VARIABLE;
		func->u.name = FunDec->children[0]->IDvalue;
		gen_intercode3("FUNCTION", func);
	}
	else
	{
		Operand func = (Operand)malloc(sizeof(struct Operand_));
		func->kind = VARIABLE;
		func->u.name = FunDec->children[0]->IDvalue;
		gen_intercode3("FUNCTION", func);
		translate_VarList(FunDec->children[2], sym_table);
	}
}

void translate_ExtDecList(node* ExtDecList, tablenode* sym_table)
{
	if(ExtDecList->numchild == 1)
	{
		translate_VarDec(ExtDecList->children[0], sym_table);
	}
	else
	{
		translate_VarDec(ExtDecList->children[0], sym_table);
		translate_ExtDecList(ExtDecList->children[2], sym_table);
	}
}

void translate_ExtDef(node* ExtDef, tablenode* sym_table)
{
	if(ExtDef->numchild == 2)
	{
		return;
	}
	else
	{
		if(strcmp(ExtDef->children[1]->name, "ExtDecList") == 0)
		{
			translate_ExtDecList(ExtDef->children[1], sym_table);
		}
		if(strcmp(ExtDef->children[1]->name, "FunDec") == 0)
		{
			translate_FunDec(ExtDef->children[1], sym_table);
			translate_CompSt(ExtDef->children[2], sym_table);
		}
	}
}

void translate_ExtDefList(node* ExtDefList, tablenode* sym_table)
{
	if (ExtDefList == NULL) 
    	{
    		return;
    	}
    	else
    	{
    		translate_ExtDef(ExtDefList->children[0], sym_table);
    		translate_ExtDefList(ExtDefList->children[1], sym_table);
    	}
}

void translate_Program(node* Program, tablenode* sym_table)
{
	translate_ExtDefList(Program->children[0], sym_table);
}

void intergenerate(node* root, tablenode* sym_table, FILE* fp)
{
	InterCodes first = geninterCodeList();
	last = first;
	translate_Program(root, sym_table);
	InterCodes tmp = (InterCodes)malloc(sizeof(struct InterCodes_));
	tmp = first->next;
	while(tmp!=NULL)
	{
		if(tmp->code->kind == LABEL_IR)
		{
			if(tmp->code->u.oneop.op->kind == LABEL_OP)
			{
				fprintf(fp, "LABEL %s :\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == FUNCTION)
		{
			if(tmp->code->u.oneop.op->kind == VARIABLE)
			{
				fprintf(fp, "FUNCTION %s :\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == ASSIGN)
		{
			if(tmp->code->u.assign.left->kind == VARIABLE)
			{
				if(tmp->code->u.assign.right->kind == VARIABLE)
				{
					fprintf(fp, "%s := %s\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.name);
				}
				if(tmp->code->u.assign.right->kind == ADDRESS)
				{
					fprintf(fp, "%s := &%s\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.name);
				}
				if(tmp->code->u.assign.right->kind == DER)
				{
					fprintf(fp, "%s := *%s\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.name);
				}
				if(tmp->code->u.assign.right->kind == CONSTANT)
				{
					fprintf(fp, "%s := #%d\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.value);
				}
			}
			if(tmp->code->u.assign.left->kind == DER)
			{
				if(tmp->code->u.assign.right->kind == VARIABLE)
				{
					fprintf(fp, "*%s := %s\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.name);
				}
				if(tmp->code->u.assign.right->kind == CONSTANT)
				{
					fprintf(fp, "*%s := #%d\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.value);
				}
			}
		}
		else if(tmp->code->kind == ADD1)
		{
			if(tmp->code->u.binop.result->kind == VARIABLE)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := %s + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := %s + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := %s + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := %s + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := #%d + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := #%d + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := #%d + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := #%d + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := &%s + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := &%s + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := &%s + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := &%s + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := *%s + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := *%s + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := *%s + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := *%s + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}
			if(tmp->code->u.binop.result->kind == DER)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := %s + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := %s + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := %s + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := %s + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := #%d + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := #%d + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := #%d + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := #%d + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := &%s + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := &%s + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := &%s + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := &%s + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := *%s + %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := *%s + #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := *%s + &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := *%s + *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}	
		}
		else if(tmp->code->kind == SUB1)
		{
			if(tmp->code->u.binop.result->kind == VARIABLE)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := %s - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := %s - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := %s - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := %s - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := #%d - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := #%d - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := #%d - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := #%d - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := &%s - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := &%s - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := &%s - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := &%s - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := *%s - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := *%s - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := *%s - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := *%s - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}
			if(tmp->code->u.binop.result->kind == DER)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := %s - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := %s - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := %s - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := %s - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := #%d - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := #%d - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := #%d - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := #%d - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := &%s - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := &%s - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := &%s - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := &%s - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := *%s - %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := *%s - #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := *%s - &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := *%s - *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}	
		}
		else if(tmp->code->kind == MUL1)
		{
			if(tmp->code->u.binop.result->kind == VARIABLE)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := %s * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := %s * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := %s * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := %s * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := #%d * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := #%d * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := #%d * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := #%d * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := &%s * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := &%s * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := &%s * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := &%s * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := *%s * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := *%s * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := *%s * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := *%s * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}
			if(tmp->code->u.binop.result->kind == DER)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := %s * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := %s * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := %s * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := %s * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := #%d * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := #%d * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := #%d * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := #%d * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := &%s * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := &%s * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := &%s * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := &%s * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := *%s * %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := *%s * #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := *%s * &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := *%s * *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}	
		}
		else if(tmp->code->kind == DIV1)
		{
			if(tmp->code->u.binop.result->kind == VARIABLE)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := %s / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := %s / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := %s / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := %s / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := #%d / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := #%d / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := #%d / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := #%d / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := &%s / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := &%s / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := &%s / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := &%s / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "%s := *%s / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "%s := *%s / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "%s := *%s / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "%s := *%s / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}
			if(tmp->code->u.binop.result->kind == DER)
			{
				if(tmp->code->u.binop.op1->kind == VARIABLE)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := %s / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := %s / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := %s / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := %s / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == CONSTANT)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := #%d / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := #%d / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := #%d / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := #%d / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.value, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == ADDRESS)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := &%s / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := &%s / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := &%s / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := &%s / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
				if(tmp->code->u.binop.op1->kind == DER)
				{
					if(tmp->code->u.binop.op2->kind == VARIABLE)
					{
						fprintf(fp, "*%s := *%s / %s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == CONSTANT)
					{
						fprintf(fp, "*%s := *%s / #%d\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.value);
					}
					if(tmp->code->u.binop.op2->kind == ADDRESS)
					{
						fprintf(fp, "*%s := *%s / &%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
					if(tmp->code->u.binop.op2->kind == DER)
					{
						fprintf(fp, "*%s := *%s / *%s\n", tmp->code->u.binop.result->u.name, tmp->code->u.binop.op1->u.name, tmp->code->u.binop.op2->u.name);
					}
				}
			}	
		}
		else if(tmp->code->kind == GOTO)
		{
			if(tmp->code->u.oneop.op->kind == LABEL_OP)
			{
				fprintf(fp, "GOTO %s\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == IF_GOTO)
		{
			if(tmp->code->u.ifgoto.relop->u.value == 1)
			{
				if(tmp->code->u.ifgoto.x->kind == VARIABLE)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF %s > %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF %s > #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF %s > *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == CONSTANT)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF #%d > %s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF #%d > #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF #%d > *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == DER)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF *%s > %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF *%s > #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF *%s > *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}	
			}
			if(tmp->code->u.ifgoto.relop->u.value == 2)
			{
				if(tmp->code->u.ifgoto.x->kind == VARIABLE)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF %s < %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF %s < #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF %s < *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == CONSTANT)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF #%d < %s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF #%d < #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF #%d < *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == DER)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF *%s < %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF *%s < #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF *%s < *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}	
			}
			if(tmp->code->u.ifgoto.relop->u.value == 3)
			{
				if(tmp->code->u.ifgoto.x->kind == VARIABLE)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF %s >= %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF %s >= #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF %s >= *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == CONSTANT)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF #%d >= %s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF #%d >= #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF #%d >= *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == DER)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF *%s >= %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF *%s >= #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF *%s >= *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}		
			}
			if(tmp->code->u.ifgoto.relop->u.value == 4)
			{
				if(tmp->code->u.ifgoto.x->kind == VARIABLE)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF %s <= %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF %s <= #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF %s <= *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == CONSTANT)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF #%d <= %s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF #%d <= #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF #%d <= *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == DER)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF *%s <= %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF *%s <= #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF *%s <= *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}		
			}
			if(tmp->code->u.ifgoto.relop->u.value == 5)
			{
				if(tmp->code->u.ifgoto.x->kind == VARIABLE)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF %s == %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF %s == #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF %s == *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == CONSTANT)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF #%d == %s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF #%d == #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF #%d == *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == DER)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF *%s == %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF *%s == #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF *%s == *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}		
			}
			if(tmp->code->u.ifgoto.relop->u.value == 6)
			{
				if(tmp->code->u.ifgoto.x->kind == VARIABLE)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF %s != %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF %s != #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF %s != *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == CONSTANT)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF #%d != %s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF #%d != #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF #%d != *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.value, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}
				if(tmp->code->u.ifgoto.x->kind == DER)
				{
					if(tmp->code->u.ifgoto.y->kind == VARIABLE)
					{
						fprintf(fp, "IF *%s != %s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == CONSTANT)
					{
						fprintf(fp, "IF *%s != #%d GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.value, tmp->code->u.ifgoto.z->u.name);
					}
					if(tmp->code->u.ifgoto.y->kind == DER)
					{
						fprintf(fp, "IF *%s != *%s GOTO %s\n", tmp->code->u.ifgoto.x->u.name, tmp->code->u.ifgoto.y->u.name, tmp->code->u.ifgoto.z->u.name);
					}
				}		
			}	
		}
		else if(tmp->code->kind == RETURN1)
		{
			if(tmp->code->u.oneop.op->kind == VARIABLE)
			{
				fprintf(fp, "RETURN %s\n", tmp->code->u.oneop.op->u.name);
			}
			if(tmp->code->u.oneop.op->kind == CONSTANT)
			{
				fprintf(fp, "RETURN #%d\n", tmp->code->u.oneop.op->u.value);
			}
			if(tmp->code->u.oneop.op->kind == DER)
			{
				fprintf(fp, "RETURN *%s\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == DEC)
		{
			if(tmp->code->u.dec.op->kind == VARIABLE)
			{
				fprintf(fp, "DEC %s %d\n", tmp->code->u.dec.op->u.name, tmp->code->u.dec.size);
			}
		}
		else if(tmp->code->kind == ARG)
		{
			if(tmp->code->u.oneop.op->kind == VARIABLE)
			{
				fprintf(fp, "ARG %s\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == CALL)
		{
			fprintf(fp, "%s := CALL %s\n", tmp->code->u.assign.left->u.name, tmp->code->u.assign.right->u.name);
		}
		else if(tmp->code->kind == PARAM)
		{
			if(tmp->code->u.oneop.op->kind == VARIABLE)
			{
				fprintf(fp, "PARAM %s\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == READ)
		{
			if(tmp->code->u.oneop.op->kind == VARIABLE)
			{
				fprintf(fp, "READ %s\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else if(tmp->code->kind == WRITE)
		{
			if(tmp->code->u.oneop.op->kind == VARIABLE)
			{
				fprintf(fp, "WRITE %s\n", tmp->code->u.oneop.op->u.name);
			}
		}
		else{}
		tmp = tmp->next;
	}
}
























#endif
