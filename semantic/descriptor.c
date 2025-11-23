#include "descriptor.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* ============================================================
 * Вспомогательные
 * ============================================================ */

static char *strdup_safe(const char *s) {
    if (!s) return NULL;
    char *r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}

/* ============================================================
 *  COOL-type → JVM descriptor
 * ============================================================ */

char *cool_type_to_descriptor(const char *type) {
    if (!type) return strdup_safe("");

    if (strcmp(type, "Int") == 0)      return strdup_safe("I");
    if (strcmp(type, "Bool") == 0)     return strdup_safe("Z");
    if (strcmp(type, "String") == 0)   return strdup_safe("Ljava/lang/String;");
    if (strcmp(type, "Object") == 0)   return strdup_safe("Ljava/lang/Object;");
    if (strcmp(type, "SELF_TYPE") == 0) return strdup_safe("LSELF_TYPE;");

    /* Если уже похоже на дескриптор L...; */
    size_t len = strlen(type);
    if (len > 2 && type[0] == 'L' && type[len-1] == ';')
        return strdup_safe(type);

    /* Пользовательский класс */
    char *buf = malloc(len + 3);
    sprintf(buf, "L%s;", type);
    return buf;
}

/* ============================================================
 *  Дескриптор метода
 * ============================================================ */

char *make_method_descriptor(FormalList *formals, const char *return_type) {
    size_t size = 128;
    char *buf = malloc(size);
    size_t pos = 0;

    buf[pos++] = '(';

    for (FormalList *f = formals; f; f = f->next) {
        char *d = cool_type_to_descriptor(f->node->type);
        size_t dl = strlen(d);

        if (pos + dl + 16 >= size) {
            size = (pos + dl + 16) * 2;
            buf = realloc(buf, size);
        }

        memcpy(buf + pos, d, dl);
        pos += dl;
        free(d);
    }

    buf[pos++] = ')';

    char *ret = cool_type_to_descriptor(return_type ? return_type : "Object");
    size_t rl = strlen(ret);

    if (pos + rl + 2 >= size) {
        size = pos + rl + 16;
        buf = realloc(buf, size);
    }

    memcpy(buf + pos, ret, rl);
    pos += rl;
    buf[pos] = '\0';
    free(ret);

    return buf;
}

/* ============================================================
 *  Обёртки для добавления в ConstantPool
 * ============================================================ */

int cp_add_class_from_cool(ConstantTable *cp, const char *cool_class_name) {
    if (!cp || !cool_class_name) return 0;
    return const_add_class(cp, cool_class_name);
}

int cp_add_fieldref_from_cool(ConstantTable *cp,
                              const char *class_name,
                              const char *field_name,
                              const char *cool_type)
{
    if (!cp || !class_name || !field_name) return 0;

    int cls_idx = cp_add_class_from_cool(cp, class_name);

    char *desc = cool_type_to_descriptor(cool_type ? cool_type : "Object");
    int nat = const_add_name_and_type(cp, field_name, desc);
    free(desc);

    return const_add_fieldref(cp, cls_idx, nat);
}

int cp_add_methodref_from_cool(ConstantTable *cp,
                               const char *class_name,
                               const char *method_name,
                               FormalList *formals,
                               const char *return_type)
{
    if (!cp || !class_name || !method_name) return 0;

    int cls_idx = cp_add_class_from_cool(cp, class_name);

    char *desc = make_method_descriptor(formals, return_type);
    int nat = const_add_name_and_type(cp, method_name, desc);
    free(desc);

    return const_add_methodref(cp, cls_idx, nat);
}

/* ============================================================
 *  FeatureNode обёртки
 * ============================================================ */

int cp_add_methodref_from_feature(ConstantTable *cp,
                                  const char *class_name,
                                  FeatureNode *method_feature)
{
    if (!cp || !class_name || !method_feature) return 0;
    if (method_feature->kind != FEATURE_METHOD) return 0;

    return cp_add_methodref_from_cool(
        cp,
        class_name,
        method_feature->name,
        method_feature->method.formals,
        method_feature->method.return_type
    );
}

int cp_add_fieldref_from_feature(ConstantTable *cp,
                                 const char *class_name,
                                 FeatureNode *attr_feature)
{
    if (!cp || !class_name || !attr_feature) return 0;
    if (attr_feature->kind != FEATURE_ATTR) return 0;

    return cp_add_fieldref_from_cool(
        cp,
        class_name,
        attr_feature->name,
        attr_feature->attr.type
    );
}

/* ============================================================
 *  Dispatch обёртки — используют ClassTable
 * ============================================================ */

int cp_add_methodref_from_dispatch(ConstantTable *cp,
                                   ClassTable *ct,
                                   const char *static_type,
                                   ExprNode *dispatch_expr)
{
    if (!cp || !ct || !static_type || !dispatch_expr) return 0;
    if (dispatch_expr->kind != EXPR_DISPATCH) return 0;

    const char *method_name = dispatch_expr->dispatch.method;

    /* Находим метод по иерархии — это важно для JVM invokevirtual */
    MethodInfo *m = class_lookup_method(ct, static_type, method_name);
    if (!m) return 0;

    /* Генерируем дескриптор */
    char *desc = methodinfo_get_descriptor(m);

    /* Создаём NameAndType */
    int nat = const_add_name_and_type(cp, m->name, desc);

    /* Добавляем Class */
    int cls_idx = const_add_class(cp, static_type);

    /* Methodref */
    int mref = const_add_methodref(cp, cls_idx, nat);

    free(desc);
    return mref;
}

int cp_add_methodref_from_static_dispatch(ConstantTable *cp,
                                          ClassTable *ct,
                                          ExprNode *static_dispatch_expr)
{
    if (!cp || !ct || !static_dispatch_expr) return 0;
    if (static_dispatch_expr->kind != EXPR_STATIC_DISPATCH) return 0;

    const char *class_name = static_dispatch_expr->static_dispatch.type;
    const char *method_name = static_dispatch_expr->static_dispatch.method;

    MethodInfo *m = class_lookup_method(ct, class_name, method_name);
    if (!m) return 0;

    char *desc = methodinfo_get_descriptor(m);

    int nat = const_add_name_and_type(cp, m->name, desc);
    int cls_idx = const_add_class(cp, class_name);
    int mref = const_add_methodref(cp, cls_idx, nat);

    free(desc);
    return mref;
}
