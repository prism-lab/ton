cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(TON-Rocks
        VERSION 0.5 # Listing the version is nice here since it sets lots of useful variables
        DESCRIPTION "Telegram Open Network (TON) maintained by ton-rocks team"
        LANGUAGES C CXX # not sure if we use C, but let it be here
)

#TODO: do we need this option?
# The POSITION_INDEPENDENT_CODE property
# determines whether position independent executables or shared libraries will be created.
# This property is True by default for SHARED and MODULE library targets and False otherwise.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#FIXME: Not sure if we can effort this because of openSSL licensing.
#set(OPENSSL_USE_STATIC_LIBS TRUE)

# Prevent in-source build
get_filename_component(TON_REAL_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" REALPATH)
get_filename_component(TON_REAL_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}" REALPATH)

if (TON_REAL_BINARY_DIR STREQUAL TON_REAL_SOURCE_DIR)
  message("  Out-of-source build should be used to build TON.")
  message("  You need to remove the files already created by CMake and")
  message("  rerun CMake from a new directory:")
  message("  rm -rf CMakeFiles CMakeCache.txt")
  message("  mkdir build")
  message("  cd build")
  message("  cmake ..")
  message(FATAL_ERROR "In-source build failed.")
endif()


# Projects options:
option(TON_USE_ROCKSDB    "Use \"ON\" to enable RocksDb."                     OFF )
option(TON_USE_ABSEIL     "Use \"ON\" to enable Abseil."                      OFF )
option(TON_USE_JEMALLOC   "Use \"ON\" to enable JeMalloc."                    OFF )
option(TONLIB_ENABLE_JNI  "Use \"ON\" to enable JNI-compatible TonLib API."       )
option(TON_USE_ASAN       "Use \"ON\" to enable AddressSanitizer."            OFF )
option(TON_USE_TSAN       "Use \"ON\" to enable ThreadSanitizer."             OFF )
option(TON_USE_UBSAN      "Use \"ON\" to enable UndefinedBehaviorSanitizer."  OFF )
option(ENABLE_TESTS       "Use \"OFF\" to disable tests"                      ON  )



#[[
TODO: Удалить этот кусок нахуй,
      но сперва убедиться, что `HAVE_SSE42` нигде не используется.
##################################### это копипаст из crc32c/cmakelists.txt ##########################

# HAVE_SSE42 for crc32c and rocksdb
include(CheckCXXSourceCompiles)
# Check for SSE4.2 support in the compiler.
set(OLD_CMAKE_REQURED_FLAGS ${CMAKE_REQUIRED_FLAGS})
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} /arch:AVX")
else(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -msse4.2")
endif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
check_cxx_source_compiles("
#if defined(_MSC_VER)
#include <intrin.h>
#else  // !defined(_MSC_VER)
#include <cpuid.h>
#include <nmmintrin.h>
#endif  // defined(_MSC_VER)

int main() {
  _mm_crc32_u8(0, 0); _mm_crc32_u32(0, 0);
#if defined(_M_X64) || defined(__x86_64__)
   _mm_crc32_u64(0, 0);
#endif // defined(_M_X64) || defined(__x86_64__)
  return 0;
}
"  CRC32C_HAVE_SSE42)
set(CMAKE_REQUIRED_FLAGS ${OLD_CMAKE_REQURED_FLAGS})
##################################### end это копипаст из crc32c/cmakelists.txt ###########################

#################################### а этот кусок вообще из rocksdb/cmake
if(NOT MSVC)
  set(CMAKE_REQUIRED_FLAGS "-msse4.2 -mpclmul")
endif()
CHECK_CXX_SOURCE_COMPILES("
#include <cstdint>
#include <nmmintrin.h>
#include <wmmintrin.h>
int main() {
  volatile uint32_t x = _mm_crc32_u32(0, 0);
  const auto a = _mm_set_epi64x(0, 0);
  const auto b = _mm_set_epi64x(0, 0);
  const auto c = _mm_clmulepi64_si128(a, b, 0x00);
  auto d = _mm_cvtsi128_si64(c);
}
" ROCKSDB_HAVE_SSE42)
unset(CMAKE_REQUIRED_FLAGS)
####################################### вот до сюда
if (ROCKSDB_HAVE_SSE42 AND CRC32C_HAVE_SSE42)
  set(HAVE_SSE42 TRUE)
else()
  set(HAVE_SSE42 FALSE)
endif()
]]

#TODO: do we need this?
set(TON_ARCH "native" CACHE STRING "Architecture, will be passed to -march=")

#[[ FIXME CHECKME LOVEME
option(USE_COROUTINES "experimental support of coroutines" OFF)
if (USE_COROUTINES)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(TD_HAVE_COROUTINES 1)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcoroutines-ts")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif()


message("Add ton")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake" ${CMAKE_MODULE_PATH})

# Configure CCache if available
find_program(CCACHE_FOUND ccache)
#set(CCACHE_FOUND 0)
if (CCACHE_FOUND)
  message(STATUS "Found ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
else()
  message(STATUS "Could NOT find ccache")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(GCC 1)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CLANG 1)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  set(INTEL 1)
elseif (NOT MSVC)
  message(FATAL_ERROR "Compiler isn't supported")
endif()

include(CheckCXXCompilerFlag)

if (GCC OR CLANG OR INTEL)
  if (WIN32 AND INTEL)
    set(STD14_FLAG /Qstd=c++14)
  else()
    set(STD14_FLAG -std=c++14)
  endif()
  check_cxx_compiler_flag(${STD14_FLAG} HAVE_STD14)
  if (NOT HAVE_STD14)
    string(REPLACE "c++14" "c++1y" STD14_FLAG "${STD14_FLAG}")
    check_cxx_compiler_flag(${STD14_FLAG} HAVE_STD1Y)
    set(HAVE_STD14 ${HAVE_STD1Y})
  endif()
elseif (MSVC)
  set(HAVE_STD14 MSVC_VERSION>=1900)
endif()

if (NOT HAVE_STD14)
  message(FATAL_ERROR "No C++14 support in the compiler. Please upgrade the compiler.")
endif()

set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

if (TON_ARCH AND NOT MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=${TON_ARCH}")
endif()
if (THREADS_HAVE_PTHREAD_ARG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

if (TON_USE_JEMALLOC)
  find_package(JeMalloc REQUIRED)
endif()

set(MEMPROF "" CACHE STRING "Use one of \"ON\", \"FAST\" or \"SAFE\" to enable memory profiling. \
Works under macOS and Linux when compiled using glibc. \
In FAST mode stack is unwinded only using frame pointers, which may fail. \
In SAFE mode stack is unwinded using backtrace function from execinfo.h, which may be very slow. \
By default both methods are used to achieve maximum speed and accuracy")

if (CLANG OR GCC)
  if (MEMPROF)
    check_cxx_compiler_flag(-no-pie CXX_NO_PIE_FLAG)
    if (CXX_NO_PIE_FLAG)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -no-pie")
    elseif (APPLE)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-no_pie")
    endif()
  endif()
endif()

if (MSVC)
  if (CMAKE_CXX_FLAGS_DEBUG MATCHES "/RTC1")
    string(REPLACE "/RTC1" " " CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  endif()
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /W4 /wd4100 /wd4127 /wd4324 /wd4456 /wd4457 /wd4458 /wd4505 /wd4702")
elseif (CLANG OR GCC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STD14_FLAG} -fno-omit-frame-pointer")
  if (APPLE)
    #use "-Wl,-exported_symbols_list,${CMAKE_CURRENT_SOURCE_DIR}/export_list" for exported symbols
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fvisibility=hidden -Wl,-dead_strip,-x,-S")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -fvisibility=hidden -Wl,-dead_strip,-x,-S")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -Wl,--exclude-libs,ALL")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
    if (NOT TON_USE_ASAN AND NOT TON_USE_TSAN AND NOT MEMPROF)
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--exclude-libs,ALL")
    endif()
  endif()
elseif (INTEL)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STD14_FLAG}")
endif()

if (WIN32)
  add_definitions(-DNTDDI_VERSION=0x06020000 -DWINVER=0x0602 -D_WIN32_WINNT=0x0602 -DNOMINMAX -DUNICODE -D_UNICODE)
endif()
if (CYGWIN)
  add_definitions(-D_DEFAULT_SOURCE=1 -DFD_SETSIZE=4096)
endif()

if (NOT ANDROID) # _FILE_OFFSET_BITS is broken in ndk r15 and r15b and doesn't work prior to Android 7.0
  add_definitions(-D_FILE_OFFSET_BITS=64)
endif()

set(INTERNAL_COMPILE "0")
#BEGIN internal
  add_definitions(-D_INTERNAL_COMPILE=1)
  set(INTERNAL_COMPILE "1")
#END internal

set(TONLIB_COMPILE "0")
#BEGIN tonlib
  add_definitions(-D_TONLIB_COMPILE=1)
  set(TONLIB_COMPILE "1")
#END tonlib

include(AddCXXCompilerFlag)
if (MSVC)
  add_cxx_compiler_flag("/experimental:external /external:anglebrackets /external:W0")
endif()
if (NOT MSVC)
  add_cxx_compiler_flag("-Wall")
endif()
add_cxx_compiler_flag("-Wextra")
add_cxx_compiler_flag("-Wimplicit-fallthrough=2")
add_cxx_compiler_flag("-Wpointer-arith")
add_cxx_compiler_flag("-Wcast-qual")
add_cxx_compiler_flag("-Wsign-compare")
add_cxx_compiler_flag("-Wduplicated-branches")
add_cxx_compiler_flag("-Wduplicated-cond")
add_cxx_compiler_flag("-Walloc-zero")
add_cxx_compiler_flag("-Wlogical-op")
add_cxx_compiler_flag("-Wno-tautological-compare")
add_cxx_compiler_flag("-Wpointer-arith")
add_cxx_compiler_flag("-Wvla")
add_cxx_compiler_flag("-Wnon-virtual-dtor")
add_cxx_compiler_flag("-Wno-unused-parameter")
add_cxx_compiler_flag("-Wconversion")
add_cxx_compiler_flag("-Wno-sign-conversion")
add_cxx_compiler_flag("-Qunused-arguments")
add_cxx_compiler_flag("-Wno-unused-private-field")
add_cxx_compiler_flag("-Wno-redundant-move")
#add_cxx_compiler_flag("-Werror")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem /usr/include/c++/v1")
if (CLANG)
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()
if (TON_USE_ASAN)
  if (CLANG)
    add_cxx_compiler_flag("-stdlib=libc++")
  endif()
  add_cxx_compiler_flag("-fsanitize=address")
  add_definitions(-DTD_USE_ASAN=1)
endif()
if (TON_USE_TSAN)
  if (CLANG)
    add_cxx_compiler_flag("-stdlib=libc++")
  endif()
  add_cxx_compiler_flag("-fsanitize=thread")
endif()
if (TON_USE_UBSAN)
  if (CLANG)
    add_cxx_compiler_flag("-stdlib=libc++")
  endif()
  add_cxx_compiler_flag("-fsanitize=undefined")
endif()
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=leak")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finstrument-functions")

#Compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
]]

