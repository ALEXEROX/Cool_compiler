#include "semantic_program.h"
#include "semantic_class.h"    /* мы подключим semantic_class() */
#include "error_report.h"      /* твой модуль ошибок, если есть */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Главная функция семантического анализа программы:
 *  1) создаёт ClassTable
 *  2) заносит классы
 *  3) проверяет наследование
 *  4) вызывает semantic_class для каждого класса
 *  5) проверяет наличие Main.main
 */
bool semantic_program(ProgramNode *p)
{
    if (!p) return false;

    /* ---------------------------
       1. Создаём таблицу классов
       --------------------------- */
    ClassTable *ct = class_table_create();
    if (!ct) {
        fprintf(stderr, "FATAL: cannot allocate ClassTable\n");
        return false;
    }
    p->root_classinfo = NULL; /* при необходимости можно хранить там ClassTable */

    /* ---------------------------
       2. Регистрируем классы программы
       --------------------------- */
    for (ClassList *cl = p->classes; cl; cl = cl->next) {
        ClassNode *c = cl->node;

        if (!class_table_add_class(ct, c)) {
            /* class_table_add_class сам должен выдать ошибку:
               - дублирование класса
               - неправильное имя
               - запрещённое имя (SELF_TYPE)
            */
            return false;
        }
    }

    /* ---------------------------
       3. Проверка наследования
       --------------------------- */
    if (!class_table_build_inheritance(ct)) {
        /* ошибка уже сообщена */
        return false;
    }

    /* ---------------------------
       4. Семантика каждого класса
       --------------------------- */
    for (ClassList *cl = p->classes; cl; cl = cl->next) {
        ClassNode *c = cl->node;

        if (!semantic_class(ct, c)) {
            /* semantic_class сообщает ошибки самостоятельно */
            return false;
        }
    }

    /* ---------------------------
       5. Проверка наличия Main.main
       --------------------------- */

    ClassInfo *main_info = class_table_find(ct, "Main");
    if (!main_info) {
        printf("Class Main is not defined");
        return false;
    }

    MethodInfo *main_method = class_find_method(main_info, "main");
    if (!main_method) {
        printf("Class Main must define method main()");
        return false;
    }

    /* Проверка: у Main.main не должно быть параметров */
    if (main_method->param_count != 0) {
        printf("Main.main() must have no arguments");
        return false;
    }

    /* Проверка типа возврата Main.main */
    if (strcmp(main_method->return_type, "Object") != 0 &&
        strcmp(main_method->return_type, "SELF_TYPE") != 0 &&
        strcmp(main_method->return_type, "Int") != 0 &&
        strcmp(main_method->return_type, "Bool") != 0 &&
        strcmp(main_method->return_type, "String") != 0)
    {
        /* В Cool — Main.main может возвращать что угодно, но в JVM лучше Object.
           Если хочешь строго — оставь эту проверку, иначе убери. */
    }

    /* Если дошли сюда — семантика успешна. */
    return true;
}

