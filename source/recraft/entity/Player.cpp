#include "Player.h"
#include "misc/Collision.h"
#include "entity/Damage.h"
#include "gui/DebugUI.h"

const float MaxWalkVelocity = 4.3f;
const float MaxFallVelocity = -50.f;
const float GravityPlusFriction = 10.f;

Player::Player(World* world) {
    this->world = world;

    InitializeInventory();
}

void Player::InitializeInventory() {
    int l = 0;
    inventory[l++] = (ItemStack){Block_Stone, 0, 1};
    inventory[l++] = (ItemStack){Block_Dirt, 0, 1};
    inventory[l++] = (ItemStack){Block_Grass, 0, 1};
    inventory[l++] = (ItemStack){Block_Cobblestone, 0, 1};
    inventory[l++] = (ItemStack){Block_Sand, 0, 1};
    inventory[l++] = (ItemStack){Block_Log, 0, 1};
    inventory[l++] = (ItemStack){Block_Leaves, 0, 1};
    inventory[l++] = (ItemStack){Block_Glass, 0, 1};
    inventory[l++] = (ItemStack){Block_Stonebrick, 0, 1};
    inventory[l++] = (ItemStack){Block_Brick, 0, 1};
    inventory[l++] = (ItemStack){Block_Planks, 0, 1};
    for (int i = 0; i < 16; i++)
        inventory[l++] = (ItemStack){Block_Wool, static_cast<u8>(i), 1};
    inventory[l++] = (ItemStack){Block_Bedrock, 0, 1};
    inventory[l++] = (ItemStack){Block_Gravel, 0, 1};
    inventory[l++] = (ItemStack){Block_Coarse, 0, 1};
    inventory[l++] = (ItemStack){Block_Door_Top, 0, 1};
    inventory[l++] = (ItemStack){Block_Door_Bottom, 0, 1};
    inventory[l++] = (ItemStack){Block_Snow_Grass, 0, 1};
    inventory[l++] = (ItemStack){Block_Snow, 0, 1};
    inventory[l++] = (ItemStack){Block_Obsidian, 0, 1};
    inventory[l++] = (ItemStack){Block_Netherrack, 0, 1};
    inventory[l++] = (ItemStack){Block_Sandstone, 0, 1};
    inventory[l++] = (ItemStack){Block_Smooth_Stone, 0, 1};
    inventory[l++] = (ItemStack){Block_Crafting_Table, 0, 1};
    inventory[l++] = (ItemStack){Block_Grass_Path, 0, 1};
    inventory[l++] = (ItemStack){Block_Lava, 0, 1};
    inventory[l++] = (ItemStack){Block_Water, 0, 1};
    inventory[l++] = (ItemStack){Block_Iron_Block, 0, 1};
    inventory[l++] = (ItemStack){Block_Iron_Ore, 0, 1};
    inventory[l++] = (ItemStack){Block_Coal_Block, 0, 1};
    inventory[l++] = (ItemStack){Block_Coal_Ore, 0, 1};
    inventory[l++] = (ItemStack){Block_Gold_Block, 0, 1};
    inventory[l++] = (ItemStack){Block_Gold_Ore, 0, 1};
    inventory[l++] = (ItemStack){Block_Diamond_Block, 0, 1};
    inventory[l++] = (ItemStack){Block_Diamond_Ore, 0, 1};
    inventory[l++] = (ItemStack){Block_Emerald_Block, 0, 1};
    inventory[l++] = (ItemStack){Block_Emerald_Ore, 0, 1};
    inventory[l++] = (ItemStack){Block_Furnace, 0, 1};

    for (int i = 0; i < Inventory::calculateQuickSelectSlots(); i++)
        quickSelectBar[i] = (ItemStack){Block_Air, 0, 0};
}

void Player::Update(void* dmg) {
    view.x = -sinf(yaw) * cosf(pitch);
    view.y = sinf(pitch);
    view.z = -cosf(yaw) * cosf(pitch);

    blockInSight = Raycast_Cast(world, position + mc::Vector3d(0,PLAYER_EYEHEIGHT,0), view, &viewRayCast);
    blockInActionRange = blockInSight && viewRayCast.distSqr < 3.5f * 3.5f * 3.5f;

    HandleFallDamage();
    HandleFireDamage();
    HandleHunger();
    HandleRespawn(dmg);
}

void Player::HandleFallDamage() {
    if (velocity.y <= -12) {
        rndy = round(velocity.y);
        if (World_GetBlock(world, position.x, position.y - 1, position.z) != Block_Air) {
            hp = hp + rndy;
            rndy = 0;
        }
    }
}

void Player::HandleFireDamage() {
    if (World_GetBlock(world, position.x, position.y, position.z) == Block_Lava) {
      //  DebugUI_Log("ur burning lol");
        OvertimeDamage("Fire", 10);
    }
}

