cmake_minimum_required(VERSION 3.22)
include(${CMAKE_SOURCE_DIR}/tools/Toolchain.cmake)

project(Re-Craft-3DS)

# Meta
set(APP_NAME "Re-Craft-3DS")
set(APP_DESC "a Minecraft 1.10.x - 1.12.x client implemented on the 3DS (and new 3DS)")
set(APP_AUTHOR "SomeRandoLameo")
set(APP_ICON "${CMAKE_SOURCE_DIR}/icon.png")
set(APP_ROMFS "${CMAKE_SOURCE_DIR}/romfs")

# Shader
make_shader("${CMAKE_SOURCE_DIR}/source/recraft/rendering/world.v.pica" "world")
make_shader("${CMAKE_SOURCE_DIR}/source/recraft/rendering/gui.v.pica" "gui")

# Vendor
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor)

# Project
file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/source/*.c ${CMAKE_SOURCE_DIR}/source/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include/recraft
    ${DEVKITPRO}/portlibs/3ds/include
    ${DEVKITPRO}/portlibs/3ds/include/opus
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    m
    z
    ctru
    citro3d
    mclib
    sino
    ini
    mpack
    miniz
    imgui-ctr
    stb
    world_asm # World Shader
    gui_asm # gui shader
)
add_dependencies(${PROJECT_NAME} world_asm gui_asm imgui_impl_c3d_asm)

# Generate 3DSX
ctr_generate_smdh(
    ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh
    NAME "${APP_NAME}"
    DESCRIPTION "${APP_DESC}"
    AUTHOR "${APP_AUTHOR}"
    ICON "${APP_ICON}"
)
ctr_create_3dsx(
    ${PROJECT_NAME}
    OUTPUT "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.3dsx"
    SMDH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh"
    ROMFS "${APP_ROMFS}"
    DEPENDS create_resources
)

# Command to send to console (make send)
add_custom_target(
    send
    COMMAND 3dslink -a "${3DS_IP}" "${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.3dsx"
)
add_subdirectory(${CMAKE_SOURCE_DIR}/server)

