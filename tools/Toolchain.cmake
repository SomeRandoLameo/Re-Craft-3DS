cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(3DS_IP "" CACHE STRING "3ds IP (make send)")

if (ALLWARNINGS OR EXTRAWARNINGS)
    add_compile_options(-Wall)
    if (EXTRAWARNINGS)
        add_compile_options(
            -Wextra -Wpedantic -Wconversion -Wshadow -Wundef
            -Wdouble-promotion -Wformat=2 -Wstrict-overflow=5
        )
    endif()
else()
    if (NOT WARNINGS)
        add_compile_options(
            -Wno-abi -Wno-deprecated-declarations -Wno-unused-result
            $<$<COMPILE_LANGUAGE:CXX>:-Wno-changes-meaning>
        )
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options ( -Ofast -flto=1 ) # -Ofast has -O3 with extras
    add_link_options    ( -Ofast -flto=auto ) # lto takes a while, release only
endif()

# Shader
find_program(PICASSO NAMES picasso REQUIRED)
function(make_shader arg1 arg2)
    # These only exist cause i was stupid
    set(__FILE ${arg1})
    set(__NAME ${arg2})
    execute_process(
        COMMAND ${PICASSO} -o "${CMAKE_BINARY_DIR}/${__NAME}.shbin" "${__FILE}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    dkp_add_embedded_binary_library(${__NAME}_asm "${CMAKE_BINARY_DIR}/${__NAME}.shbin")
endfunction()
