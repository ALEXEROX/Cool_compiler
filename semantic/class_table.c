#include "class_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ---- утилиты ---- */
static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

/* ---- создание/инициализация/освобождение ---- */
ClassTable *class_table_create(void) {
    ClassTable *ct = malloc(sizeof(ClassTable));
    class_table_init(ct);
    return ct;
}

void class_table_init(ClassTable *ct) {
    if (!ct) return;
    ct->head = NULL;
    ct->count = 0;
}

static void free_attr_list(AttrInfo *a) {
    while (a) {
        AttrInfo *n = a->next;
        free(a->name);
        free(a->type);
        free(a);
        a = n;
    }
}

static void free_method_list(MethodInfo *m) {
    while (m) {
        MethodInfo *n = m->next;
        free(m->name);
        for (int i = 0; i < m->param_count; ++i) {
            free(m->param_names[i]);
            free(m->param_types[i]);
        }
        free(m->param_names);
        free(m->param_types);
        free(m->return_type);
        free(m);
        m = n;
    }
}

static void free_children_list(ClassChild *ch) {
    while (ch) {
        ClassChild *n = ch->next;
        free(ch);
        ch = n;
    }
}

void class_table_free(ClassTable *ct) {
    if (!ct) return;
    ClassInfo *c = ct->head;
    while (c) {
        ClassInfo *n = c->next;
        free(c->name);
        if (c->parent) free(c->parent);
        if (c->ast) { /* AST освобождается отдельно */ }
        free_attr_list(c->attrs);
        free_method_list(c->methods);
        free_children_list(c->children);
        free(c);
        c = n;
    }
    ct->head = NULL;
    ct->count = 0;
}

/* ---- добавление класса из AST ---- */
ClassInfo *class_table_add_class(ClassTable *ct, ClassNode *cnode) {
    if (!ct || !cnode) return NULL;
    if (!cnode->name) return NULL;

    /* не добавляем дубликаты: если класс с таким именем уже есть — возвращаем существующий */
    ClassInfo *exist = class_table_find(ct, cnode->name);
    if (exist) {
        /* если ast ещё не привязан - привяжем */
        if (!exist->ast) exist->ast = cnode;
        return exist;
    }

    ClassInfo *ci = malloc(sizeof(ClassInfo));
    memset(ci, 0, sizeof(ClassInfo));
    ci->id = ++ct->count;
    ci->name = strdup_safe(cnode->name);
    ci->parent = cnode->parent ? strdup_safe(cnode->parent) : NULL;
    ci->ast = cnode;
    ci->parent_info = NULL;
    ci->children = NULL;
    ci->attrs = NULL;
    ci->methods = NULL;
    ci->visit_state = 0;
    ci->next = ct->head;
    ct->head = ci;
    return ci;
}

/* ---- поиск класса по имени ---- */
ClassInfo *class_table_find(ClassTable *ct, const char *name) {
    if (!ct || !name) return NULL;
    for (ClassInfo *c = ct->head; c; c = c->next) {
        if (strcmp(c->name, name) == 0) return c;
    }
    return NULL;
}

/* ---- добавить attr/method из FeatureNode ---- */
AttrInfo *class_add_attr_from_feature(ClassInfo *cls, FeatureNode *feat) {
    if (!cls || !feat) return NULL;
    if (feat->kind != FEATURE_ATTR) return NULL;

    AttrInfo *a = malloc(sizeof(AttrInfo));
    memset(a, 0, sizeof(AttrInfo));
    a->name = strdup_safe(feat->name);
    a->type = strdup_safe(feat->attr.type);
    a->ast = feat;
    a->next = cls->attrs;
    /* id: порядковый номер в списке */
    int id = 1;
    for (AttrInfo *it = cls->attrs; it; it = it->next) id++;
    a->id = id;
    cls->attrs = a;
    /* связать в AST */
    feat->attr_info = a;
    return a;
}

