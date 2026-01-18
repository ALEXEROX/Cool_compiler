//
// Created by Дмитрий on 17.01.2026.
//

#include "emit_constant_pool.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* ===============================
 * Constant pool emission
 * =============================== */

void emit_constant_pool(FILE *out, ConstantTable *cp) {
    if (!out || !cp) return;

    /*
     * JVM требует:
     * constant_pool_count = last_index + 1
     * индекс 0 зарезервирован
     */
    write_u2(out, cp->count + 1);

    for (ConstantEntry *e = cp->head; e; e = e->next) {

        write_u1(out, (uint8_t)e->kind);

        switch (e->kind) {

            case CONST_Utf8: {
                uint16_t len = (uint16_t)strlen(e->utf8);
                write_u2(out, len);
                fwrite(e->utf8, 1, len, out);
                break;
            }

            case CONST_Integer:
                write_u4(out, (uint32_t)e->int_value);
                break;

            case CONST_String:
            case CONST_Class:
                write_u2(out, (uint16_t)e->ref_utf8_index);
                break;

            case CONST_NameAndType:
                write_u2(out, (uint16_t)e->name_index);
                write_u2(out, (uint16_t)e->type_index);
                break;

            case CONST_Fieldref:
            case CONST_Methodref:
                write_u2(out, (uint16_t)e->class_index);
                write_u2(out, (uint16_t)e->name_and_type_index);
                break;

            default:
                fprintf(stderr,
                        "emit_constant_pool: unknown constant kind %d (cp index %d)\n",
                        e->kind, e->index);
                exit(1);
        }
    }
}