void Player::HandleHunger() {
    svcSleepThread(10000000);
    hungertimer = hungertimer + 1;
    if (hungertimer == 400 && hunger != 0) {
        hunger = hunger - 1;
        hungertimer = 0;
    }
    if (hunger == 0) {
        svcSleepThread(10000000);
        if (hungertimer == 400) {
            hp = hp - 1;
            hungertimer = 0;
        }
    }
}

void Player::HandleRespawn(void* arg1) {
    // Stupid patch (need to get rid of that)
    Damage* dmg = (Damage*)arg1;
    if (hp <= 0) {
        if (difficulty != 4) {
            if (!spawnset) {
                if (dmg->cause == NULL) {
                  //  DebugUI_Log("Player died");
                } else {
                   // DebugUI_Log("Died by %s", dmg->cause);
                }
                //DebugUI_Log("No spawn was set");
                position.x = 0.0;

                int spawnY = 1;
                while (World_GetBlock(world, spawnPos.x, spawnPos.y, spawnPos.z) != Block_Air)
                    spawnY++;

                bool shouldOffset = world->genSettings.type != WorldGen_SuperFlat;
                position.y = shouldOffset ? spawnY + 1 : spawnY;
                position.z = 0.0;
            }
            if (spawnset) {
                if (dmg->cause == NULL) {
                //    DebugUI_Log("Player died");
                } else {
               //     DebugUI_Log("Died by %s", dmg->cause);
                }
                position.x = spawnPos.x;

                int spawnY = 1;
                while (World_GetBlock(world, spawnPos.x, spawnY, spawnPos.z) != Block_Air)
                    spawnY++;

                bool shouldOffset = world->genSettings.type != WorldGen_SuperFlat;
                position.y = shouldOffset ? spawnY + 1 : spawnY;
                position.z = spawnPos.z;
            }
            hp = 20;
            hunger = 20;
        } else {
           // DebugUI_Log("lol ur world is gone");
        }
    }
}

