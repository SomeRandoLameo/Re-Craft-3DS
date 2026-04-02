cmake_minimum_required(VERSION 3.22)

execute_process(
    COMMAND git rev-parse --short HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_SHORT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

find_program(MAKEROM makerom)
find_program(BANNERTOOL bannertool)

project(Re-Craft-3DS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

set(3DS_IP "" CACHE STRING "3ds IP (make send)")
option(RC_BUILD_WITHOUT_ASSETS OFF)
option(RC_ENABLE_LOGS OFF) # SD Destroyer

set(APP_NAME "Re-Craft-3DS")
set(APP_DESC "a Minecraft client implemented on the 3DS (and new 3DS)")
set(APP_AUTHOR "SomeRandoLameo")
set(APP_ICON "${CMAKE_SOURCE_DIR}/app/icon.png")
set(APP_ROMFS "${CMAKE_SOURCE_DIR}/romfs")

# CIA Stuff
set(APP_BANNER "${CMAKE_SOURCE_DIR}/app/banner.cgfx")
set(APP_BANNERAUDIO "${CMAKE_SOURCE_DIR}/app/banner-audio.wav")
set(APP_RSF "${CMAKE_SOURCE_DIR}/app/build-cia.rsf")
set(APP_LOGO "${CMAKE_SOURCE_DIR}/app/splash.lz")

add_subdirectory(vendor)

add_executable(${PROJECT_NAME}
    source/main.cpp
    source/recraft/block/BlockEvent.cpp
    source/recraft/block/Block.cpp
    source/recraft/client/entity/Damage.cpp
    source/recraft/client/entity/Entity.cpp
    source/recraft/client/entity/Player.cpp
    source/recraft/client/gui/CT_Inventory.cpp
    source/recraft/client/gui/DebugUI.cpp
    source/recraft/client/gui/Gui.cpp
    source/recraft/client/gui/GuiChat.cpp
    source/recraft/input/InputManager.cpp
    source/recraft/input/PlayerInput.cpp
    source/recraft/inventory/ItemStack.cpp
    source/recraft/mcbridge/MCBridge.cpp
    source/recraft/misc/CommandLine.cpp
    source/recraft/misc/Crash.cpp
    source/recraft/misc/Raycast.cpp
    source/recraft/misc/VecMath.cpp
    source/recraft/ReCraftCore.cpp
    source/recraft/client/renderer/Camera.cpp
    source/recraft/client/renderer/Tessellator.cpp
    source/recraft/client/renderer/Frustum.cpp
    source/recraft/client/renderer/Clouds.cpp
    source/recraft/client/model/ModelChest.cpp
    source/recraft/client/model/ModelX.cpp
    source/recraft/client/renderer/Cursor.cpp
    source/recraft/client/renderer/Hand.cpp
    source/recraft/client/renderer/PolyGen.cpp
    source/recraft/client/renderer/Renderer.cpp
    source/recraft/client/renderer/TextureMap.cpp
    source/recraft/client/renderer/VBOCache.cpp
    source/recraft/client/renderer/WorldRenderer.cpp
    #    back_mdb_src/block/ModelBlock.cpp
    #    back_mdb_src/block/ModelPool.cpp
    #    back_mdb_src/block/BlockPart.cpp
    #    back_mdb_src/block/BlockPartFace.cpp
    #    back_mdb_src/block/BlockFaceUV.cpp
    #    back_mdb_src/block/ModelResourceLocation.cpp
    #    back_mdb_src/block/ModelRotation.cpp
    #    back_mdb_src/block/Variant.cpp
    #    back_mdb_src/block/VariantList.cpp
    source/recraft/world/ChunkWorker.cpp
        source/recraft/world/Chunk.cpp
        source/recraft/world/World.cpp
    source/recraft/world/Direction.cpp
    source/recraft/world/savegame/SaveManager.cpp
    source/recraft/world/savegame/SuperChunk.cpp
    source/recraft/world/WorkQueue.cpp
    source/recraft/world/NetworkWorld.cpp
    source/recraft/world/worldgen/SmeaGen.cpp
    source/recraft/world/worldgen/EmptyGen.cpp
    source/recraft/world/worldgen/SuperFlatGen.cpp
    source/recraft/client/gui/ImGuiManager.cpp
    source/recraft/client/gui/GuiScreen.cpp
    source/recraft/client/gui/SelectWorldScreen.cpp
    source/recraft/client/gui/CreateWorldScreen.cpp
    source/recraft/client/gui/ConfirmScreen.cpp
    source/recraft/client/gui/DeleteWorldScreen.cpp
    source/recraft/client/gui/GuiInGame.cpp
    source/recraft/client/gui/StartScreen.cpp
    source/recraft/client/gui/GuiButton.cpp
    source/recraft/phys/AABB.cpp
    source/recraft/util/ResourceLocation.cpp
    #    back_mdb_src/block/ModelBakery.cpp
    source/recraft/block/material/Material.cpp
    source/recraft/block/material/MapColor.cpp
    source/recraft/item/EnumDyeColor.cpp
    source/recraft/item/Item.cpp
    source/recraft/util/text/TextFormatting.cpp
    source/recraft/block/material/MaterialLiquid.cpp
    source/recraft/block/material/MaterialTransparent.cpp
    source/recraft/block/material/MaterialLogic.cpp
    source/recraft/block/material/MaterialPortal.cpp
    source/recraft/block/material/MaterialWeb.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC
    include/recraft
    ${DEVKITPRO}/portlibs/3ds/include
    ${DEVKITPRO}/portlibs/3ds/include/opus
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    -D_GNU_SOURCE=1
    -DGIT_COMMIT="${GIT_SHORT_HASH}"
    -DGIT_BRANCH="${GIT_BRANCH}"
)
if(RC_ENABLE_LOGS)
    target_compile_definitions(${PROJECT_NAME} PUBLIC
    -DRC_LOGGING=1
)
endif()
target_compile_options(${PROJECT_NAME} PUBLIC
    -O3 -Ofast -ffunction-sections -fdata-sections
    -fomit-frame-pointer -mword-relocations
    #-fno-exceptions
    -Wno-changes-meaning
    # rip
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

if(MAKEROM AND BANNERTOOL)
    message(STATUS "Found Makerom and Bannertool. Building Cia...")

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr
        COMMAND ${BANNERTOOL} makebanner
        -ci ${APP_BANNER}
        -a ${APP_BANNERAUDIO}
        -o ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr
        DEPENDS ${APP_BANNER} ${APP_BANNERAUDIO}
        COMMENT "Creating Banner for cia file..."
        VERBATIM
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cia
        COMMAND ${MAKEROM}
        -f cia -target t -exefslogo
        -o "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cia"
        -elf "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf"
        -rsf ${APP_RSF}
        -banner "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr"
        -icon "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh"
        -logo ${APP_LOGO} -DAPP_ROMFS="${APP_ROMFS}"
        -major 1 -minor 0 -micro 0
        -DAPP_VERSION_MAJOR=1
        DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.smdh
        COMMENT "Creating Cia file..."
        VERBATIM
    )

    add_custom_target(make_banner ALL
        DEPENDS ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bnr
    )

    add_custom_target(make_cia ALL
        DEPENDS make_banner ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.3dsx ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.cia
    )
endif()

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

# Add clang-format target if clang-format was found
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    message(STATUS "Clang Format found.")
    set(SOURCES ${CMAKE_SOURCE_DIR}/source ${CMAKE_SOURCE_DIR}/include)
    set(COMMENT "Formatting Project Sources")
    if(WIN32)
        add_custom_target(clang-format
            COMMAND powershell.exe -Command "Get-ChildItem '${SOURCES}/*' -Include *.cpp,*.hpp -Recurse | Foreach {&'${CLANG_FORMAT}' -i $_.fullname}"
            COMMENT ${CCOMMENT})
    else()
        add_custom_target(clang-format
            COMMAND find ${SOURCES} -iname *.hpp -o -iname *.cpp | xargs ${CLANG_FORMAT} -i
            COMMENT ${COMMENT}
        )
    endif()
    unset(COMMENT)
endif()