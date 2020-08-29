cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

#TODO: add these libs as git repos

#[[   #FIXME:
if (TON_USE_ABSEIL)
  message("Add abseil-cpp")
  add_subdirectory(third-party/abseil-cpp EXCLUDE_FROM_ALL)
  set(ABSL_FOUND 1)
endif()
]]

# Эти две строки пришли из тона уже закомменченные.
#add_subdirectory(third-party/libcuckoo EXCLUDE_FROM_ALL)
#add_subdirectory(third-party/junction EXCLUDE_FROM_ALL)

if (WIN32) #True on windows systems including win64.
    message("Add wingetopt")
    add_subdirectory(wingetopt EXCLUDE_FROM_ALL)
    set(WINGETOPT_FOUND TRUE)
    message(STATUS "Use wingetopt")
endif()

# crc32c library
set(CRC32C_BUILD_TESTS OFF CACHE BOOL "Build CRC32C's unit tests")
set(CRC32C_BUILD_BENCHMARKS OFF CACHE BOOL "Build CRC32C's benchmarks")
set(CRC32C_USE_GLOG OFF CACHE BOOL "Build CRC32C's tests with Google Logging")
set(CRC32C_INSTALL OFF CACHE BOOL "Install CRC32C's header and library")
message("Add crc32c")
add_subdirectory(crc32c EXCLUDE_FROM_ALL)



#[[ FIXME:
if (TON_USE_ROCKSDB)
  if (ANDROID)
    set(PORTABLE ON CACHE BOOL "portable")
  endif()
  set(WITH_GFLAGS OFF CACHE BOOL "build with GFlags")
  set(WITH_TESTS OFF CACHE BOOL "build with tests")
  set(WITH_TOOLS OFF CACHE BOOL "build with tools")
  set(FAIL_ON_WARNINGS OFF CACHE BOOL "fail on warnings")
  message("Add rocksdb")
  #add_subdirectory(third-party/rocksdb EXCLUDE_FROM_ALL)
endif()


]]

option(USE_LIBRAPTORQ "use libraptorq for tests" OFF)
if (USE_LIBRAPTORQ)
    set(USE_LZ4 OFF CACHE BOOL "use lz4")
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(CLANG_STDLIB "ON") # for libraptorq
    endif()
    message("Add libraptorq")
    add_subdirectory(third-party/libraptorq EXCLUDE_FROM_ALL)
endif()

