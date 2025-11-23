#pragma once

#include <stdbool.h>
#include "../Nodes/ast.h"
#include "constant_pool.h"
#include "descriptor.h"


/* Информация об атрибуте (поле) класса */
typedef struct AttrInfo {
    int id;                 /* порядковый номер в таблице атрибутов (можно использовать при кодогенерации) */
    char *name;
    char *type;             /* имя типа в формате Cool (например "Int", "MyClass") */
    FeatureNode *ast;       /* ссылка на AST-узел (если нужна) */
    struct AttrInfo *next;
} AttrInfo;

/* Информация о методе */
typedef struct MethodInfo {
    int id;                 /* порядковый номер в таблице методов */
    char *name;
    int param_count;
    char **param_names;     /* имена параметров (массив) */
    char **param_types;     /* типы параметров (Cool-имена) */
    char *return_type;      /* Cool-имя возвращаемого типа */
    FeatureNode *ast;       /* ссылка на AST-узел метода */
    struct MethodInfo *next;
} MethodInfo;

/* Информация о классе */
typedef struct ClassInfo {
    int id;                 /* порядковый номер класса (можно для индексации) */
    char *name;             /* имя класса в форме "Pkg/Class" или "Class" */
    char *parent;           /* имя родителя (NULL для Object или если не задан) */
    ClassNode *ast;         /* ссылка на AST-узел класса */
    AttrInfo *attrs;        /* список атрибутов */
    MethodInfo *methods;    /* список методов */
    struct ClassInfo *next;
} ClassInfo;

/* Таблица классов */
typedef struct {
    ClassInfo *head;
    int count;
} ClassTable;

/* Инициализация / освобождение */
void class_table_init(ClassTable *ct);
void class_table_free(ClassTable *ct);

/* Добавление/поиск классов */
ClassInfo *class_table_add(ClassTable *ct, const char *name, const char *parent, ClassNode *ast);
ClassInfo *class_table_find(ClassTable *ct, const char *name);

/* Добавление атрибута (из AST-узла feature типа ATTR) */
AttrInfo *class_add_attr_from_feature(ClassInfo *cls, FeatureNode *feat);

/* Добавление метода (из AST-узла feature типа METHOD) */
MethodInfo *class_add_method_from_feature(ClassInfo *cls, FeatureNode *feat);

/* Поиск метода в классе и по иерархии предков.
   Возвращает MethodInfo* для первого найденного (в самом классе, иначе в родителях), или NULL. */
MethodInfo *class_lookup_method(ClassTable *ct, const char *class_name, const char *method_name);

/* Проверки наследования:
   - is_subtype(ct, child, parent) — true, если child <: parent (включая равенство).
   - lub(ct, A, B) — возвращает имя типа (malloc'ed) наименьшего общего предка; caller должен free(). */
bool is_subtype(ClassTable *ct, const char *child, const char *parent);
char *lub(ClassTable *ct, const char *A, const char *B);

/* Утилита: получить JVM-описание сигнатуры метода (malloc'ed строка) */
char *methodinfo_get_descriptor(MethodInfo *m);

/* Создать MethodInfo вручную (используется внутри) */
MethodInfo *methodinfo_create(const char *name, FormalList *formals, const char *return_type);

/* Печать для отладки */
void class_table_print(ClassTable *ct, FILE *out);
