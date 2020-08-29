cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

set(FIFT_SOURCE
        fift/Dictionary.cpp
        fift/Fift.cpp
        fift/IntCtx.cpp
        fift/Continuation.cpp
        fift/SourceLookup.cpp
        fift/utils.cpp
        fift/words.cpp

        fift/Dictionary.h
        fift/Fift.h
        fift/IntCtx.h
        fift/Continuation.h
        fift/SourceLookup.h
        fift/utils.h
        fift/words.h
)

add_library(fift-lib ${FIFT_SOURCE})
target_include_directories(fift-lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(fift-lib PUBLIC ton_crypto ton_db tdutils ton_block)
set_target_properties(fift-lib PROPERTIES OUTPUT_NAME fift)

add_executable(fift fift/fift-main.cpp)
target_link_libraries(fift PUBLIC fift-lib)
if (WINGETOPT_FOUND)
    target_link_libraries_system(fift wingetopt)
endif()


set(FIFT_TEST_SOURCE
        fift/test/fift.cpp
        PARENT_SCOPE
)


install(TARGETS fift func RUNTIME DESTINATION bin)
install(DIRECTORY fift/lib/ DESTINATION lib/fift)
