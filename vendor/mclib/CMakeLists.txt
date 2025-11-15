cmake_minimum_required(VERSION 3.22)

project(mclib)

add_library(mclib STATIC
    source/mclib/block/Banner.cpp
    source/mclib/block/Beacon.cpp
    source/mclib/block/Bed.cpp
    source/mclib/block/Block.cpp
    source/mclib/block/BlockEntity.cpp
    source/mclib/block/BrewingStand.cpp
    source/mclib/block/Chest.cpp
    source/mclib/block/Dispenser.cpp
    source/mclib/block/Dropper.cpp
    source/mclib/block/EnchantmentTable.cpp
    source/mclib/block/EndGateway.cpp
    source/mclib/block/FlowerPot.cpp
    source/mclib/block/Furnace.cpp
    source/mclib/block/Hopper.cpp
    source/mclib/block/InventoryBlock.cpp
    source/mclib/block/Jukebox.cpp
    source/mclib/block/MonsterSpawner.cpp
    source/mclib/block/NoteBlock.cpp
    source/mclib/block/Piston.cpp
    source/mclib/block/RedstoneComparator.cpp
    source/mclib/block/ShulkerBox.cpp
    source/mclib/block/Sign.cpp
    source/mclib/block/Skull.cpp
    source/mclib/common/DataBuffer.cpp
    source/mclib/common/DyeColor.cpp
    source/mclib/common/MCString.cpp
    source/mclib/common/Position.cpp
    source/mclib/common/UUID.cpp
    source/mclib/common/VarInt.cpp
    source/mclib/core/AuthToken.cpp
    source/mclib/core/Client.cpp
    source/mclib/core/ClientSettings.cpp
    source/mclib/core/Compression.cpp
    source/mclib/core/Connection.cpp
    source/mclib/core/Encryption.cpp
    source/mclib/core/PlayerManager.cpp
    source/mclib/entity/EntityManager.cpp
    source/mclib/entity/Metadata.cpp
    source/mclib/inventory/Hotbar.cpp
    source/mclib/inventory/Inventory.cpp
    source/mclib/inventory/Slot.cpp
    source/mclib/nbt/NBT.cpp
    source/mclib/nbt/Tag.cpp
    source/mclib/network/IPAddress.cpp
    source/mclib/network/Network.cpp
    source/mclib/network/Socket.cpp
    source/mclib/network/TCPSocket.cpp
    source/mclib/network/UDPSocket.cpp
    source/mclib/protocol/Protocol.cpp
    source/mclib/protocol/packets/Packet.cpp
    source/mclib/protocol/packets/PacketDispatcher.cpp
    source/mclib/protocol/packets/PacketFactory.cpp
    source/mclib/protocol/packets/PacketHandler.cpp
    source/mclib/util/Forge.cpp
    source/mclib/util/Hash.cpp
    source/mclib/util/HTTPClient.cpp
    source/mclib/util/Utility.cpp
    source/mclib/util/VersionFetcher.cpp
    source/mclib/util/Yggdrasil.cpp
    source/mclib/world/Chunk.cpp
    source/mclib/world/World.cpp
)


target_include_directories(mclib PUBLIC
    include/mclib
    include/mclib/block
    include/mclib/common
    include/mclib/core
    include/mclib/entity
    include/mclib/inventory
    include/mclib/nbt
    include/mclib/network
    include/mclib/protocol
    include/mclib/protocol/packets
    include/mclib/util
    include/mclib/world
    ${DEVKITPRO}/portlibs/3ds/include
)

target_compile_options(mclib PUBLIC
    -Wno-changes-meaning
)
target_link_libraries(mclib PUBLIC z curl mbedtls mbedx509 mbedcrypto)