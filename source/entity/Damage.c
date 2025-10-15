#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "Player.h"
#include "Damage.h"

void OvertimeDamage(char* cause, int time) {
    Player* player = GetPlayer();

    if (player == NULL) {
        return;
    }

    if (strcmp(cause, "fire") == 0) {
        while (time > 0) {
            player->hp -= 1;
            sleep(1);
            time -= 1;
        }
    }
    else if (strcmp(cause, "poison") == 0) {
        while (time > 0) {
            player->hp -= 1;
            sleep(1);
            time -= 1;
        }
    }
}