#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "../entity/Player.h"

#include "../world/CT_World.h"

typedef struct {
    char worldName[WORLD_NAME_SIZE];
    GeneratorSettings settings;    
} WorldSelect_Result;

void WorldSelect_Init();
void WorldSelect_Deinit();

void WorldSelect_Render();
bool WorldSelect_Update(char* out_worldpath, char* out_name, WorldGenType* worldType, bool* newWorld, bool* isMP);

void WorldSelect_ScanWorlds();