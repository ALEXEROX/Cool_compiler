//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include "../../semantic/class_table.h"

/*
 * Генерация section fields_info[]
 *
 * JVM:
 *   u2 fields_count
 *   field_info fields[fields_count]
 */
void emit_fields(FILE *out, ClassInfo *cls);

