#include "object_env.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Вспомогательная безопасная strdup */
static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

/* Инициализация */
void object_env_init(ObjectEnv *env) {
    if (!env) return;
    env->top = NULL;
    env->next_local_index = 0;
}

/* Освобождение всех фреймов и биндингов */
void object_env_free(ObjectEnv *env) {
    if (!env) return;
    while (env->top) {
        object_env_exit_scope(env);
    }
    env->next_local_index = 0;
}

/* Вход в новую область видимости */
void object_env_enter_scope(ObjectEnv *env) {
    if (!env) return;
    ScopeFrame *f = malloc(sizeof(ScopeFrame));
    f->vars = NULL;
    f->next = env->top;
    env->top = f;
}

/* Вспомог: освободить список VarBinding */
static void free_var_list(VarBinding *v) {
    while (v) {
        VarBinding *n = v->next;
        if (v->name) free(v->name);
        if (v->type) free(v->type);
        free(v);
        v = n;
    }
}

/* Выход из текущей области видимости */
void object_env_exit_scope(ObjectEnv *env) {
    if (!env || !env->top) return;
    ScopeFrame *f = env->top;
    env->top = f->next;
    /* освобождаем биндинги */
    free_var_list(f->vars);
    free(f);
    /* Примечание: индексы локалей не «откатываются» — это нормальное поведение для JVM,
       т.к. локальные индексы уникальны внутри метода; если нужен реальный откат,
       следует хранить стек checkpoint'ов next_local_index. */
}

/* Добавить переменную в текущий фрейм */
bool object_env_add(ObjectEnv *env, const char *name, const char *type, int *out_index) {
    if (!env || !name) return false;
    if (!env->top) object_env_enter_scope(env);

    /* Проверяем существование в текущем фрейме (shadowing в рамках одного фрейма запрещено) */
    for (VarBinding *v = env->top->vars; v; v = v->next) {
        if (strcmp(v->name, name) == 0) return false; /* duplicate in same scope */
    }

    VarBinding *vb = malloc(sizeof(VarBinding));
    vb->name = strdup_safe(name);
    vb->type = type ? strdup_safe(type) : NULL;
    vb->index = env->next_local_index++;
    vb->next = env->top->vars;
    env->top->vars = vb;

    if (out_index) *out_index = vb->index;
    return true;
}

/* Поиск переменной по имени сверху вниз */
VarBinding *object_env_lookup(ObjectEnv *env, const char *name) {
    if (!env || !name) return NULL;
    for (ScopeFrame *f = env->top; f; f = f->next) {
        for (VarBinding *v = f->vars; v; v = v->next) {
            if (strcmp(v->name, name) == 0) return v;
        }
    }
    return NULL;
}

int object_env_get_next_index(ObjectEnv *env) {
    if (!env) return 0;
    return env->next_local_index;
}

/* Вход в метод: добавляем 'this' и параметры */
int object_env_enter_method(ObjectEnv *env, const char *class_name, FormalList *params) {
    if (!env) return 0;
    /* Откроем новый фрейм */
    object_env_enter_scope(env);
    int added = 0;

    /* 'this' → индекс 0 */
    if (class_name) {
        /* Если next_local_index не 0 (например, повторный вызов), принудительно выставим 0 */
        /* Но стандартный путь — env новый, next_local_index==0 */
        if (env->next_local_index != 0) {
            /* Если уже была нумерация — не меняем существующие индексы, просто продолжаем.
               Обычно object_env_enter_method вызывается на чистом env или после object_env_free. */
        }
        int idx;
        /* добавляем 'this' с type = class_name */
        object_env_add(env, "this", class_name, &idx);
        (void)idx;
        added++;
    }

    /* параметры — в порядке списка (первый парамет получает индекс 1) */
    for (FormalList *f = params; f; f = f->next) {
        const char *pname = f->node->name;
        const char *ptype = f->node->type;
        int idx;
        /* Если имя совпадает с 'this' — это ошибка семантики, но object_env просто вернёт false */
        bool ok = object_env_add(env, pname, ptype, &idx);
        (void)ok;
        added++;
    }

    return added;
}

/* Печать для отладки */
void object_env_print(ObjectEnv *env, FILE *out) {
    if (!env) return;
    fprintf(out, "ObjectEnv: next_local_index=%d\n", env->next_local_index);
    int frame_no = 0;
    for (ScopeFrame *f = env->top; f; f = f->next) {
        fprintf(out, " Frame #%d:\n", frame_no++);
        for (VarBinding *v = f->vars; v; v = v->next) {
            fprintf(out, "   name=%s type=%s index=%d\n", v->name, v->type ? v->type : "(null)", v->index);
        }
    }
}
