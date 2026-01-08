#include "entity/Player.h"
#include "misc/Collision.h"
#include "entity/Damage.h"
#include "gui/DebugUI.h"
#include "mcbridge/MCBridge.h"
#include "entity/PlayerController.h"

const float MaxWalkVelocity = 4.3f;
const float MaxFallVelocity = -50.f;
const float GravityPlusFriction = 10.f;

Player::Player(World* world) {
    m_world = world;

    InitializeInventory();
}

//TODO: Separate from player
void Player::InitializeInventory() {
    int l = 0;
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Stone),1,0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Dirt), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Grass), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Cobblestone), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Sand), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Log), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Leaves), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Glass), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Stonebrick), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Brick), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Planks), 1, 0);
    for (int i = 0; i < 16; i++) {
        inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Wool), 1,static_cast<u8>(i));
    }
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Bedrock), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Gravel), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Coarse), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Door_Top), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Door_Bottom), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Snow_Grass), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Snow), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Obsidian), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Netherrack), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Sandstone), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Smooth_Stone), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Crafting_Table), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Grass_Path), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Lava), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Water), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Iron_Block), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Iron_Ore), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Coal_Block), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Coal_Ore), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Gold_Block), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Gold_Ore), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Diamond_Block), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Diamond_Ore), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Emerald_Block), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Emerald_Ore), 1, 0);
    inventory[l++] = mc::inventory::Slot(static_cast<s32>(Block::Furnace), 1, 0);

    for (auto & i : quickSelectBar) {
        i = mc::inventory::Slot(static_cast<s32>(Block::Air), 0, 0);
    }

}

void Player::Update(Damage* dmg) {
    view.x = -sinf(yaw) * cosf(pitch);
    view.y = sinf(pitch);
    view.z = -cosf(yaw) * cosf(pitch);

    blockInSight = Raycast_Cast(m_world, position + mc::Vector3d(0,PLAYER_EYEHEIGHT,0), view, &viewRayCast);
    blockInActionRange = blockInSight && viewRayCast.distSqr < 3.5f * 3.5f * 3.5f;

    if(gamemode != 1){
        HandleFallDamage();
        HandleFireDamage();
        HandleHunger();
    }

    HandleRespawn(dmg);
}

