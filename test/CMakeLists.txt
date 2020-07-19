cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

#TESTS
add_executable(test-ed25519 test-td-main.cpp ${ED25519_TEST_SOURCE})
target_link_libraries(test-ed25519 PRIVATE ton_crypto)

add_executable(test-vm test-td-main.cpp ${TONVM_TEST_SOURCE})
target_link_libraries(test-vm PRIVATE ton_crypto fift-lib)

add_executable(test-smartcont test-td-main.cpp ${SMARTCONT_TEST_SOURCE})
target_link_libraries(test-smartcont PRIVATE smc-envelope fift-lib ton_db)

add_executable(test-cells test-td-main.cpp ${CELLS_TEST_SOURCE})
target_link_libraries(test-cells PRIVATE ton_crypto)

add_executable(test-fift test-td-main.cpp ${FIFT_TEST_SOURCE})
target_link_libraries(test-fift PRIVATE fift-lib)

add_executable(test-tdutils test-td-main.cpp ${TDUTILS_TEST_SOURCE})
target_link_libraries(test-tdutils PRIVATE tdutils ${CMAKE_THREAD_LIBS_INIT} memprof ${JEMALLOC_LIBRARIES})
#target_link_libraries_system(test-tdutils absl::base absl::container absl::hash )
#target_link_libraries_system(test-tdutils libcuckoo)
#target_include_directories(test-tdutils PRIVATE SYSTEM ${JUNCTION_ALL_INCLUDE_DIRS})
#target_link_libraries(test-tdutils PRIVATE ${JUNCTION_ALL_LIBRARIES})

add_executable(test-tdactor test-td-main.cpp ${TDACTOR_TEST_SOURCE})
target_link_libraries(test-tdactor PRIVATE tdactor ${CMAKE_THREAD_LIBS_INIT})

add_executable(test-net test-td-main.cpp ${NET_TEST_SOURCE})
target_link_libraries(test-net PRIVATE tdnet tdutils ${CMAKE_THREAD_LIBS_INIT})

#BEGIN tonlib
add_executable(test-tonlib ${TONLIB_ONLINE_TEST_SOURCE})
target_link_libraries(test-tonlib tdutils tdactor adnllite tl_api ton_crypto ton_block tl_tonlib_api tonlib)

add_executable(test-tonlib-offline test-td-main.cpp ${TONLIB_OFFLINE_TEST_SOURCE})
target_link_libraries(test-tonlib-offline tdutils tdactor adnllite tl_api ton_crypto ton_block fift-lib tl_tonlib_api tonlib)

if (NOT CMAKE_CROSSCOMPILING)
    add_dependencies(test-tonlib-offline gen_fif)
endif()
#END tonlib

