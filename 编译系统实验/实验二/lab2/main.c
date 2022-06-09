#include <stdio.h>
#include "Tree.h"
#include "semantic.h"

Tree root;
extern int yyparse();
extern void yyrestart(FILE*);

int main(int argc, char **argv)
{
    if (argc <= 1) return 1;
    FILE *f = fopen(argv[1], "r");
    if (!f)
    {
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();

    initSymbolTable();
    semantic(root, 0);

    return 0;
}
