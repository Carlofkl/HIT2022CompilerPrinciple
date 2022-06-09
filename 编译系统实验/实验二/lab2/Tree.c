#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include "Tree.h"

Tree createTerminal(int line, char* unit, char* src)
{
    Tree t = (Tree)malloc(sizeof(struct TNode));
    t->child = t->brother = NULL;
    t->line = line;
    t->flag = 1;
    strncpy(t->unit, unit, 35);
    if (!src)
        t->s[0] = '\0';
    else
        strncpy(t->s, src, 35);
    return t;
}

Tree createNonTerminal(char* unit)
{
    Tree t = (Tree)malloc(sizeof(struct TNode));
    t->child = t->brother = NULL;
    t->flag = 0;
    strncpy(t->unit, unit, 35);
    return t;
}

Tree insertNode(Tree p, int num, ...)
{
    va_list tlist;
    va_start(tlist, num);
    Tree tmp;
    for (int i = 0; i < num; ++i)
    {
        tmp = va_arg(tlist, Tree);
        if (!tmp)
            continue;
        tmp->brother = p->child;
        p->child = tmp;
        p->line = tmp->line;
    }
    va_end(tlist);
    return p;
}

void preTraversal(Tree head, int num)
{
    if (!head) return ;
    for (int i = 0; i < num; ++i)
        printf(" ");
    if (head->flag)
    {
        printf("%s", head->unit);
        if (head->s[0] != '\0')
            printf(": %s", head->s);
        printf("\n");
    }
    else
    {
        printf("%s (%d)\n", head->unit, head->line);
    }
    preTraversal(head->child, num + 2);
    preTraversal(head->brother, num);
}

// int main()
// {
//     Tree p1 = createNonTerminal("1");
//     Tree p2 = createTerminal(2, "2", "2");
//     Tree p3 = createNonTerminal("3");
//     Tree p4 = createNonTerminal("4");
//     Tree p5 = createTerminal(3, "5", "5");
//     Tree p6 = createTerminal(3, "6", NULL);
//     Tree p7 = createTerminal(3, "7", NULL);
//     p3 = insertNode(p3, 1, p5);
//     p4 = insertNode(p4, 2, p7, p6);
//     p1 = insertNode(p1, 3, p4, p3, p2);
//     preTraversal(p1, 0);
//     return 0;
// }