#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "../Nodes/ast.h"


/* Запись о локальной переменной/биндинге */
typedef struct VarBinding {
    char *name;               /* имя переменной */
    char *type;               /* тип в формате Cool (строка) */
    int index;                /* номер в таблице локальных переменных (JVM local index) */
    struct VarBinding *next;  /* следующий биндинг в текущем фрейме */
} VarBinding;

/* Один фрейм (область видимости) */
typedef struct ScopeFrame {
    VarBinding *vars;         /* список переменных в этом фрейме */
    struct ScopeFrame *next;  /* указатель на предыдущий фрейм (стек) */
} ScopeFrame;

/* Окружение (стек областей видимости) */
typedef struct {
    ScopeFrame *top;          /* верхний фрейм стека */
    int next_local_index;     /* следующий свободный индекс локальной переменной (будет присвоен новой переменной) */
} ObjectEnv;

/* --- Инициализация / очистка --- */
void object_env_init(ObjectEnv *env);
void object_env_free(ObjectEnv *env); /* освобождает все фреймы и биндинги */

/* --- Работа со стеком областей видимости --- */
void object_env_enter_scope(ObjectEnv *env); /* новый фрейм (например, при входе в блок или let) */
void object_env_exit_scope(ObjectEnv *env);  /* выход из текущего фрейма и освобождение его биндингов */

/* --- Работа с переменными --- */
/*
 * Добавляет переменную в текущую (верхнюю) область видимости.
 * Если уже есть переменная с таким именем в текущем фрейме — возвращает false.
 * При успешном добавлении присваивает переменной индекс (out_index != NULL → туда записывается индекс).
 */
bool object_env_add(ObjectEnv *env, const char *name, const char *type, int *out_index);

/*
 * Ищет переменную по имени, начиная с верхнего фрейма вниз.
 * Возвращает указатель на VarBinding (внутренний объект) или NULL, если не найдено.
 * Не освобождай возвращённый указатель — он управляется окружением.
 */
VarBinding *object_env_lookup(ObjectEnv *env, const char *name);

/* Возвращает текущий следующий свободный индекс (не изменяет состояние) */
int object_env_get_next_index(ObjectEnv *env);

/* --- Утилиты для работы с методом --- */
/*
 * Инициализирует окружение для метода:
 *  - создаёт новый фрейм,
 *  - добавляет implicit 'this' с index 0 и type = class_name (если class_name != NULL),
 *  - добавляет параметры из FormalList в порядке списка с индексами 1..n,
 *  - устанавливает next_local_index = param_count + 1.
 *
 * Возвращает количество добавленных переменных (including 'this' if added).
 */
int object_env_enter_method(ObjectEnv *env, const char *class_name, FormalList *params);

/*
 * Простой принт текущего окружения в поток out (для отладки).
 */
void object_env_print(ObjectEnv *env, FILE *out);
