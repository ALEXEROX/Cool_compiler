//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include "../../Nodes/ast.h"   /* ExprNode, ExprKind */
#include "emit_bytecode.h"

void emit_expr(BytecodeBuffer *bc, ExprNode *e, int class_index);