#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "semantic.h"
#include "Tree.h"

FieldList head = NULL;

void initSymbolTable()
{
    head = createField();
}

// 检查符号表是否有相同变量
// 返回 1 表示可以向符号表添加符号
int check(FieldList field)
{
    // printf("in check\n");
    FieldList p = head->tail;
    while (p)
    {
        if (!strcmp(p->name, field->name))
            return 0;
        p = p->tail;
    }
    return 1;
}

// 查找符号
FieldList search(char* name)
{
    // printf("in search\n");
    FieldList p = head->tail;
    while (p)
    {
        if (!strcmp(p->name, name))
            return p;
        p = p->tail;
    }
    return NULL;
}

// 向符号表添加符号
void addTable(FieldList field)
{
    // printf("in addTable\n");
    field->tail = head->tail;
    head->tail = field;
}

void printTable()
{
    // printf("in printTable\n");
    FieldList p = head->tail;
    while (p)
    {
        printf("symbol: %s\n", p->name);
        p = p->tail;
    }
}

// 创建类型
Type createType(Kind kind, int num, ...)
{
    Type t = (Type)malloc(sizeof(struct Type_));
    t->kind = kind;
    va_list tlist;
    va_start(tlist, num);
    switch(kind)
    {
        case BASIC:
            t->u.basic = va_arg(tlist, Basic_type);
            // printf("创建基本类型: %d\n", t->u.basic);
            break;
        case ARRAY:
            t->u.array.size = va_arg(tlist, int);
            t->u.array.elem = va_arg(tlist, Type);
            // printf("创建数组类型\n");
            break;
    }
    va_end(tlist);
    return t;
}

// 创建符号
FieldList createField()
{
    FieldList tmp = (FieldList)malloc(sizeof(struct FieldList_));
    tmp->name = (char*)malloc(sizeof(35 * sizeof(char)));
    tmp->type = NULL;
    tmp->tail = NULL;
    return tmp;
}

// 释放符号
void freeField(FieldList field)
{
    // printf("in freeField\n");
    free(field->name);
    while (field->tail)
        freeField(field->tail);
    free(field);
}

void ExtDef(Tree root)
{
    // ExtDef -> Specifier ExtDecList SEMI
    //         | Specifier SEMI
    //         | Specifier FunDec CompSt
    // printf("in ExtDef\n");

    FieldList tmp = (FieldList)malloc(sizeof(struct FieldList_));
    tmp->name = (char*)malloc(sizeof(35 * sizeof(char)));

    Type specifier_type = Specifier(root->child);
    if (!strcmp(root->child->brother->unit, "ExtDecList")) // 定义基本类型变量
        ExtDecList(root->child->brother, specifier_type);

    return ;
}

// 解析类型
Type Specifier(Tree root)
{
    // printf("in Specifier\n");
    if (!strcmp(root->child->unit, "TYPE"))
    {
        // printf("TYPE: %s\n", root->child->s);
        if (!strcmp(root->child->s, "int"))
            return createType(BASIC, 1, INT);
        else
            return createType(BASIC, 1, FLOAT);
    }
    else
        return NULL;
}

void ExtDecList(Tree root, Type type)
{
    // ExtDecList -> VarDec
    //             | VarDec COMMA ExtDecList
    // printf("in ExtDecList\n");

    Tree p = root;
    while (p)
    {
        VarDec(p->child, type);

        // 同时定义多个变量
        if (p->child->brother)
            p = p->child->brother->brother;
        else
            break;
    }
}

void VarDec(Tree root, Type type)
{
    // VarDec -> ID
    //         | VarDec LB INT RB
    // printf("in VarDec\n");

    Tree id = root;
    // 不断迭代找到变量名
    while (id->child)
        id = id->child;

    FieldList tmp = createField();
    strcpy(tmp->name, id->s);
    if (!strcmp(root->child->unit, "ID"))
    {// 基本类型变量
        tmp->type = type;
        // printf("创建基本变量: %d %s\n", tmp->type->u.basic, tmp->name);
    }
    else
    {// 数组
        Tree vardec = root->child;
        while (vardec->brother)
        {
            Type array = createType(ARRAY, 2, atoi(vardec->brother->brother->s),
                type);
            type = array;
            vardec = vardec->child;
        }
        tmp->type = type;
        // printf("创建数组变量: %s\n", tmp->name);
    }

    if (!check(tmp))
    {
        printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", 
            root->line, tmp->name);
        freeField(tmp);
    }
    else
        addTable(tmp);
}

