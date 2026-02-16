#include "gui/Gui.hpp"

#include <stdarg.h>

#include "ReCraftCore.hpp"
#include "amethyst/iron.hpp"
#include "gui/SpriteBatch.hpp"
#include "input/InputManager.hpp"
#include "misc/NumberUtils.hpp"
#include "rendering/VertexFmt.hpp"


Gui::Gui() {
    RenderData = new Iron::Drawlist();
    RenderData->SetFont(ReCraftCore::GetInstance()->GetAssetManager()->Get<Iron::Font>("font"));

    paddingX = 2;
    paddingY = 3;
    windowX = 0;
    windowY = 0;
    relativeX = 0;
    relativeY = 0;
}

Gui::~Gui() {
    if (RenderData) {
        delete RenderData;
        RenderData = nullptr;
    }
}

int Gui::relativeToAbsoluteSize(float s) { return (int)((float)currentRow.unpaddedWidth * s); }

void Gui::Frame() {
	relativeX = 0;
	relativeY = 0;
	windowX = 0;
	windowY = 0;
}

void Gui::Offset(int x, int y) {
	windowX = x;
	windowY = y;
}

int Gui::RelativeWidth(float x) { return /*SpriteBatch_GetWidth()*/1 * x; }
int Gui::RelativeHeight(float y) { return /*SpriteBatch_GetHeight()*/1 * y; }

void Gui::BeginRowCenter(int width, int count) {
	windowX = /*SpriteBatch_GetWidth()*/1 / 2 - width / 2;
    Gui::BeginRow(width, count);
}

void Gui::BeginRow(int width, int count) {
	currentRow = (Row){width, 0, width - paddingX * 2 - paddingX * count};
	relativeX = paddingX;
	relativeY = 0;
}
void Gui::EndRow() { windowY += currentRow.highestElement + paddingY; }



void Gui::Label(float size, bool shadow, int16_t color, bool center, const char* text, ...) {
	int wrap = size <= 0.f ? (currentRow.width - relativeX - paddingX) : relativeToAbsoluteSize(size);
	int yTextSize = 0;

	va_list vl;
	va_start(vl, text);
	int xOffset = 0;
	if (center) {
		int textWidth = 0;//SpriteBatch_CalcTextWidthVargs(text, vl);
		if (textWidth <= wrap) xOffset = wrap / 2 - textWidth / 2;
	}
	int xTextSize =
	    0; //SpriteBatch_PushTextVargs(windowX + relativeX + xOffset, windowY + relativeY, 0, INT16_MAX, false, wrap, &yTextSize, text, vl);
	va_end(vl);
	relativeX += (size <= 0.f ? xTextSize : wrap) + paddingX;
	currentRow.highestElement = MAX(currentRow.highestElement, yTextSize);
}

bool Gui::Button(float size, const char* label, ...) {
	va_list vl;
	va_start(vl, label);

	int textWidth = 0;//SpriteBatch_CalcTextWidthVargs(label, vl);
	int x = windowX + relativeX;
	int y = windowY + relativeY - BUTTON_TEXT_PADDING;
	int w = (size <= 0.f) ? textWidth + SliceSize : relativeToAbsoluteSize(size);

	bool pressed = Gui::IsCursorInside(x, y, w, BUTTON_HEIGHT);
	int middlePieceSize = w - SliceSize * 2;

	RenderData->DrawTex(ReCraftCore::GetInstance()->GetAssetManager()->Get<Amy::Texture>("GuiTexture_Widgets"));

	int baseTextureY = 66 + (pressed * BUTTON_HEIGHT);

	DrawTexturedModalRect(x, y, 0, baseTextureY, SliceSize, BUTTON_HEIGHT);
	DrawTexturedModalRect(x + SliceSize, y, SliceSize, baseTextureY, middlePieceSize, BUTTON_HEIGHT);
	DrawTexturedModalRect(x + SliceSize + middlePieceSize, y, 192, baseTextureY, SliceSize, BUTTON_HEIGHT);

    RenderData->DrawText(Amy::fvec2((x + (w / 2 - textWidth / 2)),
                                    (y + (BUTTON_HEIGHT - CHAR_HEIGHT) / 2))*2,label, Amy::Color(255, 255, 255, 255));

	va_end(vl);

	// Update layout
	relativeX += w + paddingX;
	currentRow.highestElement = MAX(currentRow.highestElement, BUTTON_HEIGHT);

	// Check if button was clicked
	return Input::isKeyUp(KEY_TOUCH) && Gui::WasCursorInside(x, y, w, BUTTON_HEIGHT);
}

void Gui::Space(float space) { relativeX += relativeToAbsoluteSize(space) + paddingX; }
void Gui::VerticalSpace(int y) { windowY += y; }

bool Gui::IsCursorInside(int x, int y, int w, int h) {
    int sclInputX = Input::getTouch().px / 1;//SpriteBatch_GetScale();
    int sclInputY = Input::getTouch().py / 1;//SpriteBatch_GetScale();
	return sclInputX != 0 && sclInputY != 0 && sclInputX >= x && sclInputX < x + w && sclInputY >= y && sclInputY < y + h;
}
bool Gui::WasCursorInside(int x, int y, int w, int h) {
    int sclOldInputX = Input::getTouchPrev().px / 1;//SpriteBatch_GetScale();
    int sclOldInputY = Input::getTouchPrev().py / 1;//SpriteBatch_GetScale();
	return sclOldInputX != 0 && sclOldInputY != 0 && sclOldInputX >= x && sclOldInputX < x + w && sclOldInputY >= y &&
	       sclOldInputY < y + h;
}
bool Gui::EnteredCursorInside(int x, int y, int w, int h) {
    int sclOldInputX = Input::getTouchPrev().px / 1;//SpriteBatch_GetScale();
    int sclOldInputY = Input::getTouchPrev().py / 1;//SpriteBatch_GetScale();

	return (sclOldInputX == 0 && sclOldInputY == 0) && Gui::IsCursorInside(x, y, w, h);
}
void Gui::GetCursorMovement(int* x, int* y) {
    if ((Input::getTouch().px == 0 && Input::getTouch().py == 0) ||
        (Input::getTouchPrev().px == 0 && Input::getTouchPrev().py == 0)) {
		*x = 0;
		*y = 0;
		return;
	}
    *x = Input::getTouch().px / 1/*SpriteBatch_GetScale()*/ - Input::getTouchPrev().px / 1/*SpriteBatch_GetScale()*/;
    *y = Input::getTouch().px / 1/*SpriteBatch_GetScale()*/ - Input::getTouchPrev().py / 1/*SpriteBatch_GetScale()*/;
}

void Gui::DrawTexturedModalRect(int x, int y, int textureX, int textureY, int width, int height) {
    RenderData->DrawFromAtlas(
        Amy::fvec2(x * 2, y * 2),
        Amy::fvec2(width * 2, height * 2),
        Amy::fvec4(textureX, textureY, textureX + width, textureY + height),
        Amy::Color(255, 255, 255, 255)
    );
}