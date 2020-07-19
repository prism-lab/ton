cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

add_subdirectory(tdutils)
add_subdirectory(memprof)
add_subdirectory(tdactor)
add_subdirectory(tdnet)
add_subdirectory(tddb)
add_subdirectory(tdtl)
add_subdirectory(tl)
add_subdirectory(terminal)
add_subdirectory(keys)
add_subdirectory(networking)
add_subdirectory(crypto)

#BEGIN tonlib
add_subdirectory(tonlib)
#END tonlib

#BEGIN internal
if (NOT TON_ONLY_TONLIB)
    add_subdirectory(common)
    add_subdirectory(tdfec)
    add_subdirectory(keyring)
    add_subdirectory(fec)
    add_subdirectory(dht)
    add_subdirectory(catchain)
    add_subdirectory(validator-session)
    add_subdirectory(validator)
    add_subdirectory(storage)
    add_subdirectory(http)
endif()
#END internal
