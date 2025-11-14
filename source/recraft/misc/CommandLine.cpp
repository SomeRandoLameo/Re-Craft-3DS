#include "misc/CommandLine.h"
#include "world/savegame/SaveManager.h"

#include <3ds.h>


#include <mpack/mpack.h>

#include <stdio.h>
#include <string.h>

void CommandLine_Activate(World* world, Player* player,DebugUI* debugUi) {
	static SwkbdState swkbd;
	static char textBuffer[64];
	swkbdInit(&swkbd, SWKBD_TYPE_WESTERN, 2, 64);
	swkbdSetHintText(&swkbd, "Enter command");

	int button = swkbdInputText(&swkbd, textBuffer, sizeof(textBuffer));
	if (button == SWKBD_BUTTON_CONFIRM) {
		CommandLine_Execute(world, player, debugUi, textBuffer);
	}
}

void CommandLine_Execute(World* world, Player* player, DebugUI* debugUi, const char* text) {
	int length = strlen(text);
	mpack_writer_t writer;
	mpack_error_t err = mpack_writer_destroy(&writer);
	if (length >= 1 && text[0] == '/'&&player->cheats==true) {
		if (length >= 9) {
			float x, y, z;
			if (sscanf(&text[1], "tp %f %f %f", &x, &y, &z) == 3) {
				player->position.x = x;
				player->position.y = y + 1;
				player->position.z = z;
                debugUi->Log("Teleported to %f, %f %f", x, y, z);
			}
		}
		//int playerid;
		if (length == 2 && text[1] == 'k') {
			player->hp=0;
            debugUi->Log("Killed player");
		}
		int hp;
		if (sscanf(&text[1],"hp %i",&hp)) {
			if (hp>0&&hp<21){
				player->hp=hp;
                debugUi->Log("Set player hp to %i", hp);
			} else {
                debugUi->Log("Cannot set hp to %i",hp);
			}
		}
		float sx,sy,sz;
		if (sscanf(&text[1], "ws %f %f %f", &sx, &sy, &sz) == 3) {
			player->spawnx = sx;
			player->spawny = sy;
			player->spawnz = sz;
			player->spawnset=1;
			mpack_write_cstr(&writer, "sx");
			mpack_write_float(&writer,player->spawnx);
			mpack_write_cstr(&writer, "sy");
			mpack_write_float(&writer,player->spawny);
			mpack_write_cstr(&writer, "sz");
			mpack_write_float(&writer,player->spawnz);
			mpack_write_cstr(&writer, "ss");
			mpack_write_int(&writer,player->spawnset);
            debugUi->Log("Set spawn to %f, %f %f", sx, sy, sz);
			if (err != mpack_ok) {
                debugUi->Log("Mpack error %d while saving world manifest", err);
                debugUi->Log("Save file possibly corrupted, don't hit me plz");
			}
		}
		int gm;
		if (sscanf(&text[1],"gm %i",&gm)) {
			if (gm>0&&gm<5){
				player->gamemode=gm;
                debugUi->Log("Set gamemode to %i", gm);
			} else {
                debugUi->Log("Cannot set gamemode to %i",gm);
			}

		}
		int hunger;
		if (sscanf(&text[1],"hunger %i",&hunger)) {
			if (hunger>0&&hunger<5){
				player->hunger=hunger;
                debugUi->Log("Set hunger to %i", hunger);
			} else {
                debugUi->Log("Cannot set hunger to %i",hunger);
			}

		}
		int diff;
		if (sscanf(&text[1],"diff %i",&diff)) {
			if (diff>0&&diff<6) {
				player->difficulty=diff;
                debugUi->Log("Set difficulty to %i", diff);
			} else {
                debugUi->Log("Cannot set difficulty to %i",diff);
			}
		}
		/*int bx, by, bz; 					idfk how to get it to read strings as arguments, compiler isn't liking it
		char block;
		if (sscanf(&text[1], "sb %i %i %i", &bx, &by, &bz) == 3 &&) {
			World_SetBlock(player->world, bx,by,bz, block);
			DebugUI_Log("Block at %f, %f %f is now %c", bx, by, bz,block);
		}*/
		
	}
}