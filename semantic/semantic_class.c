#include "semantic_class.h"
#include "semantic_method.h"
#include "class_table.h"
#include "../Nodes/ast.h"
#include <stdio.h>
#include <string.h>

#include "object_env.h"
#include "semantic_expr.h"

/*
 * Обёртка для совместимости с существующим semantic_program.c
 * Всё, что ей нужно сделать — найти ClassInfo по имени класса.
 */
bool semantic_class(ClassTable *ct, ClassNode *cnode) {
    if (!ct || !cnode) {
        fprintf(stderr, "INTERNAL ERROR: semantic_class called with null args\n");
        return false;
    }

    ClassInfo *ci = class_table_find(ct, cnode->name);
    if (!ci) {
        fprintf(stderr,
                "INTERNAL ERROR: semantic_class: class '%s' not found in ClassTable\n",
                cnode->name);
        return false;
    }

    return semantic_check_class(ct, ci);
}


/* Проверить, что тип существует */
static bool type_exists(ClassTable *ct, const char *type) {
    if (!type) return false;
    if (strcmp(type, "SELF_TYPE") == 0) return true;
    return class_table_find(ct, type) != NULL;
}

/* Проверка переопределения атрибута: запрещено */
static bool check_attr_override(ClassInfo *cls, AttrInfo *attr) {
    ClassInfo *p = cls->parent_info;
    while (p) {
        for (AttrInfo *a = p->attrs; a; a = a->next) {
            if (strcmp(a->name, attr->name) == 0) {
                fprintf(stderr,
                        "Semantic error: attribute '%s' in class %s overrides inherited attribute\n",
                        attr->name, cls->name);
                return false;
            }
        }
        p = p->parent_info;
    }
    return true;
}

/* Проверка сигнатуры метода при override */
static bool method_signatures_match(ClassInfo *cls, MethodInfo *m, MethodInfo *parent) {
    if (m->param_count != parent->param_count) {
        fprintf(stderr,
                "Semantic error: method %s.%s overrides with different number of parameters\n",
                cls->name, m->name);
        return false;
    }

    for (int i = 0; i < m->param_count; ++i) {
        if (strcmp(m->param_types[i], parent->param_types[i]) != 0) {
            fprintf(stderr,
                    "Semantic error: method %s.%s overrides with incompatible parameter types\n",
                    cls->name, m->name);
            return false;
        }
    }

    if (strcmp(m->return_type, parent->return_type) != 0) {
        fprintf(stderr,
                "Semantic error: method %s.%s overrides with different return type\n",
                cls->name, m->name);
        return false;
    }

    return true;
}

/* Проверка переопределения методов */
static bool check_method_override(ClassTable *ct, ClassInfo *cls, MethodInfo *m) {
    if (!cls->parent_info) return true;

    MethodInfo *p = class_lookup_method(ct, cls->parent_info->name, m->name);
    if (!p) return true; /* не переопределяет */

    /* Проверяем сигнатуру */
    return method_signatures_match(cls, m, p);
}

/* Проверить валидность типов параметров и результата */
static bool check_method_types(ClassTable *ct, ClassInfo *cls, MethodInfo *m) {
    for (int i = 0; i < m->param_count; ++i) {
        if (!type_exists(ct, m->param_types[i])) {
            fprintf(stderr,
                    "Semantic error: in class %s method %s: parameter %s has undefined type %s\n",
                    cls->name, m->name, m->param_names[i], m->param_types[i]);
            return false;
        }
    }

    if (!type_exists(ct, m->return_type)) {
        fprintf(stderr,
                "Semantic error: in class %s method %s: return type %s is undefined\n",
                cls->name, m->name, m->return_type);
        return false;
    }
    return true;
}

/* Проверка типов атрибутов */
static bool check_attr_type(ClassTable *ct, ClassInfo *cls, AttrInfo *a) {
    if (!type_exists(ct, a->type)) {
        fprintf(stderr,
                "Semantic error: attribute %s in class %s has undefined type %s\n",
                a->name, cls->name, a->type);
        return false;
    }
    return true;
}

/* -------------------------------------------------------------------------
   Основная функция семантики класса
   ------------------------------------------------------------------------- */
bool semantic_check_class(ClassTable *ct, ClassInfo *cls) {
    bool ok = true;

    ClassNode *cnode = cls->ast;
    if (!cnode) return true;

    /* 1. Обработка feature-ов: заполнение таблицы класса */
    for (FeatureList *fl = cnode->features; fl; fl = fl->next) {
        FeatureNode *f = fl->node;
        if (f->kind == FEATURE_ATTR) {
            AttrInfo *a = class_add_attr_from_feature(cls, f);
            if (!check_attr_type(ct, cls, a))
                ok = false;
            if (!check_attr_override(cls, a))
                ok = false;

            if (f->attr.init) {
                ObjectEnv env;
                object_env_init(&env);

                /* self доступен */
                object_env_enter_scope(&env);
                object_env_add(&env, "self", "SELF_TYPE", NULL);

                if (!semantic_check_expr(ct, cls, &env, f->attr.init))
                    ok = false;
                else if (!is_subtype(ct, f->attr.init->static_type, a->type, cls)) {
                    fprintf(
                        stderr,
                        "Semantic error: initializer type '%s' does not conform to attribute type '%s' in class %s\n",
                        f->attr.init->static_type, a->type, cls->name);
                    ok = false;
                }

                object_env_exit_scope(&env);
                object_env_free(&env);
            }
        }
        else if (f->kind == FEATURE_METHOD) {
            MethodInfo *m = class_add_method_from_feature(cls, f);

            if (!check_method_override(ct, cls, m))
                ok = false;

            if (!check_method_types(ct, cls, m))
                ok = false;
        }

    }



    /* 2. Семантика тела методов */
    for (MethodInfo *m = cls->methods; m; m = m->next) {
        if (!semantic_check_method(ct, cls, m))
            ok = false;
    }

    return ok;
}
