cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

if (NOT OPENSSL_FOUND)
  find_package(OpenSSL REQUIRED)
endif()

set(TON_CRYPTO_SOURCE
  Ed25519.cpp

  common/bigint.cpp
  common/refcnt.cpp
  common/refint.cpp
  common/bigexp.cpp
  common/bitstring.cpp
  common/base64.cpp
  common/adnl_id_encode.cpp

  ellcurve/Ed25519.cpp
  ellcurve/Fp25519.cpp
  ellcurve/Montgomery.cpp
  ellcurve/TwEdwards.cpp

  openssl/bignum.cpp
  openssl/residue.cpp
  openssl/rand.cpp

  vm/stack.cpp
  vm/atom.cpp
  vm/continuation.cpp
  vm/dict.cpp
  vm/memo.cpp
  vm/dispatch.cpp
  vm/opctable.cpp
  vm/cp0.cpp
  vm/stackops.cpp
  vm/tupleops.cpp
  vm/arithops.cpp
  vm/cellops.cpp
  vm/contops.cpp
  vm/dictops.cpp
  vm/debugops.cpp
  vm/tonops.cpp
  vm/boc.cpp
  vm/utils.cpp
  vm/vm.cpp

  tl/tlblib.cpp

  Ed25519.h

  common/AtomicRef.h
  common/bigint.hpp
  common/bitstring.h
  common/refcnt.hpp
  common/refint.h
  common/bigexp.h
  common/base64.h
  common/adnl_id_encode.h
  common/linalloc.hpp
  common/promiseop.hpp

  ellcurve/Ed25519.h
  ellcurve/Fp25519.h
  ellcurve/Montgomery.h
  ellcurve/TwEdwards.h

  openssl/bignum.h
  openssl/digest.hpp
  openssl/rand.hpp
  openssl/residue.h

  tl/tlbc-aux.h
  tl/tlbc-data.h
  tl/tlblib.hpp

  vm/arithops.h
  vm/atom.h
  vm/boc.h
  vm/box.hpp
  vm/cellops.h
  vm/continuation.h
  vm/contops.h
  vm/cp0.h
  vm/debugops.h
  vm/dict.h
  vm/dictops.h
  vm/excno.hpp
  vm/fmt.hpp
  vm/log.h
  vm/memo.h
  vm/opctable.h
  vm/stack.hpp
  vm/stackops.h
  vm/tupleops.h
  vm/tonops.h
  vm/vmstate.h
  vm/utils.h
  vm/vm.h

  vm/cells.h
  vm/cellslice.h

  vm/cells/Cell.cpp
  vm/cells/CellBuilder.cpp
  vm/cells/CellHash.cpp
  vm/cells/CellSlice.cpp
  vm/cells/CellString.cpp
  vm/cells/CellTraits.cpp
  vm/cells/CellUsageTree.cpp
  vm/cells/DataCell.cpp
  vm/cells/LevelMask.cpp
  vm/cells/MerkleProof.cpp
  vm/cells/MerkleUpdate.cpp

  vm/cells/Cell.h
  vm/cells/CellBuilder.h
  vm/cells/CellHash.h
  vm/cells/CellSlice.h
  vm/cells/CellString.h
  vm/cells/CellTraits.h
  vm/cells/CellUsageTree.h
  vm/cells/CellWithStorage.h
  vm/cells/DataCell.h
  vm/cells/ExtCell.h
  vm/cells/LevelMask.h
  vm/cells/MerkleProof.h
  vm/cells/MerkleUpdate.h
  vm/cells/PrunnedCell.h
  vm/cells/UsageCell.h
  vm/cells/VirtualCell.h
  vm/cells/VirtualizationParameters.h

  vm/db/StaticBagOfCellsDb.h
  vm/db/StaticBagOfCellsDb.cpp
)

set(TON_DB_SOURCE
  vm/db/DynamicBagOfCellsDb.cpp
  vm/db/CellStorage.cpp
  vm/db/TonDb.cpp

  vm/db/DynamicBagOfCellsDb.h
  vm/db/CellHashTable.h
  vm/db/CellStorage.h
  vm/db/TonDb.h
)



set(ED25519_TEST_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/test/Ed25519.cpp
  PARENT_SCOPE
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





add_library(ton_crypto STATIC ${TON_CRYPTO_SOURCE})
target_include_directories(ton_crypto PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(ton_crypto PUBLIC ${OPENSSL_CRYPTO_LIBRARY} tdutils tddb_utils)
if (NOT WIN32)
  target_link_libraries(ton_crypto PUBLIC dl z)
endif()
target_include_directories(ton_crypto SYSTEM PUBLIC $<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIR}>)

add_library(ton_db STATIC ${TON_DB_SOURCE})
target_include_directories(ton_db PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>)
target_link_libraries(ton_db PUBLIC tdutils tddb ton_crypto)



add_executable(test-ed25519-crypto test/test-ed25519-crypto.cpp)
target_include_directories(test-ed25519-crypto PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(test-ed25519-crypto PUBLIC ton_crypto)



add_executable(tlbc tl/tlbc.cpp)
target_include_directories(tlbc PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(tlbc PUBLIC ton_crypto src_parser)
if (WINGETOPT_FOUND)
  target_link_libraries_system(tlbc wingetopt)
else()
  message("something probably went wrong with tl/tlbc")
endif()



add_library(pow-miner-lib util/Miner.cpp util/Miner.h)
target_include_directories(pow-miner-lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(pow-miner-lib PUBLIC ton_crypto ton_block)

add_executable(pow-miner util/pow-miner.cpp)
target_link_libraries(pow-miner PRIVATE ton_crypto ton_block pow-miner-lib)

if (WINGETOPT_FOUND)
  target_link_libraries_system(pow-miner wingetopt)
endif()



