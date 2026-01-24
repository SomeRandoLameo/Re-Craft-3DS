#include "mcbridge/MCBridge.hpp"
#include <iostream>

MCBridge::~MCBridge() {
    stopBackgroundThread();
    disconnect();
}

bool MCBridge::connect() {
    if (m_connected.load()) {
        std::cout << "Already connected\n";
        return true;
    }

    const u16 port = 25565;
    const std::string password("");

    try {
        mc::util::VersionFetcher versionFetcher(m_ipAddress, port);
        auto version = versionFetcher.GetVersion();
        mc::block::BlockRegistry::GetInstance()->RegisterVanillaBlocks(version);

        std::cout << "Connected successfully to server\n";

        LightLock_Lock(&m_clientMutex);

        m_dispatcher = std::make_unique<mc::protocol::packets::PacketDispatcher>();
        m_client = std::make_unique<mc::core::Client>(m_dispatcher.get(), version);

        versionFetcher.GetForge().SetConnection(m_client->GetConnection());

        m_client->GetPlayerController()->SetHandleFall(true);
        m_client->GetConnection()->GetSettings()
                .SetMainHand(mc::MainHand::Right)
                .SetViewDistance(static_cast<s32>(1));

        mc::core::AuthToken token;
        m_client->Login(m_ipAddress, port, m_username, password, mc::core::UpdateMethod::Manual);

        LightLock_Unlock(&m_clientMutex);

        m_connected.store(true);
        return true;

    } catch (std::exception& e) {
        std::cout << "Connection error: " << e.what() << std::endl;
        LightLock_Lock(&m_clientMutex);
        m_dispatcher.reset();
        m_client.reset();
        LightLock_Unlock(&m_clientMutex);
        m_connected.store(false);
        return false;
    }
}

void MCBridge::disconnect() {
    m_connected.store(false);
    LightLock_Lock(&m_clientMutex);
    m_client.reset();
    m_dispatcher.reset();
    LightLock_Unlock(&m_clientMutex);
    std::cout << "Disconnected from server\n";
}

void MCBridge::update() {
    LightLock_Lock(&m_clientMutex);
    if (m_client && m_connected.load()) {
        try {
            m_client->Update();
        } catch (std::exception& e) {
            std::cout << "Update error: " << e.what() << std::endl;
            m_connected.store(false);
        }
    }
    LightLock_Unlock(&m_clientMutex);
}

void MCBridge::startBackgroundThread() {
    if (m_running.load()) {
        std::cout << "Background thread already running\n";
        return;
    }

    m_shouldStop.store(false);

    m_threadStack = linearAlloc(THREAD_STACK_SIZE);
    if (!m_threadStack) {
        std::cout << "Failed to allocate thread stack\n";
        return;
    }

    m_thread = threadCreate(threadFunc, this,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY,
                            THREAD_CORE,
                            false);

    if (m_thread == nullptr) {
        std::cout << "Failed to create thread\n";
        linearFree(m_threadStack);
        m_threadStack = nullptr;
        return;
    }

    m_running.store(true);
    std::cout << "Background thread started\n";
}

void MCBridge::stopBackgroundThread() {
    if (!m_running.load()) {
        return;
    }

    m_shouldStop.store(true);

    if (m_thread) {
        threadJoin(m_thread, U64_MAX);
        threadFree(m_thread);
        m_thread = nullptr;
    }

    if (m_threadStack) {
        linearFree(m_threadStack);
        m_threadStack = nullptr;
    }

    m_running.store(false);
    std::cout << "Background thread stopped\n";
}

void MCBridge::threadFunc(void* arg) {
    MCBridge* bridge = static_cast<MCBridge*>(arg);
    bridge->backgroundLoop();
}

void MCBridge::backgroundLoop() {
    std::cout << "Background loop started\n";

    while (!m_shouldStop.load()) {
        if (m_connected.load()) {
            update();
        }
        svcSleepThread(50000000LL); // 50ms
    }

    std::cout << "Background loop ended\n";
}

