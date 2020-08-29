cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(TON_CRYPTO_SOURCE
        tvm/stack.cpp
        tvm/atom.cpp
        tvm/continuation.cpp
        tvm/dict.cpp
        tvm/memo.cpp
        tvm/dispatch.cpp
        tvm/opctable.cpp
        tvm/cp0.cpp
        tvm/stackops.cpp
        tvm/tupleops.cpp
        tvm/arithops.cpp
        tvm/cellops.cpp
        tvm/contops.cpp
        tvm/dictops.cpp
        tvm/debugops.cpp
        tvm/tonops.cpp
        tvm/boc.cpp
        tvm/utils.cpp
        tvm/vm.cpp

        tvm/arithops.h
        tvm/atom.h
        tvm/boc.h
        tvm/box.hpp
        tvm/cellops.h
        tvm/continuation.h
        tvm/contops.h
        tvm/cp0.h
        tvm/debugops.h
        tvm/dict.h
        tvm/dictops.h
        tvm/excno.hpp
        tvm/fmt.hpp
        tvm/log.h
        tvm/memo.h
        tvm/opctable.h
        tvm/stack.hpp
        tvm/stackops.h
        tvm/tupleops.h
        tvm/tonops.h
        tvm/vmstate.h
        tvm/utils.h
        tvm/vm.h

        tvm/cells.h
        tvm/cellslice.h

        tvm/cells/Cell.cpp
        tvm/cells/CellBuilder.cpp
        tvm/cells/CellHash.cpp
        tvm/cells/CellSlice.cpp
        tvm/cells/CellString.cpp
        tvm/cells/CellTraits.cpp
        tvm/cells/CellUsageTree.cpp
        tvm/cells/DataCell.cpp
        tvm/cells/LevelMask.cpp
        tvm/cells/MerkleProof.cpp
        tvm/cells/MerkleUpdate.cpp

        tvm/cells/Cell.h
        tvm/cells/CellBuilder.h
        tvm/cells/CellHash.h
        tvm/cells/CellSlice.h
        tvm/cells/CellString.h
        tvm/cells/CellTraits.h
        tvm/cells/CellUsageTree.h
        tvm/cells/CellWithStorage.h
        tvm/cells/DataCell.h
        tvm/cells/ExtCell.h
        tvm/cells/LevelMask.h
        tvm/cells/MerkleProof.h
        tvm/cells/MerkleUpdate.h
        tvm/cells/PrunnedCell.h
        tvm/cells/UsageCell.h
        tvm/cells/VirtualCell.h
        tvm/cells/VirtualizationParameters.h

        tvm/db/StaticBagOfCellsDb.h
        tvm/db/StaticBagOfCellsDb.cpp
)

set(TON_DB_SOURCE
        tvm/db/DynamicBagOfCellsDb.cpp
        tvm/db/CellStorage.cpp
        tvm/db/TonDb.cpp

        tvm/db/DynamicBagOfCellsDb.h
        tvm/db/CellHashTable.h
        tvm/db/CellStorage.h
        tvm/db/TonDb.h
)

set(TONDB_TEST_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test-db.cpp
        PARENT_SCOPE
)

set(CELLS_TEST_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/test-cells.cpp
        PARENT_SCOPE
        )

set(TONVM_TEST_SOURCE
        ${CMAKE_CURRENT_SOURCE_DIR}/test/vm.cpp
        PARENT_SCOPE
        )

add_library(ton_db STATIC ${TON_DB_SOURCE})
target_include_directories(ton_db PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(ton_db PUBLIC tdutils tddb ton_crypto)