#BEGIN internal
if (NOT TON_ONLY_TONLIB)
    add_executable(test-db test-td-main.cpp ${TONDB_TEST_SOURCE})
    target_link_libraries(test-db PRIVATE ton_db memprof tdfec)

    add_executable(test-storage test-td-main.cpp ${STORAGE_TEST_SOURCE})
    target_link_libraries(test-storage PRIVATE storage ton_db memprof tl_api tl-utils fec rldp2)

    add_executable(test-rocksdb test-rocksdb.cpp)
    target_link_libraries(test-rocksdb PRIVATE memprof tddb tdutils)

    add_executable(test-tddb test-td-main.cpp ${TDDB_TEST_SOURCE})
    target_link_libraries(test-tddb PRIVATE tdutils tddb ${CMAKE_THREAD_LIBS_INIT} memprof)

    add_executable(test-fec test-td-main.cpp ${FEC_TEST_SOURCE})
    target_link_libraries(test-fec PRIVATE tdfec tdutils ${CMAKE_THREAD_LIBS_INIT})
    if (USE_LIBRAPTORQ)
        target_link_libraries(test-fec PRIVATE third_party_fec)
        target_compile_definitions(test-fec PRIVATE "USE_LIBRAPTORQ=1")
    endif()

    add_executable(test-hello-world test-hello-world.cpp )
    target_link_libraries(test-hello-world tl_api ton_crypto)

    add_executable(test-adnl test-adnl.cpp)
    target_link_libraries(test-adnl adnl adnltest dht tl_api)
    add_executable(test-dht test-dht.cpp)
    target_link_libraries(test-dht adnl adnltest dht tl_api)
    add_executable(test-rldp test-rldp.cpp)
    target_link_libraries(test-rldp adnl adnltest dht rldp tl_api)
    add_executable(test-rldp2 test-rldp2.cpp)
    target_link_libraries(test-rldp2 adnl adnltest dht rldp2 tl_api)
    add_executable(test-validator-session-state test-validator-session-state.cpp)
    target_link_libraries(test-validator-session-state adnl dht rldp validatorsession tl_api)

    #add_executable(test-node test-node.cpp)
    #target_link_libraries(test-node overlay tdutils tdactor adnl tl_api dht
    #  catchain validatorsession)

    add_executable(test-catchain test-catchain.cpp)
    target_link_libraries(test-catchain overlay tdutils tdactor adnl adnltest rldp tl_api dht
            catchain )
    #add_executable(test-validator-session test-validator-session.cpp)
    #target_link_libraries(test-validator-session overlay tdutils tdactor adnl tl_api dht
    #  catchain validatorsession)
    add_executable(test-ton-collator test-ton-collator.cpp)
    target_link_libraries(test-ton-collator overlay tdutils tdactor adnl tl_api dht
            catchain validatorsession validator-disk ton_validator validator-disk )
    #add_executable(test-validator test-validator.cpp)
    #target_link_libraries(test-validator overlay tdutils tdactor adnl tl_api dht
    #    rldp catchain validatorsession ton-node validator ton_validator validator memprof ${JEMALLOC_LIBRARIES})
    #add_executable(test-ext-server test-ext-server.cpp)
    #target_link_libraries(test-ext-server tdutils tdactor adnl tl_api dht )
    #add_executable(test-ext-client test-ext-client.cpp)
    #target_link_libraries(test-ext-client tdutils tdactor adnl tl_api tl-lite-utils)

    add_executable(test-http test-http.cpp)
    target_link_libraries(test-http PRIVATE tonhttp)

    get_directory_property(HAS_PARENT PARENT_DIRECTORY)
    if (HAS_PARENT)
        set(ALL_TEST_SOURCE
                ${TDUTILS_TEST_SOURCE}
                ${TDACTOR_TEST_SOURCE}
                ${NET_TEST_SOURCE}
                ${TDDB_TEST_SOURCE}
                ${FEC_TEST_SOURCE}
                ${ED25519_TEST_SOURCE}
                ${TONDB_TEST_SOURCE}
                ${CELLS_TEST_SOURCE} # ${TONVM_TEST_SOURCE} ${FIFT_TEST_SOURCE} ${TONLIB_ONLINE_TEST_SOURCE}
                PARENT_SCOPE)
    endif()
    add_library(all_tests INTERFACE)
    target_link_libraries(all_tests INTERFACE tdutils tdactor tdnet tdfec ton_db ton_crypto fift-lib)
endif()
#END internal

enable_testing()
set(TEST_OPTIONS "--regression ${CMAKE_CURRENT_SOURCE_DIR}/regression-tests.ans --filter -Bench")
separate_arguments(TEST_OPTIONS)
add_test(test-ed25519-crypto crypto/test-ed25519-crypto)
add_test(test-ed25519 test-ed25519)
add_test(test-vm test-vm ${TEST_OPTIONS})
add_test(test-fift test-fift ${TEST_OPTIONS})
add_test(test-cells test-cells ${TEST_OPTIONS})
add_test(test-smartcont test-smartcont)
add_test(test-net test-net)
add_test(test-actors test-tdactor)

#BEGIN tonlib
add_test(test-tdutils test-tdutils)
add_test(test-tonlib-offline test-tonlib-offline)
#END tonlib

#BEGIN internal
if (NOT TON_ONLY_TONLIB)
    add_test(test-adnl test-adnl)
    add_test(test-dht test-dht)
    add_test(test-rldp test-rldp)
    add_test(test-rldp2 test-rldp2)
    #add_test(test-validator-session-state test-validator-session-state)
    add_test(test-catchain test-catchain)

    add_test(test-fec test-fec)
    add_test(test-tddb test-tddb ${TEST_OPTIONS})
    add_test(test-db test-db ${TEST_OPTIONS})
endif()
#END internal

