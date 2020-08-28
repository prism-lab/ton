cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(PARSER_SOURCE
        parser/srcread.cpp
        parser/lexer.cpp
        parser/symtable.cpp

        parser/srcread.h
        parser/lexer.h
        parser/symtable.h
)

add_library(src_parser ${PARSER_SOURCE})
target_include_directories(src_parser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(src_parser PUBLIC ton_crypto)

