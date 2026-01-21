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

void bc_init(BytecodeBuffer *bc, int param_count) {
    bc->data = NULL;
    bc->size = 0;
    bc->capacity = 0;
    bc->locals_count = 1 + param_count;
    bc->special_locals = 16;
}

int add_local(BytecodeBuffer *bc, const char *name, const char *type) {
    if (!bc) return -1;

    if (bc->locals_count >= 16) {
        fprintf(stderr, "Error: exceeded max locals (16)\n");
        return -1;
    }

    int index = bc->locals_count;

    bc->locals[index].id = index;
    strncpy(bc->locals[index].name, name, sizeof(bc->locals[index].name) - 1);
    bc->locals[index].name[sizeof(bc->locals[index].name) - 1] = '\0';
    strncpy(bc->locals[index].type, type, sizeof(bc->locals[index].type) - 1);
    bc->locals[index].type[sizeof(bc->locals[index].type) - 1] = '\0';

    bc->locals_count++;

    return index;
}

int find_local(BytecodeBuffer *bc, const char *name) {
    if (!bc || !name) return -1;

    for (int i = 0; i < bc->locals_count; i++) {
        if (strcmp(bc->locals[i].name, name) == 0) {
            return i;
        }
    }

    return -1; // не найдено
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

void emit_ldc(BytecodeBuffer *bc, int cp_index) {
    bc_emit_u1(bc, 0x12);      /* ldc */
    bc_emit_u1(bc, cp_index); /* index (u1!) */
}

void emit_ldc_classref(BytecodeBuffer *bc, int class_index) {
    bc_emit_u1(bc, 0x12); // ldc
    bc_emit_u1(bc, class_index); // индекс класса в CP
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

void emit_checkcast(BytecodeBuffer *bc, int class_index) {
    bc_emit_u1(bc, 0xC0);     /* checkcast */
    bc_emit_u2(bc, class_index);
}

int emit_ifeq(BytecodeBuffer *bc) {
    int pos = bc->size;
    bc_emit_u1(bc, 0x99); /* ifeq */
    bc_emit_u2(bc, 0);    /* placeholder */
    return pos;
}

int emit_goto(BytecodeBuffer *bc) {
    int pos = bc->size;
    bc_emit_u1(bc, 0xA7); /* goto */
    bc_emit_u2(bc, 0);
    return pos;
}

void patch_jump(BytecodeBuffer *bc, int jump_pos, int target_pos) {
    int offset = target_pos - jump_pos;
    bc->data[jump_pos + 1] = (offset >> 8) & 0xFF;
    bc->data[jump_pos + 2] = offset & 0xFF;
}

// возвращает позицию placeholder для будущей метки
int emit_label_placeholder(BytecodeBuffer *bc) {
    return bc->size;
}

// вставляет "метку" (на самом деле просто помечаем offset)
void emit_label(BytecodeBuffer *bc, int label_pos) {
    // для простого варианта ничего не делаем,
    // реальные оффсеты вычисляются через patch_jump
}

// временный goto (placeholder)
int emit_goto_placeholder(BytecodeBuffer *bc) {
    return emit_goto(bc);
}

// patch для goto/ifeq/ifne
void patch_label(BytecodeBuffer *bc, int placeholder, int target_pos) {
    patch_jump(bc, placeholder, target_pos);
}

void emit_instanceof(BytecodeBuffer *bc, int class_index) {
    bc_emit_u1(bc, 0xC1);  // instanceof
    bc_emit_u2(bc, class_index);
}

int allocate_tmp_local(void *node) {
    static int counter = 1; // простой счетчик
    return counter++;
}

void emit_push_null(BytecodeBuffer *bc) {
    bc_emit_u1(bc, 0x01);  // aconst_null
}

