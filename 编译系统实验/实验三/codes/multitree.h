#ifndef NUM_TREE
#define NUM_TREE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t
{
    char* name;
    int numchild; 
    char* typevalue;
    char* IDvalue;
    int intvalue;
    int relopvalue;
    float floatvalue; 
    struct node_t** children;
    int linenum; 
    int type ;
    int dim ;
    int arraytype;
    int canleft;
} node;


node* Create_new_node( char* nodename , int number , node** childs , int yylineno)
{
	node* A = (node*)malloc(sizeof(node));
	A->name = nodename;
	A->numchild = number;
	A->children = childs;
	A->linenum = yylineno;	
	return A;
}

void Travel (node* root,int space)
{
	int i = 0;
	for(int j = 0;j < space;j++)
	{
		printf(" ");
	}
	if (strcmp(root->name, "TYPE") == 0) 
	{
		printf("%s: %s\n",root->name,root->typevalue);
	}
	else if (strcmp(root->name, "ID") == 0) 
	{
		printf("%s: %s\n",root->name,root->IDvalue);
	}
	else if (strcmp(root->name, "INT") == 0) 
	{
		printf("%s: %d\n",root->name,root->intvalue);
	}
	else if (strcmp(root->name, "FLOAT") == 0) 
	{
		printf("%s: %f\n",root->name,root->floatvalue);
	}
	else if (strcmp(root->name, "SEMI") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "COMMA") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "ASSIGNOP") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "RELOP") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "PLUS") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "MINUS") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "STAR") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "DIV") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "AND") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "OR") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "DOT") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "NOT") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "LP") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "RP") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "LB") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "RB") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "LC") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "RC") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "STRUCT") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "RETURN") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "IF") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "ELSE") == 0) 
	{
		printf("%s\n",root->name);
	}
	else if (strcmp(root->name, "WHILE") == 0) 
	{
		printf("%s\n",root->name);
	}
	else 
	{
		printf("%s (%d)\n",root->name,root->linenum);
	}
	space +=2;
	while(i<(root->numchild))
	{
		if(root->children[i] != NULL)
	        {
			Travel (root->children[i],space);
			i++;
	        }else i++;
	}
}



#define  YYERROR_VERBOSE 1

#endif
