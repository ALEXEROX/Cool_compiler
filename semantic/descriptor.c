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

static char *rename_builtin(char *s) {
    if (!s) return NULL;
    if (strcmp(s, "String") == 0)   return strdup_safe("Ljava/lang/String;");
    if (strcmp(s, "Object") == 0)   return strdup_safe("Lcool/runtime/Object;");
    if (strcmp(s, "IO") == 0)   return strdup_safe("Lcool/runtime/IO;");
    if (strcmp(s, "Array") == 0)   return strdup_safe("Lcool/runtime/Array;");
    if (strcmp(s, "IntArray") == 0)   return strdup_safe("Lcool/runtime/IntArray;");
    return s;
}

// Находит класс, где определён метод (т.е. первый метод с таким именем по цепочке наследования)
ClassInfo *find_method_owner(ClassInfo *cls, const char *method_name) {
    ClassInfo *cur = cls;
    while (cur->parent_info) {
        cur = cur->parent_info;
        for (MethodInfo *m = cur->methods; m; m = m->next) {
            if (strcmp(m->name, method_name) == 0) {
                return cur;  // вернули класс-владелец
            }
        }

    }
    return cls;
}

/* ============================================================
 *  COOL-type → JVM descriptor
 * ============================================================ */

char *cool_type_to_descriptor(const char *type, const char *class_name) {
    if (!type) return strdup_safe("");

    if (strcmp(type, "Int") == 0)      return strdup_safe("I");
    if (strcmp(type, "Bool") == 0)     return strdup_safe("Z");
    if (strcmp(type, "String") == 0)   return strdup_safe("Ljava/lang/String;");
    if (strcmp(type, "Object") == 0)   return strdup_safe("Lcool/runtime/Object;");
    if (strcmp(type, "IO") == 0)   return strdup_safe("Lcool/runtime/IO;");
    if (strcmp(type, "Array") == 0)   return strdup_safe("Lcool/runtime/Array;");
    if (strcmp(type, "IntArray") == 0)   return strdup_safe("Lcool/runtime/IntArray;");
    if (strcmp(type, "SELF_TYPE") == 0) {
        char *self = malloc(strlen(class_name) + 3);
        sprintf(self, "L%s;", class_name);
        return strdup_safe(self);
    }

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

char *make_method_descriptor(FormalList *formals, char *return_type, char *class_name) {

    return_type = rename_builtin(return_type);
    class_name = rename_builtin(class_name);

    size_t size = 128;
    char *buf = malloc(size);
    size_t pos = 0;

    buf[pos++] = '(';

    for (FormalList *f = formals; f; f = f->next) {
        char *d = cool_type_to_descriptor(f->node->type,class_name);
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

    char *ret = cool_type_to_descriptor(return_type ? return_type : "Object", class_name);
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

int cp_add_class_from_cool(ConstantTable *cp, char *cool_class_name) {
    if (!cp || !cool_class_name) return 0;
    return const_add_class(cp, cool_class_name);
}

int cp_add_fieldref_from_cool(ConstantTable *cp,
                              char *class_name,
                              const char *field_name,
                              char *cool_type)
{
    if (!cp || !class_name || !field_name) return 0;

    cool_type = rename_builtin(cool_type);
    int cls_idx = cp_add_class_from_cool(cp, class_name);

    char *desc = cool_type_to_descriptor(cool_type ? cool_type : "Object",class_name);
    int nat = const_add_name_and_type(cp, field_name, desc);
    free(desc);

    return const_add_fieldref(cp, cls_idx, nat);
}

int cp_add_methodref_from_cool(ConstantTable *cp, char *class_name, const char *method_name, FormalList *formals, char *return_type)
{
    if (!cp || !class_name || !method_name) return 0;

    return_type = rename_builtin(return_type);

    int cls_idx = cp_add_class_from_cool(cp, class_name);

    char *desc = make_method_descriptor(formals, return_type, class_name);
    int nat = const_add_name_and_type(cp, method_name, desc);
    free(desc);

    return const_add_methodref(cp, cls_idx, nat);
}

/* ============================================================
 *  FeatureNode обёртки
 * ============================================================ */

int cp_add_methodref_from_feature(ConstantTable *cp, ClassInfo cl, FeatureNode *method_feature)
{
    if (!cp || !cl.name || !method_feature) return 0;
    if (method_feature->kind != FEATURE_METHOD) return 0;

    return cp_add_methodref_from_cool(cp, cl.name, method_feature->name, method_feature->method.formals, method_feature->method.return_type);
}

int cp_add_fieldref_from_feature(ConstantTable *cp,
                                 char *class_name,
                                 FeatureNode *attr_feature)
{
    if (!cp || !class_name || !attr_feature) return 0;
    if (attr_feature->kind != FEATURE_ATTR) return 0;

    class_name = rename_builtin(class_name);

    return cp_add_fieldref_from_cool(
        cp,
        class_name,
        attr_feature->name,
        attr_feature->attr.type
    );
}
