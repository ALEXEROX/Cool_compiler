//
// Created by Дмитрий on 02.01.2026.
//

#include "semantic_expr.h"

#include <stdarg.h>

#include "class_table.h"
#include "object_env.h"
#include "../Nodes/ast.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Вспомогательное strdup */
static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

/* Вспомог: установить тип в узле (освобождает старый, если есть) */
static void set_expr_type(ExprNode *e, const char *type) {
    if (!e) return;
    if (e->static_type) free(e->static_type);
    e->static_type = type ? strdup_safe(type) : NULL;
}

/* Вспомог: сообщить об ошибке с указанием строки (если есть) */
static void sem_error(ExprNode *e, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    fprintf(stderr, "Semantic error: ");

    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

/* Проверка существования типа (SELF_TYPE разрешаем здесь как существующий) */
static bool type_exists(ClassTable *ct, const char *type) {
    if (!type) return false;
    if (strcmp(type, "SELF_TYPE") == 0) return true;
    return class_table_find(ct, type) != NULL;
}

/* Утилита: получить тип VarBinding/атрибута/типа идентификатора */
static const char *binding_type_for_object(ClassTable *ct, ClassInfo *cls, ObjectEnv *env, ExprNode *e, const char *name, VarBinding **out_vb, AttrInfo **out_attr) {
    if (out_vb) *out_vb = NULL;
    if (out_attr) *out_attr = NULL;

    /* 1) локальные переменные */
    VarBinding *vb = object_env_lookup(env, name);
    if (vb) {
        if (out_vb) *out_vb = vb;
        return vb->type;
    }

    /* 2) атрибуты текущего класса (и родителей) */
    if (cls) {
        ClassInfo *cur = cls;
        while (cur) {
            for (AttrInfo *a = cur->attrs; a; a = a->next) {
                if (strcmp(a->name, name) == 0) {
                    if (out_attr) *out_attr = a;
                    return a->type;
                }
            }
            cur = cur->parent_info;
        }
    }

    /* 3) special 'self' - bound to SELF_TYPE */
    if (strcmp(name, "self") == 0) {
        if (out_vb) *out_vb = NULL; /* 'this' is not VarBinding in env maybe */
        return "SELF_TYPE";
    }

    /* not found */
    return NULL;
}

static const char *resolve_receiver_class(ClassInfo *cls, const char *t) {
    if (strcmp(t, "SELF_TYPE") == 0)
        return cls->name;
    return t;
}


/* Основная функция: реализуем только часть узлов здесь */
bool semantic_check_expr(ClassTable *ct, ClassInfo *cls, ObjectEnv *env, ExprNode *expr) {
    if (!expr) return true;

    switch (expr->kind) {
        case EXPR_INT_CONST:
            set_expr_type(expr, "Int");
            return true;

        case EXPR_STR_CONST:
            set_expr_type(expr, "String");
            return true;

        case EXPR_BOOL_CONST:
            set_expr_type(expr, "Bool");
            return true;

        case EXPR_OBJECT: {
            const char *name = expr->object.name;
            if (!name) {
                sem_error(expr, "object: empty identifier");
                set_expr_type(expr, "Object");
                return false;
            }

            VarBinding *vb = NULL;
            AttrInfo *attr = NULL;
            const char *t = binding_type_for_object(ct, cls, env, expr, name, &vb, &attr);
            if (!t) {
                sem_error(expr, "undefined identifier '%s'", name);
                set_expr_type(expr, "Object"); /* ставим безопасный тип */
                return false;
            }

            /* заполним ссылки в AST для codegen */
            expr->var_binding = vb;
            expr->resolved_attr = attr;

            set_expr_type(expr, t);
            return true;
        }

        case EXPR_ASSIGN: {
            /* assign: name <- expr */
            const char *name = expr->assign.name;
            ExprNode *rhs = expr->assign.expr;
            if (!name) {
                sem_error(expr, "assignment to empty name");
                set_expr_type(expr, "Object");
                return false;
            }

            /* Найдём переменную — только существующая переменная (self нельзя присваивать) */
            VarBinding *vb = object_env_lookup(env, name);
            if (!vb) {
                /* Если нет локальной — может быть атрибут */
                AttrInfo *attr = NULL;
                const char *attr_type = NULL;
                if (cls) {
                    ClassInfo *cur = cls;
                    while (cur) {
                        for (AttrInfo *a = cur->attrs; a; a = a->next) {
                            if (strcmp(a->name, name) == 0) {
                                attr = a;
                                attr_type = a->type;
                                break;
                            }
                        }
                        if (attr) break;
                        cur = cur->parent_info;
                    }
                }
                if (!attr) {
                    sem_error(expr, "assignment to undefined variable '%s'", name);
                    set_expr_type(expr, "Object");
                    return false;
                } else {
                    vb = NULL;
                    expr->resolved_attr = attr;
                }
            }

            if (strcmp(name, "self") == 0) {
                sem_error(expr, "assignment to 'self' is not allowed");
                set_expr_type(expr, "Object");
                return false;
            }

            /* Проверим RHS */
            if (!semantic_check_expr(ct, cls, env, rhs)) {
                set_expr_type(expr, "Object");
                return false;
            }

            const char *rhs_t = rhs->static_type;
            const char *lhs_t = vb ? vb->type : expr->resolved_attr ? expr->resolved_attr->type : NULL;
            if (!lhs_t) lhs_t = "Object";

            /* Проверка совместимости: rhs_t <: lhs_t */
            if (!is_subtype(ct, rhs_t, lhs_t)) {
                sem_error(expr, "type mismatch in assignment to %s: RHS type %s does not conform to LHS type %s", name, rhs_t ? rhs_t : "(null)", lhs_t);
                set_expr_type(expr, "Object");
                return false;
            }

            /* успешное присваивание: тип выражения равен типу RHS */
            set_expr_type(expr, rhs_t);
            return true;
        }

        case EXPR_UNOP: {
            ExprNode *e = expr->unop.expr;

            if (!semantic_check_expr(ct, cls, env, e)) {
                set_expr_type(expr, "Object");
                return false;
            }

            const char *t = e->static_type;

            switch (expr->unop.op) {
                case OP_NEG: /* ~ */
                    if (strcmp(t, "Int") != 0) {
                        sem_error(expr, "operator '~' expects Int, got %s", t);
                        set_expr_type(expr, "Int");
                        return false;
                    }
                    set_expr_type(expr, "Int");
                    return true;

                case OP_NOT: /* not */
                    if (strcmp(t, "Bool") != 0) {
                        sem_error(expr, "operator 'not' expects Bool, got %s", t);
                        set_expr_type(expr, "Bool");
                        return false;
                    }
                    set_expr_type(expr, "Bool");
                    return true;

                case OP_ISVOID:
                    /* isvoid разрешён для любого типа */
                    set_expr_type(expr, "Bool");
                    return true;

                default:
                    sem_error(expr, "unknown unary operator");
                    set_expr_type(expr, "Object");
                    return false;
            }
        }

        case EXPR_BINOP: {
            ExprNode *l = expr->binop.left;
            ExprNode *r = expr->binop.right;

            if (!semantic_check_expr(ct, cls, env, l) ||
                !semantic_check_expr(ct, cls, env, r)) {
                set_expr_type(expr, "Object");
                return false;
            }

            const char *lt = l->static_type;
            const char *rt = r->static_type;

            switch (expr->binop.op) {

                /* ---------- Арифметика ---------- */
                case OP_PLUS:
                case OP_MINUS:
                case OP_MUL:
                case OP_DIV:
                    if (strcmp(lt, "Int") != 0 || strcmp(rt, "Int") != 0) {
                        sem_error(expr,
                                  "arithmetic operation expects Int, got %s and %s",
                                  lt, rt);
                        set_expr_type(expr, "Int");
                        return false;
                    }
                    set_expr_type(expr, "Int");
                    return true;

                /* ---------- Сравнение ---------- */
                case OP_LT:
                case OP_LE:
                    if (strcmp(lt, "Int") != 0 || strcmp(rt, "Int") != 0) {
                        sem_error(expr,
                                  "comparison expects Int, got %s and %s",
                                  lt, rt);
                        set_expr_type(expr, "Bool");
                        return false;
                    }
                    set_expr_type(expr, "Bool");
                    return true;

                /* ---------- Равенство ---------- */
                case OP_EQ: {
                    bool basic_l =
                        strcmp(lt, "Int") == 0 ||
                        strcmp(lt, "Bool") == 0 ||
                        strcmp(lt, "String") == 0;

                    bool basic_r =
                        strcmp(rt, "Int") == 0 ||
                        strcmp(rt, "Bool") == 0 ||
                        strcmp(rt, "String") == 0;

                    if (basic_l || basic_r) {
                        if (strcmp(lt, rt) != 0) {
                            sem_error(expr,
                                      "illegal equality comparison between %s and %s",
                                      lt, rt);
                            set_expr_type(expr, "Bool");
                            return false;
                        }
                    }
                    set_expr_type(expr, "Bool");
                    return true;
                }

                /* ---------- Логика (если используется) ---------- */
                case OP_AND:
                case OP_OR:
                    if (strcmp(lt, "Bool") != 0 || strcmp(rt, "Bool") != 0) {
                        sem_error(expr,
                                  "logical operation expects Bool, got %s and %s",
                                  lt, rt);
                        set_expr_type(expr, "Bool");
                        return false;
                    }
                    set_expr_type(expr, "Bool");
                    return true;

                default:
                    sem_error(expr, "unknown binary operator");
                    set_expr_type(expr, "Object");
                    return false;
            }
        }

        case EXPR_IF: {
            ExprNode *cond = expr->if_expr.cond;
            ExprNode *then_e = expr->if_expr.then_branch;
            ExprNode *else_e = expr->if_expr.else_branch;

            bool ok = true;

            if (!semantic_check_expr(ct, cls, env, cond))
                ok = false;
            if (!semantic_check_expr(ct, cls, env, then_e))
                ok = false;
            if (!semantic_check_expr(ct, cls, env, else_e))
                ok = false;

            if (!ok) {
                set_expr_type(expr, "Object");
                return false;
            }

            if (strcmp(cond->static_type, "Bool") != 0) {
                sem_error(expr,
                          "if condition must have type Bool, got %s",
                          cond->static_type);
                /* продолжаем, чтобы вычислить lub */
            }

            char *t = lub(ct, then_e->static_type, else_e->static_type);
            if (!t) {
                set_expr_type(expr, "Object");
                return false;
            }

            set_expr_type(expr, t);
            free(t);
            return true;
        }

        case EXPR_WHILE: {
            ExprNode *cond = expr->while_expr.cond;
            ExprNode *body = expr->while_expr.body;

            bool ok = true;

            if (!semantic_check_expr(ct, cls, env, cond))
                ok = false;
            if (!semantic_check_expr(ct, cls, env, body))
                ok = false;

            if (!ok) {
                set_expr_type(expr, "Object");
                return false;
            }

            if (strcmp(cond->static_type, "Bool") != 0) {
                sem_error(expr,
                          "while condition must have type Bool, got %s",
                          cond->static_type);
            }

            /* В Cool тип while всегда Object */
            set_expr_type(expr, "Object");
            return true;
        }

        case EXPR_BLOCK: {
            ExprList *el = expr->block.exprs;
            ExprNode *last = NULL;
            bool ok = true;

            for (; el; el = el->next) {
                last = el->node;
                if (!semantic_check_expr(ct, cls, env, last))
                    ok = false;
            }

            if (!last) {
                sem_error(expr, "empty block");
                set_expr_type(expr, "Object");
                return false;
            }

            set_expr_type(expr, last->static_type);
            return ok;
        }

        case EXPR_NEW: {
            const char *type = expr->new_expr.type;

            if (!type_exists(ct, type)) {
                sem_error(expr, "unknown type '%s' in new-expression", type);
                set_expr_type(expr, "Object");
                return false;
            }

            /* new SELF_TYPE разрешён */
            set_expr_type(expr, type);
            return true;
        }

        case EXPR_LET: {
            LetList *ll = expr->let_expr.bindings;
            ExprNode *body = expr->let_expr.body;
            bool ok = true;

            /* Входим в новую область */
            object_env_enter_scope(env);

            for (; ll; ll = ll->next) {
                LetBindingNode *b = ll->binding;

                const char *name = b->name;
                const char *type = b->type;

                if (strcmp(name, "self") == 0) {
                    sem_error(expr, "cannot bind 'self' in let");
                    ok = false;
                    continue;
                }

                if (!type_exists(ct, type)) {
                    sem_error(expr,
                              "undefined type '%s' in let binding '%s'",
                              type, name);
                    ok = false;
                }

                /* Проверяем инициализацию, если есть */
                if (b->init) {
                    if (!semantic_check_expr(ct, cls, env, b->init)) {
                        ok = false;
                    } else if (!is_subtype(ct,
                                           b->init->static_type,
                                           type)) {
                        sem_error(expr,
                                  "initializer type '%s' does not conform to '%s' in let binding '%s'",
                                  b->init->static_type, type, name);
                        ok = false;
                                           }
                }

                int local_index = -1;
                if (!object_env_add(env, name, type, &local_index)) {
                    sem_error(expr,
                              "duplicate let variable '%s'",
                              name);
                    ok = false;
                    continue;
                }

                /* Заполняем семантические поля */
                b->local_index = local_index;
                b->var_binding = object_env_lookup(env, name);
            }

            /* Семантика тела let */
            if (!semantic_check_expr(ct, cls, env, body))
                ok = false;

            set_expr_type(expr, body->static_type);

            /* Выходим из области */
            object_env_exit_scope(env);

            return ok;
        }

        case EXPR_DISPATCH: {
            ExprNode *caller = expr->dispatch.caller;
            ExprList *args = expr->dispatch.args;
            const char *method_name = expr->dispatch.method;
            bool ok = true;

            if (!semantic_check_expr(ct, cls, env, caller)) {
                set_expr_type(expr, "Object");
                return false;
            }

            const char *recv_type =
                resolve_receiver_class(cls, caller->static_type);

            MethodInfo *m =
                class_lookup_method(ct, recv_type, method_name);

            if (!m) {
                sem_error(expr,
                          "dispatch to undefined method '%s' in class '%s'",
                          method_name, recv_type);
                set_expr_type(expr, "Object");
                return false;
            }

            /* Проверка аргументов */
            int i = 0;
            ExprList *al = args;
            for (; al && i < m->param_count; al = al->next, i++) {
                if (!semantic_check_expr(ct, cls, env, al->node))
                    ok = false;
                else if (!is_subtype(ct,
                                      al->node->static_type,
                                      m->param_types[i])) {
                    sem_error(expr,
                              "argument %d of method '%s' has type '%s', expected '%s'",
                              i + 1, m->name,
                              al->node->static_type,
                              m->param_types[i]);
                    ok = false;
                                      }
            }

            if (al || i < m->param_count) {
                sem_error(expr,
                          "wrong number of arguments in call to '%s'",
                          m->name);
                ok = false;
            }

            /* Результирующий тип */
            if (strcmp(m->return_type, "SELF_TYPE") == 0)
                set_expr_type(expr, caller->static_type);
            else
                set_expr_type(expr, m->return_type);

            /* Для codegen */
            expr->resolved_method = m;
            expr->dispatch_class = class_table_find(ct, recv_type);

            return ok;
        }

        case EXPR_STATIC_DISPATCH: {
            ExprNode *caller = expr->static_dispatch.caller;
            const char *type = expr->static_dispatch.type;
            const char *method_name = expr->static_dispatch.method;
            ExprList *args = expr->static_dispatch.args;
            bool ok = true;

            if (!type_exists(ct, type)) {
                sem_error(expr,
                          "unknown type '%s' in static dispatch",
                          type);
                set_expr_type(expr, "Object");
                return false;
            }

            if (!semantic_check_expr(ct, cls, env, caller)) {
                set_expr_type(expr, "Object");
                return false;
            }

            const char *caller_type =
                resolve_receiver_class(cls, caller->static_type);

            if (!is_subtype(ct, caller_type, type)) {
                sem_error(expr,
                          "expression type '%s' does not conform to declared static dispatch type '%s'",
                          caller_type, type);
                ok = false;
            }

            MethodInfo *m =
                class_lookup_method(ct, type, method_name);

            if (!m) {
                sem_error(expr,
                          "static dispatch to undefined method '%s' in class '%s'",
                          method_name, type);
                set_expr_type(expr, "Object");
                return false;
            }

            /* Проверка аргументов */
            int i = 0;
            ExprList *al = args;
            for (; al && i < m->param_count; al = al->next, i++) {
                if (!semantic_check_expr(ct, cls, env, al->node))
                    ok = false;
                else if (!is_subtype(ct,
                                      al->node->static_type,
                                      m->param_types[i])) {
                    sem_error(expr,
                              "argument %d of method '%s' has type '%s', expected '%s'",
                              i + 1, m->name,
                              al->node->static_type,
                              m->param_types[i]);
                    ok = false;
                }
            }

            if (al || i < m->param_count) {
                sem_error(expr,
                          "wrong number of arguments in call to '%s'",
                          m->name);
                ok = false;
            }

            if (strcmp(m->return_type, "SELF_TYPE") == 0)
                set_expr_type(expr, caller->static_type);
            else
                set_expr_type(expr, m->return_type);

            /* Для codegen */
            expr->resolved_method = m;
            expr->dispatch_class =
                class_table_find(ct, type);

            return ok;
        }

        case EXPR_CASE: {
            ExprNode *e = expr->case_expr.expr;
            CaseList *cl = expr->case_expr.cases;
            bool ok = true;

            /* 1. Проверяем выражение */
            if (!semantic_check_expr(ct, cls, env, e)) {
                set_expr_type(expr, "Object");
                return false;
            }

            /* 2. Проверяем уникальность типов веток */
            for (CaseList *c1 = cl; c1; c1 = c1->next) {
                const char *t1 = c1->node->type;

                if (!type_exists(ct, t1)) {
                    sem_error(expr,
                              "undefined type '%s' in case branch",
                              t1);
                    ok = false;
                    continue;
                }

                if (strcmp(t1, "SELF_TYPE") == 0) {
                    sem_error(expr,
                              "SELF_TYPE is not allowed in case branch");
                    ok = false;
                }

                for (CaseList *c2 = c1->next; c2; c2 = c2->next) {
                    if (strcmp(t1, c2->node->type) == 0) {
                        sem_error(expr,
                                  "duplicate branch type '%s' in case expression",
                                  t1);
                        ok = false;
                    }
                }
            }

            /* 3. Вычисляем тип через lub */
            char *result_type = NULL;

            for (; cl; cl = cl->next) {
                CaseNode *cn = cl->node;

                if (strcmp(cn->name, "self") == 0) {
                    sem_error(expr,
                              "cannot bind 'self' in case branch");
                    ok = false;
                    continue;
                }

                /* Новая область для ветки */
                object_env_enter_scope(env);

                int local_index = -1;
                if (!object_env_add(env,
                                    cn->name,
                                    cn->type,
                                    &local_index)) {
                    sem_error(expr,
                              "duplicate variable '%s' in case branch",
                              cn->name);
                    ok = false;
                } else {
                    cn->local_index = local_index;
                    cn->var_binding = object_env_lookup(env, cn->name);
                }

                if (!semantic_check_expr(ct, cls, env, cn->expr)) {
                    ok = false;
                } else {
                    if (!result_type) {
                        result_type = strdup(cn->expr->static_type);
                    } else {
                        char *tmp = lub(ct, result_type,
                                        cn->expr->static_type);
                        free(result_type);
                        result_type = tmp;
                    }
                }

                object_env_exit_scope(env);
            }

            if (!result_type) {
                set_expr_type(expr, "Object");
                return false;
            }

            set_expr_type(expr, result_type);
            free(result_type);
            return ok;
        }


        default:
            /* Другие случаи не реализованы в этой части */
            sem_error(expr, "semantic_check_expr: unsupported expr kind");
            set_expr_type(expr, "Object");
            return false;
    }

}
