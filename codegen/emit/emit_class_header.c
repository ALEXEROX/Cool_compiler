//
// Created by Дмитрий on 17.01.2026.
//

#include "emit_class_header.h"
#include <stdio.h>

#include "emit_constant_pool.h"
#include "emit_fields.h"
#include "emit_methods.h"

/* ===============================
 * Class header emission
 * =============================== */

void emit_class_header(FILE *out, ClassInfo *cls, ConstantTable *cp) {
    if (!out || !cls || !cp) return;

    /* magic + version */
    write_u4(out, 0xCAFEBABE);
    write_u2(out, 0);    // minor
    write_u2(out, 52);   // major

    /* constant pool */
    emit_constant_pool(out, cp);

    /* access_flags */
    write_u2(out, 0x0021);  // ACC_PUBLIC | ACC_SUPER

    /* this_class */
    write_u2(out, (uint16_t)cls->class_cp_index);

    /* super_class */
    if (!cls->parent_info)
        write_u2(out, 0);  // Object = 0
    else
        write_u2(out, (uint16_t)cls->parent_info->class_cp_index);

    /* interfaces_count */
    write_u2(out, 0);

    /* fields */
    emit_fields(out, cls);

    /* methods */
    emit_methods(out, cls, cp);

    /* attributes_count (например, SourceFile) */
    write_u2(out, 0);
}
