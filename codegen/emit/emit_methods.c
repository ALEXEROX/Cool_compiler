//
// Created by Дмитрий on 18.01.2026.
//
#include "emit_methods.h"

#include <stdint.h>
#include <stdio.h>

#include "emit_utils.h"
#include "../../semantic/class_table.h"

/* ACC_PUBLIC */
#define ACC_PUBLIC 0x0001

/* индекс UTF8 "Code" в constant pool */
#define CODE_ATTR_NAME_INDEX 1
/* -------------------------------------------------
 * Methods emission
 * ------------------------------------------------- */
void emit_methods(FILE *out, ClassInfo *cls) {
    if (!out || !cls) {
        write_u2(out, 0);
        return;
    }

    /* 1. считаем методы */
    int count = 0;
    for (MethodInfo *m = cls->methods; m; m = m->next) {
        count++;
    }

    write_u2(out, (uint16_t)count);

    /* 2. пишем каждый method_info */
    for (MethodInfo *m = cls->methods; m; m = m->next) {

        /* access_flags */
        write_u2(out, ACC_PUBLIC);

        /* name_index */
        write_u2(out, (uint16_t)m->name_utf8_index);

        /* descriptor_index */
        write_u2(out, (uint16_t)m->descriptor_utf8_index);

        /* attributes_count = 1 (Code) */
        write_u2(out, 1);

        emit_code_attribute(out, m, CODE_ATTR_NAME_INDEX);
    }
}
