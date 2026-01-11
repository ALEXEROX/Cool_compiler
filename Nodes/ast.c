#include "ast.h"
#include <stdlib.h>
#include <string.h>

ProgramNode *program = NULL;

int AST_NODE_ID_COUNTER = 0;

static char* dupstr(const char* s) {
    if (!s) return NULL;
    char* r = (char *)malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

ProgramNode* make_program(ClassList* classes) {
    ProgramNode* p = (ProgramNode*)malloc(sizeof(ProgramNode));
    p->id = NEW_ID();
    p->classes = classes;
    return p;
}

ClassNode* make_class(const char* name, const char* parent, FeatureList* features) {
    ClassNode* c = (ClassNode*)malloc(sizeof(ClassNode));
    c->id = NEW_ID();
    c->name = dupstr(name);
    c->parent = parent ? dupstr(parent) : NULL;
    c->features = features;
    return c;
}

FeatureNode* make_method(const char* name, FormalList* formals, const char* type, ExprNode* body) {
    FeatureNode* f = (FeatureNode*)malloc(sizeof(FeatureNode));
    f->id = NEW_ID();
    f->kind = FEATURE_METHOD;
    f->name = dupstr(name);
    f->method.formals = formals;
    f->method.return_type = dupstr(type);
    f->method.body = body;
    return f;
}

FeatureNode* make_attr(const char* name, const char* type, ExprNode* init) {
    FeatureNode* f = (FeatureNode*)malloc(sizeof(FeatureNode));
    f->id = NEW_ID();
    f->kind = FEATURE_ATTR;
    f->name = dupstr(name);
    f->attr.type = dupstr(type);
    f->attr.init = init;
    return f;
}

FormalNode* make_formal(const char* name, const char* type) {
    FormalNode* n = (FormalNode*)malloc(sizeof(FormalNode));
    n->id = NEW_ID();
    n->name = dupstr(name);
    n->type = dupstr(type);
    return n;
}

LetBindingNode* make_let_binding(const char* name, const char* type, ExprNode* init) {
    LetBindingNode* l = (LetBindingNode*)malloc(sizeof(LetBindingNode));
    l->id = NEW_ID();
    l->name = dupstr(name);
    l->type = dupstr(type);
    l->init = init;
    return l;
}

CaseNode* make_case_item(const char* name, const char* type, ExprNode* expr) {
    CaseNode* c = (CaseNode*)malloc(sizeof(CaseNode));
    c->id = NEW_ID();
    c->name = dupstr(name);
    c->type = dupstr(type);
    c->expr = expr;
    return c;
}

ExprNode* make_assign(const char* name, ExprNode* expr) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_ASSIGN;
    e->assign.name = dupstr(name);
    e->assign.expr = expr;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_binop(BinOpKind op, ExprNode* left, ExprNode* right) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_BINOP;
    e->binop.op = op;
    e->binop.left = left;
    e->binop.right = right;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_unop(UnOpKind op, ExprNode* expr) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_UNOP;
    e->unop.op = op;
    e->unop.expr = expr;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_object(const char* name) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_OBJECT;
    e->object.name = dupstr(name);
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_int(int value) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_INT_CONST;
    e->int_const.value = value;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_string(const char* value) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_STR_CONST;
    e->str_const.value = dupstr(value);
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_bool(bool value) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_BOOL_CONST;
    e->bool_const.value = value;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_dispatch(ExprNode* caller, const char* method, ExprList* args) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_DISPATCH;
    e->dispatch.caller = caller;
    e->dispatch.method = dupstr(method);
    e->dispatch.args = args;
    e->static_type = NULL;
    e->var_binding = NULL;
    e->dispatch_class = NULL;
    return e;
}