bool Player::CanMove(mc::Vector3d newVec) {
    for (int x = -1; x < 2; x++) {
        for (int y = 0; y < 3; y++) {
            for (int z = -1; z < 2; z++) {
                int pX = FastFloor(newVec.x) + x;
                int pY = FastFloor(newVec.y) + y;
                int pZ = FastFloor(newVec.z) + z;
                if (World_GetBlock(world, pX, pY, pZ) != Block_Air &&
                    World_GetBlock(world, pX, pY, pZ) != Block_Lava &&
                    World_GetBlock(world, pX, pY, pZ) != Block_Water) {
                    if (AABB_Overlap(newVec.x - PLAYER_COLLISIONBOX_SIZE / 2.f, newVec.y,
                                     newVec.z - PLAYER_COLLISIONBOX_SIZE / 2.f,
                                     PLAYER_COLLISIONBOX_SIZE, PLAYER_HEIGHT, PLAYER_COLLISIONBOX_SIZE,
                                     pX, pY, pZ, 1.f, 1.f, 1.f)) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void Player::Jump(mc::Vector3d accl) {
    if (grounded && !flying) {
        velocity.x = accl.x * 1.1f;
        velocity.z = accl.z * 1.1f;
        velocity.y = 6.7f;
        jumped = true;
        crouching = false;
    }
}

void Player::Move(float dt, mc::Vector3d accl) {
    breakPlaceTimeout -= dt;
    simStepAccum += dt;
    const float SimStep = 1.f / 60.f;

    while (simStepAccum >= SimStep) {
        velocity.y -= GravityPlusFriction * SimStep * 2.f;
        if (velocity.y < MaxFallVelocity)
            velocity.y = MaxFallVelocity;

        if (flying)
            velocity.y = 0.f;

        float speedFactor = 1.f;
        if (!grounded && !flying) {
            speedFactor = jumped ? 0.2f : 0.6f;
        } else if (flying) {
            speedFactor = 2.f;
        } else if (crouching) {
            speedFactor = 0.5f;
        }

        mc::Vector3d newPos =  position + ( Vector3d_Scale(velocity, SimStep) + Vector3d_Scale(accl, SimStep * speedFactor) );

        mc::Vector3d finalPos = position;

        bool wallCollision = false, wasGrounded = grounded;

        grounded = false;
        for (int j = 0; j < 3; j++) {
            int i = (int[]){0, 2, 1}[j];
            bool collision = false;
            mc::Vector3d axisStep = finalPos;
            axisStep.values[i] = newPos.values[i];

            Box playerBox = Box_Create(
                    axisStep.x - PLAYER_COLLISIONBOX_SIZE / 2.f,
                    axisStep.y,
                    axisStep.z - PLAYER_COLLISIONBOX_SIZE / 2.f,
                    PLAYER_COLLISIONBOX_SIZE,
                    PLAYER_HEIGHT,
                    PLAYER_COLLISIONBOX_SIZE
            );

            for (int x = -1; x < 2; x++) {
                for (int y = 0; y < 3; y++) {
                    for (int z = -1; z < 2; z++) {
                        int pX = FastFloor(axisStep.x) + x;
                        int pY = FastFloor(axisStep.y) + y;
                        int pZ = FastFloor(axisStep.z) + z;

                        if (World_GetBlock(world, pX, pY, pZ) != Block_Air &&
                            World_GetBlock(world, pX, pY, pZ) != Block_Lava &&
                            World_GetBlock(world, pX, pY, pZ) != Block_Water) {
                            Box blockBox = Box_Create(pX, pY, pZ, 1, 1, 1);

                            mc::Vector3d normal(0.f, 0.f, 0.f);
                            float depth = 0.f;
                            int face = 0;

                            bool intersects = Collision_BoxIntersect(blockBox, playerBox, 0, &normal, &depth, &face);
                            collision |= intersects;
                        }
                    }
                }
            }

            if (!collision) {
                finalPos.values[i] = newPos.values[i];
            } else if (i == 1) {
                if (velocity.y < 0.f || accl.y < 0.f)
                    grounded = true;
                jumped = false;
                velocity.x = 0.f;
                velocity.y = 0.f;
                velocity.z = 0.f;
            } else {
                wallCollision = true;
                if (i == 0)
                    velocity.x = 0.f;
                else
                    velocity.z = 0.f;
            }
        }

        mc::Vector3d movDiff = finalPos - position;

        if (grounded && flying)
            flying = false;

        if (wallCollision && autoJumpEnabled) {

            mc::Vector3d nrmDiff = newPos - position;
            nrmDiff.Normalize();

            Block block = World_GetBlock(world,
                                         FastFloor(finalPos.x + nrmDiff.x),
                                         FastFloor(finalPos.y + nrmDiff.y) + 2,
                                         FastFloor(finalPos.z + nrmDiff.z)
            );
            Block landingBlock = World_GetBlock(world,
                                                FastFloor(finalPos.x + nrmDiff.x),
                                                FastFloor(finalPos.y + nrmDiff.y) + 1,
                                                FastFloor(finalPos.z + nrmDiff.z)
            );

            if ((block == Block_Air || block == Block_Lava || block == Block_Water) &&
                landingBlock != Block_Air && landingBlock != Block_Lava && landingBlock != Block_Water) {
                Jump(accl);
            }
        }

        if (crouching && crouchAdd > -0.3f)
            crouchAdd -= SimStep * 2.f;
        if (!crouching && crouchAdd < 0.0f)
            crouchAdd += SimStep * 2.f;

        if (crouching && !grounded && wasGrounded && finalPos.y < position.y &&
            movDiff.x != 0.f && movDiff.z != 0.f) {
            finalPos = position;
            grounded = true;
            velocity.y = 0.f;
        }

        position = finalPos;

        velocity.x = velocity.x * 0.95f;
       // velocity.y = velocity.y;
        velocity.z = velocity.z * 0.95f;

        if (ABS(velocity.x) < 0.1f)
            velocity.x = 0.f;
        if (ABS(velocity.z) < 0.1f)
            velocity.z = 0.f;

        simStepAccum -= SimStep;
    }
}

void Player::PlaceBlock() {
    if (world && blockInActionRange && breakPlaceTimeout < 0.f) {
        const int* offset = DirectionToOffset[viewRayCast.direction];

        if (AABB_Overlap(
                position.x - PLAYER_COLLISIONBOX_SIZE / 2.f, position.y,
                position.z - PLAYER_COLLISIONBOX_SIZE / 2.f,
                PLAYER_COLLISIONBOX_SIZE, PLAYER_HEIGHT, PLAYER_COLLISIONBOX_SIZE,
                viewRayCast.x + offset[0], viewRayCast.y + offset[1],
                viewRayCast.z + offset[2], 1.f, 1.f, 1.f))
            return;

        World_SetBlockAndMeta(world,
                              viewRayCast.x + offset[0],
                              viewRayCast.y + offset[1],
                              viewRayCast.z + offset[2],
                              quickSelectBar[quickSelectBarSlot].block,
                              quickSelectBar[quickSelectBarSlot].meta
        );
    }

    if (breakPlaceTimeout < 0.f)
        breakPlaceTimeout = PLAYER_PLACE_REPLACE_TIMEOUT;
}

void Player::BreakBlock() {
    if (world && blockInActionRange && breakPlaceTimeout < 0.f) {
        World_SetBlock(world, viewRayCast.x, viewRayCast.y, viewRayCast.z, Block_Air);
    }

    if (breakPlaceTimeout < 0.f)
        breakPlaceTimeout = PLAYER_PLACE_REPLACE_TIMEOUT;
}