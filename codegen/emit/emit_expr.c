//
// Created by Дмитрий on 18.01.2026.
//
#include "emit_expr.h"
#include <assert.h>
#include <string.h>
#include "../../semantic/constant_pool.h"
#include "../../semantic/class_table.h"
#include "../../semantic/descriptor.h"

static bool is_int_type(const char *t) {
    return t && (strcmp(t, "Int") == 0 || strcmp(t, "Bool") == 0);
}

/* ============================================================
 * load / store по типу
 * ============================================================ */
static void emit_load(BytecodeBuffer *bc, ExprNode *e) {
    if (is_int_type(e->static_type))
        emit_iload(bc, e->local_index);
    else
        emit_aload(bc, e->local_index);
}

static void emit_store(BytecodeBuffer *bc, ExprNode *e) {
    if (is_int_type(e->static_type))
        emit_istore(bc, e->local_index);
    else
        emit_astore(bc, e->local_index);
}

/* ============================================================
 * emit_expr
 * ============================================================ */
void emit_expr(BytecodeBuffer *bc, ExprNode *e, ClassInfo *cls, ConstantTable* ct) {
    assert(e);

    switch (e->kind) {

    /* ---------- literals ---------- */

    case EXPR_INT_CONST:
        emit_iconst(bc, e->int_const.value);
        break;

    case EXPR_BOOL_CONST:
        emit_iconst(bc, e->bool_const.value ? 1 : 0);
        break;

    case EXPR_STR_CONST:
        /* строка уже должна быть в constant pool */
        emit_ldc(bc, e->stringref_index);
        break;

    /* ---------- variables ---------- */

    case EXPR_OBJECT:
            if (strcmp(e->object.name, "self") == 0) {
                emit_aload_0(bc);
            }
            else if (e->resolved_attr) {
                // this.<field>
                emit_aload_0(bc);
                emit_getfield(bc, e->resolved_attr->fieldref_index);
            }
            else {
                // локал (let / arg / case)
                if (find_local(bc,e->object.name) != -1) {
                    e->local_index = find_local(bc,e->object.name);
                }
                emit_load(bc, e);
            }
            break;

    /* ---------- assignment ---------- */

    case EXPR_ASSIGN:
            if (e->resolved_attr) {
                emit_expr(bc, e->assign.expr, cls, ct); // value
                emit_aload_0(bc);                               // this
                emit_swap(bc);                                  // this, value
                emit_putfield(bc, e->resolved_attr->fieldref_index);

                // результат присваивания = значение поля
                emit_aload_0(bc);
                emit_getfield(bc, e->resolved_attr->fieldref_index);
            } else {
                if (find_local(bc, e->assign.name) != -1)
                    e->local_index = find_local(bc, e->assign.name);
                else if (e->local_index < 1) {
                    e->local_index = bc->special_locals++;
                }
                emit_expr(bc, e->assign.expr, cls, ct);
                emit_store(bc, e);
                emit_load(bc, e);
            }
            break;



    /* ---------- binary ops ---------- */

    case EXPR_BINOP: {
    // сначала вычисляем левый и правый операнды
    emit_expr(bc, e->binop.left, cls, ct);
    if (e->binop.op != OP_AND)
        emit_expr(bc, e->binop.right, cls, ct);

    switch (e->binop.op) {
        case OP_PLUS:  emit_iadd(bc); break;
        case OP_MINUS: emit_isub(bc); break;
        case OP_MUL:   emit_imul(bc); break;
        case OP_DIV:   emit_idiv(bc); break;
        case OP_AND: {
            // вычисляем левый операнд
            emit_expr(bc, e->binop.left, cls, ct);
            // stack: ..., a

            int j_false = emit_ifeq(bc);
            // if a == 0 -> goto false

            // a == true → вычисляем правый
            emit_expr(bc, e->binop.right, cls, ct);
            // stack: ..., b

            int j_end = emit_goto(bc);

            // false branch
            int false_pos = bc->size;
            patch_jump(bc, j_false, false_pos);
            emit_iconst(bc, 0);

            // end
            patch_jump(bc, j_end, bc->size);
            break;
        }
        case OP_OR:    bc_emit_u1(bc, 0x80); break; // ior

        case OP_LT: {
            int j_false = bc->size;
            bc_emit_u1(bc, 0xA2); // if_icmpge
            bc_emit_u2(bc, 0);    // placeholder

            // true branch
            emit_iconst(bc, 1);

            int j_end = emit_goto(bc);

            // false branch
            int false_pos = bc->size;
            patch_jump(bc, j_false, false_pos);

            emit_iconst(bc, 0);

            int end_pos = bc->size;
            patch_jump(bc, j_end, end_pos);

            break;
        }

        case OP_LE: {
            // stack: ..., a, b

            int j_false = bc->size;
            bc_emit_u1(bc, 0xA3); // if_icmpgt
            bc_emit_u2(bc, 0);    // placeholder

            // true
            emit_iconst(bc, 1);

            int j_end = emit_goto(bc);

            // false
            int false_pos = bc->size;
            patch_jump(bc, j_false, false_pos);

            emit_iconst(bc, 0);

            int end_pos = bc->size;
            patch_jump(bc, j_end, end_pos);

            break;
        }

        case OP_EQ: {
            // stack: ..., a, b

            int j_false = bc->size;
            bc_emit_u1(bc, 0x9F); // if_icmpne
            bc_emit_u2(bc, 0);    // placeholder

            // true
            emit_iconst(bc, 1);

            int j_end = emit_goto(bc);

            // false
            int false_pos = bc->size;
            patch_jump(bc, j_false, false_pos);

            emit_iconst(bc, 0);

            int end_pos = bc->size;
            patch_jump(bc, j_end, end_pos);

            break;
        }

        default:
            fprintf(stderr, "Unknown binary op\n");
            break;
        }
        break;
    }


    /* ---------- unary ops ---------- */

        case EXPR_UNOP: {
        emit_expr(bc, e->unop.expr, cls, ct);

        switch (e->unop.op) {
            case OP_NEG:
                bc_emit_u1(bc, 0x74); // ineg
                break;

            case OP_NOT:
                // logical NOT: iconst_1 xor
                emit_iconst(bc, 1);
                bc_emit_u1(bc, 0x82); // ixor
                break;

            case OP_ISVOID: {
                // if null -> push 1 else push 0
                int true_label = emit_label_placeholder(bc);
                int end_label = emit_label_placeholder(bc);
                bc_emit_u1(bc, 0xC6); // ifnull
                bc_emit_u2(bc, 0);     // placeholder
                emit_iconst(bc, 0);    // not null -> 0
                int goto_end = emit_goto_placeholder(bc);
                emit_label(bc, true_label);
                emit_iconst(bc, 1);    // null -> 1
                emit_label(bc, end_label);
                patch_label(bc, true_label, bc->size);
                patch_label(bc, goto_end, bc->size);
                break;
            }

            default:
                fprintf(stderr, "Unknown unary op\n");
                break;
        }
        break;
        }

    /* ---------- dispatch ---------- */

    case EXPR_DISPATCH:{
            /* receiver */
            if (e->dispatch.caller) {
                emit_expr(bc, e->dispatch.caller, cls, ct);
                if (strcmp(e->dispatch.caller->static_type, e->dispatch_class->name) != 0) {
                    emit_checkcast(bc, e->dispatch_class->class_cp_index);
                }
            } else {
                /* implicit self */
                bc_emit_u1(bc, 0x2A); // aload_0
            }

            /* arguments */
            for (ExprList *it = e->dispatch.args; it; it = it->next) {
                emit_expr(bc, it->node, cls, ct);
            }

            /* invokevirtual */
            bc_emit_u1(bc, 0xB6);
            bc_emit_u2(bc, e->resolved_method->methodref_index);
            if (strcmp(e->resolved_method->return_type, "SELF_TYPE") == 0) {
                emit_checkcast(bc, e->dispatch_class->class_cp_index);
            }
            break;
        }

    case EXPR_STATIC_DISPATCH: {
        emit_expr(bc, e->static_dispatch.caller, cls, ct);

        for (ExprList *it = e->static_dispatch.args; it; it = it->next)
            emit_expr(bc, it->node, cls, ct);

        emit_invokespecial(bc, e->resolved_method->methodref_index);
        break;
    }

    /* ---------- new ---------- */

    case EXPR_NEW:
        int cl_id = const_add_class(ct, e->static_type);
        emit_new(bc, cl_id);
        emit_dup(bc);
        if (cls->parent_info) {
            int id = const_add_name_and_type(ct,"<init>","()V");
            emit_invokespecial(bc, const_add_methodref(ct,cl_id,id));
        }
        else
            emit_invokespecial(bc, 7);
        emit_checkcast(bc, cl_id);
        break;

    /* ---------- block ---------- */

    case EXPR_BLOCK: {
        ExprList *it = e->block.exprs;
        while (it && it->next) {
            emit_expr(bc, it->node, cls, ct);
            emit_pop(bc);
            it = it->next;
        }
        if (it)
            emit_expr(bc, it->node, cls, ct);
        break;
    }

    case EXPR_IF: {
        emit_expr(bc, e->if_expr.cond, cls, ct);
        int j_else = emit_ifeq(bc);
        emit_expr(bc, e->if_expr.then_branch, cls, ct);
        int j_end = emit_goto(bc);
        patch_jump(bc, j_else, bc->size);
        emit_expr(bc, e->if_expr.else_branch, cls, ct);
        patch_jump(bc, j_end, bc->size);
        break;
    }

    case EXPR_WHILE: {
        int loop_start = bc->size;
        emit_expr(bc, e->while_expr.cond, cls, ct);
        int j_end = emit_ifeq(bc);
        emit_expr(bc, e->while_expr.body, cls, ct);
        emit_pop(bc);
        emit_goto(bc);
        patch_jump(bc, bc->size - 3, loop_start);
        patch_jump(bc, j_end, bc->size);
        emit_iconst(bc, 0); /* while возвращает Object/Void */
        break;
    }


    case EXPR_CASE: {
        ExprNode *scrutinee = e->case_expr.expr;

        // Вычисляем выражение, по которому делаем case
        emit_expr(bc, scrutinee, cls, ct);

        // Сохраняем результат в временный локал
        int tmp_index = allocate_tmp_local(e);
        emit_istore(bc, tmp_index);

        CaseList *it = e->case_expr.cases;
        int end_label = emit_goto(bc);  // переход в конец case

        while (it) {
            CaseNode *c = it->node;

            // Загружаем скринтинированное значение
            emit_iload(bc, tmp_index);

            emit_instanceof(bc, cls->class_cp_index);

            // Проверяем: если не тот тип — пропускаем ветку
            int next_label = emit_ifeq(bc);

            // Если совпало — кладём скринтинированное значение в локал ветки
            emit_iload(bc, tmp_index);
            emit_istore(bc, c->local_index);

            // Генерируем тело ветки
            emit_expr(bc, c->expr, cls, ct);

            // Переход на конец case после выполнения ветки
            int goto_end = emit_goto(bc);

            // Патчим jump для несовпадения типа на следующую ветку
            patch_jump(bc, next_label, bc->size);

            it = it->next;

            // Патчим jump в конец case для текущей ветки
            patch_jump(bc, goto_end, bc->size);
        }

        // В конце case освобождаем временный локал
        emit_iload(bc, tmp_index);
        patch_jump(bc, end_label, bc->size);
        } break;

        case EXPR_LET: {
            LetList *binding = e->let_expr.bindings;
            int count = 0;
            // Обрабатываем инициализации и выделяем слоты
            while (binding) {
                int slot = add_local(bc,binding->binding->name,binding->binding->type);  // выделяем новый локал
                binding->binding->local_index = slot;// запоминаем индекс в let-узле

                if (binding->binding->init) {
                    emit_expr(bc, binding->binding->init, cls, ct);
                } else {
                    // Default для примитивов
                    if (strcmp(binding->binding->type, "Int") == 0 ||
                        strcmp(binding->binding->type, "Bool") == 0) {
                        emit_iconst(bc,0);
                        } else {
                            emit_push_null(bc); // для объектов
                        }
                }


                if (strcmp(binding->binding->type, "Int") == 0 ||
                    strcmp(binding->binding->type, "Bool") == 0) {
                        emit_istore(bc, slot);}
                else {
                    emit_astore(bc, slot);
                }
                // сохраняем в локал
                count = count + 1;
                binding = binding->next;
            }

            // Генерим тело let
            emit_expr(bc, e->let_expr.body, cls, ct);
            break;
        }








    default:
        assert(!"emit_expr: unsupported ExprKind");
    }
}
