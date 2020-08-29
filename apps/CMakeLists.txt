cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

add_subdirectory(blockchain-explorer)
add_subdirectory(dht-server)
add_subdirectory(lite-client)
add_subdirectory(rldp-http-proxy)
add_subdirectory(utils)
add_subdirectory(validator-engine)
add_subdirectory(validator-engine-console)

add_executable(adjust-block adjust-block.cpp)
target_link_libraries(adjust-block
        PUBLIC
            ton_crypto
            fift-lib
            ton_block
)
if (WINGETOPT_FOUND)
    target_link_libraries_system(adjust-block wingetopt)
endif()

add_executable(create-state create-state.cpp)
target_link_libraries(create-state
        PUBLIC
            ton_crypto
            fift-lib
            ton_block
)
if (TONLIB_COMPILE)
    target_link_libraries(create-state PUBLIC tonlib)
endif()
if (WINGETOPT_FOUND)
    target_link_libraries_system(create-state wingetopt)
endif()

add_executable(dump-block dump-block.cpp)
target_link_libraries(dump-block
        PUBLIC
            ton_crypto
            fift-lib
            ton_block
)
if (WINGETOPT_FOUND)
    target_link_libraries_system(dump-block wingetopt)
endif()

add_executable(create-hardfork create-hardfork.cpp)
target_link_libraries(create-hardfork
        overlay
        tdutils
        tdactor
        adnl
        tl_api
        dht
        rldp
        catchain
        validatorsession
        full-node
        validator-hardfork
        ton_validator
        validator-hardfork
        fift-lib
        memprof
        ${JEMALLOC_LIBRARIES}
)
install(TARGETS create-hardfork RUNTIME DESTINATION bin)
