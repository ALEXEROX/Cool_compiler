#include <locale.h>
#include <stdio.h>

#include "Lexer/lex.yy.c"
#include "Parser/cool_code.tab.c"
#include "Dot/ast_dot.c"
#include "semantic/semantic_program.h"
#include "codegen/emit/emit_class_header.h"

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
    if (!yyin) {
        perror("Cannot open input file");
        return 1;
    }

    yyparse();
    save_ast_dot(program);
    system("C:\\\"Program Files\"\\Graphviz\\bin\\dot.exe Dot\\cool_dot.dot -Tsvg > dot.svg");
    ClassTable *ct = semantic_program(program);

    if (ct != NULL) {
        printf("Semantic OK\n");
    }

    save_ast_dot(program);
    system("C:\\\"Program Files\"\\Graphviz\\bin\\dot.exe Dot\\cool_dot.dot -Tsvg > semantic_dot.svg");

    char name[255];
    for (ClassInfo *c = ct->head; c; c = c->next) {
        snprintf(name, sizeof(name), "%s.class",c->name);
        FILE *f = fopen(name, "wb");
        emit_class_header(f, c,program->constant_table);
        fclose(f);
    }



    return 0;
}