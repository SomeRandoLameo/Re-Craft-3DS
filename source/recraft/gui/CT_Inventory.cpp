#include "CT_Inventory.h"

#include "Gui.h"
#include "SpriteBatch.h"
#include "../rendering/VertexFmt.h"

static ItemStack *sourceStack = NULL, *proposedSourceStack = NULL;

static void clickAtStack(ItemStack* stack) {
	if (sourceStack == NULL && stack != proposedSourceStack) {
		proposedSourceStack = stack;
	} else if (proposedSourceStack == stack) {
		sourceStack = stack;
		proposedSourceStack = NULL;
	} else if (sourceStack != NULL) {
		if (sourceStack != stack) ItemStack_Transfer(sourceStack, stack);
		sourceStack = NULL;
	}
}

void Inventory_renderHotbar(int x, int y, ItemStack* stacks, int count, int* selected, GameState* gameState) {
	SpriteBatch_BindGuiTexture(GuiTexture_Widgets);
	
	for (int i = 0; i < count; i++) {
		SpriteBatch_SetScale(1);
		int rx = (i * 20 + x + 3) * 2;
		int ry = (y + 3) * 2;
		if (stacks[i].amount > 0) SpriteBatch_PushIcon(stacks[i].block, stacks[i].meta, rx, ry, 11);
		if (Gui_EnteredCursorInside(rx - 4, ry - 4, 18 * 2, 18 * 2)) {
			*selected = i;
			clickAtStack(&stacks[i]);

            if (*gameState == GameState_Playing_OnLine)
            {
               //TODO: client->GetHotbar().SelectSlot(i);
            }
		}
		SpriteBatch_SetScale(2);
		if (sourceStack == &stacks[i]) {
			SpriteBatch_PushSingleColorQuad(rx / 2 - 2, ry / 2 - 2, 9, 18, 18, SHADER_RGB(20, 5, 2));
			SpriteBatch_BindGuiTexture(GuiTexture_Widgets);
		}
		if (i < count - 2) {
			SpriteBatch_PushQuad(i * 20 + 21 + x, y, 10, 20, 22, 21, 0, 20, 22);
		}
	}
	SpriteBatch_SetScale(2);

	SpriteBatch_PushQuad(x, y, 10, 21, 22, 0, 0, 21, 22);
	SpriteBatch_PushQuad(21 + 20 * (count - 2) + x, y, 10, 21, 22, 161, 0, 21, 22);

	SpriteBatch_PushQuad(x + *selected * 20 - 1, y - 1, 14, 24, 24, 0, 22, 24, 24);
}

int Inventory_Draw(int x, int y, int w, ItemStack* stacks, int count, int _site) {
	SpriteBatch_SetScale(1);

	int headX = x;
	int headY = y;
	int site = _site;
	bool even = false;
	bool newLine = false;

	const int16_t colors[2] = {SHADER_RGB_DARKEN(SHADER_RGB(20, 20, 21), 9), SHADER_RGB_DARKEN(SHADER_RGB(20, 20, 21), 8)};
	if (count > INVENTORY_MAX_PER_SITE)
	{
		Gui_Offset(0, 60);
		if (Gui_Button(0.f, " << ") && site > 1)
		{
			site--;
		}
		Gui_Offset(270, 60);
		if (Gui_Button(0.f, " >> ") && site*INVENTORY_MAX_PER_SITE<count)
		{
			site++;
			
		}
	}
	int startindex = (site-1) * INVENTORY_MAX_PER_SITE;
	for (int i = startindex; i < fmin(site*INVENTORY_MAX_PER_SITE,count); i++)
	{
		if (stacks[i].block && stacks[i].amount > 0)  //only draw valid inventory
		{
			newLine = false;
			if ((headX + 16) >= w)
			{
				headX = x;
				headY += 17;
				newLine = true;
			}
			if (stacks[i].amount > 0)
				SpriteBatch_PushIcon(stacks[i].block, stacks[i].meta, headX * 2, headY * 2, 10);
			if (Gui_EnteredCursorInside(headX * 2, headY * 2, 16 * 2, 16 * 2))
				clickAtStack(&stacks[i]);
			SpriteBatch_PushSingleColorQuad(headX * 2, headY * 2, 9, 16 * 2, 16 * 2,
				sourceStack == &stacks[i] ? SHADER_RGB(20, 5, 2) : colors[even]);
			even ^= true;
			headX += 16;
			if (newLine)
			{
				even = false;
				//draw separator between "inventory lines"
				SpriteBatch_PushSingleColorQuad(x * 2, (headY - 1) * 2, 10, (w - 32) * 2, 2, SHADER_RGB(7, 7, 7));
			}
		}
	}

	SpriteBatch_SetScale(2);
	return site;
}