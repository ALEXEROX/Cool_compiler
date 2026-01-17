//
// Created by Дмитрий on 16.01.2026.
//
#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    FILE *out;
} JVMWriter;

/* lifecycle */
JVMWriter *jvm_writer_open(const char *filename);
void jvm_writer_close(JVMWriter *w);

/* primitive writes (big-endian!) */
void jvm_write_u1(JVMWriter *w, uint8_t v);
void jvm_write_u2(JVMWriter *w, uint16_t v);
void jvm_write_u4(JVMWriter *w, uint32_t v);

/* raw bytes */
void jvm_write_bytes(JVMWriter *w, const void *data, size_t size);

