#include "ast_dot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../semantic/class_table.h"
#include "../semantic/object_env.h"

static FILE* OUT;

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

    fprintf(OUT, "  node%d [label=\"%s \"];\n", id, esc);

    free(esc);
}

static void print_expr_node(ExprNode* e, const char* title) {
    char buf[512];

    if (e->static_type) {
        snprintf(buf, sizeof(buf),
                 "%s\n: %s",
                 title,
                 e->static_type);
    } else {
        snprintf(buf, sizeof(buf),
                 "%s\n: <no type>",
                 title);
    }

    print_node(e->id, buf);
}


static void edge(int from, int to, const char* label) {
    fprintf(OUT, "  node%d -> node%d [label=\"%s\"];\n", from, to, label);
}

static const char* binop_name(BinOpKind op) {
    switch (op) {
        case OP_PLUS: return "+";
        case OP_MINUS:return "-";
        case OP_MUL:  return "*";
        case OP_DIV:  return "/";
        case OP_LT:   return "<";
        case OP_LE:   return "<=";
        case OP_EQ:   return "=";
        case OP_AND:  return "and";
        case OP_OR:   return "or";
    }
    return "?";
}

static const char* unop_name(UnOpKind op) {
    switch(op){
        case OP_NEG:    return "~";
        case OP_ISVOID: return "isvoid";
        case OP_NOT:    return "not";
    }
    return "?";
}

static void print_expr(ExprNode* e);

static void print_expr_list(ExprList* l, int from) {
    int idx = 1;
    while(l){
        print_expr(l->node);
        char label[16]; snprintf(label,sizeof(label),"arg%d",idx++);
        edge(from, l->node->id, label);
        l = l->next;
    }
}

static void print_case_list(CaseList* l, int from){
    while(l){
        CaseNode* c = l->node;
        char buf[256]; snprintf(buf,sizeof(buf),"%s : %s",c->name,c->type);
        print_node(c->id, buf);
        edge(from, c->id, "case");
        print_expr(c->expr);
        edge(c->id, c->expr->id, "expr");
        l = l->next;
    }
}

static void print_let_list(LetList* l, int from){
    int idx=1;
    while(l){
        LetBindingNode* b = l->binding;
        char buf[256]; snprintf(buf,sizeof(buf),"%s : %s",b->name,b->type);
        print_node(b->id,buf);
        edge(from, b->id, "binding");
        if(b->init){
            print_expr(b->init);
            edge(b->id, b->init->id, "init");
        }
        l=l->next; idx++;
    }
}

