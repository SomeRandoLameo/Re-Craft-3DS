#pragma once

#include "Player.h"

typedef struct {
    char cause;
    int time;
}Damage;

void OvertimeDamage(char* cause, int time);