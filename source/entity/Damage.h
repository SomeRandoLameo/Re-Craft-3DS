#pragma once

#include "Player.h"
//TODO: STD::STRING <- char*
typedef struct {
    char* cause;
    int time;
}Damage;

void OvertimeDamage(char* cause, int time);