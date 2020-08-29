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
)


set(ED25519_TEST_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/test/Ed25519.cpp
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






