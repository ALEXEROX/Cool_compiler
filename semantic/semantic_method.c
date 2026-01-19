#include "semantic_method.h"
#include "semantic_expr.h"
#include "object_env.h"
#include <stdio.h>
#include <string.h>

/* Проверка существования типа */
static bool type_exists(ClassTable *ct, const char *type) {
    if (!type) return false;
    if (strcmp(type, "SELF_TYPE") == 0) return true;
    return class_table_find(ct, type) != NULL;
}

/* Нельзя использовать SELF_TYPE как имя параметра типа */
static bool check_param_type(ClassTable *ct, const char *type, const char *param_name, const char *method_name, const char *cls) {
    if (strcmp(type, "SELF_TYPE") == 0) {
        fprintf(stderr,
                "Semantic error: parameter '%s' of method '%s.%s' cannot have type SELF_TYPE\n",
                param_name, cls, method_name);
        return false;
    }
    if (!type_exists(ct, type)) {
        fprintf(stderr,
                "Semantic error: parameter '%s' of method '%s.%s' has undefined type '%s'\n",
                param_name, cls, method_name, type);
        return false;
    }
    return true;
}

/* Проверка возвращаемого типа */
static bool check_return_type(ClassTable *ct, const char *type, const char *method_name, const char *cls) {
    if (!type_exists(ct, type)) {
        fprintf(stderr,
                "Semantic error: method '%s.%s' has undefined return type '%s'\n",
                cls, method_name, type);
        return false;
    }
    return true;
}

/* ---------------------------------------------------------------------
   Главная функция: semantic_check_method
   --------------------------------------------------------------------- */
bool semantic_check_method(ClassTable *ct, ClassInfo *cls, MethodInfo *m, ConstantTable *cp) {
    bool ok = true;

    /* 1. Проверяем корректность объявленных типов */
    for (int i = 0; i < m->param_count; ++i) {
        if (!check_param_type(ct, m->param_types[i], m->param_names[i], m->name, cls->name))
            ok = false;
    }
    if (!check_return_type(ct, m->return_type, m->name, cls->name))
        ok = false;

    if (!ok) return false;

    /* 2. Создаём окружение для локальных переменных метода */
    ObjectEnv env;
    object_env_init(&env);

    /* 3. Добавляем параметры в окружение */
    for (int i = 0; i < m->param_count; ++i) {
        const char *pname = m->param_names[i];
        const char *ptype = m->param_types[i];

        /* Проверка дубликатов */
        if (object_env_lookup(&env, pname) != NULL) {
            fprintf(stderr,
                    "Semantic error: duplicate parameter '%s' in method %s.%s\n",
                    pname, cls->name, m->name);
            ok = false;
            continue;
        }

        int out_index = -1;
        if (!object_env_add(&env, pname, ptype, &out_index)) {
            fprintf(stderr,
                    "Semantic error: cannot add parameter '%s' to environment\n",
                    pname);
            ok = false;
            continue;
        }

        VarBinding *vb = object_env_lookup(&env, pname);

        /* Записываем в формал семантические поля */
        if (m->ast) {
            FormalList *fl = m->ast->method.formals;
            int idx = 0;
            while (fl) {
                if (idx == i) {
                    fl->node->local_index = out_index;
                    fl->node->var_binding = vb;
                    break;
                }
                fl = fl->next;
                idx++;
            }
        }
    }

    /* 4. Семантика тела метода */
    if (m->ast == NULL || m->ast->method.body == NULL) {
        fprintf(stderr,
                "Semantic error: method %s.%s has no body\n",
                cls->name, m->name);
        return false;
    }

    ExprNode *body = m->ast->method.body;

    if (!semantic_check_expr(ct, cls, &env, body, cp))
        ok = false;

    if (!ok)
        return false;

    /* 5. Проверяем, что тип тела метода <: тип результата */
    if (body->static_type == NULL) {
        fprintf(stderr,
                "Semantic error: method %s.%s produced no type for body\n",
                cls->name, m->name);
        return false;
    }

    const char *actual = body->static_type;
    const char *expected = m->return_type;

    /* SELF_TYPE нормализация */
    const char *actual_norm =
        (strcmp(actual, "SELF_TYPE") == 0) ? cls->name : actual;

    const char *expected_norm =
        (strcmp(expected, "SELF_TYPE") == 0) ? cls->name : expected;

    if (!is_subtype(ct, actual_norm, expected_norm, cls)) {
        fprintf(stderr,
            "Semantic error: method %s.%s body type '%s' does not conform to declared return type '%s'\n",
            cls->name, m->name,
            actual, expected);
        ok = false;
    }

    return ok;
}