void Def(Tree root)
{
    // Def -> Specifier DecList SEMI
    // printf("in Def\n");

    Type type = Specifier(root->child);
    DecList(root->child->brother, type);
}

void DecList(Tree root, Type type)
{
    // DecList -> Dec
    //          | Dec COMMA DecList
    // printf("in DecList\n");
    
    Tree p = root;
    while (p)
    {
        Dec(p->child, type);

        // 同时定义多个变量
        if (p->child->brother)
            p = p->child->brother->brother;
        else
            break;
    }
}

void Dec(Tree root, Type type)
{
    // Dec -> VarDec
    //      | VarDec ASSIGNOP Exp
    // printf("in Dec\n");

    VarDec(root->child, type);
}

Type Exp(Tree root)
{
    // Exp -> Exp ASSIGNOP Exp
    //      | Exp AND Exp
    //      | Exp OR Exp
    //      | Exp RELOP Exp
    //      | Exp PLUS Exp
    //      | Exp MINUS Exp
    //      | Exp STAR Exp
    //      | Exp DIV Exp
    //      | LP Exp RP
    //      | MINUS Exp
    //      | NOT Exp
    //      | ID LP Args RP
    //      | ID LP RP
    //      | Exp LB Exp RB
    //      | Exp DOT ID
    //      | ID
    //      | INT
    //      | FLOAT
    // printf("in Exp\n");

    Tree p = root->child;
    if (!strcmp(p->unit, "INT"))
        return createType(BASIC, 1, INT);
    else if (!strcmp(p->unit, "FLOAT"))
        return createType(BASIC, 1, FLOAT);
    else if (!strcmp(p->unit, "ID"))
    {
        FieldList sym = search(p->s);
        if (!sym)
            printf("Error type 1 at Line %d: Undefined variable \"%s\".\n",
                p->line, p->s);
        else
            return sym->type;
    }
    // Exp -> Exp ASSIGNOP Exp
    //      | Exp AND Exp
    //      | Exp OR Exp
    //      | Exp RELOP Exp
    //      | Exp PLUS Exp
    //      | Exp MINUS Exp
    //      | Exp STAR Exp
    //      | Exp DIV Exp
    //      | Exp LB Exp RB
    //      | Exp DOT ID
    else if (!strcmp(p->unit, "Exp"))
    {
        if (!strcmp(p->brother->unit, "LB"))
        {// 数组
            Type t1 = Exp(p);
            Type t2 = Exp(p->brother->brother);
            if (!t1 || !t2)
                return NULL;
            if (t1->kind != ARRAY)
                printf("Error type 10 at Line %d: \"%s\" is not an array.\n",
                    p->line, p->child->s);
            else if (t2->kind != BASIC || t2->u.basic != INT)
                printf("Error type 12 at Line %d: \"%s\" is not an integer.\n",
                    p->line, p->brother->brother->child->s);
        }
        else if (strcmp(p->brother->unit, "DOT"))
        {// 二元运算
            Type t1 = Exp(p);
            Type t2 = Exp(p->brother->brother);
            if (!t1 || !t2)
                return NULL;

            if (!strcmp(p->brother->unit, "ASSIGNOP"))
            {
                if (!strcmp(p->child->unit, "INT") ||
                    !strcmp(p->child->unit, "FLOAT"))
                    printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", p->line);
                else if (!strcmp(p->child->unit, "ID"))
                {
                    if (t1->u.basic != t2->u.basic)
                        printf("Error type 5 at Line %d: Type mismatched for assignment.\n",
                            p->line);
                }
            }
            else
            {
                if (t1->u.basic != t2->u.basic)
                    printf("Error type 7 at Line %d: Type mismatched for operands.\n",
                        p->line);
            }
        }
    }
    return NULL;
}

void semantic(Tree root, int num)
{
    if (!root) return ;

    // 语法分析树
    // for (int i = 0; i < num; ++i)
    //     printf(" ");
    // if (root->flag)
    // {
    //     printf("%s", root->unit);
    //     if (root->s[0] != '\0')
    //         printf(": %s", root->s);
    //     printf("\n");
    // }
    // else
    // {
    //     printf("%s (%d)\n", root->unit, root->line);
    // }

    if (!strcmp(root->unit, "ExtDef"))
        ExtDef(root);
    if (!strcmp(root->unit, "Def"))
        Def(root);
    if (!strcmp(root->unit, "Exp"))
        Exp(root);
    semantic(root->child, num + 2);
    semantic(root->brother, num);
}
