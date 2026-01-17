//
// Created by Дмитрий on 17.01.2026.
//

#include "emit_class_header.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* ===============================
 * Big-endian writers
 * =============================== */

static void write_u1(FILE *out, uint8_t v) {
    fputc(v, out);
}

static void write_u2(FILE *out, uint16_t v) {
    fputc((v >> 8) & 0xFF, out);
    fputc(v & 0xFF, out);
}

static void write_u4(FILE *out, uint32_t v) {
    fputc((v >> 24) & 0xFF, out);
    fputc((v >> 16) & 0xFF, out);
    fputc((v >> 8) & 0xFF, out);
    fputc(v & 0xFF, out);
}

/* ===============================
 * Class header emission
 * =============================== */

void emit_class_header(FILE *out, ClassInfo *cls) {
    if (!out || !cls) return;

    /* magic */
    write_u4(out, 0xCAFEBABE);

    /* version: Java 8 */
    write_u2(out, 0);     /* minor */
    write_u2(out, 52);    /* major */

    /*
     * access_flags:
     * ACC_PUBLIC | ACC_SUPER
     */
    write_u2(out, 0x0021);

    /*
     * this_class:
     * индекс CONSTANT_Class в constant pool
     */
    if (cls->class_cp_index <= 0) {
        fprintf(stderr,
                "emit_class_header: class %s has invalid class_cp_index\n",
                cls->name);
        exit(1);
    }
    write_u2(out, (uint16_t)cls->class_cp_index);

    /*
     * super_class:
     * для Object = 0
     * иначе индекс Class родителя
     */
    if (!cls->parent_info) {
        write_u2(out, 0);
    } else {
        if (cls->parent_info->class_cp_index <= 0) {
            fprintf(stderr,
                    "emit_class_header: parent class %s has invalid class_cp_index\n",
                    cls->parent_info->name);
            exit(1);
        }
        write_u2(out, (uint16_t)cls->parent_info->class_cp_index);
    }
}
