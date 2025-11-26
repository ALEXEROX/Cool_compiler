#pragma once

#include "class_table.h"
#include "../Nodes/ast.h"
#include <stdbool.h>

/*
 * Основная функция семантики класса:
 * принимает ClassInfo* (структура из ClassTable)
 */
bool semantic_check_class(ClassTable *ct, ClassInfo *cls);

/*
 * Обычная обёртка под твой текущий код:
 * принимает ClassNode* и сама ищет ClassInfo.
 */
bool semantic_class(ClassTable *ct, ClassNode *cnode);
