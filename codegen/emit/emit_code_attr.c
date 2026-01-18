//
// Created by Дмитрий on 18.01.2026.
//

#include "emit_code_attr.h"
#include "emit_utils.h"
#include <stdint.h>
#include <stdio.h>

void emit_code_attribute(FILE *out, MethodInfo *m, int code_utf8_index)
{
    if (!out || !m) return;

    /* attribute_name_index -> "Code" */
    write_u2(out, (uint16_t)code_utf8_index);

    /* ---- bytecode ---- */
    uint8_t code_buf[16];
    int pc = 0;

    /* aconst_null */
    code_buf[pc++] = 0x01;
    /* areturn */
    code_buf[pc++] = 0xB0;

    uint32_t code_length = pc;

    /*
     * attribute_length =
     * max_stack(2) + max_locals(2)
     * + code_length field(4)
     * + code
     * + exception_table_length(2)
     * + attributes_count(2)
     */
    uint32_t attribute_length =
            2 + 2 + 4 + code_length + 2 + 2;

    write_u4(out, attribute_length);

    /* max_stack */
    write_u2(out, 1);

    /* max_locals
     * 1 = this
     * + параметры
     */
    write_u2(out, 1 + m->param_count);

    /* code_length */
    write_u4(out, code_length);

    /* code[] */
    for (uint32_t i = 0; i < code_length; i++) {
        write_u1(out, code_buf[i]);
    }

    /* exception_table_length */
    write_u2(out, 0);

    /* attributes_count */
    write_u2(out, 0);
}

