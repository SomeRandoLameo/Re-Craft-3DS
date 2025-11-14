cmake_minimum_required(VERSION 3.22)

# Ini
add_library(ini STATIC ini/ini.c)
target_include_directories(ini PUBLIC ini)

# Lodepng
add_library(lodepng STATIC lodepng/lodepng.cpp)
target_include_directories(lodepng PUBLIC lodepng)

# Miniz
add_library(miniz STATIC miniz/miniz.c)
target_include_directories(miniz PUBLIC miniz)

# Mpack
add_library(mpack STATIC mpack/mpack.c)
target_include_directories(mpack PUBLIC mpack)

# Sino
add_library(sino STATIC sino/sino.c)
target_include_directories(sino PUBLIC .)

# Vec
add_library(vec STATIC vec/vec.c)
target_include_directories(vec PUBLIC vec)

# ImGui
add_subdirectory(imgui-ctr)

# STB Libraries
include_directories(stb)

# MCLIB
add_subdirectory(mclib)