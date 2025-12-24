#pragma once



//mclib
#include <mclib/common/Vector.h>

#include "world/CT_World.h"
#include "gui/CT_Inventory.h"
#include "inventory/ItemStack.h"
#include "entity/Damage.h"
#include "misc/Raycast.h"
#include "misc/VecMath.h"
#include "misc/InputData.h"

class PlayerControlScheme;
class PlayerController;

#define PLAYER_EYEHEIGHT (1.65f)
#define PLAYER_HEIGHT (1.8f)
#define PLAYER_COLLISIONBOX_SIZE (0.65f)
#define PLAYER_HALFEYEDIFF (0.07f)
#define PLAYER_PLACE_REPLACE_TIMEOUT (0.2f)

class Player {
public:
    Player(World* world);
    ~Player() = default;

    void Update(Damage* dmg);
    void UpdateMovement(PlayerControlScheme m_controlScheme, InputData input, float dt);
    void Move(float dt, mc::Vector3d accl);
    void PlaceBlock();
    void BreakBlock();
    void Jump(mc::Vector3d accl);
    bool CanMove(mc::Vector3d newVec);

    mc::Vector3d position = mc::Vector3d(0.f, 0.f, 0.f);
    mc::Vector3d velocity = mc::Vector3d(0, 0, 0);

    float pitch = 0.f;
    float yaw = 0.f;
    float bobbing = 0.f;
    float fovAdd = 0.f;
    float crouchAdd = 0.f;
    bool grounded = false;
    bool jumped = false;
    bool sprinting = false;
    bool flying = false;
    bool crouching = false;


    // experience is a value between 0 and 0.99*
    float experience = 0.1;
    float experienceLevel = 5000000;

    mc::Vector3d view = mc::Vector3d(0, 0, -1);
    bool autoJumpEnabled = true;

    float simStepAccum = 0.f;

    float breakPlaceTimeout = 0.f;
    int hungertimer = 0;

    int hp = 20;
    int armour = 0;
    int oxygen = 0;
    int hunger = 20;

    int difficulty = 1;

    float rndy = 0.f;

    mc::Vector3d spawnPos = mc::Vector3d(0, 0, 0);

    bool spawnset = false;

    int gamemode = 0;
    bool cheats = false;

    int quickSelectBarSlot = 0;
    int inventorySite = 1;
    mc::inventory::Slot quickSelectBar[9];

    Raycast_Result viewRayCast;
    bool blockInSight = false;
    bool blockInActionRange = false;
    mc::inventory::Slot inventory[64];

private:
    World* m_world = nullptr;

    float m_breakPlaceTimeout = 0.f;
    float m_flyTimer = -1.f;

    bool m_openedCmd = false;

    void InitializeInventory();
    void HandleFallDamage();
    void HandleFireDamage();
    void HandleHunger();
    void HandleRespawn(Damage* dmg);
};