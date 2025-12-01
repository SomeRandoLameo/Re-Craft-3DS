#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../entity/Player.h"

#include "../world/CT_World.h"

typedef struct {
    char worldName[WORLD_NAME_SIZE];
    GeneratorSettings settings;    
} WorldSelect_Result;

typedef struct {
    uint32_t lastPlayed;
    char name[WORLD_NAME_SIZE];
    char path[256];
} WorldInfo;

static std::vector<WorldInfo> worlds;


typedef enum { MenuState_SelectWorld, MenuState_ConfirmDeletion, MenuState_WorldOptions } MenuState;

static int scroll = 0;
static float velocity = 0.f;
static int selectedWorld = -1;

static bool clicked_play = false;
static bool clicked_new_world = false;
static bool clicked_mp_connect = false;
static bool clicked_delete_world = false;

static bool confirmed_world_options = false;
static bool canceled_world_options = false;

static bool confirmed_deletion = false;
static bool canceled_deletion = false;

static WorldGenType worldGenType = WorldGen_SuperFlat;

static gamemode gamemode1=Gamemode_Survival;

static char* gamemodestr[]={"Survival","Creative","Adventure","Spectator"};

static char* worldGenTypesStr[] = {"Smea", "Superflat"};

static MenuState menustate = MenuState_SelectWorld;

static float max_velocity = 20.f;


void WorldSelect_Init();
void WorldSelect_Deinit();

void WorldSelect_Render();
bool WorldSelect_Update(char* out_worldpath, char* out_name, WorldGenType* worldType, Player* player, bool* newWorld, bool* isMP);

void WorldSelect_ScanWorlds();