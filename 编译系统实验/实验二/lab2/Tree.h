#ifndef __TREE_H
#define __TREE_H

typedef struct TNode* Tree;
struct TNode
{
    Tree child;
    Tree brother;
    int line;
    int flag;           // 表示是否为终结符
    char unit[35];      // 语法/词法单元
    char s[35];         // 词法单元对应的信息
};

Tree createTerminal(int line, char* unit, char* src);
Tree createNonTerminal(char* unit);
Tree insertNode(Tree p, int num, ...);
void preTraversal(Tree head, int num);

#endif