#TODO: Do We really need to have latex docs or we can just convert them to something like markdown once?
find_package(LATEX)
if (LATEX_FOUND)
  include(UseLATEX)
  add_latex_document(docs/ton.tex TARGET_NAME ton_white_paper)
  add_latex_document(docs/tvm.tex TARGET_NAME ton_vm_description)
  add_latex_document(docs/tblkch.tex TARGET_NAME ton_blockchain_description)
  add_latex_document(docs/fiftbase.tex TARGET_NAME fift_basic_description)
  add_latex_document(docs/catchain.tex TARGET_NAME catchain_consensus_description)
else()
  message(" LATEX not found.")
  message(" Docs will not be built")
endif()

#[[ FIXME:
function(target_link_libraries_system target)
  set(libs ${ARGN})
  foreach(lib ${libs})
    get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${target} SYSTEM PUBLIC ${lib_include_dirs})
    target_link_libraries(${target} PUBLIC ${lib})
  endforeach(lib)
endfunction(target_link_libraries_system)

if (TON_USE_ROCKSDB)
  option(TDDB_USE_ROCKSDB "Use rockdb" ON)
endif()
]]

#TODO: apps dir
#add_subdirectory(apps)



set(WINGETOPT_FOUND FALSE)
add_subdirectory(third-party)

add_subdirectory(src)


# Testing
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND ${ENABLE_TESTS})
  enable_testing()
  add_subdirectory(test)
endif()