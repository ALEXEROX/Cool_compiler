#include <locale.h>
#include <stdio.h>

#include "Lexer/lex.yy.c"
#include "Parser/Cool_code.tab.c"
#include "Dot/ast_dot.c"
#include "Dump/ast_dump.c"

extern ProgramNode *program;


int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");

    char filename[255];

    if (argc == 2) {
        strcpy(filename, argv[1]);
    }
    else {
        printf("Enter file name\n");
        scanf("%s", &filename);
    }

    yyin = fopen(filename, "r");
    yyparse();
    save_ast_dot(program);
    system("Dot/dot.exe Dot/cool_dot.dot -Tsvg > dot.svg");

    ast_dump_text("Dump/dump.txt", program);

    return 0;
}