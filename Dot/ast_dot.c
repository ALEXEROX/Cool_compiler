#include "ast_dot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable: 4996)

static FILE* OUT;

/* -------------------------------------------------------
   ЭКРАНИРОВАНИЕ СТРОКИ ДЛЯ DOT
------------------------------------------------------- */

static char* escape_str(const char* s) {
    if (!s) return _strdup("");

    size_t len = strlen(s);
    char* r = malloc(len * 2 + 1);  /* максимум */
    char* p = r;

    for (size_t i = 0; i < len; i++) {
        if (s[i] == '"' || s[i] == '\\')
            *p++ = '\\';
        *p++ = s[i];
    }
    *p = 0;
    return r;
}

/* -------------------------------------------------------
   ПЕЧАТЬ УЗЛА
   label вида:   <text> (#id)
------------------------------------------------------- */

static void print_node(int id, const char* label) {
    char* esc = escape_str(label);

    fprintf(OUT, "  node%d [label=\"%s (#%d)\"];\n", id, esc, id);

    free(esc);
}

/* -------------------------------------------------------
   ПЕЧАТЬ ДУГИ
------------------------------------------------------- */

static void edge(int from, int to, const char* label) {
    fprintf(OUT, "  node%d -> node%d [label=\"%s\"];\n", from, to, label);
}

/* -------------------------------------------------------
   ПЕЧАТЬ ОПЕРАЦИИ
------------------------------------------------------- */

static const char* binop_name(BinOpKind op) {
    switch (op) {
    case OP_PLUS:  return "+";
    case OP_MINUS: return "-";
    case OP_MUL:   return "*";
    case OP_DIV:   return "/";
    case OP_LT:    return "<";
    case OP_LE:    return "<=";
    case OP_EQ:    return "=";
    case OP_AND:   return "and";
    case OP_OR:    return "or";
    }
    return "?";
}

static const char* unop_name(UnOpKind op) {
    switch (op) {
    case OP_NEG:    return "~";
    case OP_ISVOID: return "isvoid";
    case OP_NOT:    return "not";
    }
    return "?";
}

/* -------------------------------------------------------
   ОБХОД ВЫРАЖЕНИЙ
------------------------------------------------------- */

static void print_expr(ExprNode* e);

static void print_expr_list(ExprList* l, int from) {
    int arg_index = 1;
    while (l) {
        print_expr(l->node);
        edge(from, l->node->id, arg_index == 1 ? "arg1" :
            arg_index == 2 ? "arg2" : "arg3");
        arg_index++;
        l = l->next;
    }
}

static void print_case_list(CaseList* l, int from) {
    int idx = 1;
    while (l) {
        CaseNode* c = l->node;

        char buf[256];
        snprintf(buf, sizeof(buf), "%s : %s", c->name, c->type);
        print_node(c->id, buf);

        print_expr(c->expr);
        edge(from, c->id, "case");
        edge(c->id, c->expr->id, "arg1");

        l = l->next;
        idx++;
    }
}

static void print_let_list(LetList* l, int from) {
    int idx = 1;
    while (l) {
        LetBindingNode* b = l->binding;

        char buf[256];
        snprintf(buf, sizeof(buf), "%s : %s", b->name, b->type);
        print_node(b->id, buf);

        if (b->init) {
            print_expr(b->init);
            edge(b->id, b->init->id, "arg1");
        }

        edge(from, b->id, "binding");

        l = l->next;
        idx++;
    }
}

