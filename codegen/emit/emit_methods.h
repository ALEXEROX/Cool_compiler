//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include <stdint.h>
#include "emit_code_attr.h"

struct ClassInfo;

/*
 * Генерация method_info[] для одного класса
 */
void emit_methods(FILE *out, struct ClassInfo *cls, ConstantTable* ct);