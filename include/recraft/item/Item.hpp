#pragma once

#include <cstdint>
#include <string>
#include "block/Block.hpp"
#pragma once

#include <stdint.h>
#include "block/Block.hpp"

// Lets hope these are all
enum class ItemID : uint16_t {
    // Non-block items (256+)
    Iron_Shovel = 256,
    Iron_Pickaxe = 257,
    Iron_Axe = 258,
    Flint_And_Steel = 259,
    Apple = 260,
    Bow = 261,
    Arrow = 262,
    Coal = 263,
    Diamond = 264,
    Iron_Ingot = 265,
    Gold_Ingot = 266,
    Iron_Sword = 267,
    Wooden_Sword = 268,
    Wooden_Shovel = 269,
    Wooden_Pickaxe = 270,
    Wooden_Axe = 271,
    Stone_Sword = 272,
    Stone_Shovel = 273,
    Stone_Pickaxe = 274,
    Stone_Axe = 275,
    Diamond_Sword = 276,
    Diamond_Shovel = 277,
    Diamond_Pickaxe = 278,
    Diamond_Axe = 279,
    Stick = 280,
    Bowl = 281,
    Mushroom_Stew = 282,
    Golden_Sword = 283,
    Golden_Shovel = 284,
    Golden_Pickaxe = 285,
    Golden_Axe = 286,
    String = 287,
    Feather = 288,
    Gunpowder = 289,
    Wooden_Hoe = 290,
    Stone_Hoe = 291,
    Iron_Hoe = 292,
    Diamond_Hoe = 293,
    Golden_Hoe = 294,
    Wheat_Seeds = 295,
    Wheat = 296,
    Bread = 297,
    Leather_Helmet = 298,
    Leather_Chestplate = 299,
    Leather_Leggings = 300,
    Leather_Boots = 301,
    Chainmail_Helmet = 302,
    Chainmail_Chestplate = 303,
    Chainmail_Leggings = 304,
    Chainmail_Boots = 305,
    Iron_Helmet = 306,
    Iron_Chestplate = 307,
    Iron_Leggings = 308,
    Iron_Boots = 309,
    Diamond_Helmet = 310,
    Diamond_Chestplate = 311,
    Diamond_Leggings = 312,
    Diamond_Boots = 313,
    Golden_Helmet = 314,
    Golden_Chestplate = 315,
    Golden_Leggings = 316,
    Golden_Boots = 317,
    Flint = 318,
    Porkchop = 319,
    Cooked_Porkchop = 320,
    Painting = 321,
    Golden_Apple = 322,
    Sign = 323,
    Wooden_Door = 324,
    Bucket = 325,
    Water_Bucket = 326,
    Lava_Bucket = 327,
    Minecart = 328,
    Saddle = 329,
    Iron_Door = 330,
    Redstone = 331,
    Snowball = 332,
    Boat = 333,
    Leather = 334,
    Milk_Bucket = 335,
    Brick = 336,
    Clay_Ball = 337,
    Reeds = 338,
    Paper = 339,
    Book = 340,
    Slime_Ball = 341,
    Chest_Minecart = 342,
    Furnace_Minecart = 343,
    Egg = 344,
    Compass = 345,
    Fishing_Rod = 346,
    Clock = 347,
    Glowstone_Dust = 348,
    Fish = 349,
    Cooked_Fish = 350,
    Dye = 351,
    Bone = 352,
    Sugar = 353,
    Cake = 354,
    Bed = 355,
    Repeater = 356,
    Cookie = 357,
    Filled_Map = 358,
    Shears = 359,
    Melon = 360,
    Pumpkin_Seeds = 361,
    Melon_Seeds = 362,
    Beef = 363,
    Cooked_Beef = 364,
    Chicken = 365,
    Cooked_Chicken = 366,
    Rotten_Flesh = 367,
    Ender_Pearl = 368,
    Blaze_Rod = 369,
    Ghast_Tear = 370,
    Gold_Nugget = 371,
    Nether_Wart = 372,
    Potion = 373,
    Glass_Bottle = 374,
    Spider_Eye = 375,
    Fermented_Spider_Eye = 376,
    Blaze_Powder = 377,
    Magma_Cream = 378,
    Brewing_Stand = 379,
    Cauldron = 380,
    Ender_Eye = 381,
    Speckled_Melon = 382,
    Spawn_Egg = 383,
    Experience_Bottle = 384,
    Fire_Charge = 385,
    Writable_Book = 386,
    Written_Book = 387,
    Emerald = 388,
    Item_Frame = 389,
    Flower_Pot = 390,
    Carrot = 391,
    Potato = 392,
    Baked_Potato = 393,
    Poisonous_Potato = 394,
    Map = 395,
    Golden_Carrot = 396,
    Skull = 397,
    Carrot_On_A_Stick = 398,
    Nether_Star = 399,
    Pumpkin_Pie = 400,
    Fireworks = 401,
    Firework_Charge = 402,
    Enchanted_Book = 403,
    Comparator = 404,
    Netherbrick = 405,
    Quartz = 406,
    Tnt_Minecart = 407,
    Hopper_Minecart = 408,
    Prismarine_Shard = 409,
    Prismarine_Crystals = 410,
    Rabbit = 411,
    Cooked_Rabbit = 412,
    Rabbit_Stew = 413,
    Rabbit_Foot = 414,
    Rabbit_Hide = 415,
    Armor_Stand = 416,
    Iron_Horse_Armor = 417,
    Golden_Horse_Armor = 418,
    Diamond_Horse_Armor = 419,
    Lead = 420,
    Name_Tag = 421,
    Command_Block_Minecart = 422,
    Mutton = 423,
    Cooked_Mutton = 424,
    Banner = 425,
    End_Crystal = 426,
    Spruce_Door = 427,
    Birch_Door = 428,
    Jungle_Door = 429,
    Acacia_Door = 430,
    Dark_Oak_Door = 431,
    Chorus_Fruit = 432,
    Chorus_Fruit_Popped = 433,
    Beetroot = 434,
    Beetroot_Seeds = 435,
    Beetroot_Soup = 436,
    Dragon_Breath = 437,
    Splash_Potion = 438,
    Spectral_Arrow = 439,
    Tipped_Arrow = 440,
    Lingering_Potion = 441,
    Shield = 442,
    Elytra = 443,
    Spruce_Boat = 444,
    Birch_Boat = 445,
    Jungle_Boat = 446,
    Acacia_Boat = 447,
    Dark_Oak_Boat = 448,
    Totem_Of_Undying = 449,
    Shulker_Shell = 450,
    Iron_Nugget = 452,
    Knowledge_Book = 453,

