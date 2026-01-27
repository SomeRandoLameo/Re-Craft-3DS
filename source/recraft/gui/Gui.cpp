#include "gui/Gui.hpp"

#include <stdarg.h>

#include "ReCraftCore.hpp"
#include "amethyst/iron.hpp"
#include "gui/SpriteBatch.hpp"
#include "input/InputManager.hpp"
#include "misc/NumberUtils.hpp"
#include "rendering/VertexFmt.hpp"

struct Row{
	int width;
	int highestElement;
	int unpaddedWidth;
};

Amy::Iron::Drawlist* Gui::RenderData = nullptr;

static Row currentRow;
static int relativeX, relativeY;
static int windowX, windowY;
static int paddingX, paddingY;

void Gui::Init() {
	paddingX = 2;
	paddingY = 3;

    //RenderData = new Iron::Drawlist();
    //TODO: MC Bitmap pixel font loaded from assets :D
    //RenderData->SetFont(ReCraftCore::GetInstance()->GetAssetManager()->Get<Iron::Font>("font"));
}

static inline int relativeToAbsoluteSize(float s) { return (int)((float)currentRow.unpaddedWidth * s); }

void Gui::Deinit() {}

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

int Gui::RelativeWidth(float x) { return SpriteBatch_GetWidth() * x; }
int Gui::RelativeHeight(float y) { return SpriteBatch_GetHeight() * y; }

void Gui::BeginRowCenter(int width, int count) {
	windowX = SpriteBatch_GetWidth() / 2 - width / 2;
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
		int textWidth = SpriteBatch_CalcTextWidthVargs(text, vl);
		if (textWidth <= wrap) xOffset = wrap / 2 - textWidth / 2;
	}
	int xTextSize =
	    SpriteBatch_PushTextVargs(windowX + relativeX + xOffset, windowY + relativeY, 0, INT16_MAX, false, wrap, &yTextSize, text, vl);
	va_end(vl);
	relativeX += (size <= 0.f ? xTextSize : wrap) + paddingX;
	currentRow.highestElement = MAX(currentRow.highestElement, yTextSize);
}

bool Gui::Button(float size, const char* label, ...) {
	va_list vl;
	va_start(vl, label);

	int textWidth = SpriteBatch_CalcTextWidthVargs(label, vl);

	int x = windowX + relativeX;
	int y = windowY + relativeY - BUTTON_TEXT_PADDING;
	int w = (size <= 0.f) ? textWidth + SliceSize : relativeToAbsoluteSize(size);

	bool pressed = Gui::IsCursorInside(x, y, w, BUTTON_HEIGHT);

	int middlePieceSize = w - SliceSize * 2;

	SpriteBatch_BindGuiTexture(GuiTexture::Widgets);
    SpriteBatch_PushQuad(x, y, -3, SliceSize, 20, 0, 46 + (pressed * BUTTON_HEIGHT * 2), SliceSize, 20);
    SpriteBatch_PushQuad(x + SliceSize, y, -3, middlePieceSize, 20, SliceSize, 46 + (pressed * BUTTON_HEIGHT * 2), middlePieceSize, 20);
    SpriteBatch_PushQuad(x + SliceSize + middlePieceSize, y, -3, SliceSize, 20, 192, 46 + (pressed * BUTTON_HEIGHT * 2), SliceSize, 20);
    //RenderData->DrawTex(ReCraftCore::GetInstance()->GetAssetManager()->Get<Amy::Texture>("GuiTexture_Widgets"));
   // RenderData->DrawRectFilled(Amy::fvec2(x, y), Amy::fvec2(SLICE_SIZE, 20), Amy::Color(255, 255, 255, 255));
   // RenderData->DrawRectFilled(Amy::fvec2(x + SLICE_SIZE, y), Amy::fvec2(middlePieceSize, 20), Amy::Color(255, 255, 255, 255));
   // RenderData->DrawRectFilled(Amy::fvec2(x + SLICE_SIZE + middlePieceSize, y), Amy::fvec2(SLICE_SIZE, 20), Amy::Color(255, 255, 255, 255));



	SpriteBatch_PushTextVargs(x + (w / 2 - textWidth / 2), y + (BUTTON_HEIGHT - CHAR_HEIGHT) / 2, -1, SHADER_RGB(31, 31, 31), true,
				  INT_MAX, NULL, label, vl);
	va_end(vl);

	relativeX += w + paddingX;
	currentRow.highestElement = MAX(currentRow.highestElement, BUTTON_HEIGHT);

    if (Input::isKeyUp(KEY_TOUCH) && Gui::WasCursorInside(x, y, w, BUTTON_HEIGHT)) return true;

	return false;
}

void Gui::Space(float space) { relativeX += relativeToAbsoluteSize(space) + paddingX; }
void Gui::VerticalSpace(int y) { windowY += y; }

bool Gui::IsCursorInside(int x, int y, int w, int h) {
    int sclInputX = Input::getTouch().px / SpriteBatch_GetScale();
    int sclInputY = Input::getTouch().py / SpriteBatch_GetScale();
	return sclInputX != 0 && sclInputY != 0 && sclInputX >= x && sclInputX < x + w && sclInputY >= y && sclInputY < y + h;
}
bool Gui::WasCursorInside(int x, int y, int w, int h) {
    int sclOldInputX = Input::getTouchPrev().px / SpriteBatch_GetScale();
    int sclOldInputY = Input::getTouchPrev().py / SpriteBatch_GetScale();
	return sclOldInputX != 0 && sclOldInputY != 0 && sclOldInputX >= x && sclOldInputX < x + w && sclOldInputY >= y &&
	       sclOldInputY < y + h;
}
bool Gui::EnteredCursorInside(int x, int y, int w, int h) {
    int sclOldInputX = Input::getTouchPrev().px / SpriteBatch_GetScale();
    int sclOldInputY = Input::getTouchPrev().py / SpriteBatch_GetScale();

	return (sclOldInputX == 0 && sclOldInputY == 0) && Gui::IsCursorInside(x, y, w, h);
}
void Gui::GetCursorMovement(int* x, int* y) {
    if ((Input::getTouch().px == 0 && Input::getTouch().py == 0) ||
        (Input::getTouchPrev().px == 0 && Input::getTouchPrev().py == 0)) {
		*x = 0;
		*y = 0;
		return;
	}
    *x = Input::getTouch().px / SpriteBatch_GetScale() - Input::getTouchPrev().px / SpriteBatch_GetScale();
    *y = Input::getTouch().px / SpriteBatch_GetScale() - Input::getTouchPrev().py / SpriteBatch_GetScale();
}