MethodInfo *methodinfo_create(const char *name, FormalList *formals, const char *return_type) {
    MethodInfo *m = malloc(sizeof(MethodInfo));
    memset(m, 0, sizeof(MethodInfo));
    m->name = strdup_safe(name ? name : "");
    int cnt = 0;
    for (FormalList *f = formals; f; f = f->next) cnt++;
    m->param_count = cnt;
    if (cnt > 0) {
        m->param_names = malloc(sizeof(char*) * cnt);
        m->param_types = malloc(sizeof(char*) * cnt);
        int i = 0;
        for (FormalList *f = formals; f; f = f->next, ++i) {
            m->param_names[i] = strdup_safe(f->node->name);
            m->param_types[i] = strdup_safe(f->node->type);
        }
    } else {
        m->param_names = NULL;
        m->param_types = NULL;
    }
    m->return_type = strdup_safe(return_type ? return_type : "Object");
    m->ast = NULL;
    m->next = NULL;
    return m;
}

MethodInfo *class_add_method_from_feature(ClassInfo *cls, FeatureNode *feat) {
    if (!cls || !feat) return NULL;
    if (feat->kind != FEATURE_METHOD) return NULL;

    MethodInfo *m = methodinfo_create(feat->name, feat->method.formals, feat->method.return_type);
    m->ast = feat;

    /* id: порядковый номер в списке */
    int id = 1;
    for (MethodInfo *it = cls->methods; it; it = it->next) id++;
    m->id = id;

    m->next = cls->methods;
    cls->methods = m;

    feat->method_info = m;
    return m;
}

/* ---- вспомог: добавить pointer child в parent ---- */
static void class_add_child(ClassInfo *parent, ClassInfo *child) {
    if (!parent || !child) return;
    ClassChild *ch = malloc(sizeof(ClassChild));
    ch->child = child;
    ch->next = parent->children;
    parent->children = ch;
}

/* ---- проверка на запрещённые родители ---- */
static bool is_forbidden_parent(const char *pname) {
    if (!pname) return false;
    /* В Cool обычно запрещено наследовать от Int, String, Bool, SELF_TYPE */
    if (strcmp(pname, "Int") == 0) return true;
    if (strcmp(pname, "String") == 0) return true;
    if (strcmp(pname, "Bool") == 0) return true;
    if (strcmp(pname, "SELF_TYPE") == 0) return true;
    return false;
}

/* ---- построение связей parent_info и проверка ошибок ---- */
static bool link_parents_and_check(ClassTable *ct) {
    bool ok = true;
    /* проверяем каждый класс: parent должен существовать (кроме Object) */
    for (ClassInfo *c = ct->head; c; c = c->next) {
        if (!c->parent) continue;
        if (is_forbidden_parent(c->parent)) {
            fprintf(stderr, "Semantic error: class %s cannot inherit from %s\n", c->name, c->parent);
            ok = false;
            continue;
        }
        ClassInfo *p = class_table_find(ct, c->parent);
        if (!p) {
            fprintf(stderr, "Semantic error: class %s inherits from undefined class %s\n", c->name, c->parent);
            ok = false;
            continue;
        }
        c->parent_info = p;
        class_add_child(p, c);
    }
    return ok;
}

/* ---- цикл-детекция DFS ---- */
static bool dfs_check_cycle(ClassInfo *c) {
    if (!c) return false;
    if (c->visit_state == 1) {
        /* нашли цикл */
        fprintf(stderr, "Semantic error: inheritance cycle detected at class %s\n", c->name);
        return true;
    }
    if (c->visit_state == 2) return false;
    c->visit_state = 1;
    if (c->parent_info) {
        if (dfs_check_cycle(c->parent_info)) return true;
    }
    c->visit_state = 2;
    return false;
}

/* ---- основная функция построения наследования ---- */
bool class_table_build_inheritance(ClassTable *ct) {
    if (!ct) return false;

    /* Проверка: наличие Object */
    ClassInfo *object = class_table_find(ct, "Object");
    if (!object) {
        fprintf(stderr, "Semantic error: class Object is not defined\n");
        return false;
    }

    /* Связываем parent_info и children; также проверяем существование родителя */
    if (!link_parents_and_check(ct)) {
        return false;
    }

    /* Детектируем циклы:
       пометим все как unvisited (0) — done в class_table_add_class уже 0 */
    for (ClassInfo *c = ct->head; c; c = c->next) c->visit_state = 0;

    for (ClassInfo *c = ct->head; c; c = c->next) {
        if (c->visit_state == 0) {
            if (dfs_check_cycle(c)) return false;
        }
    }
    return true;
}

