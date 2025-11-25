#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "../Nodes/ast.h"
#include "constant_pool.h"
#include "descriptor.h"

/* Предварительные объявления (если понадобятся) */
typedef struct ClassInfo ClassInfo;
typedef struct MethodInfo MethodInfo;
typedef struct AttrInfo AttrInfo;

/* Информация об атрибуте (поле) класса */
typedef struct AttrInfo {
    int id;                 /* порядковый номер в таблице атрибутов */
    char *name;
    char *type;             /* имя типа в формате Cool */
    FeatureNode *ast;       /* ссылка на AST-узел */
    AttrInfo *next;
} AttrInfo;

/* Информация о методе */
typedef struct MethodInfo {
    int id;                 /* порядковый номер в таблице методов */
    char *name;
    int param_count;
    char **param_names;
    char **param_types;
    char *return_type;
    FeatureNode *ast;
    MethodInfo *next;
} MethodInfo;

/* Список детей (для удобства) */
typedef struct ClassChild {
    ClassInfo *child;
    struct ClassChild *next;
} ClassChild;

/* Информация о классе */
typedef struct ClassInfo {
    int id;
    char *name;             /* имя класса (как в AST), e.g. "Main" или "pkg/Sub" */
    char *parent;           /* имя родителя (строка, из AST) */
    ClassNode *ast;         /* оригинальный AST-узел */

    /* семантические связи */
    struct ClassInfo *parent_info; /* ссылка на родителя (NULL если нет) */
    ClassChild *children;          /* список прямых потомков */

    AttrInfo *attrs;
    MethodInfo *methods;

    /* вспомогательные флаги для построения графа */
    int visit_state; /* 0 = unvisited, 1 = visiting, 2 = done */

    ClassInfo *next; /* для списка в ClassTable */
} ClassInfo;

/* Таблица классов */
typedef struct {
    ClassInfo *head;
    int count;
} ClassTable;

/* Инициализация / создание / освобождение */
ClassTable *class_table_create(void);
void class_table_init(ClassTable *ct);
void class_table_free(ClassTable *ct);

/* Добавление/поиск классов */
/* Добавляет класс из AST; возвращает созданный или найденный ClassInfo.
   Не строит связи родителя — это сделает class_table_build_inheritance(). */
ClassInfo *class_table_add_class(ClassTable *ct, ClassNode *cnode);

/* Поиск по имени */
ClassInfo *class_table_find(ClassTable *ct, const char *name);

/* Добавление атрибута/метода в конкретный ClassInfo (из FeatureNode) */
AttrInfo *class_add_attr_from_feature(ClassInfo *cls, FeatureNode *feat);
MethodInfo *class_add_method_from_feature(ClassInfo *cls, FeatureNode *feat);

/* Построить наследование: связывает parent_info и children,
   проверяет:
     - существование родителя,
     - запрещённые родители (Int/String/Bool/SELF_TYPE),
     - циклы наследования,
     - наличие Object.
   Возвращает true если всё ок (иначе записывает ошибки и возвращает false). */
bool class_table_build_inheritance(ClassTable *ct);

/* Поиск метода в классе и предках */
MethodInfo *class_lookup_method(ClassTable *ct, const char *class_name, const char *method_name);

/* Проверки наследования и вспомогательные функции */
bool is_subtype(ClassTable *ct, const char *child, const char *parent);
char *lub(ClassTable *ct, const char *A, const char *B); /* возвращает malloc'ed строку */

/* Получение JVM-описания сигнатуры метода (malloc'ed строка) */
char *methodinfo_get_descriptor(MethodInfo *m);

/* Создать MethodInfo вручную (используется внутри, но экспортируем) */
MethodInfo *methodinfo_create(const char *name, FormalList *formals, const char *return_type);

/* Печать для отладки */
void class_table_print(ClassTable *ct, FILE *out);
