//
// Created by Дмитрий on 18.01.2026.
//

#include "emit_fields.h"
#include "emit_utils.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * field_info {
 *   u2 access_flags;
 *   u2 name_index;
 *   u2 descriptor_index;
 *   u2 attributes_count;
 * }
 */

void emit_fields(FILE *out, ClassInfo *cls) {
    if (!out || !cls) return;

    /* ===============================
     * fields_count
     * =============================== */

    int count = 0;
    for (AttrInfo *a = cls->attrs; a; a = a->next)
        count++;

    write_u2(out, (uint16_t)count);

    /* ===============================
     * field_info[]
     * =============================== */

    for (AttrInfo *a = cls->attrs; a; a = a->next) {

        /*
         * access_flags
         * private field
         */
        write_u2(out, 0x0002); /* ACC_PRIVATE */

        /*
         * name_index (CONSTANT_Utf8)
         */
        if (a->name_utf8_index <= 0) {
            fprintf(stderr,
                "emit_fields: attribute %s in class %s has no name_utf8_index\n",
                a->name, cls->name);
            exit(1);
        }
        write_u2(out, (uint16_t)a->name_utf8_index);

        /*
         * descriptor_index (CONSTANT_Utf8)
         */
        if (a->descriptor_utf8_index <= 0) {
            fprintf(stderr,
                "emit_fields: attribute %s in class %s has no descriptor_utf8_index\n",
                a->name, cls->name);
            exit(1);
        }
        write_u2(out, (uint16_t)a->descriptor_utf8_index);

        /*
         * attributes_count
         * (пока без ConstantValue)
         */
        write_u2(out, 0);
    }
}

