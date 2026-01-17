//
// Created by Дмитрий on 16.01.2026.
//
#pragma once

#include "../../semantic/class_table.h"

/* access flags */
#define JVM_ACC_PUBLIC    0x0001
#define JVM_ACC_PRIVATE   0x0002
#define JVM_ACC_PROTECTED 0x0004
#define JVM_ACC_STATIC    0x0008
#define JVM_ACC_FINAL     0x0010
#define JVM_ACC_SUPER     0x0020

/* magic & version */
#define JVM_MAGIC 0xCAFEBABE
#define JVM_MAJOR 52   /* Java 8 */
#define JVM_MINOR 0

int jvm_return_opcode(const char *cool_type);

/* load/store opcode by Cool type */
int jvm_load_opcode(const char *cool_type);
int jvm_store_opcode(const char *cool_type);