void Player::UpdateMovement(PlayerControlScheme m_controlScheme, InputData input, float dt) {
    Damage dmg;

    Input in = {0};
    convertPlatformInput(&input, in.keys, in.keysdown, in.keysup);

    float jump          = in.IsKeyDown(m_controlScheme.jump, &in);
    float crouch        = in.IsKeyDown(m_controlScheme.crouch, &in);

    float forward       = in.IsKeyDown(m_controlScheme.forward, &in);
    float backward      = in.IsKeyDown(m_controlScheme.backward, &in);
    float strafeLeft    = in.IsKeyDown(m_controlScheme.strafeLeft, &in);
    float strafeRight   = in.IsKeyDown(m_controlScheme.strafeRight, &in);

    mc::Vector3d forwardVec(-sinf(yaw), 0.f, -cosf(yaw));
    mc::Vector3d rightVec = Vector3d_crs(forwardVec, mc::Vector3d(0, 1, 0));

    mc::Vector3d movement(0, 0, 0);
    movement = movement + Vector3d_Scale(forwardVec, forward);
    movement = movement - Vector3d_Scale(forwardVec, backward);
    movement = movement + Vector3d_Scale(rightVec, strafeRight);
    movement = movement - Vector3d_Scale(rightVec, strafeLeft);

    if (flying) {
        movement = movement + mc::Vector3d(0.f, jump, 0.f);
        movement = movement + mc::Vector3d(0.f, crouch, 0.f);
    }

    if (Vector3d_MagSqr(movement) > 0.f) {
        float speed = 4.3f * Vector3d_mag(mc::Vector3d(-strafeLeft + strafeRight, -crouch + jump, -forward + backward));
        bobbing += speed * 1.5f * dt;
        movement.Normalize();
        movement = Vector3d_Scale(movement, speed);
    }

    float lookLeft  = in.IsKeyDown(m_controlScheme.lookLeft, &in);
    float lookRight = in.IsKeyDown(m_controlScheme.lookRight, &in);
    float lookUp    = in.IsKeyDown(m_controlScheme.lookUp, &in);
    float lookDown  = in.IsKeyDown(m_controlScheme.lookDown, &in);

    yaw += (lookLeft + -lookRight) * 160.f * DEG_TO_RAD * dt;
    pitch += (-lookDown + lookUp) * 160.f * DEG_TO_RAD * dt;
    pitch = CLAMP(pitch, -DEG_TO_RAD * 89.9f, DEG_TO_RAD * 89.9f);

    float placeBlock = in.IsKeyDown(m_controlScheme.placeBlock, &in);
    float breakBlock = in.IsKeyDown(m_controlScheme.breakBlock, &in);

    if (placeBlock > 0.f) {
        PlaceBlock();
    }

    if (breakBlock > 0.f) {
        BreakBlock();
    }


    if (jump > 0.f) {
        Jump(mc::Vector3d(movement.x, movement.y, movement.z));
    }


    bool releasedJump = in.WasKeyReleased(m_controlScheme.jump, &in);
    if (m_flyTimer >= 0.f) {
        if (jump > 0.f) {
            flying ^= true;
        }

        m_flyTimer += dt;
        if (m_flyTimer > 0.25f) {
            m_flyTimer = -1.f;
        }

    } else if (releasedJump) {
        m_flyTimer = 0.f;
    }

    bool releasedCrouch = in.WasKeyReleased(m_controlScheme.crouch, &in);
    crouching ^= !flying && releasedCrouch;

    bool switchBlockLeft  = in.WasKeyPressed(m_controlScheme.switchBlockLeft, &in);
    bool switchBlockRight = in.WasKeyPressed(m_controlScheme.switchBlockRight, &in);

    if (switchBlockLeft && --quickSelectBarSlot == -1) {
        quickSelectBarSlot = 8;
    }

    if (switchBlockRight && ++quickSelectBarSlot == 9) {
        quickSelectBarSlot = 0;
    }
/*
    if (m_openedCmd) {
        dt = 0.f;
        m_openedCmd = false;
    }

    bool cmdLine = WasKeyPressed(m_controlScheme.openCmd, &agnosticInput);
    if (cmdLine) {
        CommandLine_Activate(world, this, debugUi);
        m_openedCmd = true;
    }
*/
    Move(dt, mc::Vector3d(movement.x, movement.y, movement.z));
    Update(&dmg);
}

void Player::HandleFallDamage() {
    if (velocity.y <= -12) {
        rndy = round(velocity.y);
        if (m_world->GetBlock(mc::Vector3i(position.x, position.y - 1, position.z)) != Block::Air) {
            hp = hp + rndy;
            rndy = 0;
        }
    }
}