// TODO: make it stop...
Block MCBridge::MCLibBlockToCTBlock(u32 BlockType){
    // These are MCLib Mapping IDs, IDK how they correspond to Minecraft IDs but this works
    if(BlockType == 0)         return Block::Air;
    else if(BlockType == 16)   return Block::Stone;
    else if(BlockType == 17)   return Block::Stone;
    else if(BlockType == 18)   return Block::Stone;
    else if(BlockType == 19)   return Block::Stone;
    else if(BlockType == 20)   return Block::Stone;
    else if(BlockType == 21)   return Block::Stone;
    else if(BlockType == 22)   return Block::Stone;
    else if(BlockType == 32)   return Block::Grass;
    else if(BlockType == 48)   return Block::Dirt;
    else if(BlockType == 49)   return Block::Dirt;
    else if(BlockType == 50)   return Block::Dirt;
    else if(BlockType == 64)   return Block::Cobblestone;
    else if(BlockType == 80)   return Block::Planks;
    else if(BlockType == 81)   return Block::Planks;
    else if(BlockType == 82)   return Block::Planks;
    else if(BlockType == 83)   return Block::Planks;
    else if(BlockType == 84)   return Block::Planks;
    else if(BlockType == 85)   return Block::Planks;
    // 96 - 101 = sapling
    else if(BlockType == 112)  return Block::Bedrock;
    else if(BlockType == 128)  return Block::Water; // Flowing water, actually
    else if(BlockType == 144)  return Block::Water;
    else if(BlockType == 160)  return Block::Lava;  // Flowing lava, actually
    else if(BlockType == 176)  return Block::Lava;
    else if(BlockType == 192)  return Block::Sand;
    else if(BlockType == 193)  return Block::Sand;
    else if(BlockType == 298)  return Block::Gravel;
    else if(BlockType == 224)  return Block::Gold_Ore;
    else if(BlockType == 240)  return Block::Iron_Ore;
    else if(BlockType == 256)  return Block::Coal_Ore;
    else if(BlockType == 272)  return Block::Log;
    else if(BlockType == 273)  return Block::Log;
    else if(BlockType == 274)  return Block::Log;
    else if(BlockType == 275)  return Block::Log;
    else if(BlockType == 288)  return Block::Leaves;
    else if(BlockType == 289)  return Block::Leaves;
    else if(BlockType == 290)  return Block::Leaves;
    else if(BlockType == 291)  return Block::Leaves;
    // 304 - 305 = sponge
    else if(BlockType == 320)  return Block::Glass;
    // 336 = lapis ore
    // 352 = lapis block
    // 368 = dispenser
    else if(BlockType == 384)  return Block::Sandstone;
    else if(BlockType == 385)  return Block::Sandstone;
    else if(BlockType == 386)  return Block::Sandstone;
    // 400 = note block
    // 416 = bed
    // 432 = powered rail
    // 448 = detector rail
    // 464 = sticky piston
    // 480 = web
    // 496 - 498 = tall grass
    // 512 = dead bush
    // 528 = piston
    // 544 = piston head
    else if(BlockType == 560)  return Block::Wool;
    else if(BlockType == 561)  return Block::Wool;
    else if(BlockType == 562)  return Block::Wool;
    else if(BlockType == 563)  return Block::Wool;
    else if(BlockType == 564)  return Block::Wool;
    else if(BlockType == 565)  return Block::Wool;
    else if(BlockType == 566)  return Block::Wool;
    else if(BlockType == 567)  return Block::Wool;
    else if(BlockType == 568)  return Block::Wool;
    else if(BlockType == 569)  return Block::Wool;
    else if(BlockType == 570)  return Block::Wool;
    else if(BlockType == 571)  return Block::Wool;
    else if(BlockType == 572)  return Block::Wool;
    else if(BlockType == 573)  return Block::Wool;
    else if(BlockType == 574)  return Block::Wool;
    else if(BlockType == 575)  return Block::Wool;
    //registry->RegisterBlock(new Block("minecraft:yellow_flower", 592, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 608, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 609, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 610, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 611, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 612, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 613, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 614, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 615, true));
    //registry->RegisterBlock(new Block("minecraft:red_flower", 616, true));
    //registry->RegisterBlock(new Block("minecraft:brown_mushroom", 624, true));
    //registry->RegisterBlock(new Block("minecraft:red_mushroom", 640, true));
    else if(BlockType == 656)  return Block::Gold_Block;
    else if(BlockType == 672)  return Block::Iron_Block;
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 688, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 689, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 690, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 691, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 692, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 693, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 694, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab", 695, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 704, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 705, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 706, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 707, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 708, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 709, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 710, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab", 711, true));
    else if(BlockType == 720)  return Block::Brick;
    //registry->RegisterBlock(new Block("minecraft:tnt", 736, true));
    //registry->RegisterBlock(new Block("minecraft:bookshelf", 752, true));
    //registry->RegisterBlock(new Block("minecraft:mossy_cobblestone", 768, true));
    else if(BlockType == 784)  return Block::Obsidian;
    //registry->RegisterBlock(new Block("minecraft:obsidian", 784, true));
    //registry->RegisterBlock(new Block("minecraft:torch", 800, true));
    //registry->RegisterBlock(new Block("minecraft:fire", 816, true));
    //registry->RegisterBlock(new Block("minecraft:mob_spawner", 832, true));
    //registry->RegisterBlock(new Block("minecraft:oak_stairs", 848, true));
    //registry->RegisterBlock(new Block("minecraft:chest", 864, true));
    //registry->RegisterBlock(new Block("minecraft:redstone_wire", 880, true));
    else if(BlockType == 896)  return Block::Diamond_Ore;
    else if(BlockType == 912)  return Block::Diamond_Block;
    else if(BlockType == 928)  return Block::Crafting_Table;
    //registry->RegisterBlock(new Block("minecraft:wheat", 944, true));
    //registry->RegisterBlock(new Block("minecraft:farmland", 960, true));
    else if(BlockType == 928)  return Block::Furnace;
    //registry->RegisterBlock(new Block("minecraft:lit_furnace", 992, true));
    //registry->RegisterBlock(new Block("minecraft:standing_sign", 1008, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_door", 1024, true)); This one is commented out because upper and lower door parts are separate
    //registry->RegisterBlock(new Block("minecraft:ladder", 1040, true));
    //registry->RegisterBlock(new Block("minecraft:rail", 1056, true));
    //registry->RegisterBlock(new Block("minecraft:stone_stairs", 1072, true));
    //registry->RegisterBlock(new Block("minecraft:wall_sign", 1088, true));
    //registry->RegisterBlock(new Block("minecraft:lever", 1104, true));
    //registry->RegisterBlock(new Block("minecraft:stone_pressure_plate", 1120, true));
    //registry->RegisterBlock(new Block("minecraft:iron_door", 1136, true)); This one is commented out because upper and lower door parts are separate
    //registry->RegisterBlock(new Block("minecraft:wooden_pressure_plate", 1152, true));
    //registry->RegisterBlock(new Block("minecraft:redstone_ore", 1168, true));
    //registry->RegisterBlock(new Block("minecraft:lit_redstone_ore", 1184, true));
    //registry->RegisterBlock(new Block("minecraft:unlit_redstone_torch", 1200, true));
    //registry->RegisterBlock(new Block("minecraft:redstone_torch", 1216, true));
    //registry->RegisterBlock(new Block("minecraft:stone_button", 1232, true));
    //registry->RegisterBlock(new Block("minecraft:snow_layer", 1248, true));
    //registry->RegisterBlock(new Block("minecraft:ice", 1264, true));
    //registry->RegisterBlock(new Block("minecraft:snow", 1280, true));
    //registry->RegisterBlock(new Block("minecraft:cactus", 1296, true));
    //registry->RegisterBlock(new Block("minecraft:clay", 1312, true));
    //registry->RegisterBlock(new Block("minecraft:reeds", 1328, true));
    //registry->RegisterBlock(new Block("minecraft:jukebox", 1344, true));
    //registry->RegisterBlock(new Block("minecraft:fence", 1360, true));
    //registry->RegisterBlock(new Block("minecraft:pumpkin", 1376, true));
    else if(BlockType == 1392)  return Block::Netherrack;
    //registry->RegisterBlock(new Block("minecraft:netherrack", 1392, true));
    //registry->RegisterBlock(new Block("minecraft:soul_sand", 1408, true));
    //registry->RegisterBlock(new Block("minecraft:glowstone", 1424, true));
    //registry->RegisterBlock(new Block("minecraft:portal", 1440, true));
    //registry->RegisterBlock(new Block("minecraft:lit_pumpkin", 1456, true));
    //registry->RegisterBlock(new Block("minecraft:cake", 1472, true));
    //registry->RegisterBlock(new Block("minecraft:unpowered_repeater", 1488, true));
    //registry->RegisterBlock(new Block("minecraft:powered_repeater", 1504, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1520, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1521, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1522, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1523, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1524, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1525, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1526, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1527, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1528, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1529, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1530, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1531, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1532, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1533, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1534, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass", 1535, true));
    //registry->RegisterBlock(new Block("minecraft:trapdoor", 1536, true));
    //registry->RegisterBlock(new Block("minecraft:monster_egg", 1552, true));
    //registry->RegisterBlock(new Block("minecraft:monster_egg", 1553, true));
    //registry->RegisterBlock(new Block("minecraft:monster_egg", 1554, true));
    //registry->RegisterBlock(new Block("minecraft:monster_egg", 1555, true));
    //registry->RegisterBlock(new Block("minecraft:monster_egg", 1556, true));
    //registry->RegisterBlock(new Block("minecraft:monster_egg", 1557, true));
    else if(BlockType == 1568)  return Block::Stonebrick;
    else if(BlockType == 1569)  return Block::Stonebrick;
    else if(BlockType == 1570)  return Block::Stonebrick;
    else if(BlockType == 1571)  return Block::Stonebrick;
    //registry->RegisterBlock(new Block("minecraft:brown_mushroom_block", 1584, true));
    //registry->RegisterBlock(new Block("minecraft:red_mushroom_block", 1600, true));
    //registry->RegisterBlock(new Block("minecraft:iron_bars", 1616, true));
    //registry->RegisterBlock(new Block("minecraft:glass_pane", 1632, true));
    //registry->RegisterBlock(new Block("minecraft:melon_block", 1648, true));
    //registry->RegisterBlock(new Block("minecraft:pumpkin_stem", 1664, true));
    //registry->RegisterBlock(new Block("minecraft:melon_stem", 1680, true));
    //registry->RegisterBlock(new Block("minecraft:vine", 1696, true));
    //registry->RegisterBlock(new Block("minecraft:fence_gate", 1712, true));
    //registry->RegisterBlock(new Block("minecraft:brick_stairs", 1728, true));
    //registry->RegisterBlock(new Block("minecraft:stone_brick_stairs", 1744, true));
    //registry->RegisterBlock(new Block("minecraft:mycelium", 1760, true));
    //registry->RegisterBlock(new Block("minecraft:waterlily", 1776, true));
    //registry->RegisterBlock(new Block("minecraft:nether_brick", 1792, true));
    //registry->RegisterBlock(new Block("minecraft:nether_brick_fence", 1808, true));
    //registry->RegisterBlock(new Block("minecraft:nether_brick_stairs", 1824, true));
    //registry->RegisterBlock(new Block("minecraft:nether_wart", 1840, true));
    //registry->RegisterBlock(new Block("minecraft:enchanting_table", 1856, true));
    //registry->RegisterBlock(new Block("minecraft:brewing_stand", 1872, true));
    //registry->RegisterBlock(new Block("minecraft:cauldron", 1888, true));
    //registry->RegisterBlock(new Block("minecraft:end_portal", 1904, true));
    //registry->RegisterBlock(new Block("minecraft:end_portal_frame", 1920, true));
    //registry->RegisterBlock(new Block("minecraft:end_stone", 1936, true));
    //registry->RegisterBlock(new Block("minecraft:dragon_egg", 1952, true));
    //registry->RegisterBlock(new Block("minecraft:redstone_lamp", 1968, true));
    //registry->RegisterBlock(new Block("minecraft:lit_redstone_lamp", 1984, true));
    //registry->RegisterBlock(new Block("minecraft:double_wooden_slab", 2000, true));
    //registry->RegisterBlock(new Block("minecraft:double_wooden_slab", 2001, true));
    //registry->RegisterBlock(new Block("minecraft:double_wooden_slab", 2002, true));
    //registry->RegisterBlock(new Block("minecraft:double_wooden_slab", 2003, true));
    //registry->RegisterBlock(new Block("minecraft:double_wooden_slab", 2004, true));
    //registry->RegisterBlock(new Block("minecraft:double_wooden_slab", 2005, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_slab", 2016, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_slab", 2017, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_slab", 2018, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_slab", 2019, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_slab", 2020, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_slab", 2021, true));
    //registry->RegisterBlock(new Block("minecraft:cocoa", 2032, true));
    //registry->RegisterBlock(new Block("minecraft:sandstone_stairs", 2048, true));
    else if(BlockType == 2064)  return Block::Emerald_Ore;
    //registry->RegisterBlock(new Block("minecraft:ender_chest", 2080, true));
    //registry->RegisterBlock(new Block("minecraft:tripwire_hook", 2096, true));
    //registry->RegisterBlock(new Block("minecraft:tripwire_hook", 2112, true));
    //registry->RegisterBlock(new Block("minecraft:emerald_block", 2128, true));
    //registry->RegisterBlock(new Block("minecraft:spruce_stairs", 2144, true));
    //registry->RegisterBlock(new Block("minecraft:birch_stairs", 2160, true));
    //registry->RegisterBlock(new Block("minecraft:jungle_stairs", 2176, true));
    //registry->RegisterBlock(new Block("minecraft:command_block", 2192, true));
    //registry->RegisterBlock(new Block("minecraft:beacon", 2208, true));
    //registry->RegisterBlock(new Block("minecraft:cobblestone_wall", 2224, true));
    //registry->RegisterBlock(new Block("minecraft:cobblestone_wall", 2225, true));
    //registry->RegisterBlock(new Block("minecraft:flower_pot", 2240, true));
    //registry->RegisterBlock(new Block("minecraft:carrots", 2256, true));
    //registry->RegisterBlock(new Block("minecraft:potatoes", 2272, true));
    //registry->RegisterBlock(new Block("minecraft:wooden_button", 2288, true));
    //registry->RegisterBlock(new Block("minecraft:skull", 2304, true));
    //registry->RegisterBlock(new Block("minecraft:anvil", 2320, true));
    //registry->RegisterBlock(new Block("minecraft:trapped_chest", 2336, true));
    //registry->RegisterBlock(new Block("minecraft:light_weighted_pressure_plate", 2352, true));
    //registry->RegisterBlock(new Block("minecraft:heavy_weighted_pressure_plate", 2368, true));
    //registry->RegisterBlock(new Block("minecraft:unpowered_comparator", 2384, true));
    //registry->RegisterBlock(new Block("minecraft:powered_comparator", 2400, true));
    //registry->RegisterBlock(new Block("minecraft:daylight_detector", 2416, true));
    //registry->RegisterBlock(new Block("minecraft:redstone_block", 2432, true));
    //registry->RegisterBlock(new Block("minecraft:quartz_ore", 2448, true));
    //registry->RegisterBlock(new Block("minecraft:hopper", 2464, true));
    //registry->RegisterBlock(new Block("minecraft:quartz_block", 2480, true));
    //registry->RegisterBlock(new Block("minecraft:quartz_block", 2481, true));
    //registry->RegisterBlock(new Block("minecraft:quartz_block", 2482, true));
    //registry->RegisterBlock(new Block("minecraft:quartz_stairs", 2496, true));
    //registry->RegisterBlock(new Block("minecraft:activator_rail", 2512, true));
    //registry->RegisterBlock(new Block("minecraft:dropper", 2528, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2544, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2545, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2546, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2547, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2548, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2549, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2550, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2551, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2552, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2553, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2554, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2555, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2556, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2557, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2558, true));
    //registry->RegisterBlock(new Block("minecraft:stained_hardened_clay", 2559, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2560, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2561, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2562, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2563, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2564, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2565, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2566, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2567, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2568, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2569, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2570, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2571, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2572, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2573, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2574, true));
    //registry->RegisterBlock(new Block("minecraft:stained_glass_pane", 2575, true));
    //registry->RegisterBlock(new Block("minecraft:leaves2", 2576, true)); wth are leaves2, are they leaves but better???
    //registry->RegisterBlock(new Block("minecraft:leaves2", 2577, true));
    //registry->RegisterBlock(new Block("minecraft:log2", 2592, true)); wth is log2, are they logs but better???
    //registry->RegisterBlock(new Block("minecraft:log2", 2593, true));
    //registry->RegisterBlock(new Block("minecraft:acacia_stairs", 2608, true));
    //registry->RegisterBlock(new Block("minecraft:dark_oak_stairs", 2624, true));
    //registry->RegisterBlock(new Block("minecraft:slime", 2640, true));
    //registry->RegisterBlock(new Block("minecraft:barrier", 2656, true));
    //registry->RegisterBlock(new Block("minecraft:iron_trapdoor", 2672, true));
    //registry->RegisterBlock(new Block("minecraft:prismarine", 2688, true));
    //registry->RegisterBlock(new Block("minecraft:prismarine", 2689, true));
    //registry->RegisterBlock(new Block("minecraft:prismarine", 2690, true));
    //registry->RegisterBlock(new Block("minecraft:sea_lantern", 2704, true));
    //registry->RegisterBlock(new Block("minecraft:hay_block", 2720, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2736, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2737, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2738, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2739, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2740, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2741, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2742, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2743, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2744, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2745, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2746, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2747, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2748, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2749, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2750, true));
    //registry->RegisterBlock(new Block("minecraft:carpet", 2751, true));
    //registry->RegisterBlock(new Block("minecraft:hardened_clay", 2752, true));
    else if(BlockType == 2768)  return Block::Coal_Block;
    //registry->RegisterBlock(new Block("minecraft:coal_block", 2768, true));
    //registry->RegisterBlock(new Block("minecraft:packed_ice", 2784, true));
    //registry->RegisterBlock(new Block("minecraft:double_plant", 2800, true));
    //registry->RegisterBlock(new Block("minecraft:double_plant", 2801, true));
    //registry->RegisterBlock(new Block("minecraft:double_plant", 2802, true));
    //registry->RegisterBlock(new Block("minecraft:double_plant", 2803, true));
    //registry->RegisterBlock(new Block("minecraft:double_plant", 2804, true));
    //registry->RegisterBlock(new Block("minecraft:double_plant", 2805, true));
    //registry->RegisterBlock(new Block("minecraft:standing_banner", 2816, true));
    //registry->RegisterBlock(new Block("minecraft:wall_banner", 2832, true));
    //registry->RegisterBlock(new Block("minecraft:daylight_detector_inverted", 2848, true));
    //registry->RegisterBlock(new Block("minecraft:red_sandstone", 2864, true));
    //registry->RegisterBlock(new Block("minecraft:red_sandstone", 2865, true));
    //registry->RegisterBlock(new Block("minecraft:red_sandstone", 2866, true));
    //registry->RegisterBlock(new Block("minecraft:red_sandstone_stairs", 2880, true));
    //registry->RegisterBlock(new Block("minecraft:double_stone_slab2", 2896, true));
    //registry->RegisterBlock(new Block("minecraft:stone_slab2", 2912, true));
    //registry->RegisterBlock(new Block("minecraft:spruce_fence_gate", 2928, true));
    //registry->RegisterBlock(new Block("minecraft:birch_fence_gate", 2944, true));
    //registry->RegisterBlock(new Block("minecraft:jungle_fence_gate", 2960, true));
    //registry->RegisterBlock(new Block("minecraft:dark_oak_fence_gate", 2976, true));
    //registry->RegisterBlock(new Block("minecraft:acacia_fence_gate", 2992, true));
    //registry->RegisterBlock(new Block("minecraft:spruce_fence", 3008, true));
    //registry->RegisterBlock(new Block("minecraft:birch_fence", 3024, true));
    //registry->RegisterBlock(new Block("minecraft:jungle_fence", 3040, true));
    //registry->RegisterBlock(new Block("minecraft:dark_oak_fence", 3056, true));
    //registry->RegisterBlock(new Block("minecraft:acacia_fence", 3072, true));
    //registry->RegisterBlock(new Block("minecraft:spruce_door", 3088, true));
    //registry->RegisterBlock(new Block("minecraft:birch_door", 3104, true));
    //registry->RegisterBlock(new Block("minecraft:jungle_door", 3120, true));
    //registry->RegisterBlock(new Block("minecraft:acacia_door", 3136, true));
    //registry->RegisterBlock(new Block("minecraft:dark_oak_door", 3152, true));
    //registry->RegisterBlock(new Block("minecraft:end_rod", 3168, true));
    //registry->RegisterBlock(new Block("minecraft:chorus_plant", 3184, true));
    //registry->RegisterBlock(new Block("minecraft:chorus_flower", 3200, true));
    //registry->RegisterBlock(new Block("minecraft:purpur_block", 3216, true));
    //registry->RegisterBlock(new Block("minecraft:purpur_pillar", 3232, true));
    //registry->RegisterBlock(new Block("minecraft:purpur_stairs", 3248, true));
    //registry->RegisterBlock(new Block("minecraft:purpur_double_slab", 3264, true));
    //registry->RegisterBlock(new Block("minecraft:purpur_slab", 3280, true));
    //registry->RegisterBlock(new Block("minecraft:end_bricks", 3296, true));
    //registry->RegisterBlock(new Block("minecraft:beetroots", 3312, true));
    //registry->RegisterBlock(new Block("minecraft:grass_path", 3328, true));
    //registry->RegisterBlock(new Block("minecraft:end_gateway", 3344, true));
    //registry->RegisterBlock(new Block("minecraft:repeating_command_block", 3360, true));
    //registry->RegisterBlock(new Block("minecraft:chain_command_block", 3376, true));
    //registry->RegisterBlock(new Block("minecraft:frosted_ice", 3392, true));
    //registry->RegisterBlock(new Block("minecraft:magma", 3408, true));
    //registry->RegisterBlock(new Block("minecraft:nether_wart_block", 3424, true));
    //registry->RegisterBlock(new Block("minecraft:red_nether_brick", 3440, true));
    //registry->RegisterBlock(new Block("minecraft:bone_block", 3456, true));
    //registry->RegisterBlock(new Block("minecraft:structure_void", 3472, true));
    //registry->RegisterBlock(new Block("minecraft:observer", 3488, true));
    //registry->RegisterBlock(new Block("minecraft:white_shulker_box", 3504, true));
    //registry->RegisterBlock(new Block("minecraft:orange_shulker_box", 3520, true));
    //registry->RegisterBlock(new Block("minecraft:magenta_shulker_box", 3536, true));
    //registry->RegisterBlock(new Block("minecraft:light_blue_shulker_box", 3552, true));
    //registry->RegisterBlock(new Block("minecraft:yellow_shulker_box", 3568, true));
    //registry->RegisterBlock(new Block("minecraft:lime_shulker_box", 3584, true));
    //registry->RegisterBlock(new Block("minecraft:pink_shulker_box", 3600, true));
    //registry->RegisterBlock(new Block("minecraft:gray_shulker_box", 3616, true));
    //registry->RegisterBlock(new Block("minecraft:silver_shulker_box", 3632, true));
    //registry->RegisterBlock(new Block("minecraft:cyan_shulker_box", 3648, true));
    //registry->RegisterBlock(new Block("minecraft:purple_shulker_box", 3664, true));
    //registry->RegisterBlock(new Block("minecraft:blue_shulker_box", 3680, true));
    //registry->RegisterBlock(new Block("minecraft:brown_shulker_box", 3696, true));
    //registry->RegisterBlock(new Block("minecraft:green_shulker_box", 3712, true));
    //registry->RegisterBlock(new Block("minecraft:red_shulker_box", 3728, true));
    //registry->RegisterBlock(new Block("minecraft:black_shulker_box", 3744, true));
    //registry->RegisterBlock(new Block("minecraft:white_glazed_terracotta", 3760, true));
    //registry->RegisterBlock(new Block("minecraft:orange_glazed_terracotta", 3776, true));
    //registry->RegisterBlock(new Block("minecraft:magenta_glazed_terracotta", 3792, true));
    //registry->RegisterBlock(new Block("minecraft:light_blue_glazed_terracotta", 3808, true));
    //registry->RegisterBlock(new Block("minecraft:yellow_glazed_terracotta", 3824, true));
    //registry->RegisterBlock(new Block("minecraft:lime_glazed_terracotta", 3840, true));
    //registry->RegisterBlock(new Block("minecraft:pink_glazed_terracotta", 3856, true));
    //registry->RegisterBlock(new Block("minecraft:gray_glazed_terracotta", 3872, true));
    //registry->RegisterBlock(new Block("minecraft:light_gray_glazed_terracotta", 3888, true));
    //registry->RegisterBlock(new Block("minecraft:cyan_glazed_terracotta", 3904, true));
    //registry->RegisterBlock(new Block("minecraft:purple_glazed_terracotta", 3920, true));
    //registry->RegisterBlock(new Block("minecraft:blue_glazed_terracotta", 3936, true));
    //registry->RegisterBlock(new Block("minecraft:brown_glazed_terracotta", 3952, true));
    //registry->RegisterBlock(new Block("minecraft:green_glazed_terracotta", 3968, true));
    //registry->RegisterBlock(new Block("minecraft:red_glazed_terracotta", 3984, true));
    //registry->RegisterBlock(new Block("minecraft:black_glazed_terracotta", 4000, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4016, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4017, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4018, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4019, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4020, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4021, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4022, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4023, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4024, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4025, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4026, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4027, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4028, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4029, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4030, true));
    //registry->RegisterBlock(new Block("minecraft:concrete", 4031, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4032, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4033, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4034, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4035, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4036, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4037, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4038, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4039, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4040, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4041, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4042, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4043, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4044, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4045, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4046, true));
    //registry->RegisterBlock(new Block("minecraft:concrete_powder", 4047, true));
    //registry->RegisterBlock(new Block("minecraft:structure_block", 4080, true));


    else return Block::Air;
}

mc::inventory::Slot MCBridge::CTItemStackToMCLIBSlot(ItemStack in){
    return mc::inventory::Slot(static_cast<s32>(in.block),in.amount,in.meta);
}

ItemStack MCBridge::MCLIBSlotToCTItemStack(mc::inventory::Slot in){
    return (ItemStack){static_cast<Block>(in.GetItemId()),in.GetItemDamage(),in.GetItemCount()};
}