#include "class_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Вспомогательные */
static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

/* Инициализация таблицы */
void class_table_init(ClassTable *ct) {
    ct->head = NULL;
    ct->count = 0;
}

/* Очистка вспомогательных структур */
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

/* Освобождение таблицы */
void class_table_free(ClassTable *ct) {
    ClassInfo *c = ct->head;
    while (c) {
        ClassInfo *n = c->next;
        free(c->name);
        if (c->parent) free(c->parent);
        free_attr_list(c->attrs);
        free_method_list(c->methods);
        free(c);
        c = n;
    }
    ct->head = NULL;
    ct->count = 0;
}

/* Добавление класса */
ClassInfo *class_table_add(ClassTable *ct, const char *name, const char *parent, ClassNode *ast) {
    if (!ct || !name) return NULL;

    /* Если уже есть — вернуть существующий */
    ClassInfo *exist = class_table_find(ct, name);
    if (exist) {
        /* возможно обновить parent/ast если нужно */
        if (!exist->parent && parent) exist->parent = strdup_safe(parent);
        if (!exist->ast && ast) exist->ast = ast;
        return exist;
    }

    ClassInfo *c = malloc(sizeof(ClassInfo));
    memset(c, 0, sizeof(ClassInfo));
    c->id = ++ct->count;
    c->name = strdup_safe(name);
    c->parent = parent ? strdup_safe(parent) : NULL;
    c->ast = ast;
    c->attrs = NULL;
    c->methods = NULL;
    c->next = ct->head;
    ct->head = c;
    return c;
}

/* Поиск класса по имени */
ClassInfo *class_table_find(ClassTable *ct, const char *name) {
    if (!ct || !name) return NULL;
    for (ClassInfo *c = ct->head; c; c = c->next) {
        if (strcmp(c->name, name) == 0) return c;
    }
    return NULL;
}

/* Создание MethodInfo из formals (копируем имена и типы) */
MethodInfo *methodinfo_create(const char *name, FormalList *formals, const char *return_type) {
    MethodInfo *m = malloc(sizeof(MethodInfo));
    memset(m, 0, sizeof(MethodInfo));
    m->name = strdup_safe(name ? name : "");
    /* подсчёт формалов */
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

/* Добавление метода из FeatureNode */
MethodInfo *class_add_method_from_feature(ClassInfo *cls, FeatureNode *feat) {
    if (!cls || !feat) return NULL;
    if (feat->kind != FEATURE_METHOD) return NULL;

    /* Проверка дубликатов по имени и arity — простая */
    for (MethodInfo *mm = cls->methods; mm; mm = mm->next) {
        if (strcmp(mm->name, feat->name) == 0 && mm->param_count == 0 /* arity compare later */) {
            /* Не проверяем сигнатуру полностью здесь — семантика должна это сделать.
               Всё же, если нужно, можно более строго сравнивать mm->param_types.. */
        }
    }

    MethodInfo *m = methodinfo_create(feat->name, feat->method.formals, feat->method.return_type);
    m->ast = feat;
    /* id: поставим текущее количество методов +1 (плюс можно пересчитать) */
    int id = 1;
    for (MethodInfo *it = cls->methods; it; it = it->next) id++;
    m->id = id;
    /* вставляем в начало списка */
    m->next = cls->methods;
    cls->methods = m;
    return m;
}

/* Добавление атрибута из FeatureNode */
AttrInfo *class_add_attr_from_feature(ClassInfo *cls, FeatureNode *feat) {
    if (!cls || !feat) return NULL;
    if (feat->kind != FEATURE_ATTR) return NULL;

    AttrInfo *a = malloc(sizeof(AttrInfo));
    memset(a, 0, sizeof(AttrInfo));
    a->name = strdup_safe(feat->name);
    a->type = strdup_safe(feat->attr.type);
    a->ast = feat;
    int id = 1;
    for (AttrInfo *it = cls->attrs; it; it = it->next) id++;
    a->id = id;
    a->next = cls->attrs;
    cls->attrs = a;
    return a;
}

/* Поиск метода в классе (только в самом классе) */
static MethodInfo *class_find_method_local(ClassInfo *cls, const char *method_name) {
    if (!cls || !method_name) return NULL;
    for (MethodInfo *m = cls->methods; m; m = m->next) {
        if (strcmp(m->name, method_name) == 0) return m;
    }
    return NULL;
}

/* Поиск метода в иерархии: ищем в class_name, если нет — идём по родителю и т.д. */
MethodInfo *class_lookup_method(ClassTable *ct, const char *class_name, const char *method_name) {
    if (!ct || !class_name || !method_name) return NULL;
    ClassInfo *c = class_table_find(ct, class_name);
    while (c) {
        MethodInfo *m = class_find_method_local(c, method_name);
        if (m) return m;
        if (!c->parent) break;
        c = class_table_find(ct, c->parent);
    }
    return NULL;
}

/* is_subtype: с учётом равенства */
bool is_subtype(ClassTable *ct, const char *child, const char *parent) {
    if (!ct || !child || !parent) return false;
    if (strcmp(child, parent) == 0) return true;
    ClassInfo *c = class_table_find(ct, child);
    while (c && c->parent) {
        if (strcmp(c->parent, parent) == 0) return true;
        c = class_table_find(ct, c->parent);
    }
    return false;
}

/* lub: простая реализация: поднимаем по цепочке предков и ищем пересечение */
char *lub(ClassTable *ct, const char *A, const char *B) {
    if (!ct) return NULL;
    if (!A) return strdup_safe(B ? B : "Object");
    if (!B) return strdup_safe(A ? A : "Object");
    if (strcmp(A, B) == 0) return strdup_safe(A);

    /* Соберём список предков A */
    int max_depth = 1024;
    char **ancA = malloc(sizeof(char*) * max_depth);
    int na = 0;
    char *cur = strdup_safe(A);
    while (cur) {
        ancA[na++] = strdup_safe(cur);
        ClassInfo *ci = class_table_find(ct, cur);
        if (!ci || !ci->parent) break;
        char *p = ci->parent;
        free(cur);
        cur = strdup_safe(p);
        if (na >= max_depth-1) break;
    }
    if (cur) free(cur);

    /* Пройдём предков B и найдём первый, который в ancA */
    char *res = NULL;
    char *curB = strdup_safe(B);
    while (curB) {
        for (int i = 0; i < na; ++i) {
            if (strcmp(curB, ancA[i]) == 0) {
                res = strdup_safe(curB);
                break;
            }
        }
        if (res) break;
        ClassInfo *ci = class_table_find(ct, curB);
        if (!ci || !ci->parent) break;
        char *p = ci->parent;
        free(curB);
        curB = strdup_safe(p);
    }
    if (curB) free(curB);

    for (int i = 0; i < na; ++i) free(ancA[i]);
    free(ancA);

    if (!res) res = strdup_safe("Object");
    return res;
}

/* Получение JVM-дескриптора метода из MethodInfo */
char *methodinfo_get_descriptor(MethodInfo *m) {
    if (!m) return strdup_safe("()V");
    /* Сформировать временный FormalList-подобный интерфейс не нужно — используем make_method_descriptor,
       но он ожидает FormalList. Для простоты создадим временный список Formals. */
    FormalList *head = NULL;
    FormalList *tail = NULL;
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

    /* cleanup временных формалов */
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

/* Печать таблицы для отладки */
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
