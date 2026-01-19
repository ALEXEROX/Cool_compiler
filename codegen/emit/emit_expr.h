//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include "../../Nodes/ast.h"   /* ExprNode, ExprKind */
#include "emit_bytecode.h"

typedef struct LocalVar {
    const char *name;
    int index;
    struct LocalVar *next;
} LocalVar;

typedef struct EmitContext {
    LocalVar *locals;
    int next_local;
} EmitContext;

void emit_expr(BytecodeBuffer *bc, ExprNode *e, int class_index, ConstantTable* ct);