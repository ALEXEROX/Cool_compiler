//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include "../../semantic/class_table.h"

/*
 * Генерация атрибута Code для метода
 */
void emit_code_attribute(FILE *out, MethodInfo *m, int code_utf8_index);

