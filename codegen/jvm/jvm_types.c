//
// Created by Дмитрий on 16.01.2026.
//

#include "jvm_types.h"
#include <string.h>

int jvm_return_opcode(const char *cool_type) {
    if (!cool_type) return 0xB1; /* return */

    if (!strcmp(cool_type, "Int"))  return 0xAC; /* ireturn */
    if (!strcmp(cool_type, "Bool")) return 0xAC;

    /* Object, String, SELF_TYPE, user classes */
    return 0xB0; /* areturn */
}

int jvm_load_opcode(const char *cool_type) {
    if (!strcmp(cool_type, "Int"))  return 0x15; /* iload */
    if (!strcmp(cool_type, "Bool")) return 0x15;
    return 0x19; /* aload */
}

int jvm_store_opcode(const char *cool_type) {
    if (!strcmp(cool_type, "Int"))  return 0x36; /* istore */
    if (!strcmp(cool_type, "Bool")) return 0x36;
    return 0x3A; /* astore */
}
