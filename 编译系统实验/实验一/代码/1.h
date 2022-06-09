# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdlib.h>
# define maxNum 10
extern int yylineno;
extern char* yytext;
extern int yylex(void);

struct TREE{
    int line;
    char* name;
    int n; // num of children
    struct TREE *child[maxNum];
    union{
        char* ID;
        int INT;
        float FLOAT;
    };
};
typedef struct TREE *treeNode;

int hasFault;
int nodeNum;
treeNode nodeList[1000];
int nodeIsChild[1000];

int stoi(char * p);

void setChildTag (treeNode node);

treeNode newNode(char *name, int num, int yyline, treeNode childList[]);

void preOrder(treeNode t, int level);

void printTree();

void yyerror(char *msg);
