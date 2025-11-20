#include "ast_dot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static FILE* OUT;

static char* escape_str(const char* s) {
    if (!s) return strdup("");
    size_t len = strlen(s);
    char* r = malloc(len * 2 + 1);
    char* p = r;
    for (size_t i = 0; i < len; i++) {
        if (s[i] == '"' || s[i] == '\\') *p++ = '\\';
        *p++ = s[i];
    }
    *p = 0;
    return r;
}

static void print_node(const char* label) {
    char* esc = escape_str(label);
    fprintf(OUT, "  node%p [label=\"%s\"];\n", (void*)label, esc); // уникальный указатель вместо id
    free(esc);
}

static void edge(void* from, void* to, const char* label) {
    fprintf(OUT, "  node%p -> node%p [label=\"%s\"];\n", from, to, label);
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

static void print_expr_list(ExprList* l, void* from) {
    int idx = 1;
    while(l){
        print_expr(l->node);
        char label[16]; snprintf(label,sizeof(label),"arg%d",idx++);
        edge(from, l->node, label);
        l = l->next;
    }
}

static void print_case_list(CaseList* l, void* from){
    while(l){
        CaseNode* c = l->node;
        char buf[256]; snprintf(buf,sizeof(buf),"%s : %s",c->name,c->type);
        print_node(buf);
        edge(from, c, "case");
        print_expr(c->expr);
        edge(c, c->expr, "expr");
        l = l->next;
    }
}

static void print_let_list(LetList* l, void* from){
    int idx=1;
    while(l){
        LetBindingNode* b = l->binding;
        char buf[256]; snprintf(buf,sizeof(buf),"%s : %s",b->name,b->type);
        print_node(buf);
        edge(from, b, "binding");
        if(b->init){
            print_expr(b->init);
            edge(b, b->init, "init");
        }
        l=l->next; idx++;
    }
}

static void print_expr(ExprNode* e){
    if(!e) return;

    switch(e->kind){
        case EXPR_ASSIGN:
            print_node(e->assign.name);
            print_expr(e->assign.expr);
            edge(e, e->assign.expr, "arg1");
            break;
        case EXPR_BINOP: {
            const char* op = binop_name(e->binop.op);
            print_node(op);
            print_expr(e->binop.left);
            print_expr(e->binop.right);
            edge(e, e->binop.left, "left");
            edge(e, e->binop.right,"right");
            break;
        }
        case EXPR_UNOP: {
            const char* op = unop_name(e->unop.op);
            print_node(op);
            print_expr(e->unop.expr);
            edge(e, e->unop.expr,"arg1");
            break;
        }
        case EXPR_OBJECT:
            print_node(e->object.name);
            break;
        case EXPR_INT_CONST: {
            char buf[64]; snprintf(buf,sizeof(buf),"%d",e->int_const.value);
            print_node(buf);
            break;
        }
        case EXPR_STR_CONST: {
            char* esc = escape_str(e->str_const.value);
            char buf[512]; snprintf(buf,sizeof(buf),"\"%s\"",esc);
            print_node(buf); free(esc);
            break;
        }
        case EXPR_BOOL_CONST:
            print_node(e->bool_const.value?"true":"false");
            break;
        case EXPR_DISPATCH:
            print_node(e->dispatch.method);
            if(e->dispatch.caller){ print_expr(e->dispatch.caller); edge(e,e->dispatch.caller,"caller"); }
            print_expr_list(e->dispatch.args,e);
            break;
        case EXPR_STATIC_DISPATCH:{
            char buf[256]; snprintf(buf,sizeof(buf),"%s@%s", e->static_dispatch.method,e->static_dispatch.type);
            print_node(buf);
            if(e->static_dispatch.caller){ print_expr(e->static_dispatch.caller); edge(e,e->static_dispatch.caller,"caller"); }
            print_expr_list(e->static_dispatch.args,e);
            break;
        }
        case EXPR_IF:
            print_node("if");
            print_expr(e->if_expr.cond); edge(e,e->if_expr.cond,"cond");
            print_expr(e->if_expr.then_branch); edge(e,e->if_expr.then_branch,"then");
            print_expr(e->if_expr.else_branch); edge(e,e->if_expr.else_branch,"else");
            break;
        case EXPR_WHILE:
            print_node("while");
            print_expr(e->while_expr.cond); edge(e,e->while_expr.cond,"cond");
            print_expr(e->while_expr.body); edge(e,e->while_expr.body,"body");
            break;
        case EXPR_BLOCK:{
            print_node("block");
            ExprList* l = e->block.exprs;
            int idx=1;
            while(l){ print_expr(l->node); char lbl[16]; snprintf(lbl,sizeof(lbl),"arg%d",idx++); edge(e,l->node,lbl); l=l->next; }
            break;
        }
        case EXPR_LET:
            print_node("let");
            print_let_list(e->let_expr.bindings,e);
            print_expr(e->let_expr.body); edge(e,e->let_expr.body,"body");
            break;
        case EXPR_CASE:
            print_node("case");
            print_expr(e->case_expr.expr); edge(e,e->case_expr.expr,"expr");
            print_case_list(e->case_expr.cases,e);
            break;
        case EXPR_NEW:
            print_node(e->new_expr.type);
            break;
    }
}

static void print_feature_list(FeatureList* fl, void* from){
    int attr_idx=1, method_idx=1;
    while(fl){
        FeatureNode* f = fl->node;
        print_node(f->name);
        if(f->kind==FEATURE_METHOD){ edge(from,f,"method"); }
        else{ edge(from,f,"attr"); }

        if(f->kind==FEATURE_METHOD){
            FormalList* p=f->method.formals;
            int idx=1;
            while(p){ char buf[256]; snprintf(buf,sizeof(buf),"%s : %s",p->node->name,p->node->type); print_node(buf); edge(f,p->node,"arg1"); p=p->next; idx++; }
            if(f->method.body){ print_expr(f->method.body); edge(f,f->method.body,"body"); }
        } else { if(f->attr.init){ print_expr(f->attr.init); edge(f,f->attr.init,"init"); } }
        fl=fl->next;
    }
}

static void print_class_list(ClassList* cl, void* from){
    while(cl){
        ClassNode* c = cl->node;
        print_node(c->name);
        edge(from,c,"class");
        print_feature_list(c->features,c);
        cl=cl->next;
    }
}

void save_ast_dot(ProgramNode* p) {
    OUT = fopen("Dot/cool_dot.dot", "w");

    fprintf(OUT, "digraph AST {\n");
    fprintf(OUT, "  node [shape=box];\n");

    print_node("program");
    print_class_list(p->classes);

    fprintf(OUT, "}\n");

    fclose(OUT);
}
