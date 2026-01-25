#pragma once

#include <stdbool.h>

// mclib
#include <mclib/common/Vector.h>

#include "gui/CT_Inventory.hpp"
#include "input/PlayerInput.hpp"
#include "inventory/ItemStack.hpp"
#include "misc/Raycast.hpp"
#include "misc/VecMath.hpp"

struct Damage;
class DebugUI;
class PadAngles;
class World;
class Player {
public:
    Player(World* world);
    ~Player() = default;

    void Update(Damage* dmg);
    void UpdateMovement(DebugUI* dbg, float dt);
    void Move(float dt, mc::Vector3f accl);
    void PlaceBlock();
    void HurtEntity();
    void BreakBlock();
    void Interact(DebugUI* dbg);

    void Jump(mc::Vector3f accl);
    bool CanMove(mc::Vector3f newVec);

    mc::Vector3f position = c_vecZero;
    mc::Vector3f velocity = c_vecZero;

    mc::Vector3f forwardVec = c_vecZero;
    mc::Vector3f rightVec   = c_vecZero;
    mc::Vector3f movement   = c_vecZero;

    float speed     = 0.f;
    float pitch     = 0.f;
    float yaw       = 0.f;
    float bobbing   = 0.f;
    float fovAdd    = 0.f;
    float crouchAdd = 0.f;
    bool  grounded  = false;
    bool  jumped    = false;
    bool  sprinting = false;
    bool  flying    = false;

    bool releasedCrouch = false;
    bool crouching      = false;

    // experience is a value between 0 and 0.99*
    float experience      = 0.1;
    float experienceLevel = 5000000;

    mc::Vector3f view            = mc::Vector3f(0, 0, -1);
    bool         autoJumpEnabled = true;

    float simStepAccum = 0.f;

    float breakPlaceTimeout = 0.f;
    int   hungertimer       = 0;

    int hp     = 20;
    int armour = 0;
    int oxygen = 0;
    int hunger = 20;

    int difficulty = 1;

    float rndy = 0.f; // whatever this is

    mc::Vector3f spawnPos = c_vecZero;

    bool spawnset = false;

    int  gamemode = 0;
    bool cheats   = false;

    int                 quickSelectBarSlot = 0;
    int                 inventorySite      = 1;
    mc::inventory::Slot quickSelectBar[9];

    Raycast_Result      viewRayCast;
    bool                blockInSight       = false;
    bool                blockInActionRange = false;
    mc::inventory::Slot inventory[64];


    static constexpr float EyeHeight           = 1.65f;
    static constexpr float Height              = 1.8f;
    static constexpr float CollisionBoxSize    = 0.65f;
    static constexpr float HalfEyeDiff         = 0.07f;
    static constexpr float PlaceReplaceTimeout = 0.2f;

    inline PadAngles* getCtrlMove() { return &m_move->data.move; }
    inline PadAngles* getCtrlLook() { return &m_move->data.look; }

private:
    World* m_world = nullptr;

    PlayerInput* m_move = nullptr;

    float m_breakPlaceTimeout = 0.f;
    float m_flyTimer          = -1.f;

    bool m_openedCmd = false;

    void InitializeInventory();
    void HandleFallDamage();
    void HandleFireDamage();
    void HandleHunger();
    void HandleRespawn(Damage* dmg);
};
