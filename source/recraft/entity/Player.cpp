#include "entity/Player.hpp"
#include "entity/Damage.hpp"
#include "input/InputManager.hpp"
#include "gui/DebugUI.hpp"
#include "mcbridge/MCBridge.hpp"
#include "misc/Collision.hpp"

const float MaxWalkVelocity = 4.3f;
const float MaxFallVelocity = -50.f;
const float GravityPlusFriction = 10.f;

// TODO: [Server thread/WARN]: Nintendo3DS moved too quickly!
// This playercontroller needs to be rewritten to move like a proper Minecraft player.
// This implementation seems off and is just craftus at core.
// Since we are aiming for Minecraft compatibility, this needs to be fixed and adjusted to controller.

// Also, this player is a mess code-wise.
// This needs to be restructured into an entity base class and player derived class. (Entity because mobs, Particles, TileEntities etc. will share a lot of code too.)
// Inventory should also be separated.

Player::Player(World* world): Entity(world), m_input(new PlayerInput()) {

    heightOffset = 1.62f;
    SetSize(0.6f, 1.8f);

    InitializeInventory();
}

//TODO: Separate from player
void Player::InitializeInventory() {
    int slotIndex = 0;
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Air),1,0); // Testing
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stone),1,0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Dirt), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Grass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Cobblestone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sand), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Log), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Leaves), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Glass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Stonebrick), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Brick), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Planks), 1, 0);
    for (u8 i = 0; i < 16; i++) {
        inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Wool), 1, i);
    }
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Bedrock), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gravel), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Coarse), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Door_Top), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Door_Bottom), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Snow_Grass), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Snow), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Obsidian), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Netherrack), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Sandstone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Smooth_Stone), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Crafting_Table), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Grass_Path), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Lava), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Water), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Iron_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Coal_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Coal_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gold_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Gold_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Diamond_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Diamond_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Emerald_Block), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Emerald_Ore), 1, 0);
    inventory[slotIndex++] = mc::inventory::Slot(static_cast<s32>(BlockID::Furnace), 1, 0);

    for (auto & i : quickSelectBar) {
        i = mc::inventory::Slot(static_cast<s32>(BlockID::Air), 0, 0);
    }

}

void Player::Update(Damage* dmg) {
    view.x = -sinf(yRot) * cosf(xRot);
    view.y = sinf(xRot);
    view.z = -cosf(yRot) * cosf(xRot);

    blockInSight = Raycast_Cast(m_world, ToVector3d(position) + mc::Vector3d(0, EyeHeight, 0), view, &viewRayCast);
    blockInActionRange = blockInSight && viewRayCast.distSqr < 3.5f * 3.5f * 3.5f;

    if(gamemode != 1){
        HandleFallDamage();
        HandleFireDamage();
        HandleHunger();
    }

    HandleRespawn(dmg);
}