ExprNode* make_static_dispatch(ExprNode* caller, const char* type, const char* method, ExprList* args) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_STATIC_DISPATCH;
    e->static_dispatch.caller = caller;
    e->static_dispatch.type = dupstr(type);
    e->static_dispatch.method = dupstr(method);
    e->static_dispatch.args = args;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_if(ExprNode* cond, ExprNode* then_e, ExprNode* else_e) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_IF;
    e->if_expr.cond = cond;
    e->if_expr.then_branch = then_e;
    e->if_expr.else_branch = else_e;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_while(ExprNode* cond, ExprNode* body) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_WHILE;
    e->while_expr.cond = cond;
    e->while_expr.body = body;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_block(ExprList* exprs) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_BLOCK;
    e->block.exprs = exprs;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_let(LetList* bindings, ExprNode* body) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_LET;
    e->let_expr.bindings = bindings;
    e->let_expr.body = body;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

LetList* make_let_list(LetBindingNode* binding) {
    LetList* list = (LetList*)malloc(sizeof(LetList));
    list->binding = binding;
    list->next = NULL;
    return list;
}

LetList* append_let_list(LetList* list, LetBindingNode* binding) {
    if (!list) return make_let_list(binding);
    LetList* cur = list;
    while (cur->next) cur = cur->next;
    cur->next = make_let_list(binding);
    return list;
}


ExprNode* make_case(ExprNode* expr, CaseList* cases) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_CASE;
    e->case_expr.expr = expr;
    e->case_expr.cases = cases;
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ExprNode* make_new(const char* type) {
    ExprNode* e = (ExprNode*)malloc(sizeof(ExprNode));
    e->id = NEW_ID();
    e->kind = EXPR_NEW;
    e->new_expr.type = dupstr(type);
    e->static_type = NULL;
    e->var_binding = NULL;
    return e;
}

ClassList* make_class_list(ClassNode* node) {
    ClassList* list = (ClassList*)malloc(sizeof(ClassList));
    list->node = node;
    list->next = NULL;
    return list;
}

ClassList* append_class_list(ClassList* list, ClassNode* node) {
    if (!list) return make_class_list(node);
    ClassList* cur = list;
    while (cur->next) cur = cur->next;
    cur->next = make_class_list(node);
    return list;
}

FeatureList* make_feature_list(FeatureNode* node) {
    FeatureList* list = (FeatureList*)malloc(sizeof(FeatureList));
    list->node = node;
    list->next = NULL;
    return list;
}

FeatureList* append_feature_list(FeatureList* list, FeatureNode* node) {
    if (!list) return make_feature_list(node);
    FeatureList* cur = list;
    while (cur->next) cur = cur->next;
    cur->next = make_feature_list(node);
    return list;
}

FormalList* make_formal_list(FormalNode* node) {
    FormalList* list = (FormalList*)malloc(sizeof(FormalList));
    list->node = node;
    list->next = NULL;
    return list;
}

FormalList* append_formal_list(FormalList* list, FormalNode* node) {
    if (!list) return make_formal_list(node);
    FormalList* cur = list;
    while (cur->next) cur = cur->next;
    cur->next = make_formal_list(node);
    return list;
}

ExprList* make_expr_list(ExprNode* node) {
    ExprList* list = (ExprList*)malloc(sizeof(ExprList));
    list->node = node;
    list->next = NULL;
    return list;
}

ExprList* append_expr_list(ExprList* list, ExprNode* node) {
    if (!list) return make_expr_list(node);
    ExprList* cur = list;
    while (cur->next) cur = cur->next;
    cur->next = make_expr_list(node);
    return list;
}

CaseList* make_case_list(CaseNode* node) {
    CaseList* list = (CaseList*)malloc(sizeof(CaseList));
    list->node = node;
    list->next = NULL;
    return list;
}

CaseList* append_case_list(CaseList* list, CaseNode* node) {
    if (!list) return make_case_list(node);
    CaseList* cur = list;
    while (cur->next) cur = cur->next;
    cur->next = make_case_list(node);
    return list;
}