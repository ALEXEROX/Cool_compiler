//
// Created by Дмитрий on 18.01.2026.
//
#include "emit_expr.h"
#include <assert.h>
#include <string.h>

static bool is_int_type(const char *t) {
    return t && strcmp(t, "Int") == 0;
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
void emit_expr(BytecodeBuffer *bc, ExprNode *e, int class_index) {
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
        emit_ldc(bc, e->methodref_index);
        break;

    /* ---------- variables ---------- */

    case EXPR_OBJECT:
        emit_load(bc, e);
        break;

    /* ---------- assignment ---------- */

    case EXPR_ASSIGN:
        emit_expr(bc, e->assign.expr, class_index);

        if (e->resolved_attr) {
            /* поле: this.field = expr */
            emit_aload_0(bc);
            emit_swap(bc);
            emit_putfield(bc, e->fieldref_index);
            emit_expr(bc, e->assign.expr, class_index); /* результат выражения */
        } else {
            /* локальная */
            emit_store(bc, e);
            emit_load(bc, e);
        }
        break;

    /* ---------- binary ops ---------- */

    case EXPR_BINOP:
        emit_expr(bc, e->binop.left, class_index);
        emit_expr(bc, e->binop.right, class_index);

        switch (e->binop.op) {
        case OP_PLUS:  emit_iadd(bc); break;
        case OP_MINUS: emit_isub(bc); break;
        case OP_MUL:   emit_imul(bc); break;
        case OP_DIV:   emit_idiv(bc); break;
        default:
            assert(!"unsupported binop");
        }
        break;

    /* ---------- unary ops ---------- */

    case EXPR_UNOP:
        emit_expr(bc, e->unop.expr, class_index);
        switch (e->unop.op) {
        case OP_NEG:
            emit_iconst(bc, -1);
            emit_imul(bc);
            break;
        default:
            assert(!"unsupported unop");
        }
        break;

    /* ---------- dispatch ---------- */

    case EXPR_DISPATCH: {
        emit_expr(bc, e->dispatch.caller, class_index);

        for (ExprList *it = e->dispatch.args; it; it = it->next)
            emit_expr(bc, it->node, class_index);

        emit_invokevirtual(bc, e->methodref_index);
        break;
    }

    case EXPR_STATIC_DISPATCH: {
        emit_expr(bc, e->static_dispatch.caller, class_index);
        emit_checkcast(bc, e->methodref_index);

        for (ExprList *it = e->static_dispatch.args; it; it = it->next)
            emit_expr(bc, it->node, class_index);

        emit_invokevirtual(bc, e->methodref_index);
        break;
    }

    /* ---------- new ---------- */

    case EXPR_NEW:
        emit_new(bc, e->methodref_index);
        emit_dup(bc);
        emit_invokespecial(bc, e->methodref_index);
        break;

    /* ---------- block ---------- */

    case EXPR_BLOCK: {
        ExprList *it = e->block.exprs;
        while (it && it->next) {
            emit_expr(bc, it->node, class_index);
            emit_pop(bc);
            it = it->next;
        }
        if (it)
            emit_expr(bc, it->node, class_index);
        break;
    }

    case EXPR_IF: {
        emit_expr(bc, e->if_expr.cond, class_index);
        int j_else = emit_ifeq(bc);
        emit_expr(bc, e->if_expr.then_branch, class_index);
        int j_end = emit_goto(bc);
        patch_jump(bc, j_else, bc->size);
        emit_expr(bc, e->if_expr.else_branch, class_index);
        patch_jump(bc, j_end, bc->size);
        break;
    }

    case EXPR_WHILE: {
        int loop_start = bc->size;
        emit_expr(bc, e->while_expr.cond, class_index);
        int j_end = emit_ifeq(bc);
        emit_expr(bc, e->while_expr.body, class_index);
        emit_goto(bc);
        patch_jump(bc, bc->size - 3, loop_start);
        patch_jump(bc, j_end, bc->size);
        emit_iconst(bc, 0); /* while возвращает Object/Void */
        break;
    }

    case EXPR_CASE: {
        ExprNode *scrutinee = e->case_expr.expr;

        // Вычисляем выражение, по которому делаем case
        emit_expr(bc, scrutinee, class_index);

        // Сохраняем результат в временный локал
        int tmp_index = allocate_tmp_local(e);
        emit_istore(bc, tmp_index);

        CaseList *it = e->case_expr.cases;
        int end_label = emit_goto(bc);  // переход в конец case

        while (it) {
            CaseNode *c = it->node;

            // Загружаем скринтинированное значение
            emit_iload(bc, tmp_index);

            emit_instanceof(bc, class_index);

            // Проверяем: если не тот тип — пропускаем ветку
            int next_label = emit_ifeq(bc);

            // Если совпало — кладём скринтинированное значение в локал ветки
            emit_iload(bc, tmp_index);
            emit_istore(bc, c->local_index);

            // Генерируем тело ветки
            emit_expr(bc, c->expr, class_index);

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







    default:
        assert(!"emit_expr: unsupported ExprKind");
    }
}
