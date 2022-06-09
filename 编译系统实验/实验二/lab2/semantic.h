#ifndef __SEMANTIC_H
#define __SEMANTIC_H

#include "Tree.h"

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;
typedef enum { BASIC, ARRAY, STRUCTURE} Kind;
typedef enum {INT, FLOAT} Basic_type;

struct Type_
{
    Kind kind;
    union 
    {
        // 基本类型
        Basic_type basic;

        // 数组类型
        struct { Type elem; int size;} array;

        // 结构体类型
        FieldList structure;
    }u;
};

struct FieldList_
{
    char* name;
    Type type;
    FieldList tail;
};

void initSymbolTable();
int check(FieldList field);
void addTable(FieldList field);
FieldList search(char* name);
void printTable();
Type createType(Kind kind, int num, ...);
FieldList createField();
void freeField(FieldList field);
void ExtDef(Tree root);
Type Specifier(Tree root);
void ExtDecList(Tree root, Type type);
void VarDec(Tree root, Type type);
void Def(Tree root);
void DecList(Tree root, Type type);
void Dec(Tree root, Type type);
Type Exp(Tree root);

void semantic(Tree root, int num);

#endif