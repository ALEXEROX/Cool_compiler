//
// Created by Дмитрий on 18.01.2026.
//

#include "emit_code_attr.h"
#include "emit_utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "emit_bytecode.h"
#include "emit_expr.h"

void emit_code_attribute(FILE *out, MethodInfo *m, int code_utf8_index, ClassInfo *cls, ConstantTable* ct)
{
    if (!out || !m) return;

    /* attribute_name_index -> "Code" */
    write_u2(out, (uint16_t)code_utf8_index);

    /* ---- Bytecode ---- */
    BytecodeBuffer bc;
    bc_init(&bc);

    /* Генерация тела метода */
    if (m->ast && m->ast->kind == FEATURE_METHOD && m->ast->method.body) {
        emit_expr(&bc, m->ast->method.body, cls, ct);
    }


    /* Добавляем правильный return */
    if (strcmp(m->return_type, "Int") == 0 || strcmp(m->return_type, "Bool") == 0) {
        bc_emit_u1(&bc, 0xAC); // ireturn
    } else if (m->return_type) {
        bc_emit_u1(&bc, 0xB0); // areturn
    } else {
        bc_emit_u1(&bc, 0xB1); // return (void)
    }

    uint32_t code_length = bc.size;



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

    /* max_stack: пока временно, можно анализировать позже */
    write_u2(out, 32);

    /* max_locals: this + параметры метода */
    write_u2(out, 32);

    /* code_length */
    write_u4(out, code_length);

    /* code[] */
    fwrite(bc.data, 1, bc.size, out);

    /* exception_table_length */
    write_u2(out, 0);

    /* attributes_count */
    write_u2(out, 0);

    bc_free(&bc);
}

void emit_init_code_attribute(FILE *out, ClassInfo *cls)
{
    /* attribute_name_index -> "Code" */
    write_u2(out, (uint16_t)1);

    /*
     * bytecode:
     *
     * Object:
     *   aload_0
     *   return
     *
     * Other:
     *   aload_0
     *   invokespecial super.<init>()V
     *   return
     */

    uint8_t code[8];
    int pc = 0;

    /* aload_0 */
    code[pc++] = 0x2A;

    if (cls->parent_info) {
        /* invokespecial */
        code[pc++] = 0xB7;
        code[pc++] = (cls->parent_info->class_cp_index >> 8) & 0xFF;
        code[pc++] = (cls->parent_info->init_cp_index) & 0xFF;
    }

    /* return */
    code[pc++] = 0xB1;

    uint32_t code_length = pc;

    /*
     * attribute_length =
     * max_stack(2)
     * + max_locals(2)
     * + code_length(4)
     * + code[]
     * + exception_table_length(2)
     * + attributes_count(2)
     */
    uint32_t attribute_length =
            2 + 2 + 4 + code_length + 2 + 2;

    write_u4(out, attribute_length);

    /* max_stack */
    write_u2(out, 1);

    /* max_locals: только this */
    write_u2(out, 1);

    /* code_length */
    write_u4(out, code_length);

    /* code[] */
    fwrite(code, 1, code_length, out);

    /* exception_table_length */
    write_u2(out, 0);

    /* attributes_count */
    write_u2(out, 0);
}


