//
// Created by Дмитрий on 18.01.2026.
//

#include "emit_bytecode.h"
#include <stdlib.h>
#include <string.h>

static void bc_ensure(BytecodeBuffer *bc, int n) {
    if (bc->size + n > bc->capacity) {
        bc->capacity = (bc->capacity + n) * 2 + 32;
        bc->data = realloc(bc->data, bc->capacity);
    }
}

void bc_init(BytecodeBuffer *bc) {
    bc->data = NULL;
    bc->size = 0;
    bc->capacity = 0;
}

void bc_free(BytecodeBuffer *bc) {
    free(bc->data);
}

void bc_emit_u1(BytecodeBuffer *bc, uint8_t v) {
    bc_ensure(bc, 1);
    bc->data[bc->size++] = v;
}

void bc_emit_u2(BytecodeBuffer *bc, uint16_t v) {
    bc_ensure(bc, 2);
    bc->data[bc->size++] = (v >> 8) & 0xFF;
    bc->data[bc->size++] = v & 0xFF;
}

/* ================= JVM ================= */

void emit_iload(BytecodeBuffer *bc, int i) {
    if (i <= 3) bc_emit_u1(bc, 0x1A + i);
    else { bc_emit_u1(bc, 0x15); bc_emit_u1(bc, i); }
}

void emit_aload(BytecodeBuffer *bc, int i) {
    if (i <= 3) bc_emit_u1(bc, 0x2A + i);
    else { bc_emit_u1(bc, 0x19); bc_emit_u1(bc, i); }
}

void emit_aload_0(BytecodeBuffer *bc) {
    bc_emit_u1(bc, 0x2A);
}

void emit_istore(BytecodeBuffer *bc, int i) {
    if (i <= 3) bc_emit_u1(bc, 0x3B + i);
    else { bc_emit_u1(bc, 0x36); bc_emit_u1(bc, i); }
}

void emit_astore(BytecodeBuffer *bc, int i) {
    if (i <= 3) bc_emit_u1(bc, 0x4B + i);
    else { bc_emit_u1(bc, 0x3A); bc_emit_u1(bc, i); }
}

void emit_iconst(BytecodeBuffer *bc, int v) {
    if (v >= -1 && v <= 5) bc_emit_u1(bc, 0x03 + v);
    else {
        bc_emit_u1(bc, 0x10);
        bc_emit_u1(bc, (uint8_t)v);
    }
}

void emit_iadd(BytecodeBuffer *bc) { bc_emit_u1(bc, 0x60); }
void emit_isub(BytecodeBuffer *bc) { bc_emit_u1(bc, 0x64); }
void emit_imul(BytecodeBuffer *bc) { bc_emit_u1(bc, 0x68); }
void emit_idiv(BytecodeBuffer *bc) { bc_emit_u1(bc, 0x6C); }

void emit_dup(BytecodeBuffer *bc)  { bc_emit_u1(bc, 0x59); }
void emit_pop(BytecodeBuffer *bc)  { bc_emit_u1(bc, 0x57); }
void emit_swap(BytecodeBuffer *bc) { bc_emit_u1(bc, 0x5F); }

void emit_getfield(BytecodeBuffer *bc, int idx) {
    bc_emit_u1(bc, 0xB4);
    bc_emit_u2(bc, idx);
}

void emit_putfield(BytecodeBuffer *bc, int idx) {
    bc_emit_u1(bc, 0xB5);
    bc_emit_u2(bc, idx);
}

void emit_invokevirtual(BytecodeBuffer *bc, int idx) {
    bc_emit_u1(bc, 0xB6);
    bc_emit_u2(bc, idx);
}

void emit_invokespecial(BytecodeBuffer *bc, int idx) {
    bc_emit_u1(bc, 0xB7);
    bc_emit_u2(bc, idx);
}

void emit_new(BytecodeBuffer *bc, int idx) {
    bc_emit_u1(bc, 0xBB);
    bc_emit_u2(bc, idx);
}

