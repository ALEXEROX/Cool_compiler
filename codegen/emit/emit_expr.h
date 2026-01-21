//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include "../../Nodes/ast.h"   /* ExprNode, ExprKind */
#include "emit_bytecode.h"

typedef struct {
    int stack_size;
    char *stack_types[32];   // "I" для int, "L<class>;" для объектов
    int locals_count;
    char *locals[32];
} Frame;


void emit_expr(BytecodeBuffer *bc, ExprNode *e, ClassInfo *cls, ConstantTable* ct);