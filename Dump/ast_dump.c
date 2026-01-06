//
// Created by Дмитрий on 02.01.2026.
//

#include "ast_dump.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static FILE *OUT;
static int INDENT = 0;

static void indent(void) {
    for (int i = 0; i < INDENT; i++)
        fputc(' ', OUT);
}

static void line(const char *fmt, ...) {
    indent();
    va_list ap;
    va_start(ap, fmt);
    vfprintf(OUT, fmt, ap);
    va_end(ap);
    fputc('\n', OUT);
}

static void dump_expr(ExprNode *e);

static void dump_class(ClassNode *c) {
    indent();
    fprintf(OUT, "(class %s %s\n",
            c->name,
            c->parent ? c->parent : "Object");

    INDENT += 2;

    for (FeatureList *f = c->features; f; f = f->next) {
        FeatureNode *feat = f->node;

        if (feat->kind == FEATURE_METHOD) {
            indent();
            fprintf(OUT, "(method %s (", feat->name);

            for (FormalList *p = feat->method.formals; p; p = p->next) {
                fprintf(OUT, "%s:%s",
                        p->node->name,
                        p->node->type);
                if (p->next) fprintf(OUT, " ");
            }

            fprintf(OUT, ") %s\n", feat->method.return_type);

            INDENT += 2;
            dump_expr(feat->method.body);
            INDENT -= 2;

            line(")");
        } else {
            indent();
            fprintf(OUT, "(attr %s %s",
                    feat->name,
                    feat->attr.type);

            if (feat->attr.init) {
                fprintf(OUT, "\n");
                INDENT += 2;
                dump_expr(feat->attr.init);
                INDENT -= 2;
                line(")");
            } else {
                fprintf(OUT, ")\n");
            }
        }
    }

    INDENT -= 2;
    line(")");
}

static void dump_expr(ExprNode *e) {
    if (!e) return;

    switch (e->kind) {

        case EXPR_INT_CONST:
            line("(int %d)", e->int_const.value);
            break;

        case EXPR_BOOL_CONST:
            line("(bool %s)", e->bool_const.value ? "true" : "false");
            break;

        case EXPR_STR_CONST:
            line("(string \"%s\")", e->str_const.value);
            break;

        case EXPR_OBJECT:
            line("(object %s)", e->object.name);
            break;

        case EXPR_ASSIGN:
            line("(assign %s", e->assign.name);
            INDENT += 2;
            dump_expr(e->assign.expr);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_BINOP:
            line("(%s", binop_name(e->binop.op));
            INDENT += 2;
            dump_expr(e->binop.left);
            dump_expr(e->binop.right);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_UNOP:
            line("(%s", unop_name(e->unop.op));
            INDENT += 2;
            dump_expr(e->unop.expr);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_IF:
            line("(if");
            INDENT += 2;
            dump_expr(e->if_expr.cond);
            dump_expr(e->if_expr.then_branch);
            dump_expr(e->if_expr.else_branch);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_WHILE:
            line("(while");
            INDENT += 2;
            dump_expr(e->while_expr.cond);
            dump_expr(e->while_expr.body);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_BLOCK:
            line("(block");
            INDENT += 2;
            for (ExprList *l = e->block.exprs; l; l = l->next)
                dump_expr(l->node);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_NEW:
            line("(new %s)", e->new_expr.type);
            break;

        case EXPR_DISPATCH:
            line("(call %s", e->dispatch.method);
            INDENT += 2;
            if (e->dispatch.caller)
                dump_expr(e->dispatch.caller);
            for (ExprList *a = e->dispatch.args; a; a = a->next)
                dump_expr(a->node);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_LET:
            line("(let");
            INDENT += 2;

            for (LetList *l = e->let_expr.bindings; l; l = l->next) {
                LetBindingNode *b = l->binding;
                indent();
                fprintf(OUT, "(%s %s",
                        b->name,
                        b->type);
                if (b->init) {
                    fprintf(OUT, "\n");
                    INDENT += 2;
                    dump_expr(b->init);
                    INDENT -= 2;
                    line(")");
                } else {
                    fprintf(OUT, ")\n");
                }
            }

            dump_expr(e->let_expr.body);
            INDENT -= 2;
            line(")");
            break;

        case EXPR_CASE:
            line("(case");
            INDENT += 2;
            dump_expr(e->case_expr.expr);
            for (CaseList *c = e->case_expr.cases; c; c = c->next) {
                indent();
                fprintf(OUT, "(branch %s %s\n",
                        c->node->name,
                        c->node->type);
                INDENT += 2;
                dump_expr(c->node->expr);
                INDENT -= 2;
                line(")");
            }
            INDENT -= 2;
            line(")");
            break;
    }
}

bool ast_dump_text(const char *filename, ProgramNode *p) {
    OUT = fopen(filename, "w");
    if (!OUT) return false;

    fprintf(OUT, "(program\n");
    INDENT = 2;

    for (ClassList *c = p->classes; c; c = c->next)
        dump_class(c->node);

    INDENT = 0;
    fprintf(OUT, ")\n");

    fclose(OUT);
    return true;
}