void Player::HandleFireDamage() {
    if (m_world->GetBlock(ToVector3i(position)) == Block::Lava) {
      //  DebugUI_Log("ur burning lol");
        //OvertimeDamage("Fire", 10);
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

void Player::HandleRespawn(Damage* dmg) {
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
                while (m_world->GetBlock( mc::ToVector3i(spawnPos)) != Block::Air)
                    spawnY++;

                bool shouldOffset = m_world->GetGenSettings().type != WorldGen_SuperFlat;
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
                while (m_world->GetBlock(ToVector3i(spawnPos)) != Block::Air)
                    spawnY++;

                bool shouldOffset = m_world->GetGenSettings().type != WorldGen_SuperFlat;
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

                mc::Vector3i blockPos(
                    FastFloor(newVec.x) + x,
                    FastFloor(newVec.y) + y,
                    FastFloor(newVec.z) + z
                );

                if (m_world->GetBlock(blockPos) != Block::Air &&
                    m_world->GetBlock(blockPos) != Block::Lava &&
                    m_world->GetBlock(blockPos) != Block::Water) {
                    if (AABB_Overlap(
                            newVec.x - PLAYER_COLLISIONBOX_SIZE / 2.f,
                            newVec.y,
                            newVec.z - PLAYER_COLLISIONBOX_SIZE / 2.f,
                            PLAYER_COLLISIONBOX_SIZE,
                            PLAYER_HEIGHT,
                            PLAYER_COLLISIONBOX_SIZE,
                            blockPos.x,
                            blockPos.y,
                            blockPos.z,
                            1.f,
                            1.f,
                            1.f
                       )
                    ) {
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
        if (velocity.y < MaxFallVelocity) {
            velocity.y = MaxFallVelocity;
        }

        if (flying) {
            velocity.y = 0.f;
        }


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
                        auto blockPos = mc::Vector3i(
                            FastFloor(axisStep.x) + x,
                            FastFloor(axisStep.y) + y,
                            FastFloor(axisStep.z) + z
                        );

                        if (m_world->GetBlock(blockPos) != Block::Air &&
                            m_world->GetBlock(blockPos) != Block::Lava &&
                            m_world->GetBlock(blockPos) != Block::Water) {
                            Box blockBox = Box_Create(blockPos.x, blockPos.y, blockPos.z, 1, 1, 1);

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
                if (velocity.y < 0.f || accl.y < 0.f) {
                    grounded = true;
                }
                jumped = false;
                velocity.x = 0.f;
                velocity.y = 0.f;
                velocity.z = 0.f;
            } else {
                wallCollision = true;
                if (i == 0) {
                    velocity.x = 0.f;
                } else {
                    velocity.z = 0.f;
                }
            }
        }

        mc::Vector3d movDiff = finalPos - position;

        if (grounded && flying) {
            flying = false;
        }

        if (wallCollision && autoJumpEnabled) {

            mc::Vector3d nrmDiff = newPos - position;
            nrmDiff.Normalize();

            Block block = m_world->GetBlock(
                mc::Vector3i(
                    FastFloor(finalPos.x + nrmDiff.x),
                    FastFloor(finalPos.y + nrmDiff.y) + 2,
                    FastFloor(finalPos.z + nrmDiff.z)
                )
            );
            Block landingBlock = m_world->GetBlock(
                mc::Vector3i(
                    FastFloor(finalPos.x + nrmDiff.x),
                    FastFloor(finalPos.y + nrmDiff.y) + 1,
                    FastFloor(finalPos.z + nrmDiff.z)
                )
            );

            if ((block == Block::Air || block == Block::Lava || block == Block::Water) &&
                landingBlock != Block::Air && landingBlock != Block::Lava && landingBlock != Block::Water) {
                Jump(accl);
            }
        }

        if (crouching && crouchAdd > -0.3f) {
            crouchAdd -= SimStep * 2.f;
        }

        if (!crouching && crouchAdd < 0.0f) {
            crouchAdd += SimStep * 2.f;
        }

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

        if (ABS(velocity.x) < 0.1f) {
            velocity.x = 0.f;
        }

        if (ABS(velocity.z) < 0.1f) {
            velocity.z = 0.f;
        }

        simStepAccum -= SimStep;
    }
}

void Player::PlaceBlock() {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        const int* offset = DirectionToOffset[viewRayCast.direction];

        if (AABB_Overlap(
                position.x - PLAYER_COLLISIONBOX_SIZE / 2.f, position.y,
                position.z - PLAYER_COLLISIONBOX_SIZE / 2.f,
                PLAYER_COLLISIONBOX_SIZE, PLAYER_HEIGHT, PLAYER_COLLISIONBOX_SIZE,
                viewRayCast.x + offset[0], viewRayCast.y + offset[1],
                viewRayCast.z + offset[2], 1.f, 1.f, 1.f))
            return;

        //TODO: Remove Ducttape
        m_world->SetBlockAndMeta(
                mc::Vector3i(
                        (int)viewRayCast.x + offset[0],
                        (int)viewRayCast.y + offset[1],
                        (int)viewRayCast.z + offset[2]
                ),
                MCBridge::MCLIBSlotToCTItemStack(quickSelectBar[quickSelectBarSlot]).block,
                MCBridge::MCLIBSlotToCTItemStack(quickSelectBar[quickSelectBarSlot]).meta
        );
    }

    if (breakPlaceTimeout < 0.f) {
        breakPlaceTimeout = PLAYER_PLACE_REPLACE_TIMEOUT;
    }
}

void Player::BreakBlock() {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        m_world->SetBlock(mc::Vector3i(viewRayCast.x, viewRayCast.y, viewRayCast.z), Block::Air);
    }

    if (breakPlaceTimeout < 0.f) {
        breakPlaceTimeout = PLAYER_PLACE_REPLACE_TIMEOUT;
    }
}