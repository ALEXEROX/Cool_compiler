#include <locale.h>
#include <stdio.h>

#include "Lexer/lex.yy.c"
#include "Parser/cool_code.tab.c"
#include "Dot/ast_dot.c"

extern ProgramNode *program;


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");

    char filename[255] = {'s','o','u','r','c','e','.','c','l','\0'};

    if (argc == 2) {
        strcpy(filename, argv[1]);
    }
    // else {
    //     printf("Enter file name\n");
    //     scanf("%s", &filename);
    // }

    yyin = fopen(filename, "r");
    yyparse();
    save_ast_dot(program);
    system("C:\\\"Program Files\"\\Graphviz\\bin\\dot.exe Dot\\cool_dot.dot -Tsvg > dot.svg");

    return 0;
}