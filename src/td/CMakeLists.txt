cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

# There is no need for tdutils to generate mime type conversion.
# tdutils is actually a git subrepo, so I define this statement here. Sorry.
set(TDUTILS_MIME_TYPE OFF CACHE BOOL "Generate mime type conversion")
if (NOT CMAKE_CROSSCOMPILING)
    if (TDUTILS_MIME_TYPE)
        set(TDMIME_AUTO tdmime_auto)
    endif()
    #add_custom_target(prepare_cross_compiling DEPENDS tl_generate_common tlb_generate_block gen_fif ${TDMIME_AUTO})
endif()

#FIXME: cyka.
set(CRC32C_FOUND 1)

add_subdirectory(tdutils)