cmake_minimum_required(VERSION 4.0)
project(Cool_parser C)

set(CMAKE_C_STANDARD 11)

add_executable(Cool_parser main.c
        semantic/class_table.c
        semantic/constant_pool.c
        semantic/descriptor.c
        semantic/object_env.c
        semantic/semantic_class.c
        semantic/semantic_expr.c
        semantic/semantic_method.c
        semantic/semantic_program.h
        semantic/semantic_program.c
        codegen/jvm/jvm_writer.c
        codegen/jvm/jvm_writer.h
        codegen/jvm/jvm_types.c
        codegen/jvm/jvm_types.h
        codegen/emit/emit_constant_pool.c
        codegen/emit/emit_constant_pool.h
)