static void print_expr(ExprNode* e) {
    if (!e) return;

    switch (e->kind) {

    case EXPR_ASSIGN: {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s", e->assign.name);
        print_node(e->id, buf);

        print_expr(e->assign.expr);
        edge(e->id, e->assign.expr->id, "arg1");
        break;
    }

    case EXPR_BINOP: {
        const char* op = binop_name(e->binop.op);
        print_node(e->id, op);

        print_expr(e->binop.left);
        print_expr(e->binop.right);
        edge(e->id, e->binop.left->id, "arg1");
        edge(e->id, e->binop.right->id, "arg2");
        break;
    }

    case EXPR_UNOP: {
        const char* op = unop_name(e->unop.op);
        print_node(e->id, op);

        print_expr(e->unop.expr);
        edge(e->id, e->unop.expr->id, "arg1");
        break;
    }

    case EXPR_OBJECT:
        print_node(e->id, e->object.name);
        break;

    case EXPR_INT_CONST: {
        char buf[64];
        snprintf(buf, sizeof(buf), "%d", e->int_const.value);
        print_node(e->id, buf);
        break;
    }

    case EXPR_STR_CONST: {
        char* esc = escape_str(e->str_const.value);
        char buf[512];
        snprintf(buf, sizeof(buf), "\"%s\"", esc);
        print_node(e->id, buf);
        free(esc);
        break;
    }

    case EXPR_BOOL_CONST:
        print_node(e->id, e->bool_const.value ? "true" : "false");
        break;

    case EXPR_DISPATCH: {
        print_node(e->id, e->dispatch.method);

        print_expr(e->dispatch.caller);
        edge(e->id, e->dispatch.caller->id, "arg1");

        print_expr_list(e->dispatch.args, e->id);
        break;
    }

    case EXPR_STATIC_DISPATCH: {
        char buf[256];
        snprintf(buf, sizeof(buf), "%s@%s",
            e->static_dispatch.method,
            e->static_dispatch.type);
        print_node(e->id, buf);

        print_expr(e->static_dispatch.caller);
        edge(e->id, e->static_dispatch.caller->id, "arg1");

        print_expr_list(e->static_dispatch.args, e->id);
        break;
    }

    case EXPR_IF: {
        print_node(e->id, "if");

        print_expr(e->if_expr.cond);
        print_expr(e->if_expr.then_branch);
        print_expr(e->if_expr.else_branch);

        edge(e->id, e->if_expr.cond->id, "arg1");
        edge(e->id, e->if_expr.then_branch->id, "arg2");
        edge(e->id, e->if_expr.else_branch->id, "arg3");
        break;
    }

    case EXPR_WHILE: {
        print_node(e->id, "while");

        print_expr(e->while_expr.cond);
        print_expr(e->while_expr.body);

        edge(e->id, e->while_expr.cond->id, "arg1");
        edge(e->id, e->while_expr.body->id, "arg2");
        break;
    }

    case EXPR_BLOCK: {
        print_node(e->id, "block");
        ExprList* l = e->block.exprs;

        int idx = 1;
        while (l) {
            print_expr(l->node);
            edge(e->id, l->node->id, "arg1");
            l = l->next;
            idx++;
        }
        break;
    }

    case EXPR_LET: {
        print_node(e->id, "let");

        print_let_list(e->let_expr.bindings, e->id);

        print_expr(e->let_expr.body);
        edge(e->id, e->let_expr.body->id, "body");
        break;
    }

    case EXPR_CASE: {
        print_node(e->id, "case");

        print_expr(e->case_expr.expr);
        edge(e->id, e->case_expr.expr->id, "arg1");

        print_case_list(e->case_expr.cases, e->id);
        break;
    }

    case EXPR_NEW:
        print_node(e->id, e->new_expr.type);
        break;
    }
}

/* -------------------------------------------------------
   CLASS / FEATURE / FORMALS / PROGRAM
------------------------------------------------------- */

static void print_feature_list(FeatureList* fl, int from) {
    while (fl) {
        FeatureNode* f = fl->node;

        print_node(f->id, f->name);
        edge(from, f->id, "feature");

        if (f->kind == FEATURE_METHOD) {
            FormalList* p = f->method.formals;
            int idx = 1;
            while (p) {
                FormalNode* fm = p->node;

                char buf[256];
                snprintf(buf, sizeof(buf), "%s : %s", fm->name, fm->type);
                print_node(fm->id, buf);
                edge(f->id, fm->id, "arg1");

                p = p->next;
                idx++;
            }

            if (f->method.body) {
                print_expr(f->method.body);
                edge(f->id, f->method.body->id, "body");
            }
        }
        else {
            if (f->attr.init) {
                print_expr(f->attr.init);
                edge(f->id, f->attr.init->id, "arg1");
            }
        }

        fl = fl->next;
    }
}

static void print_class_list(ClassList* cl, int from) {
    while (cl) {
        ClassNode* c = cl->node;

        print_node(c->id, c->name);
        edge(from, c->id, "class");

        print_feature_list(c->features, c->id);

        cl = cl->next;
    }
}

void save_ast_dot(ProgramNode* p) {
    OUT = fopen("Dot/cool_dot.dot", "w");

    fprintf(OUT, "digraph AST {\n");
    fprintf(OUT, "  node [shape=box];\n");

    print_node(p->id, "program");
    print_class_list(p->classes, p->id);

    fprintf(OUT, "}\n");

    fclose(OUT);
}
