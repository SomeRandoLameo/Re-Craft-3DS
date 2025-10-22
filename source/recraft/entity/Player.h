#pragma once

#include <stdbool.h>
#include "../world/CT_World.h"
#include "../gui/CT_Inventory.h"
#include "../inventory/ItemStack.h"
#include "Damage.h"
#include "../misc/Raycast.h"
#include "../misc/VecMath.h"

#define PLAYER_EYEHEIGHT (1.65f)
#define PLAYER_HEIGHT (1.8f)
#define PLAYER_COLLISIONBOX_SIZE (0.65f)
#define PLAYER_HALFEYEDIFF (0.07f)
#define PLAYER_PLACE_REPLACE_TIMEOUT (0.2f)

class Player {
public:
    // Constructor and destructor
    Player(World* world);
    ~Player() = default;

    // Public methods
    void Update(Damage* dmg);
    void Move(float dt, float3 accl);
    void PlaceBlock();
    void BreakBlock();
    void Jump(float3 accl);
    bool CanMove(float3 newVec);

    // Public member variables (for compatibility with existing code)
    float3 position;
    float pitch, yaw;
    float bobbing, fovAdd, crouchAdd;
    bool grounded, jumped, sprinting, flying, crouching;
    World* world;

    float3 view;
    bool autoJumpEnabled;

    float3 velocity;
    float simStepAccum;

    float breakPlaceTimeout;
    int hungertimer;

    int hp;
    int armour;
    int oxygen;
    int hunger;
    int difficulty;
    float rndy;

    float spawnx;
    float spawny;
    float spawnz;
    int spawnset;

    int gamemode;
    bool cheats;

    int quickSelectBarSlots;
    int quickSelectBarSlot;
    int inventorySite;
    ItemStack quickSelectBar[INVENTORY_QUICKSELECT_MAXSLOTS];

    Raycast_Result viewRayCast;
    bool blockInSight, blockInActionRange;
    ItemStack inventory[64];

private:
    // Private methods
    void InitializeInventory();
    void HandleFallDamage();
    void HandleFireDamage();
    void HandleHunger();
    void HandleRespawn(Damage* dmg);
};