static void print_expr(ExprNode* e){
    if(!e) return;

    switch(e->kind){
        case EXPR_ASSIGN: {
            char buf[256];
            snprintf(buf,sizeof(buf),"%s\n: %s",e->assign.name,e->static_type ? e->static_type : "?");
            print_node(e->id, buf);
            print_expr(e->assign.expr);
            edge(e->id, e->assign.expr->id, "arg1");
            break;
        }
        case EXPR_BINOP: {
            const char* op = binop_name(e->binop.op);
            char buf[128];
            snprintf(buf,sizeof(buf),"%s\n: %s",op,e->static_type ? e->static_type : "?");
            print_node(e->id, buf);
            print_expr(e->binop.left);
            print_expr(e->binop.right);
            edge(e->id, e->binop.left->id, "left");
            edge(e->id, e->binop.right->id,"right");
            break;
        }
        case EXPR_UNOP: {
            const char* op = unop_name(e->unop.op);
            print_node(e->id,op);
            print_expr(e->unop.expr);
            edge(e->id, e->unop.expr->id,"arg1");
            break;
        }
        case EXPR_OBJECT: {
            char buf[256];
            if (e->var_binding) {
                snprintf(buf,sizeof(buf),
                         "%s\n: %s\nidx=%d",
                         e->object.name,
                         e->var_binding->type,
                         e->var_binding->index);
            } else {
                snprintf(buf,sizeof(buf),
                         "%s\n: %s",
                         e->object.name,
                         e->static_type ? e->static_type : "?");
            }
            print_node(e->id, buf);
            break;
        }

        case EXPR_INT_CONST: {
            char buf[64];
            snprintf(buf,sizeof(buf),"(%d\n: %s)",e->int_const.value,e->static_type);
            print_node(e->id,buf);
            break;
        }
        case EXPR_STR_CONST: {
            char* esc = escape_str(e->str_const.value);
            char buf[512];
            snprintf(buf,sizeof(buf),"(\"%s\"\n: %s)",esc,e->static_type);
            print_node(e->id,buf); free(esc);
            break;
        }
        case EXPR_BOOL_CONST: {
            char buf[64];
            snprintf(buf,sizeof(buf),"(%d\n: %s)",e->bool_const.value,e->static_type);
            print_node(e->id,buf);
            break;
        }
        case EXPR_DISPATCH: {
            char buf[512];

            if (e->resolved_method && e->dispatch_class) {
                snprintf(buf,sizeof(buf),
                         "%s()\n: %s\nowner=%s",
                         e->dispatch.method,
                         e->static_type,
                         e->dispatch_class->name);
            } else {
                snprintf(buf,sizeof(buf),
                         "%s()\n: %s",
                         e->dispatch.method,
                         e->static_type ? e->static_type : "?");
            }

            print_node(e->id, buf);
            if(e->dispatch.caller){ print_expr(e->dispatch.caller); edge(e->id,e->dispatch.caller->id,"caller"); }
            print_expr_list(e->dispatch.args,e->id);
            break;
        }
        case EXPR_STATIC_DISPATCH:{
            char buf[256];
            snprintf(buf,sizeof(buf),"%s@%s", e->static_dispatch.method,e->static_dispatch.type);
            print_node(e->id,buf);
            if(e->static_dispatch.caller){ print_expr(e->static_dispatch.caller); edge(e->id,e->static_dispatch.caller->id,"caller"); }
            print_expr_list(e->static_dispatch.args,e->id);
            break;
        }
        case EXPR_IF: {
            char buf[128];
            snprintf(buf,sizeof(buf),"if\n: %s",e->static_type ? e->static_type : "?");
            print_node(e->id, buf);
            print_expr(e->if_expr.cond); edge(e->id,e->if_expr.cond->id,"cond");
            print_expr(e->if_expr.then_branch); edge(e->id,e->if_expr.then_branch->id,"then");
            print_expr(e->if_expr.else_branch); edge(e->id,e->if_expr.else_branch->id,"else");
            break;
        }
        case EXPR_WHILE: {
            char buf[128];
            snprintf(buf,sizeof(buf),"while\n: %s",e->static_type ? e->static_type : "?");
            print_node(e->id, buf);
            print_expr(e->while_expr.cond); edge(e->id,e->while_expr.cond->id,"cond");
            print_expr(e->while_expr.body); edge(e->id,e->while_expr.body->id,"body");
            break;
        }
        case EXPR_BLOCK:{
            char buf[128];
            snprintf(buf,sizeof(buf),"block\n: %s",e->static_type ? e->static_type : "?");
            print_node(e->id, buf);
            ExprList* l = e->block.exprs;
            int idx=1;
            while(l){ print_expr(l->node); char lbl[16]; snprintf(lbl,sizeof(lbl),"arg%d",idx++); edge(e->id,l->node->id,lbl); l=l->next; }
            break;
        }
        case EXPR_LET: {
            print_node(e->id,"let");
            print_let_list(e->let_expr.bindings,e->id);
            print_expr(e->let_expr.body); edge(e->id,e->let_expr.body->id,"body");
            break;
        }
        case EXPR_CASE: {
            print_node(e->id,"case");
            print_expr(e->case_expr.expr); edge(e->id,e->case_expr.expr->id,"expr");
            print_case_list(e->case_expr.cases,e->id);
            break;
        }
        case EXPR_NEW:
            print_node(e->id,e->new_expr.type);
            break;
    }
}

static void print_feature_list(FeatureList* fl, int from){
    int attr_idx=1, method_idx=1;
    while(fl){
        char attr[16];
        char method[16];
        FeatureNode* f = fl->node;
        print_node(f->id, f->name);
        if(f->kind==FEATURE_METHOD) {
            sprintf(method,"method_%d",method_idx++);
            edge(from,f->id,method);
        }
        else{
            sprintf(attr,"attr_%d",attr_idx++);
            edge(from,f->id,attr);
        }

        if(f->kind==FEATURE_METHOD){
            FormalList* p=f->method.formals;
            int idx=1;
            while(p){ char buf[256]; snprintf(buf,sizeof(buf),"%s : %s",p->node->name,p->node->type); print_node(p->node->id,buf); edge(f->id,p->node->id,"arg1"); p=p->next; idx++; }
            if(f->method.body){ print_expr(f->method.body); edge(f->id,f->method.body->id,"body"); }
        } else { if(f->attr.init){ print_expr(f->attr.init); edge(f->id,f->attr.init->id,"init"); } }
        fl=fl->next;
    }
}

static void print_class_list(ClassList* cl, int from){
    while(cl){
        ClassNode* c = cl->node;
        print_node(c->id, c->name);
        edge(from,c->id,"class");
        print_feature_list(c->features,c->id);
        cl=cl->next;
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
