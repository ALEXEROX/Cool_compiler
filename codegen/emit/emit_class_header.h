//
// Created by Дмитрий on 17.01.2026.
//
#pragma once
#include <stdio.h>
#include "emit_utils.h"

/*
 * Генерирует заголовок JVM .class файла:
 *  - magic
 *  - version
 *  - access_flags
 *  - this_class
 *  - super_class
 */
void emit_class_header(FILE *out);

