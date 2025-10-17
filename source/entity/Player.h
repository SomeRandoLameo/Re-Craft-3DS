#pragma once

#include <stdbool.h>
#include "../world/CT_World.h"

#include "../gui/CT_Inventory.h"
#include "../inventory/ItemStack.h"
#include "Damage.h"

#include "../misc/Raycast.h"
#include "../misc/VecMath.h"
//#include "../misc/Sound.h"

#define PLAYER_EYEHEIGHT (1.65f)
#define PLAYER_HEIGHT (1.8f)
#define PLAYER_COLLISIONBOX_SIZE (0.65f)
#define PLAYER_HALFEYEDIFF (0.07f)

#define PLAYER_PLACE_REPLACE_TIMEOUT (0.2f)

typedef struct {
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
} Player;

void Player_Init(Player* player, World* world);

void Player_Update(Player* player, /*Sound* sound, */Damage* dmg);

void Player_Move(Player* player, float dt, float3 accl);

void Player_PlaceBlock(Player* player /*Sound* sound*/);
void Player_BreakBlock(Player* player);

void Player_Jump(Player* player, float3 accl);