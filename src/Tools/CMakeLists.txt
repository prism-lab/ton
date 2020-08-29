cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(FUNC_LIB_SOURCE
        func/keywords.cpp
        func/unify-types.cpp
        func/parse-func.cpp
        func/abscode.cpp
        func/gen-abscode.cpp
        func/analyzer.cpp
        func/asmops.cpp
        func/builtins.cpp
        func/stack-transform.cpp
        func/optimize.cpp
        func/codegen.cpp
)

add_executable(func func/func.cpp ${FUNC_LIB_SOURCE})
target_include_directories(func PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(func PUBLIC ton_crypto src_parser)
if (WINGETOPT_FOUND)
    target_link_libraries_system(func wingetopt)
endif()