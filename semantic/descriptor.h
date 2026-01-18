#pragma once

#include "../Nodes/ast.h"
#include "constant_pool.h"
#include "class_table.h"

/* ============================================================
 *  Преобразование COOL-типа в JVM дескриптор
 * ============================================================ */

/**
 * Преобразует COOL-тип (например "Int", "Bool", "String", "SELF_TYPE", "MyClass")
 * в JVM-дескриптор. Возвращает malloc'ed строку.
 *
 * Примеры:
 *   "Int" -> "I"
 *   "Bool" -> "Z"
 *   "String" -> "Ljava/lang/String;"
 *   "Object" -> "Ljava/lang/Object;"
 *   "MyClass" -> "LMyClass;"
 *   "SELF_TYPE" -> "LSELF_TYPE;"  (решается позднее в семантике)
 */
char *cool_type_to_descriptor(const char *type, const char *class_name);

/* ============================================================
 *  Дескриптор метода
 * ============================================================ */

/**
 * Создаёт JVM-дескриптор метода из списка формалов и типа возврата.
 * Возвращает malloc'ed строку.
 *
 * Пример:
 *     formals = (x:Int, y:String)
 *     return_type = "Bool"
 *     => "(ILjava/lang/String;)Z"
 */
char *make_method_descriptor(FormalList *formals, const char *return_type, const char *class_name);

/* ============================================================
 *  Обёртки для constant pool: COOL → JVM
 * ============================================================ */

/** Добавляет запись Class в constant pool (через const_add_class). */
int cp_add_class_from_cool(ConstantTable *cp, const char *cool_class_name);

/** Добавляет Fieldref по имени класса, имени поля и типу (COOL). */
int cp_add_fieldref_from_cool(ConstantTable *cp,
                              const char *class_name,
                              const char *field_name,
                              const char *cool_type);

/** Добавляет Methodref по имени класса, имени метода, формалам и типу возврата. */
int cp_add_methodref_from_cool(ConstantTable *cp,
                               const char *class_name,
                               const char *method_name,
                               FormalList *formals,
                               const char *return_type);

/* ============================================================
 *  Обёртки для AST-узлов FeatureNode
 * ============================================================ */

/** Methodref из FEATURE_METHOD */
int cp_add_methodref_from_feature(ConstantTable *cp,
                                  ClassInfo cl,
                                  FeatureNode *method_feature);

/** Fieldref из FEATURE_ATTR */
int cp_add_fieldref_from_feature(ConstantTable *cp,
                                 const char *class_name,
                                 FeatureNode *attr_feature);

ClassInfo *find_method_owner(ClassInfo *cls, const char *method_name);
