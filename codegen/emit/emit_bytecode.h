//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdint.h>
#include <stdio.h>

/* ===============================
 * Буфер байткода
 * =============================== */
typedef struct {
    uint8_t *data;
    int size;
    int capacity;
} BytecodeBuffer;

/* buffer */
void bc_init(BytecodeBuffer *bc);
void bc_free(BytecodeBuffer *bc);
void bc_emit_u1(BytecodeBuffer *bc, uint8_t v);
void bc_emit_u2(BytecodeBuffer *bc, uint16_t v);
void bc_emit_u4(BytecodeBuffer *bc, uint32_t v);

/* ===============================
 * JVM instructions
 * =============================== */

/* load / store */
void emit_iload(BytecodeBuffer *bc, int index);
void emit_aload(BytecodeBuffer *bc, int index);
void emit_istore(BytecodeBuffer *bc, int index);
void emit_astore(BytecodeBuffer *bc, int index);

void emit_aload_0(BytecodeBuffer *bc);

/* constants */
void emit_iconst(BytecodeBuffer *bc, int value);
void emit_ldc(BytecodeBuffer *bc, int cp_index);

/* stack */
void emit_dup(BytecodeBuffer *bc);
void emit_pop(BytecodeBuffer *bc);
void emit_swap(BytecodeBuffer *bc);

/* arithmetic */
void emit_iadd(BytecodeBuffer *bc);
void emit_isub(BytecodeBuffer *bc);
void emit_imul(BytecodeBuffer *bc);
void emit_idiv(BytecodeBuffer *bc);

/* fields */
void emit_getfield(BytecodeBuffer *bc, int fieldref_index);
void emit_putfield(BytecodeBuffer *bc, int fieldref_index);

/* calls */
void emit_invokevirtual(BytecodeBuffer *bc, int methodref_index);
void emit_invokespecial(BytecodeBuffer *bc, int methodref_index);

/* object */
void emit_new(BytecodeBuffer *bc, int class_index);
void emit_checkcast(BytecodeBuffer *bc, int class_index);

/* return */
void emit_ireturn(BytecodeBuffer *bc);
void emit_areturn(BytecodeBuffer *bc);
void emit_return(BytecodeBuffer *bc);