    // Music discs
    Record_13 = 2256,
    Record_Cat = 2257,
    Record_Blocks = 2258,
    Record_Chirp = 2259,
    Record_Far = 2260,
    Record_Mall = 2261,
    Record_Mellohi = 2262,
    Record_Stal = 2263,
    Record_Strad = 2264,
    Record_Ward = 2265,
    Record_11 = 2266,
    Record_Wait = 2267,
};

// Helper to convert BlockID to item registry index
static inline uint16_t BlockIDToItemIndex(BlockID id) { return static_cast<uint16_t>(id); }

static inline uint16_t ItemIDToIndex(ItemID id) { return static_cast<uint16_t>(id); }

class Item {
public:
    Item() = default;
    virtual ~Item() = default;

    Item* setMaxStackSize(int size) {
        m_maxStackSize = size;
        return this;
    }
    Item* setUnlocalizedName(const std::string& name) {
        m_unlocalizedName = name;
        return this;
    }
    Item* setHasSubtypes(bool has) {
        m_hasSubtypes = has;
        return this;
    }
    Item* setFull3D() {
        m_bFull3D = true;
        return this;
    }
    Item* setMaxDamage(int damage) {
        m_maxDamage = damage;
        return this;
    }

    int getMaxStackSize() const { return m_maxStackSize; }
    const std::string& getUnlocalizedName() const { return m_unlocalizedName; }
    bool getHasSubtypes() const { return m_hasSubtypes; }
    bool isFull3D() const { return m_bFull3D; }
    bool isDamageable() const { return m_maxDamage > 0; }

protected:
    int m_maxStackSize = 64;
    int m_maxDamage = 0;
    bool m_bFull3D = false;
    bool m_hasSubtypes = false;
    std::string m_unlocalizedName;
};


class ItemRegistry {
public:
    static constexpr int MaxItems = 2268;

    static ItemRegistry& GetInstance() {
        static ItemRegistry instance;
        return instance;
    }

    void Init();

    void RegisterItem(ItemID id, Item* item) {
        int idx = static_cast<int>(id);
        if (idx < 0 || idx >= MaxItems)
            return;
        m_registry[idx] = item;
        m_occupied[idx] = true;
    }

    void RegisterItemBlock(BlockID block, Item* item = nullptr);

    static Item* GetItem(ItemID id) {
        auto& instance = GetInstance();
        int idx = static_cast<int>(id);
        if (idx < 0 || idx >= MaxItems)
            return nullptr;
        if (!instance.m_occupied[idx])
            return nullptr;
        return instance.m_registry[idx];
    }

    static Item* GetItemFromBlock(BlockID blockId) {
        auto& instance = GetInstance();
        return instance.m_blockToItem[static_cast<u8>(blockId)];
    }

private:
    ItemRegistry() {
        m_registry.fill(nullptr);
        m_occupied.fill(false);
        m_blockToItem.fill(nullptr);
    }

    std::array<Item*, MaxItems> m_registry;
    std::array<bool, MaxItems> m_occupied;
    std::array<Item*, 256> m_blockToItem;
};
