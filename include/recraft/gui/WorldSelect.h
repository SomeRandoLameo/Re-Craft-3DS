#pragma once

#include <c3d/texture.h>
#include <stdbool.h>
#include <stdint.h>
#include "../entity/Player.h"

#include "../world/CT_World.h"
#include "rendering/Clouds.h"

typedef struct {
    char worldName[World::NameSize];
    GeneratorSettings settings;    
} WorldSelect_Result;

typedef struct {
    uint32_t lastPlayed;
    char name[World::NameSize];
    char path[256];
} WorldInfo;

static std::vector<WorldInfo> worlds;


typedef enum { WorldSelect, ConfirmDeletion, WorldOptions } MenuState;

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

static const char* gamemodestr[]={"Survival","Creative","Adventure","Spectator"};

static const char* worldGenTypesStr[] = {"Smea", "Superflat"};

static MenuState menustate = MenuState::WorldSelect;

static float max_velocity = 20.f;

static char out_worldpath[256];
static char out_name[World::NameSize] = {'\0'};
static WorldGenType worldType;
static bool newWorld = false;



void WorldSelect_Init();
void WorldSelect_Deinit();

void WorldSelect_RenderTop(Clouds* m_clouds, int m_world_shader_uLocProjection, int eyeIndex, float iod, World* m_world, C3D_Tex* m_logoTex);
void WorldSelect_RenderBot();

void WorldSelect_Update(Player* player);

void WorldSelect_ScanWorlds();