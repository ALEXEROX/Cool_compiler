#include <locale.h>
#include <stdio.h>

#include "lex.yy.c"
#include "Cool_code.tab.c"
#include "ast_dot.c"

extern ProgramNode *program;


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    // char *filename;
    // if (argc == 1) {
    //     filename = argv[0];
    // }
    // else {
    //     printf("Enter full file name");
    //     scanf("%s", filename);
    // }
    yyin = fopen("../source.cl", "r");
    yyparse();
    save_ast_dot(program);
    system("\"C:/Program Files/Graphviz/bin/dot.exe\" ./cool_dot.dot -Tsvg > out.svg");

    return 0;
}