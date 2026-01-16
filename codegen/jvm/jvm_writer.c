//
// Created by Дмитрий on 16.01.2026.
//

#include "jvm_writer.h"
#include <stdlib.h>

JVMWriter *jvm_writer_open(const char *filename) {
    JVMWriter *w = malloc(sizeof(JVMWriter));
    if (!w) return NULL;

    w->out = fopen(filename, "wb");
    if (!w->out) {
        free(w);
        return NULL;
    }
    return w;
}

void jvm_writer_close(JVMWriter *w) {
    if (!w) return;
    if (w->out) fclose(w->out);
    free(w);
}

void jvm_write_u1(JVMWriter *w, uint8_t v) {
    fwrite(&v, 1, 1, w->out);
}

void jvm_write_u2(JVMWriter *w, uint16_t v) {
    uint8_t buf[2];
    buf[0] = (v >> 8) & 0xFF;
    buf[1] = v & 0xFF;
    fwrite(buf, 1, 2, w->out);
}

void jvm_write_u4(JVMWriter *w, uint32_t v) {
    uint8_t buf[4];
    buf[0] = (v >> 24) & 0xFF;
    buf[1] = (v >> 16) & 0xFF;
    buf[2] = (v >> 8) & 0xFF;
    buf[3] = v & 0xFF;
    fwrite(buf, 1, 4, w->out);
}

void jvm_write_bytes(JVMWriter *w, const void *data, size_t size) {
    fwrite(data, 1, size, w->out);
}
