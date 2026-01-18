//
// Created by Дмитрий on 18.01.2026.
//

#include "emit_class_info.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * access_flags:
 *  ACC_PUBLIC (0x0001)
 *  ACC_SUPER  (0x0020)
 */
#define ACC_PUBLIC 0x0001
#define ACC_SUPER  0x0020

void emit_class_info(FILE *out, ClassInfo *cls) {
    if (!out || !cls) return;

    /* ===============================
     * access_flags
     * =============================== */
    write_u2(out, ACC_PUBLIC | ACC_SUPER);

    /* ===============================
     * this_class
     * =============================== */
    if (cls->class_cp_index <= 0) {
        fprintf(stderr,
                "emit_class_info: class %s has invalid class_cp_index\n",
                cls->name);
        exit(1);
    }
    write_u2(out, (uint16_t)cls->class_cp_index);

    /* ===============================
     * super_class
     * =============================== */
    if (cls->parent_info) {
        if (cls->parent_info->class_cp_index <= 0) {
            fprintf(stderr,
                    "emit_class_info: parent class %s has invalid class_cp_index\n",
                    cls->parent_info->name);
            exit(1);
        }
        write_u2(out, (uint16_t)cls->parent_info->class_cp_index);
    } else {
        /* java/lang/Object */
        write_u2(out, 0);
    }

    /* ===============================
     * interfaces_count
     * =============================== */
    write_u2(out, 0);
}
