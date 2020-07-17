cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

add_executable(adjust-block adjust-block.cpp)
target_link_libraries(adjust-block PUBLIC ton_crypto fift-lib ton_block)
if (WINGETOPT_FOUND)    # Passed here from the main CMakeLists.txt file.
    target_link_libraries_system(adjust-block wingetopt)
endif()
