//
// Created by Дмитрий on 17.01.2026.
//

#include "emit_class_header.h"
#include <stdio.h>

/* ===============================
 * Class header emission
 * =============================== */

void emit_class_header(FILE *out) {
    if (!out) return;

    /* magic */
    write_u4(out, 0xCAFEBABE);

    /* version: Java 8 */
    write_u2(out, 0);     /* minor */
    write_u2(out, 52);    /* major */
}
