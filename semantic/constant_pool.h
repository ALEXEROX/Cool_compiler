#pragma once
#include <stdio.h>


/* ============================================================
 * Типы констант (официальные номера JVM)
 * ============================================================ */
typedef enum {
    CONST_Utf8        = 1,
    CONST_Integer     = 3,
    CONST_String      = 8,
    CONST_Class       = 7,
    CONST_Fieldref    = 9,
    CONST_Methodref   = 10,
    CONST_NameAndType = 12
} ConstantKind;

/* ============================================================
 * Структура записи в constant pool
 * ============================================================ */
typedef struct ConstantEntry {
    int index;                 // JVM: нумерация с 1
    ConstantKind kind;

    char *utf8;                // для UTF8
    int int_value;             // для Integer

    int ref_utf8_index;        // для String и Class

    int name_index;            // для NameAndType
    int type_index;            // для NameAndType

    int class_index;           // для Fieldref/Methodref
    int name_and_type_index;   // для Fieldref/Methodref

    struct ConstantEntry *next;
} ConstantEntry;

/* ============================================================
 * Таблица констант
 * ============================================================ */
typedef struct {
    ConstantEntry *head;
    ConstantEntry *tail;
    int count;                 // количество записей (индекс последней)
} ConstantTable;

/* ============================================================
 * API
 * ============================================================ */
void const_table_init(ConstantTable *tbl);
void const_table_free(ConstantTable *tbl);
void const_table_print(ConstantTable *tbl, FILE *out);

/* Добавление типизированных констант */
int const_add_utf8(ConstantTable *tbl, const char *s);
int const_add_integer(ConstantTable *tbl, int value);
int const_add_string(ConstantTable *tbl, const char *s);
int const_add_class(ConstantTable *tbl, const char *full_class_name_utf8);

int const_add_name_and_type(ConstantTable *tbl,
                            const char *name_utf8,
                            const char *descriptor_utf8);

int const_add_fieldref(ConstantTable *tbl,
                       int class_index,
                       int name_and_type_index);

int const_add_methodref(ConstantTable *tbl,
                        int class_index,
                        int name_and_type_index);
