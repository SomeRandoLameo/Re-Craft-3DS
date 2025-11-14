cmake_minimum_required(VERSION 3.22)

project(Re-Craft-3DS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(APP_NAME "Re-Craft-3DS")
set(APP_DESC "a Minecraft 1.10.x - 1.12.x client implemented on the 3DS (and new 3DS)")
set(APP_AUTHOR "SomeRandoLameo")
set(APP_ICON "${CMAKE_SOURCE_DIR}/icon.png")
set(APP_ROMFS "${CMAKE_SOURCE_DIR}/romfs")

find_program(PICASSO NAMES picasso REQUIRED)

function(make_shader arg1 arg2)
    # These only exist cause i was stupid
    set(__FILE ${arg1})
    set(__NAME ${arg2})
    # Need to build shaders during config stage :(
    execute_process(
        COMMAND ${PICASSO} -o "${CMAKE_BINARY_DIR}/${__NAME}.shbin" "${__FILE}"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
    dkp_add_embedded_binary_library(${__NAME}_asm "${CMAKE_BINARY_DIR}/${__NAME}.shbin")
endfunction()

make_shader("${CMAKE_SOURCE_DIR}/source/recraft/rendering/world.v.pica" "world")
make_shader("${CMAKE_SOURCE_DIR}/source/recraft/rendering/gui.v.pica" "gui")

add_subdirectory(vendor)

add_executable(${PROJECT_NAME}
    source/main.cpp

    source/recraft/blocks/BlockEvent.cpp
    source/recraft/blocks/CT_Block.cpp
    source/recraft/entity/Damage.cpp
    source/recraft/entity/Player.cpp
    source/recraft/entity/PlayerController.cpp
    source/recraft/gui/CT_Inventory.cpp
    source/recraft/gui/DebugUI.cpp
    source/recraft/gui/FontLoader.cpp
    source/recraft/gui/Gui.cpp
    source/recraft/gui/SpriteBatch.cpp
    source/recraft/gui/WorldSelect.cpp
    source/recraft/inventory/ItemStack.cpp
    source/recraft/mcbridge/MCBridge.cpp
    source/recraft/misc/Collision.cpp
    source/recraft/misc/CommandLine.cpp
    source/recraft/misc/Crash.cpp
    source/recraft/misc/Raycast.cpp
    source/recraft/misc/Sound.cpp
    source/recraft/ReCraftCore.cpp
    source/recraft/rendering/Camera.cpp
    source/recraft/rendering/Clouds.cpp
    source/recraft/rendering/Cursor.cpp
    source/recraft/rendering/Hand.cpp
    source/recraft/rendering/PolyGen.cpp
    source/recraft/rendering/Renderer.cpp
    source/recraft/rendering/TextureMap.cpp
    source/recraft/rendering/VBOCache.cpp
    source/recraft/rendering/VertexFmt.cpp
    source/recraft/rendering/WorldRenderer.cpp
    source/recraft/world/ChunkWorker.cpp
    source/recraft/world/CT_Chunk.cpp
    source/recraft/world/CT_World.cpp
    source/recraft/world/Direction.cpp
    source/recraft/world/savegame/SaveManager.cpp
    source/recraft/world/savegame/SuperChunk.cpp
    source/recraft/world/T_Chunk.cpp
    source/recraft/world/T_World.cpp
    source/recraft/world/WorkQueue.cpp
    source/recraft/world/worldgen/SmeaGen.cpp
    source/recraft/world/worldgen/SuperFlatGen.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
    include/recraft
    ${DEVKITPRO}/portlibs/3ds/include
    ${DEVKITPRO}/portlibs/3ds/include/opus
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    USE_CURL
    _3DS
)
target_compile_options(${PROJECT_NAME} PUBLIC
    -Wno-changes-meaning
)
target_link_libraries(${PROJECT_NAME} PUBLIC
    m
    z
    ctru
    citro3d
    mclib
    sino
    ini
    vec
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
    OUTPUT "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.3dsx"
    SMDH "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh"
    ROMFS "${APP_ROMFS}"
    DEPENDS create_resources
)

# 1.12 server stuff for ease of use

set(SERVER_DIR "${CMAKE_SOURCE_DIR}/server")
set(SERVER_JAR "${SERVER_DIR}/server.jar")
set(SERVER_URL "https://piston-data.mojang.com/v1/objects/8494e844e911ea0d63878f64da9dcc21f53a3463/server.jar")

# Custom target: download-server
add_custom_target(download-server
    COMMAND ${CMAKE_COMMAND} -E make_directory "${SERVER_DIR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Downloading Minecraft server JAR..."
    COMMAND curl -L "${SERVER_URL}" -o "${SERVER_JAR}"
    COMMAND ${CMAKE_COMMAND} -E echo "Download complete: ${SERVER_JAR}"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Downloading Minecraft server JAR"
)

# Custom target: start-server
add_custom_target(start-server
    COMMAND ${CMAKE_COMMAND} -E echo "Checking for Java..."
    COMMAND bash -c "command -v java >/dev/null 2>&1 || { echo >&2 'Error: Java is not installed or not in your PATH.'; echo >&2 'Please install Java 17 or newer and try again.'; exit 1; }"
    COMMAND ${CMAKE_COMMAND} -E echo "Starting Minecraft server..."
    COMMAND bash -c "cd '${SERVER_DIR}' && java -Xmx1024M -Xms1024M -jar server.jar"
    WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    COMMENT "Starting Minecraft server"
)
