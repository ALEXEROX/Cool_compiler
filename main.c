#include <locale.h>
#include <stdio.h>

#include "Lexer/lex.yy.c"
#include "Parser/Cool_code.tab.c"
#include "Dot/ast_dot.c"

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
    yyin = fopen("source.cl", "r");
    yyparse();
    save_ast_dot(program);
    system("Dot/dot.exe Dot/cool_dot.dot -Tsvg > dot.svg");

    return 0;
}