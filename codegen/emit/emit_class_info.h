//
// Created by Дмитрий on 18.01.2026.
//
#pragma once
#include <stdio.h>
#include "../../semantic/class_table.h"
#include "emit_utils.h"

/*
 * Пишет:
 *  access_flags
 *  this_class
 *  super_class
 *  interfaces_count
 *  interfaces[]
 */
void emit_class_info(FILE *out, ClassInfo *cls);
