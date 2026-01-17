//
// Created by Дмитрий on 17.01.2026.
//
#pragma once
#define EMIT_CONSTANT_POOL_H
#include <stdio.h>
#include "../../semantic/constant_pool.h"

/*
 * Сериализует ConstantTable в формате JVM
 * Пишет:
 *   u2 constant_pool_count
 *   cp_info entries[count-1]
 */
void emit_constant_pool(FILE *out, ConstantTable *cp);

