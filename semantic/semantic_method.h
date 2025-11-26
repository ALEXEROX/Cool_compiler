#pragma once

#include "class_table.h"
#include "../Nodes/ast.h"
#include <stdbool.h>

bool semantic_check_method(ClassTable *ct, ClassInfo *cls, MethodInfo *m);
