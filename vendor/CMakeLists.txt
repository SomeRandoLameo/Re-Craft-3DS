cmake_minimum_required(VERSION 3.22)

# Ini
add_library(ini STATIC ini/ini.c)
target_include_directories(ini PUBLIC ini)

# Miniz
add_library(miniz STATIC miniz/miniz.c)
target_include_directories(miniz PUBLIC miniz)

# Mpack
add_library(mpack STATIC mpack/mpack.c)
target_include_directories(mpack PUBLIC mpack)

# Sino
add_library(sino STATIC sino/sino.c)
target_include_directories(sino PUBLIC .)

# ImGui
add_subdirectory(imgui-ctr)

# STB Libraries
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)

# MCLIB
add_subdirectory(mclib)
# AMY
set(AMY_BUILD_STB_IMAGE OFF)
add_subdirectory(amethyst)
