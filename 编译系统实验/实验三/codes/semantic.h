#ifndef SEM
#define SEM
#include "multitree.h"
typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef struct node1
{
	char* symname;
	Type symtype;
	struct node1* next;
} tablenode;

struct Type_
{
	enum { BASIC, ARRAY, STRUCTURE, FUNC} kind;
	union
	{
		int basic;
		struct { Type elem; int size; } array;
	} u;
};

struct Variable
{
	char* name ;
	Type head ;
	int line;
};
tablenode* chainheader;
tablenode* Create_tablenode (char* nodename, Type nodetype, tablenode* chainheader)
{
	tablenode* A = (tablenode*)malloc(sizeof(tablenode));
	A->symname = nodename;
	A->symtype = nodetype;
	A->next = chainheader;
	return A;
}

int checkrepeat(char* nodename,int lineno)
{
	tablenode* tmp = chainheader;
	while(tmp!=NULL)
	{
		if(strcmp(tmp->symname, nodename) == 0)
		{
			printf("Error type 3 at Line %d: Redefined variable\n", lineno);
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

struct Variable getVarDec(node* root,Type type)
{
	if(strcmp(root->children[0]->name, "ID") == 0)
	{
		struct Variable array;
		array.name = root->children[0]->IDvalue;
		array.head = type;
		array.line = root->children[0]->linenum;
		return array;
	}
	else
	{
		struct Variable tmp;
		Type arraytype = (Type)malloc(sizeof(struct Type_));
		arraytype->kind = ARRAY;
		arraytype->u.array.elem = type;
		arraytype->u.array.size =  root->children[2]->intvalue;
		tmp = getVarDec(root->children[0],arraytype);
		return tmp;
	}

}

void dealparam(node* param)
{
	if(strcmp(param->children[0]->children[0]->name, "TYPE") == 0)
	{
		Type nodetype = (Type)malloc(sizeof(struct Type_));
		nodetype->kind = BASIC;
		if(strcmp(param->children[0]->children[0]->typevalue, "int") == 0)
		{
			nodetype->u.basic = 0;
		}
		if(strcmp(param->children[0]->children[0]->typevalue, "float") == 0)
		{
			nodetype->u.basic = 1;
		}
		struct Variable array  = getVarDec(param->children[1],nodetype);
		int check = checkrepeat(array.name,array.line);
		if(check == 0)
		{
			chainheader = Create_tablenode (array.name, array.head, chainheader);
		}
	}
	if(strcmp(param->children[0]->children[0]->name, "StructSpecifier") == 0)
	{
		if(strcmp(param->children[0]->children[0]->children[1]->name, "Tag") == 0)
		{
			Type nodetype = (Type)malloc(sizeof(struct Type_));
			nodetype->kind = STRUCTURE;
			int check = checkrepeat(param->children[0]->children[0]->children[1]->children[0]->IDvalue,param->children[0]->children[0]->children[1]->children[0]->linenum);
			if(check == 0)
			{
				chainheader = Create_tablenode (param->children[0]->children[0]->children[1]->children[0]->IDvalue, nodetype, chainheader);
			}
		}
	}	
	
}

int lookuptable(char* name)
{
	tablenode* tmp = chainheader;
	while(tmp!=NULL)
	{
		if(strcmp(tmp->symname, name) == 0)
		{
			if(tmp->symtype->kind == BASIC)
			{
				if(tmp->symtype->u.basic == 0)
				{
					return 0;
				}
				if(tmp->symtype->u.basic == 1)
				{
					return 1;
				}
			}
			if(tmp->symtype->kind == ARRAY)
			{
				return 1;
			}
		}
		tmp = tmp->next;
	}
	return 0;
}

void checkerror(node* root)
{
	if(strcmp(root->name, "ExtDef") == 0)
	{
		Type nodetype = (Type)malloc(sizeof(struct Type_));
		if(strcmp(root->children[0]->children[0]->name, "TYPE") == 0)
		{
			nodetype->kind = BASIC;
			if(strcmp(root->children[0]->children[0]->typevalue, "int") == 0)
			{
				nodetype->u.basic = 0;
			}
			if(strcmp(root->children[0]->children[0]->typevalue, "float") == 0)
			{
				nodetype->u.basic = 1;
			}		
		}
		if(strcmp(root->children[0]->children[0]->name, "StructSpecifier") == 0)
		{
			if(root->children[0]->children[0]->children[1] != NULL)
			{
				if(strcmp(root->children[0]->children[0]->children[1]->name, "Tag") == 0)
				{
					nodetype->kind = STRUCTURE;
					int check = checkrepeat(root->children[0]->children[0]->children[1]->children[0]->IDvalue,root->children[0]->children[0]->children[1]->children[0]->linenum);
					if(check == 0)
					{
						chainheader = Create_tablenode (root->children[0]->children[0]->children[1]->children[0]->IDvalue, nodetype, chainheader);
					}
				}
				if(strcmp(root->children[0]->children[0]->children[1]->name, "OptTag") == 0)
				{
					nodetype->kind = STRUCTURE;
					int check = checkrepeat(root->children[0]->children[0]->children[1]->children[0]->IDvalue,root->children[0]->children[0]->children[1]->children[0]->linenum);
					if(check == 0)
					{
						chainheader = Create_tablenode (root->children[0]->children[0]->children[1]->children[0]->IDvalue, nodetype, chainheader);
					}
				}
			}
			else
			{	
				nodetype->kind = STRUCTURE;	
			}	
		}
		if(strcmp(root->children[1]->name, "ExtDecList") == 0)
		{
			node* tmp1 = root->children[1];
			struct Variable array  = getVarDec(tmp1->children[0],nodetype);
			int check = checkrepeat(array.name,array.line);
			if(check == 0)
			{
				chainheader = Create_tablenode (array.name, array.head, chainheader);
			}
			while(tmp1->numchild != 1)
			{
				tmp1 = tmp1->children[2];
				struct Variable array1  = getVarDec(tmp1->children[0],nodetype);
				int check = checkrepeat(array1.name,array1.line);
				if(check == 0)
				{
					chainheader = Create_tablenode (array1.name, array1.head, chainheader);
				}	
			}
		}
		else if(strcmp(root->children[1]->name, "FunDec") == 0)
		{
			Type funtype = (Type)malloc(sizeof(struct Type_));
			funtype->kind = FUNC;
			int check = checkrepeat(root->children[1]->children[0]->IDvalue,root->children[1]->children[0]->linenum);
			if(check == 0)
			{
				chainheader = Create_tablenode (root->children[1]->children[0]->IDvalue, funtype, chainheader);
			}
			if(strcmp(root->children[1]->children[2]->name, "VarList") == 0)
			{
				node* tmp2 = root->children[1]->children[2];
				dealparam(tmp2->children[0]);
				while(tmp2->numchild != 1)
				{
					tmp2 = tmp2->children[2];
					dealparam(tmp2->children[0]);
				}
			}
		}
		else {}
	}
	
	else if(strcmp(root->name, "Def") == 0)
	{
		Type nodetype = (Type)malloc(sizeof(struct Type_));
		if(strcmp(root->children[0]->children[0]->name, "TYPE") == 0)
		{
			nodetype->kind = BASIC;
			if(strcmp(root->children[0]->children[0]->typevalue, "int") == 0)
			{
				nodetype->u.basic = 0;
			}
			if(strcmp(root->children[0]->children[0]->typevalue, "float") == 0)
			{
				nodetype->u.basic = 1;
			}		
		}
		if(strcmp(root->children[0]->children[0]->name, "StructSpecifier") == 0)
		{
			if(root->children[0]->children[0]->children[1] != NULL)
			{
				if(strcmp(root->children[0]->children[0]->children[1]->name, "Tag") == 0)
				{
					nodetype->kind = STRUCTURE;
					int check = checkrepeat(root->children[0]->children[0]->children[1]->children[0]->IDvalue,root->children[0]->children[0]->children[1]->children[0]->linenum);
					if(check == 0)
					{
						chainheader = Create_tablenode (root->children[0]->children[0]->children[1]->children[0]->IDvalue, nodetype, chainheader);
					}
				}
				if(strcmp(root->children[0]->children[0]->children[1]->name, "OptTag") == 0)
				{
					nodetype->kind = STRUCTURE;
					int check = checkrepeat(root->children[0]->children[0]->children[1]->children[0]->IDvalue,root->children[0]->children[0]->children[1]->children[0]->linenum);
					if(check == 0)
					{
						chainheader = Create_tablenode (root->children[0]->children[0]->children[1]->children[0]->IDvalue, nodetype, chainheader);
					}
				}
			}
			else
			{	
				nodetype->kind = STRUCTURE;	
			}	
		}
		node* tmp2 = root->children[1];
		struct Variable array  = getVarDec(tmp2->children[0]->children[0],nodetype);
		int check = checkrepeat(array.name,array.line);
		if(check == 0)
		{
			chainheader = Create_tablenode (array.name, array.head, chainheader);
		}
		if(tmp2->children[0]->numchild != 1)
		{
			checkerror(tmp2->children[0]->children[2]);
			if(array.head->kind == BASIC && array.head->u.basic == 0 && tmp2->children[0]->children[2]->type == 0)
			{}
			else if(array.head->kind == BASIC && array.head->u.basic == 1 && tmp2->children[0]->children[2]->type == 1)
			{}
			else 
			{
				if(tmp2->children[0]->children[2]->type != 10)
				{
					printf("Error type 5 at Line %d: Type mismatched for assignment\n", array.line);
				}
			}
		}
		while(tmp2->numchild != 1)
		{
			tmp2 = tmp2->children[2];
			struct Variable array1  = getVarDec(tmp2->children[0]->children[0],nodetype);
			int check = checkrepeat(array1.name,array1.line);
			if(check == 0)
			{
				chainheader = Create_tablenode (array1.name, array1.head, chainheader);
			}
			if(tmp2->children[0]->numchild != 1)
			{
				checkerror(tmp2->children[0]->children[2]);
				if(array.head->kind == BASIC && array.head->u.basic == 0 && tmp2->children[0]->children[2]->type == 0)
				{}
				else if(array.head->kind == BASIC && array.head->u.basic == 1 && tmp2->children[0]->children[2]->type == 1)
				{}
				else 
				{
					if(tmp2->children[0]->children[2]->type != 10)
					{
						printf("Error type 5 at Line %d: Type mismatched for assignment\n", array1.line);
					}
				}
			}	
		}
	}
	
	else if(strcmp(root->name, "Exp") == 0)
	{
		if(root->numchild == 3)
		{
			if(strcmp(root->children[1]->name, "PLUS") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else if(root->children[0]->type == 1 && root->children[2]->type == 1) 
				{
					root->type = 1;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
				
			}
			if(strcmp(root->children[1]->name, "MINUS") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else if(root->children[0]->type == 1 && root->children[2]->type == 1) 
				{
					root->type = 1;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
			}
			if(strcmp(root->children[1]->name, "STAR") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else if(root->children[0]->type == 1 && root->children[2]->type == 1) 
				{
					root->type = 1;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
			}
			if(strcmp(root->children[1]->name, "DIV") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else if(root->children[0]->type == 1 && root->children[2]->type == 1) 
				{
					root->type = 1;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
			}
			if(strcmp(root->children[1]->name, "AND") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
			}
			if(strcmp(root->children[1]->name, "OR") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
			}
			if(strcmp(root->children[1]->name, "RELOP") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0) 
				{
					root->type = 0;
					root->canleft = 0;
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[0]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[0]->linenum);
					}
					root->type = 10;
				}
			}
			if(strcmp(root->children[1]->name, "ASSIGNOP") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 0 && root->children[2]->type == 0)
				{
					if(root->children[0]->canleft == 1)
					{
						root->type = 0;
						root->canleft = 0;
					}
					else
					{
						printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable\n", root->children[0]->linenum);
						root->type = 10;
					}
				}
				else if(root->children[0]->type == 1 && root->children[2]->type == 1)
				{
					if(root->children[0]->canleft == 1)
					{
						root->type = 1;
						root->canleft = 0;
					}
					else
					{
						printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable\n", root->children[0]->linenum);
						root->type = 10;
					}
				}
				else if(root->children[0]->type == 2 && root->children[2]->type == 2)
				{
					if(root->children[0]->dim == root->children[2]->dim && root->children[0]->arraytype ==root->children[2]->arraytype)
					{
						if(root->children[0]->canleft == 1)
						{
							root->type = 2;
							root->dim = root->children[0]->dim;
							root->arraytype = root->children[0]->arraytype;
							root->canleft = 0;
						}
						else
						{
							printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable\n", root->children[0]->linenum);
							root->type = 10;
						}
					}
					else
					{
						printf("Error type 5 at Line %d: Type mismatched for assignment\n", root->children[0]->linenum);
						root->type = 10;
					}
				}
				else
				{
					if(root->children[0]->type != 10 && root->children[2]->type != 10)
					{
						printf("Error type 5 at Line %d: Type mismatched for assignment\n", root->children[0]->linenum);
					}
					root->type = 10;
				}	
			}
			if(strcmp(root->children[1]->name, "Exp") == 0)
			{
				checkerror(root->children[1]);
				if(root->children[1]->type == 2)
				{
					root->type = 2;
					root->dim = root->children[0]->dim;
					root->arraytype = root->children[0]->arraytype;
				}
				else
				{
					root->type = root->children[1]->type;
				}
				root->canleft = 0;
			}
		}
		if(root->numchild == 2)
		{
			if(strcmp(root->children[0]->name, "MINUS") == 0)
			{
				checkerror(root->children[1]);
				if(root->children[1]->type == 0 || root->children[1]->type == 1)
				{
					root->type = root->children[1]->type;
					root->canleft = 0;
				}
				else
				{
					if(root->children[1]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[1]->linenum);
					}
					root->type = 10;
				}
				
			}
			if(strcmp(root->children[1]->name, "NOT") == 0)
			{
				checkerror(root->children[1]);
				if(root->children[1]->type == 0)
				{
					root->type = root->children[1]->type;
					root->canleft = 0;
				}
				else
				{
					if(root->children[1]->type != 10)
					{
						printf("Error type 7 at Line %d: Type mismatched for operands\n", root->children[1]->linenum);
					}
					root->type = 10;
				}
			}
		}
		if(root->numchild == 4)
		{
			if(strcmp(root->children[0]->name, "Exp") == 0)
			{
				checkerror(root->children[0]);
				checkerror(root->children[2]);
				if(root->children[0]->type == 2)
				{
					if(root->children[2]->type == 0)
					{
						int dimtmp = root->children[0]->dim-1;
						if(dimtmp != 0)
						{
							root->type =2;
							root->dim = dimtmp;
							root->arraytype = root->children[0]->arraytype;
						}
						else
						{
							if(root->children[0]->arraytype == 0)
							{
								root->type =0;
							}
							if(root->children[0]->arraytype == 1)
							{
								root->type =1;
							}
						}
						root->canleft = 1;
					}
					else
					{
						if(root->children[0]->type != 10)
						{
							printf("Error type 12 at Line %d: is not an integer\n", root->children[2]->linenum);
						}
						root->type = 10;
					}			
				}
				else
				{
					if(root->children[0]->type != 10)
					{
						printf("Error type 10 at Line %d: is not an array\n", root->children[1]->linenum);
					}
					root->type = 10;
				}
			}
		}
		if(root->numchild == 1)
		{
			if(strcmp(root->children[0]->name, "ID") == 0)
			{
				tablenode* tmp = chainheader;
				while(tmp!=NULL)
				{
					if(strcmp(tmp->symname, root->children[0]->IDvalue) == 0)
					{
						if(tmp->symtype->kind == BASIC)
						{
							if(tmp->symtype->u.basic == 0)
							{
								root->type = 0;
							}
							if(tmp->symtype->u.basic == 1)
							{
								root->type = 1;
							}
						}
						if(tmp->symtype->kind == ARRAY)
						{
							int dim=1;
							root->type = 2;
							Type piao = tmp->symtype;
							while(piao->u.array.elem->kind != BASIC)
							{
								dim++;
								piao = piao->u.array.elem;
							}
							if(piao->u.array.elem->u.basic == 0)
							{
								root->arraytype = 0;
							}
							if(piao->u.array.elem->u.basic == 1)
							{
								root->arraytype = 1;
							}
							root->dim = dim;		
						}
						if(tmp->symtype->kind == STRUCTURE)
						{
							root->type = 3;
						}
						if(tmp->symtype->kind == FUNC)
						{
							root->type = 4;
						}
						root->canleft = 1;
						//if(chainheader!= NULL) {printf("piao\n");}
						break;
					}
					tmp = tmp->next;
				}
				if(tmp == NULL)
				{
					root->type = 10;
					printf("Error type 1 at Line %d: Undefined variable\n", root->children[0]->linenum);
				}
			}
			if(strcmp(root->children[0]->name, "INT") == 0)
			{
				root->type = 0;
				root->canleft = 0;
			}
			if(strcmp(root->children[0]->name, "FLOAT") == 0)
			{
				root->type = 1;
				root->canleft = 0;
			}	
		}
		
		
	}	
}
void Analysis (node* root)
{
	int i = 0;
	int iterate_child = 1;
	if (strcmp(root->name, "ExtDef") == 0) 
	{
		checkerror(root);
	}
	else if (strcmp(root->name, "Def") == 0) 
	{
		iterate_child = 0;
		checkerror(root);
	}
	else if (strcmp(root->name, "Exp") == 0) 
	{
		iterate_child = 0;
		checkerror(root);
	}
	if (iterate_child)
	while(i<(root->numchild))
	{
		if(root->children[i] != NULL)
	        {
			Analysis(root->children[i]);
			i++;
	        }else i++;
	}
}
#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
