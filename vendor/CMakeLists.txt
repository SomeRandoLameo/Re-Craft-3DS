cmake_minimum_required(VERSION 3.22)

add_library(ini STATIC ini/ini.c)
target_include_directories(ini PUBLIC ini)

add_library(lodepng STATIC lodepng/lodepng.cpp)
target_include_directories(lodepng PUBLIC lodepng)

add_library(miniz STATIC miniz/miniz.c)
target_include_directories(miniz PUBLIC miniz)

add_library(mpack STATIC mpack/mpack.c)
target_include_directories(mpack PUBLIC mpack)

add_library(sino STATIC sino/sino.c)
target_include_directories(sino PUBLIC .)

add_library(vec STATIC vec/vec.c)
target_include_directories(vec PUBLIC vec)