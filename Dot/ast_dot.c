#include "ast_dot.h"
#include <stdio.h>

static int node_counter = 0;

static int print_node(FILE* out, const char* label) {
    int id = node_counter++;
    fprintf(out, "  node%d [label=\"%s\"];\n", id, label);
    return id;
}

static void print_edge(FILE* out, int from, int to) {
    fprintf(out, "  node%d -> node%d;\n", from, to);
}

static int print_expr(FILE* out, ExprNode* expr);

static int print_expr(FILE* out, ExprNode* expr) {
    if (!expr) return -1;
    int id = -1;

    switch (expr->kind) {
    case EXPR_INT_CONST: {
        char buf[32];
        snprintf(buf, sizeof(buf), "int %d", expr->int_const.value);
        id = print_node(out, buf);
        break;
    }
    case EXPR_STR_CONST:
        id = print_node(out, expr->str_const.value);
        break;
    case EXPR_BOOL_CONST:
        id = print_node(out, expr->bool_const.value ? "true" : "false");
        break;
    case EXPR_OBJECT:
        id = print_node(out, expr->object.name);
        break;
    case EXPR_ASSIGN: {
        id = print_node(out, "=");
        int v = print_node(out, expr->assign.name);
        int rhs = print_expr(out, expr->assign.expr);
        print_edge(out, id, v);
        if (rhs != -1) print_edge(out, id, rhs);
        break;
    }
    case EXPR_BINOP: {
        const char* ops[] = { "+", "-", "*", "/", "<", "<=", "=", "and", "or" };
        const char* op = (expr->binop.op >= 0 && expr->binop.op < 9) ? ops[expr->binop.op] : "?";
        id = print_node(out, op);
        int l = print_expr(out, expr->binop.left);
        int r = print_expr(out, expr->binop.right);
        if (l != -1) print_edge(out, id, l);
        if (r != -1) print_edge(out, id, r);
        break;
    }
    case EXPR_UNOP: {
        const char* ops[] = { "~", "not", "isvoid" };
        const char* op = (expr->unop.op >= 0 && expr->unop.op < 3) ? ops[expr->unop.op] : "?";
        id = print_node(out, op);
        int sub = print_expr(out, expr->unop.expr);
        if (sub != -1) print_edge(out, id, sub);
        break;
    }
    case EXPR_IF: {
        id = print_node(out, "if");
        int c = print_expr(out, expr->if_expr.cond);
        int t = print_expr(out, expr->if_expr.then_branch);
        int e = print_expr(out, expr->if_expr.else_branch);
        if (c != -1) print_edge(out, id, c);
        if (t != -1) print_edge(out, id, t);
        if (e != -1) print_edge(out, id, e);
        break;
    }
    case EXPR_WHILE: {
        id = print_node(out, "while");
        int c = print_expr(out, expr->while_expr.cond);
        int b = print_expr(out, expr->while_expr.body);
        if (c != -1) print_edge(out, id, c);
        if (b != -1) print_edge(out, id, b);
        break;
    }
    case EXPR_BLOCK: {
        id = print_node(out, "block");
        for (ExprList* ex = expr->block.exprs; ex; ex = ex->next) {
            int sub = print_expr(out, ex->node);
            if (sub != -1) print_edge(out, id, sub);
        }
        break;
    }
    case EXPR_LET: {
        id = print_node(out, "let");
        for (LetList* l = expr->let_expr.bindings; l; l = l->next) {
            int bind_id = print_node(out, l->binding->name);
            print_edge(out, id, bind_id);
            if (l->binding->init) {
                int init_id = print_expr(out, l->binding->init);
                if (init_id != -1) print_edge(out, bind_id, init_id);
            }
        }
        int body = print_expr(out, expr->let_expr.body);
        if (body != -1) print_edge(out, id, body);
        break;
    }
    case EXPR_CASE: {
        id = print_node(out, "case");
        int e = print_expr(out, expr->case_expr.expr);
        if (e != -1) print_edge(out, id, e);
        for (CaseList* c = expr->case_expr.cases; c; c = c->next) {
            char buf[128];
            snprintf(buf, sizeof(buf), "%s : %s", c->node->name, c->node->type);
            int cname = print_node(out, buf);
            print_edge(out, id, cname);
            int cexpr = print_expr(out, c->node->expr);
            if (cexpr != -1) print_edge(out, cname, cexpr);
        }
        break;
    }
    case EXPR_DISPATCH: {
        char buf[128];
        snprintf(buf, sizeof(buf), "dispatch %s", expr->dispatch.method);
        id = print_node(out, buf);
        if (expr->dispatch.caller) {
            int caller = print_expr(out, expr->dispatch.caller);
            if (caller != -1) print_edge(out, id, caller);
        }
        for (ExprList* a = expr->dispatch.args; a; a = a->next) {
            int arg = print_expr(out, a->node);
            if (arg != -1) print_edge(out, id, arg);
        }
        break;
    }
    case EXPR_STATIC_DISPATCH: {
        char buf[128];
        snprintf(buf, sizeof(buf), "dispatch %s@%s", expr->static_dispatch.method, expr->static_dispatch.type);
        id = print_node(out, buf);
        int caller = print_expr(out, expr->static_dispatch.caller);
        if (caller != -1) print_edge(out, id, caller);
        for (ExprList* a = expr->static_dispatch.args; a; a = a->next) {
            int arg = print_expr(out, a->node);
            if (arg != -1) print_edge(out, id, arg);
        }
        break;
    }
    case EXPR_NEW: {
        char buf[64];
        snprintf(buf, sizeof(buf), "new %s", expr->new_expr.type);
        id = print_node(out, buf);
        break;
    }
    default:
        id = print_node(out, "expr?");
        break;
    }

    return id;
}

void save_ast_dot(ProgramNode* program) {
    FILE* out = fopen("Dot/cool_dot.dot", "w");
    if (!out) {
        fprintf(stderr, "Не удалось открыть cool_dot.dot для записи\n");
        return;
    }

    fprintf(out, "digraph AST {\n");
    node_counter = 0;

    if (!program) {
        fprintf(stderr, "Program == NULL\n");
        fprintf(out, "  node0 [label=\"<empty program>\"];\n}\n");
        fclose(out);
        return;
    }

    int prog_id = print_node(out, "Program");

    for (ClassList* c = program->classes; c; c = c->next) {
        ClassNode* cls = c->node;
        char buf[128];
        snprintf(buf, sizeof(buf), "class %s", cls->name);
        int class_id = print_node(out, buf);
        print_edge(out, prog_id, class_id);

        if (cls->parent) {
            char parent_buf[128];
            snprintf(parent_buf, sizeof(parent_buf), "inherits %s", cls->parent);
            int par_id = print_node(out, parent_buf);
            print_edge(out, class_id, par_id);
        }

        for (FeatureList* f = cls->features; f; f = f->next) {
            FeatureNode* feat = f->node;
            char flabel[128];

            if (feat->kind == FEATURE_METHOD)
                snprintf(flabel, sizeof(flabel), "method %s : %s", feat->name, feat->method.return_type);
            else
                snprintf(flabel, sizeof(flabel), "attr %s : %s", feat->name, feat->attr.type);

            int feat_id = print_node(out, flabel);
            print_edge(out, class_id, feat_id);

            if (feat->kind == FEATURE_METHOD && feat->method.body)
                print_edge(out, feat_id, print_expr(out, feat->method.body));
            else if (feat->kind == FEATURE_ATTR && feat->attr.init)
                print_edge(out, feat_id, print_expr(out, feat->attr.init));
        }
    }

    fprintf(out, "}\n");
    fclose(out);
}
