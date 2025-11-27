#pragma once

#include "Player.h"
//TODO: STD::STRING <- char*
typedef struct {
    char* cause;
    int time;
}Damage;

void OvertimeDamage(const char* cause, int time);