void Player::UpdateMovement(DebugUI* dbg, float dt) {
    Damage dmg;

    m_input->update();

    if (m_input->isMoved) {
        float jump  = Input::isPressed(INP_JUMP) ? 1.f : 0.f;
        float sneak = Input::isPressed(INP_SNEAK) ? 1.f : 0.f;

        float moveUp    = getCtrlMove()->up / PAD_MAXF;
        float moveDown  = getCtrlMove()->down / PAD_MAXF;
        float moveLeft  = getCtrlMove()->left / PAD_MAXF;
        float moveRight = getCtrlMove()->right / PAD_MAXF;

        forwardVec = mc::Vector3d(-sinf(yRot), 0.f, -cosf(yRot));
        rightVec   = Vector3d_crs(forwardVec, c_vecY);

        movement = Vector3d_Scale(forwardVec, moveUp);
        movement -= Vector3d_Scale(forwardVec, moveDown);
        movement -= Vector3d_Scale(rightVec, moveLeft);
        movement += Vector3d_Scale(rightVec, moveRight);

        speed = 4.3f * Vector3d_mag(mc::Vector3d(-moveLeft + moveRight, -sneak + jump, -moveUp + moveDown));
    }

    if (flying) { // fly up and down
        if (Input::isPressed(INP_FLYUP)) {
            movement += c_vecY;
        }
        if (Input::isPressed(INP_FLYDOWN)) {
            movement -= c_vecY;
        }
    }

    if (Vector3d_MagSqr(movement) > 0.f) {
        bobbing += speed * 3.5f * dt;
        movement.Normalize();
        movement = Vector3d_Scale(movement, speed);
    }

    if (m_input->isLooked) { // only recalculate when needed
        float lookUp    = getCtrlLook()->up;
        float lookDown  = getCtrlLook()->down;
        float lookLeft  = getCtrlLook()->left;
        float lookRight = getCtrlLook()->right;

        yRot += (lookLeft + -lookRight) * DEG_TO_RAD * dt;
        xRot += (-lookDown + lookUp) * DEG_TO_RAD * dt;
        xRot = CLAMP(xRot, -DEG_TO_RAD * 89.9f, DEG_TO_RAD * 89.9f);
    }

    // TODO: Segfault sometimes when rejoining the world, this is the problem.
    mc::inventory::Slot curSlot   = quickSelectBar[quickSelectBarSlot];
    auto                curStack  = MCBridge::MCLIBSlotToCTItemStack(curSlot);
    bool                slotEmpty = (curStack.block == BlockID::Air);

    bool hitEntity = viewRayCast.entity;

    if (Input::isPressed(INP_ATTACK)) {
        if (hitEntity) {
            HurtEntity();
        } else {
            BreakBlock();
        }
    }

    if (Input::isPressed(INP_USE)) {
        if (slotEmpty) {
            Interact(dbg);
        } else {
            PlaceBlock();
        }
    }


    if (Input::isPressed(INP_JUMP)) {
        Jump(mc::Vector3d(movement.x, movement.y, movement.z));
    }


    if (m_flyTimer >= 0.f) {
        if (Input::isPressed(INP_JUMP)) {
            flying ^= true;
        }

        m_flyTimer += dt;
        if (m_flyTimer > 0.25f) {
            m_flyTimer = -1.f;
        }

    } else if (Input::isReleased(INP_JUMP)) {
        m_flyTimer = 0.f;
    }

    releasedCrouch = Input::isReleased(INP_SNEAK);
    crouching ^= !flying && releasedCrouch;

    bool switchBlockLeft  = Input::isReleased(INP_HOTBAR_LEFT);
    bool switchBlockRight = Input::isReleased(INP_HOTBAR_RIGHT);

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

        bool cmdLine = Input::isReleased(INP_CHAT);
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
        if (m_world->GetBlockID(mc::Vector3i(position.x, position.y - 1, position.z)) != BlockID::Air) {
            hp = hp + rndy;
            rndy = 0;
        }
    }
}

