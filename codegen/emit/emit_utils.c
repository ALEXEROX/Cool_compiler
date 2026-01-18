//
// Created by Дмитрий on 18.01.2026.
//

#include "emit_utils.h"

/* ===============================
 * Big-endian writers
 * =============================== */

void write_u1(FILE *out, uint8_t v) {
    fputc(v, out);
}

void write_u2(FILE *out, uint16_t v) {
    fputc((v >> 8) & 0xFF, out);
    fputc(v & 0xFF, out);
}

void write_u4(FILE *out, uint32_t v) {
    fputc((v >> 24) & 0xFF, out);
    fputc((v >> 16) & 0xFF, out);
    fputc((v >> 8) & 0xFF, out);
    fputc(v & 0xFF, out);
}
