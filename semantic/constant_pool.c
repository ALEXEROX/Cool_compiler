#include "constant_pool.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "descriptor.h"
#include "../Nodes/ast.h"

/* ==========================================
 * Вспомогательные функции
 * ========================================== */
static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

static int const_table_add_entry(ConstantTable *tbl, ConstantKind kind) {
    ConstantEntry *e = malloc(sizeof(ConstantEntry));
    memset(e, 0, sizeof(ConstantEntry));

    e->kind = kind;
    e->index = ++tbl->count;
    e->next = NULL;

    if (!tbl->head) {
        tbl->head = tbl->tail = e;
    } else {
        tbl->tail->next = e;
        tbl->tail = e;
    }

    return e->index;
}

void const_table_init(ConstantTable *tbl) {
    tbl->head = tbl->tail = NULL;
    tbl->count = 0;

    const_add_utf8(tbl,"Code");
    const_add_class(tbl,"java/lang/Object");
    const_add_utf8(tbl,"<init>");
    const_add_utf8(tbl,"()V");
    int id = const_add_name_and_type(tbl,"<init>","()V");
    const_add_methodref(tbl,3,id);
}

/* ==========================================
 * Поиск UTF8
 * ========================================== */
static int const_find_utf8(ConstantTable *tbl, const char *s) {
    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_Utf8 && e->utf8 && strcmp(e->utf8, s) == 0)
            return e->index;
    }
    return 0;
}

int const_add_utf8(ConstantTable *tbl, const char *s) {
    if (!s) return 0;
    int found = const_find_utf8(tbl, s);
    if (found) return found;

    int idx = const_table_add_entry(tbl, CONST_Utf8);
    ConstantEntry *e = tbl->tail;
    e->utf8 = strdup_safe(s);
    return idx;
}

/* ==========================================
 * Integer
 * ========================================== */
static int const_find_integer(ConstantTable *tbl, int value) {
    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_Integer && e->int_value == value)
            return e->index;
    }
    return 0;
}

int const_add_integer(ConstantTable *tbl, int value) {
    int found = const_find_integer(tbl, value);
    if (found) return found;

    int idx = const_table_add_entry(tbl, CONST_Integer);
    ConstantEntry *e = tbl->tail;
    e->int_value = value;
    return idx;
}

/* ==========================================
 * String -> UTF8
 * ========================================== */
int const_add_string(ConstantTable *tbl, const char *s) {
    if (!s) return 0;

    int utf_idx = const_add_utf8(tbl, s);

    // поиск существующей записи String для этого utf8
    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_String && e->ref_utf8_index == utf_idx)
            return e->index;
    }

    int idx = const_table_add_entry(tbl, CONST_String);
    tbl->tail->ref_utf8_index = utf_idx;
    return idx;
}

/* ==========================================
 * Class -> UTF8
 * ========================================== */
int const_add_class(ConstantTable *tbl, const char *full_class_name_utf8) {
    int utf_idx = const_add_utf8(tbl, full_class_name_utf8);

    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_Class && e->ref_utf8_index == utf_idx)
            return e->index;
    }

    int idx = const_table_add_entry(tbl, CONST_Class);
    tbl->tail->ref_utf8_index = utf_idx;
    return idx;
}

/* ==========================================
 * NameAndType
 * ========================================== */
int const_add_name_and_type(ConstantTable *tbl,
                            const char *name_utf8,
                            const char *descriptor_utf8)
{
    int name_idx = const_add_utf8(tbl, name_utf8);
    int type_idx = const_add_utf8(tbl, descriptor_utf8);

    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_NameAndType &&
            e->name_index == name_idx &&
            e->type_index == type_idx)
            return e->index;
    }

    int idx = const_table_add_entry(tbl, CONST_NameAndType);
    ConstantEntry *e = tbl->tail;
    e->name_index = name_idx;
    e->type_index = type_idx;
    return idx;
}

/* ==========================================
 * Fieldref
 * ========================================== */
int const_add_fieldref(ConstantTable *tbl,
                       int class_index,
                       int name_and_type_index)
{
    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_Fieldref &&
            e->class_index == class_index &&
            e->name_and_type_index == name_and_type_index)
            return e->index;
    }

    int idx = const_table_add_entry(tbl, CONST_Fieldref);
    ConstantEntry *e = tbl->tail;
    e->class_index = class_index;
    e->name_and_type_index = name_and_type_index;
    return idx;
}

/* ==========================================
 * Methodref
 * ========================================== */
int const_add_methodref(ConstantTable *tbl,
                        int class_index,
                        int name_and_type_index)
{
    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        if (e->kind == CONST_Methodref &&
            e->class_index == class_index &&
            e->name_and_type_index == name_and_type_index)
            return e->index;
    }

    int idx = const_table_add_entry(tbl, CONST_Methodref);
    ConstantEntry *e = tbl->tail;
    e->class_index = class_index;
    e->name_and_type_index = name_and_type_index;
    return idx;
}

/* ==========================================
 * Печать
 * ========================================== */
void const_table_print(ConstantTable *tbl, FILE *out) {
    fprintf(out, "Constant pool (count=%d):\n", tbl->count);
    for (ConstantEntry *e = tbl->head; e; e = e->next) {
        fprintf(out, " #%d  kind=%d ", e->index, (int)e->kind);

        switch (e->kind) {
            case CONST_Utf8:
                fprintf(out, "UTF8 \"%s\"\n", e->utf8);
                break;

            case CONST_Integer:
                fprintf(out, "Integer %d\n", e->int_value);
                break;

            case CONST_String:
                fprintf(out, "String -> utf8=%d\n", e->ref_utf8_index);
                break;

            case CONST_Class:
                fprintf(out, "Class -> name_utf8=%d\n", e->ref_utf8_index);
                break;

            case CONST_NameAndType:
                fprintf(out, "NameAndType -> name=%d type=%d\n",
                        e->name_index, e->type_index);
                break;

            case CONST_Fieldref:
                fprintf(out, "Fieldref -> class=%d name_type=%d\n",
                        e->class_index, e->name_and_type_index);
                break;

            case CONST_Methodref:
                fprintf(out, "Methodref -> class=%d name_type=%d\n",
                        e->class_index, e->name_and_type_index);
                break;
        }
    }
}

/* ==========================================
 * Освобождение памяти
 * ========================================== */
void const_table_free(ConstantTable *tbl) {
    ConstantEntry *e = tbl->head;
    while (e) {
        ConstantEntry *n = e->next;
        if (e->utf8) free(e->utf8);
        free(e);
        e = n;
    }
    tbl->head = tbl->tail = NULL;
    tbl->count = 0;
}
