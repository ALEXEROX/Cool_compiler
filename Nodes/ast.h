#pragma once

#include <stdbool.h>
#include "../semantic/constant_pool.h"

extern int AST_NODE_ID_COUNTER;


#define NEW_ID() (++AST_NODE_ID_COUNTER)

typedef struct ClassInfo ClassInfo;
typedef struct MethodInfo MethodInfo;
typedef struct AttrInfo AttrInfo;
typedef struct VarBinding VarBinding;

typedef struct ProgramNode ProgramNode;
typedef struct ClassNode ClassNode;
typedef struct FeatureNode FeatureNode;
typedef struct FormalNode FormalNode;
typedef struct ExprNode ExprNode;
typedef struct LetBindingNode LetBindingNode;
typedef struct CaseNode CaseNode;

typedef struct ClassList {
    struct ClassNode* node;
    struct ClassList* next;
} ClassList;

typedef struct FeatureList {
    struct FeatureNode* node;
    struct FeatureList* next;
} FeatureList;

typedef struct FormalList {
    struct FormalNode* node;
    struct FormalList* next;
} FormalList;

typedef struct ExprList {
    struct ExprNode* node;
    struct ExprList* next;
} ExprList;

typedef struct CaseList {
    struct CaseNode* node;
    struct CaseList* next;
} CaseList;

typedef struct BlockExprList {
    struct ExprNode* node;
    struct BlockExprList* next;
} BlockExprList;

typedef struct LetList {
    LetBindingNode* binding;
    struct LetList* next;
} LetList;

struct ProgramNode {
    int id;
    ClassList* classes;
    /* семантика */
    ConstantTable *constant_table;
};

struct ClassNode {
    int id;
    char* name;
    char* parent;
    FeatureList* features;
    /* семантика */
    ClassInfo *info;    /* заполнится после регистрации класса в ClassTable */
    bool is_builtin;    /* признак зарезервированного класса (Object, IO и т.д.) */
};

typedef enum {
    FEATURE_METHOD,
    FEATURE_ATTR
} FeatureKind;

struct FeatureNode {
    int id;          /* строка объявления */
    FeatureKind kind;
    char* name;
    union {
        struct {
            FormalList* formals;
            char* return_type;
            ExprNode* body;
        } method;
        struct {
            char* type;
            ExprNode* init;
        } attr;
    };
    /* семантика */
    MethodInfo *method_info; /* для FEATURE_METHOD — MethodInfo из ClassTable */
    AttrInfo *attr_info;     /* для FEATURE_ATTR — AttrInfo из ClassTable */

    int cp_ref_index;        /* опционально: индекс Fieldref/Methodref в constant pool (если создан) */
    void *user_data;         /* для расширений */
};

struct FormalNode {
    int id;
    char* name;
    char* type;
    /* семантика */
    int local_index;       /* индекс локальной переменной (JVM local index) при входе в метод */
    VarBinding *var_binding; /* ссылка на VarBinding в ObjectEnv (если зарегистрирован) */
    void *user_data;
};

struct LetBindingNode {
    int id;
    char* name;
    char* type;
    ExprNode* init;
    /* семантика */
    int local_index;       /* индекс в локалях */
    VarBinding *var_binding;
    void *user_data;
};

struct CaseNode {
    int id;
    char* name;
    char* type;
    ExprNode* expr;
    /* семантика */
    int local_index;       /* индекс локальной переменной, выделенной для этой ветки */
    VarBinding *var_binding;
    void *user_data;
};

typedef enum {
    EXPR_ASSIGN,
    EXPR_BINOP,
    EXPR_UNOP,
    EXPR_OBJECT,
    EXPR_INT_CONST,
    EXPR_STR_CONST,
    EXPR_BOOL_CONST,
    EXPR_DISPATCH,
    EXPR_STATIC_DISPATCH,
    EXPR_IF,
    EXPR_WHILE,
    EXPR_BLOCK,
    EXPR_LET,
    EXPR_CASE,
    EXPR_NEW
} ExprKind;

typedef enum {
    OP_PLUS, OP_MINUS, OP_MUL, OP_DIV,
    OP_LT, OP_LE, OP_EQ, OP_AND, OP_OR
} BinOpKind;

typedef enum {
    OP_NEG, OP_NOT, OP_ISVOID
} UnOpKind;