void Player::HandleFireDamage() {
    if (m_world->GetBlockID(ToVector3i(ToVector3d(position))) == BlockID::Lava) {
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
                while (m_world->GetBlockID(mc::ToVector3i(spawnPos)) != BlockID::Air)
                    spawnY++;

                bool shouldOffset = m_world->GetGenSettings().type != WorldGenType::SuperFlat;
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
                while (m_world->GetBlockID(ToVector3i(spawnPos)) != BlockID::Air)
                    spawnY++;

                bool shouldOffset = m_world->GetGenSettings().type != WorldGenType::SuperFlat;
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

void Player::Jump(mc::Vector3d accl) {
    if (onGround && !flying) {
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
        if (!onGround && !flying) {
            speedFactor = jumped ? 0.2f : 0.6f;
        } else if (flying) {
            speedFactor = 2.f;
        } else if (crouching) {
            speedFactor = 0.5f;
        }

        mc::Vector3f newPos =  position + ToVector3f( Vector3d_Scale(velocity, SimStep) + Vector3d_Scale(accl, SimStep * speedFactor) );

        mc::Vector3f finalPos = position;

        bool wallCollision = false, wasGrounded = onGround;

        onGround = false;
        for (int j = 0; j < 3; j++) {
            int i = (int[]){0, 2, 1}[j];
            bool collision = false;
            mc::Vector3f axisStep = finalPos;
            axisStep.values[i] = newPos.values[i];

            Box playerBox = Box_Create(
                    axisStep.x - CollisionBoxSize / 2.f,
                    axisStep.y,
                    axisStep.z - CollisionBoxSize / 2.f,
                    CollisionBoxSize,
                    Height,
                    CollisionBoxSize
            );

            for (int x = -1; x < 2; x++) {
                for (int y = 0; y < 3; y++) {
                    for (int z = -1; z < 2; z++) {
                        auto blockPos = mc::Vector3i(
                            FastFloor(axisStep.x) + x,
                            FastFloor(axisStep.y) + y,
                            FastFloor(axisStep.z) + z
                        );

                        if (m_world->GetBlockID(blockPos) != BlockID::Air &&
                            m_world->GetBlockID(blockPos) != BlockID::Lava &&
                            m_world->GetBlockID(blockPos) != BlockID::Water) {
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
                    onGround = true;
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

        mc::Vector3f movDiff = finalPos - position;

        if (onGround && flying) {
            flying = false;
        }

        if (wallCollision && autoJumpEnabled) {

            mc::Vector3f nrmDiff = newPos - position;
            nrmDiff.Normalize();

            BlockID block = m_world->GetBlockID(mc::Vector3i(FastFloor(finalPos.x + nrmDiff.x),
                                                             FastFloor(finalPos.y + nrmDiff.y) + 2,
                                                             FastFloor(finalPos.z + nrmDiff.z)));
            BlockID landingBlock = m_world->GetBlockID(mc::Vector3i(FastFloor(finalPos.x + nrmDiff.x),
                                                                    FastFloor(finalPos.y + nrmDiff.y) + 1,
                                                                    FastFloor(finalPos.z + nrmDiff.z)));

            if ((block == BlockID::Air || block == BlockID::Lava || block == BlockID::Water) &&
                landingBlock != BlockID::Air && landingBlock != BlockID::Lava && landingBlock != BlockID::Water) {
                Jump(accl);
            }
        }

        if (crouching && crouchAdd > -0.3f) {
            crouchAdd -= SimStep * 2.f;
        }

        if (!crouching && crouchAdd < 0.0f) {
            crouchAdd += SimStep * 2.f;
        }

        if (crouching && !onGround && wasGrounded && finalPos.y < position.y &&
            movDiff.x != 0.f && movDiff.z != 0.f) {
            finalPos = position;
            onGround = true;
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
        mc::Vector3i placePos(
            viewRayCast.hitPos.x + offset[0],
            viewRayCast.hitPos.y + offset[1],
            viewRayCast.hitPos.z + offset[2]
        );

        // Check if block placement would intersect with player
        AABB blockBox = AABB(
            mc::Vector3f(placePos.x, placePos.y, placePos.z),
            mc::Vector3f(placePos.x + 1, placePos.y + 1, placePos.z + 1)
        );

        if (bb.Intersect(blockBox)) {
            return;
        }

        // Place the block
        m_world->SetBlockAndMeta(
            placePos,
            MCBridge::MCLIBSlotToCTItemStack(quickSelectBar[quickSelectBarSlot]).block,
            MCBridge::MCLIBSlotToCTItemStack(quickSelectBar[quickSelectBarSlot]).meta
        );
    }

    if (breakPlaceTimeout < 0.f) {
        breakPlaceTimeout = PlaceReplaceTimeout;
    }
}

void Player::BreakBlock() {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        m_world->SetBlockID(viewRayCast.hitPos, BlockID::Air);
    }

    if (breakPlaceTimeout < 0.f) {
        breakPlaceTimeout = PlaceReplaceTimeout;
    }
}

void Player::HurtEntity() {
    // TODO:
}

// TODO: When triggered, this cuts the o3DS performance in half...
void Player::Interact(DebugUI* dbg) {
    if (m_world && blockInActionRange && breakPlaceTimeout < 0.f) {
        BlockID id = m_world->GetBlockID(viewRayCast.hitPos);
        Metadata meta = m_world->GetMetadata(viewRayCast.hitPos);
        std::string logMsg = "Target at: " + to_string(viewRayCast.hitPos) +
            "  ID=" + std::to_string(static_cast<u8>(id)) +
            "  Meta=" + std::to_string(meta);

        dbg->Log(logMsg.c_str());
    }
}