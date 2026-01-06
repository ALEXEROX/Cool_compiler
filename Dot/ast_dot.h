#include "../Nodes/ast.h"

void save_ast_dot(ProgramNode* program);
static const char* binop_name(BinOpKind op);
static const char* unop_name(UnOpKind op);