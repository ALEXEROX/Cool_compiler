#include "semantic_program.h"
#include "semantic_class.h"    /* мы подключим semantic_class() */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "descriptor.h"

/*
 * Главная функция семантического анализа программы:
 *  1) создаёт ClassTable
 *  2) заносит классы
 *  3) проверяет наследование
 *  4) вызывает semantic_class для каждого класса
 *  5) проверяет наличие Main.main
 */
void *semantic_program(ProgramNode *p)
{
    if (!p) return NULL;

    /* ---------------------------
       1. Создаём таблицу классов
       --------------------------- */
    ClassTable *ct = class_table_create();
    if (!ct) {
        fprintf(stderr, "FATAL: cannot allocate ClassTable\n");
        return NULL;
    }

    ConstantTable *cp = malloc(sizeof(ConstantTable));
    const_table_init(cp);
    p->constant_table = cp;

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
            return NULL;
        }
    }

    /* ---------------------------
       3. Проверка наследования
       --------------------------- */
    if (!class_table_build_inheritance(ct)) {
        /* ошибка уже сообщена */
        return NULL;
    }

    for (ClassList *cl = p->classes; cl; cl = cl->next) {
        ClassNode *c = cl->node;

        if (!semantic_class(ct, c, p->constant_table)) {
            /* semantic_class сообщает ошибки самостоятельно */
            return NULL;
        }
    }

    /* ---------------------------
       4. Семантика каждого класса
       --------------------------- */

    for (ClassInfo *c = ct->head; c; c = c->next) {
        c->class_cp_index = const_add_class(cp, c->name);

        // Добавляем поля
        for (AttrInfo *a = c->attrs; a; a = a->next) {
            a->fieldref_index = cp_add_fieldref_from_feature(p->constant_table, c->name, a->ast);
            a->name_utf8_index = const_add_utf8(p->constant_table, a->name);
            a->descriptor_utf8_index = const_add_utf8(p->constant_table, cool_type_to_descriptor(a->ast->attr_info->type ? a->ast->attr_info->type : "Object",c->name));
        }

        // Добавляем методы
        for (MethodInfo *m = c->methods; m; m = m->next) {
            m->methodref_index = cp_add_methodref_from_feature(p->constant_table, *c, m->ast);
            if (m->methodref_index != 0) {
                m->name_utf8_index = const_add_utf8(p->constant_table, m->name);
                ClassInfo *method_owner = find_method_owner(c, m->ast->name);
                m->descriptor_utf8_index = const_add_utf8(p->constant_table, make_method_descriptor(m->ast->method.formals, m->ast->method_info->return_type,method_owner->name));
            }
        }
    }


    class_table_layout(ct);
    //class_table_print_vtables(ct);

    /* ---------------------------
       5. Проверка наличия Main.main
       --------------------------- */

    ClassInfo *main_info = class_table_find(ct, "Main");
    if (!main_info) {
        printf("Class Main is not defined");
        return NULL;
    }

    MethodInfo *main_method = class_find_method(main_info, "main");
    if (!main_method) {
        printf("Class Main must define method main()");
        return NULL;
    }

    /* Проверка: у Main.main не должно быть параметров */
    if (main_method->param_count != 0) {
        printf("Main.main() must have no arguments");
        return NULL;
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

    const_table_print(p->constant_table, stdout);
    class_table_print(ct, stdout);

    /* Если дошли сюда — семантика успешна. */
    return ct;
}

