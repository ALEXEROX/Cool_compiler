#pragma once

#include "../Nodes/ast.h"
#include "class_table.h"
#include "object_env.h"
#include "constant_pool.h"

/* Главная функция семантического анализа всей программы */
void *semantic_program(ProgramNode *p);