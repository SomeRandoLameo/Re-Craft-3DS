cmake_minimum_required(VERSION 3.22)

project(Re-Craft-3DS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(3DS_IP "" CACHE STRING "3ds IP (make send)")

set(APP_NAME "Re-Craft-3DS")
set(APP_DESC "a Minecraft client implemented on the 3DS (and new 3DS)")
set(APP_AUTHOR "SomeRandoLameo")
set(APP_ICON "${CMAKE_SOURCE_DIR}/icon.png")
set(APP_ROMFS "${CMAKE_SOURCE_DIR}/romfs")

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
    source/recraft/gui/GuiChat.cpp
    source/recraft/inventory/ItemStack.cpp
    source/recraft/mcbridge/MCBridge.cpp
    source/recraft/misc/Collision.cpp
    source/recraft/misc/CommandLine.cpp
    source/recraft/misc/Crash.cpp
    source/recraft/misc/Raycast.cpp
    source/recraft/ReCraftCore.cpp
    source/recraft/rendering/Camera.cpp
    source/recraft/rendering/Frustum.cpp
    source/recraft/rendering/Clouds.cpp
    source/recraft/rendering/Cursor.cpp
    source/recraft/rendering/Hand.cpp
    source/recraft/rendering/PolyGen.cpp
    source/recraft/rendering/Renderer.cpp
    source/recraft/rendering/TextureMap.cpp
    source/recraft/rendering/VBOCache.cpp
    source/recraft/rendering/WorldRenderer.cpp
    source/recraft/world/ChunkWorker.cpp
    source/recraft/world/CT_Chunk.cpp
    source/recraft/world/CT_World.cpp
    source/recraft/world/Direction.cpp
    source/recraft/world/savegame/SaveManager.cpp
    source/recraft/world/savegame/SuperChunk.cpp
    source/recraft/world/WorkQueue.cpp
    source/recraft/world/worldgen/SmeaGen.cpp
    source/recraft/world/worldgen/SuperFlatGen.cpp
    source/recraft/gui/ImGuiManager.cpp
    source/recraft/world/OnlineWorld.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
    include/recraft
    ${DEVKITPRO}/portlibs/3ds/include
    ${DEVKITPRO}/portlibs/3ds/include/opus
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
    mpack
    miniz
    imgui-ctr
    stb
    amethyst
)

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

# Command to send to console (make send)
add_custom_target(
    send
    COMMAND 3dslink -a "${3DS_IP}" "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.3dsx"
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
        COMMAND ${CMAKE_COMMAND} -E echo "Starting Minecraft server..."
        COMMAND java -Xmx1024M -Xms1024M -jar server.jar
        WORKING_DIRECTORY "${SERVER_DIR}"
        COMMENT "Starting Minecraft server"
        USES_TERMINAL
)