/* ---- Поиск метода в локальном классе ---- */
static MethodInfo *class_find_method_local(ClassInfo *cls, const char *method_name) {
    if (!cls || !method_name) return NULL;
    for (MethodInfo *m = cls->methods; m; m = m->next) {
        if (strcmp(m->name, method_name) == 0) return m;
    }
    return NULL;
}

/* ---- Поиск метода по иерархии ---- */
MethodInfo *class_lookup_method(ClassTable *ct, const char *class_name, const char *method_name) {
    if (!ct || !class_name || !method_name) return NULL;
    ClassInfo *c = class_table_find(ct, class_name);
    while (c) {
        MethodInfo *m = class_find_method_local(c, method_name);
        if (m) return m;
        c = c->parent_info;
    }
    return NULL;
}

/* ---- is_subtype ---- */
bool is_subtype(ClassTable *ct, const char *child, const char *parent) {
    if (!ct || !child || !parent) return false;
    if (strcmp(child, parent) == 0) return true;
    ClassInfo *c = class_table_find(ct, child);
    while (c && c->parent_info) {
        if (strcmp(c->parent_info->name, parent) == 0) return true;
        c = c->parent_info;
    }
    return false;
}

/* ---- lub (наименьший общий предок) ---- */
char *lub(ClassTable *ct, const char *A, const char *B) {
    if (!ct) return NULL;
    if (!A) return strdup_safe(B ? B : "Object");
    if (!B) return strdup_safe(A ? A : "Object");
    if (strcmp(A, B) == 0) return strdup_safe(A);

    /* Соберём предков A в массив */
    int max = 1024;
    char **anc = malloc(sizeof(char*) * max);
    int na = 0;
    ClassInfo *c = class_table_find(ct, A);
    while (c) {
        anc[na++] = strdup_safe(c->name);
        if (!c->parent_info) break;
        c = c->parent_info;
        if (na >= max-1) break;
    }

    /* Идём по предкам B и ищем совпадение */
    c = class_table_find(ct, B);
    char *res = NULL;
    while (c) {
        for (int i = 0; i < na; ++i) {
            if (strcmp(c->name, anc[i]) == 0) {
                res = strdup_safe(c->name);
                break;
            }
        }
        if (res) break;
        if (!c->parent_info) break;
        c = c->parent_info;
    }

    for (int i = 0; i < na; ++i) free(anc[i]);
    free(anc);

    if (!res) res = strdup_safe("Object");
    return res;
}

/* ---- descriptor из MethodInfo ---- */
char *methodinfo_get_descriptor(MethodInfo *m) {
    if (!m) return strdup_safe("()V");
    /* Создать временный FormalList для make_method_descriptor */
    FormalList *head = NULL, *tail = NULL;
    for (int i = 0; i < m->param_count; ++i) {
        FormalNode *fn = malloc(sizeof(FormalNode));
        fn->id = -1;
        fn->name = strdup_safe(m->param_names[i]);
        fn->type = strdup_safe(m->param_types[i]);
        FormalList *fl = malloc(sizeof(FormalList));
        fl->node = fn;
        fl->next = NULL;
        if (!head) head = tail = fl;
        else { tail->next = fl; tail = fl; }
    }

    char *desc = make_method_descriptor(head, m->return_type);

    /* cleanup */
    FormalList *it = head;
    while (it) {
        FormalList *n = it->next;
        free(it->node->name);
        free(it->node->type);
        free(it->node);
        free(it);
        it = n;
    }
    return desc;
}

/* ---- Печать для отладки ---- */
void class_table_print(ClassTable *ct, FILE *out) {
    fprintf(out, "ClassTable (count=%d)\n", ct->count);
    for (ClassInfo *c = ct->head; c; c = c->next) {
        fprintf(out, " Class #%d: %s parent=%s\n", c->id, c->name, c->parent ? c->parent : "(null)");
        fprintf(out, "  Attrs:\n");
        for (AttrInfo *a = c->attrs; a; a = a->next) {
            fprintf(out, "   - %s : %s\n", a->name, a->type);
        }
        fprintf(out, "  Methods:\n");
        for (MethodInfo *m = c->methods; m; m = m->next) {
            fprintf(out, "   * %s (", m->name);
            for (int i = 0; i < m->param_count; ++i) {
                fprintf(out, "%s:%s", m->param_names[i], m->param_types[i]);
                if (i+1 < m->param_count) fprintf(out, ", ");
            }
            fprintf(out, ") : %s\n", m->return_type);
        }
    }
}
