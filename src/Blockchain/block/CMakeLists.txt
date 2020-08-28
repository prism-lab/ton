cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(TLB_BLOCK_AUTO
        ${CMAKE_CURRENT_SOURCE_DIR}/auto/block-auto.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/auto/block-auto.h
)

set(BLOCK_SOURCE
        Binlog.h
        Binlog.cpp
        block.cpp
        block-db.cpp
        block-parse.cpp
        check-proof.cpp
        mc-config.cpp
        output-queue-merger.cpp
        transaction.cpp

        ${TLB_BLOCK_AUTO}

        block-binlog.h
        block-db-impl.h
        block-db.h
        block.h
        block-parse.h
        check-proof.h
        output-queue-merger.h
        transaction.h
        )

add_library(ton_block ${BLOCK_SOURCE})
target_include_directories(ton_block PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/block> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(ton_block PUBLIC ton_crypto tdutils tdactor tl_api)


# Code generation:
file(MAKE_DIRECTORY auto/)

set(TURN_OFF_LSAN cd .)
if (TON_USE_ASAN AND NOT WIN32)
    set(TURN_OFF_LSAN export LSAN_OPTIONS=detect_leaks=0)
endif()

if (NOT CMAKE_CROSSCOMPILING)
    set(GENERATE_TLB_CMD tlbc)
    add_custom_command(
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/block
            COMMAND ${TURN_OFF_LSAN}
            COMMAND ${GENERATE_TLB_CMD} -o block-auto -n block::gen -z block.tlb
            COMMENT "Generate block tlb source files"
            OUTPUT ${TLB_BLOCK_AUTO}
            DEPENDS tlbc block.tlb
    )
    add_custom_target(tlb_generate_block DEPENDS ${TLB_BLOCK_AUTO})
    add_dependencies(ton_block tlb_generate_block)
endif()


