//
// Created by Дмитрий on 02.01.2026.
//
#pragma once
#include "class_table.h"
#include "object_env.h"
#include "../Nodes/ast.h"
#include <stdbool.h>

/*
 * Основная функция семантики выражения.
 * - ct: таблица классов
 * - cls: текущий класс (ClassInfo*)
 * - env: текущее окружение переменных (ObjectEnv*)
 * - expr: выражение, которое надо проанализировать
 *
 * Возвращает true при отсутствии ошибок (функция сама печатает сообщения об ошибках).
 * В результате expr->static_type должен быть установлен (malloc'ed строка).
 */
bool semantic_check_expr(ClassTable *ct, ClassInfo *cls, ObjectEnv *env, ExprNode *expr, ConstantTable *cp);
