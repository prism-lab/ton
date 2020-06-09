//
// Created by Prism Machine Lab on 08-Jun-20.
//
namespace td {

#ifdef _MSC_VER
/*  Workaround for `td:size_t` on MSVC compiler.
 *  GCC is OK without this statement at least on linux.
 */
  #if defined(_WIN64)
    typedef unsigned __int64 size_t;
  #else
    typedef unsigned int size_t;
  #endif
#endif

}