struct ExprNode {
    int id;
    ExprKind kind;
    ClassInfo * dispatch_class;

    union {
        struct { char* name; ExprNode* expr; } assign;
        struct { BinOpKind op; ExprNode* left; ExprNode* right; } binop;
        struct { UnOpKind op; ExprNode* expr; } unop;
        struct { char* name; } object;
        struct { int value; } int_const;
        struct { char* value; } str_const;
        struct { bool value; } bool_const;
        struct { ExprNode* caller; char* method; ExprList* args; } dispatch;
        struct { ExprNode* caller; char* type; char* method; ExprList* args; } static_dispatch;
        struct { ExprNode* cond; ExprNode* then_branch; ExprNode* else_branch; } if_expr;
        struct { ExprNode* cond; ExprNode* body; } while_expr;
        struct { ExprList* exprs; } block;
        struct { LetList* bindings; ExprNode* body; } let_expr;
        struct { ExprNode* expr; CaseList* cases; } case_expr;
        struct { char* type; } new_expr;
    };
    /* ---------------------------
       Семантические поля (обязательные)
       --------------------------- */

    /* Результирующий статический тип выражения (например "Int", "MyClass", "SELF_TYPE" и т.д.)
       Заполняется semantic-обходом: node->static_type = strdup("...") */
    char *static_type;

    /* Для идентификаторов: если имя ссылается на локальную переменную — ссылка на VarBinding */
    VarBinding *var_binding;
    int local_index;

    /* Для dispatch: найденный метод и/или индекс в constant pool (если уже создан) */
    MethodInfo *resolved_method;   /* MethodInfo, найденный при семантике (по статическому типу caller) */
    int methodref_index;           /* индекс Methodref в ConstantTable (0 если не создан) */

    /* Для обращения к полю: resolved AttrInfo + Fieldref index */
    AttrInfo *resolved_attr;
    int fieldref_index;
    int classref_index;
    int stringref_index;

    /* Опциональные данные для codegen/доп.аннотаций */
    int tmp_local_index;           /* при генерации можно выделить временный локал */
    void *user_data;
};

ProgramNode* make_program(ClassList* classes);
ClassNode* make_class(const char* name, const char* parent, FeatureList* features);
FeatureNode* make_method(const char* name, FormalList* formals, const char* type, ExprNode* body);
FeatureNode* make_attr(const char* name, const char* type, ExprNode* init);
FormalNode* make_formal(const char* name, const char* type);
LetBindingNode* make_let_binding(const char* name, const char* type, ExprNode* init);
CaseNode* make_case_item(const char* name, const char* type, ExprNode* expr);

ExprNode* make_assign(const char* name, ExprNode* expr);
ExprNode* make_binop(BinOpKind op, ExprNode* left, ExprNode* right);
ExprNode* make_unop(UnOpKind op, ExprNode* expr);
ExprNode* make_object(const char* name);
ExprNode* make_int(int value);
ExprNode* make_string(const char* value);
ExprNode* make_bool(bool value);
ExprNode* make_dispatch(ExprNode* caller, const char* method, ExprList* args);
ExprNode* make_static_dispatch(ExprNode* caller, const char* type, const char* method, ExprList* args);
ExprNode* make_if(ExprNode* cond, ExprNode* then_e, ExprNode* else_e);
ExprNode* make_while(ExprNode* cond, ExprNode* body);
ExprNode* make_block(ExprList* exprs);
ExprNode* make_let(LetList* bindings, ExprNode* body);
ExprNode* make_case(ExprNode* expr, CaseList* cases);
ExprNode* make_new(const char* type);

ClassList* make_class_list(ClassNode* class_);
FeatureList* make_feature_list(FeatureNode* feature);
FormalList* make_formal_list(FormalNode* formal);
ExprList* make_expr_list(ExprNode* expr);
LetList* make_let_list(LetBindingNode* binding);
CaseList* make_case_list(CaseNode* case_item);

ClassList* append_class_list(ClassList* list, ClassNode* item);
FeatureList* append_feature_list(FeatureList* list, FeatureNode* item);
FormalList* append_formal_list(FormalList* list, FormalNode* item);
ExprList* append_expr_list(ExprList* list, ExprNode* item);
LetList* append_let_list(LetList* list, LetBindingNode* item);
CaseList* append_case_list(CaseList* list, CaseNode* item);



extern ProgramNode *program;