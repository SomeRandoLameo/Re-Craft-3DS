#include "gui/Gui.hpp"

#include <cstdarg>

#include "ReCraftCore.hpp"
#include "amethyst/iron.hpp"
#include "gui/SpriteBatch.hpp"
#include "input/InputManager.hpp"
#include "misc/NumberUtils.hpp"
#include "rendering/VertexFmt.hpp"

/*
 * TODO: Do we actually want layout calculations or rather fixed consts xy for each element? (like in the original game)
 *  The newer versions have some automatic layout computation, but this being quite low-end,
 *  not yet ported and 1.12 not yet having this, i rather tend to change. Lets see how this goes
*/

//TODO: Add support for Setting GuiScale
//TODO: Item Thumbnails
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

int Gui::relativeToAbsoluteSize(float s) const { return (int)((float)currentRow.unpaddedWidth * s); }

void Gui::SetDimensions(int width, int height) {
    viewWidth = width;
    viewHeight = height;
}

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

int Gui::RelativeWidth(float x) const { return viewWidth * x; }
int Gui::RelativeHeight(float y) const { return viewHeight * y; }

void Gui::BeginRowCenter(int width, int count) {
	windowX = viewWidth / 2 - width / 2;
    Gui::BeginRow(width, count);
}

void Gui::BeginRow(int width, int count) {
	currentRow = (Row){width, 0, width - paddingX * 2 - paddingX * count};
	relativeX = paddingX;
	relativeY = 0;
}
void Gui::EndRow() { windowY += currentRow.highestElement + paddingY; }


//TODO: Fix if needed
void Gui::Label(float size, bool shadow, Amy::Color color, bool center, const char* text, ...) {
	int wrap = size <= 0.f ? (currentRow.width - relativeX - paddingX) : relativeToAbsoluteSize(size);
	int yTextSize = 0;

	va_list vl;
	va_start(vl, text);
	int xOffset = 0;
	if (center) {
		int textWidth = 0;//SpriteBatch_CalcTextWidthVargs(text, vl);
		if (textWidth <= wrap) xOffset = wrap / 2 - textWidth / 2;
	}
    DrawString(text, windowX + relativeX + xOffset, windowY + relativeY, Amy::Color(255, 255, 255, 255));
	int xTextSize =
	    0; //SpriteBatch_PushTextVargs(windowX + relativeX + xOffset, windowY + relativeY, 0, INT16_MAX, false, wrap, &yTextSize, text, vl);
	va_end(vl);
	relativeX += (size <= 0.f ? xTextSize : wrap) + paddingX;
	currentRow.highestElement = MAX(currentRow.highestElement, yTextSize);
}

//TODO: Componetize
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

    Amy::Color textColor = pressed ? Amy::Color(255,255,160,255) : Amy::Color(224,224,224,255);
    DrawCenteredString(label, x + (w*0.5),y + (BUTTON_HEIGHT*0.5), textColor);

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
    int sclInputX = Input::getTouch().px / 2;//m_guiScale;
    int sclInputY = Input::getTouch().py / 2;//m_guiScale;
	return sclInputX != 0 && sclInputY != 0 && sclInputX >= x && sclInputX < x + w && sclInputY >= y && sclInputY < y + h;
}
bool Gui::WasCursorInside(int x, int y, int w, int h) {
    int sclOldInputX = Input::getTouchPrev().px / 2;//m_guiScale;
    int sclOldInputY = Input::getTouchPrev().py / 2;//m_guiScale;
	return sclOldInputX != 0 && sclOldInputY != 0 && sclOldInputX >= x && sclOldInputX < x + w && sclOldInputY >= y &&
	       sclOldInputY < y + h;
}
bool Gui::EnteredCursorInside(int x, int y, int w, int h) {
    int sclOldInputX = Input::getTouchPrev().px / 2;//m_guiScale;
    int sclOldInputY = Input::getTouchPrev().py / 2;//m_guiScale;

	return (sclOldInputX == 0 && sclOldInputY == 0) && Gui::IsCursorInside(x, y, w, h);
}
void Gui::GetCursorMovement(int* x, int* y) {
    if ((Input::getTouch().px == 0 && Input::getTouch().py == 0) ||
        (Input::getTouchPrev().px == 0 && Input::getTouchPrev().py == 0)) {
		*x = 0;
		*y = 0;
		return;
	}
    *x = Input::getTouch().px / 2/*m_guiScale*/ - Input::getTouchPrev().px / 2/*m_guiScale*/;
    *y = Input::getTouch().px / 2/*m_guiScale*/ - Input::getTouchPrev().py / 2/*m_guiScale*/;
}

void Gui::DrawTexturedModalRect(int x, int y, int textureX, int textureY, int width, int height) const {
    RenderData->DrawFromAtlas(
        Amy::ivec2(x * 2, y * 2),
        Amy::ivec2(width * 2, height * 2),
        Amy::ivec4(textureX, textureY, textureX + width, textureY + height),
        Amy::Color(255, 255, 255, 255)
    );
}

// Ideally, this would be in a separate font renderer class, but for now, it's here.
void Gui::DrawString(const std::string& text, int x, int y, const Amy::Color& color ) const {
    RenderData->DrawText(
        Amy::ivec2(x * 2, y * 2),
        text,
        color
    );
}

//This function is really fancy, from minecraft rewritten :D
//This should also be in fontrenderer
//no clue if this actually uses the alpha but i think so
Amy::Color fancyFontShadow(const Amy::Color& color){
    int rgba = (color.r << 24) | (color.g << 16) | (color.b << 8) | color.a;
    int shiftedColor = (rgba & 16579836) >> 2 | rgba & -16777216;
    return Amy::Color(shiftedColor);
}

// TODO: It seems like there is some Text anti-aliasing here. This should not be like this. This happens only in the Emulator.
void Gui::DrawStringWithShadow(const std::string& text, int x, int y, const Amy::Color& color) const {
    DrawString(text,(x + 1), (y + 1), fancyFontShadow(color));
    DrawString(text,x,y, color);
}

void Gui::DrawCenteredString(const std::string& text, int x, int y, const Amy::Color& color, bool shadow) const {
    if(shadow) {
        RenderData->DrawTextEx(
            Amy::ivec2((x + 1) * 2, (y + 1) * 2),
            text,
            fancyFontShadow(color),
            AmyTextFlags_AlignMid
        );
    }
    RenderData->DrawTextEx(
        Amy::fvec2(x * 2, y * 2),
        text,
        color,
        AmyTextFlags_AlignMid
    );
}

void Gui::DrawDefaultBackground() const{
    RenderData->DrawTex(ReCraftCore::GetInstance()->GetAssetManager()->Get<Amy::Texture>("GuiTexture_DefaultBG"));

    for (int i = 0; i < 160 / 32 + 1; i++) {
        for (int j = 0; j < 120 / 32 + 1; j++) {
            int x = i * 32;
            int y = j * 32;
            int textureX = 32;
            int textureY = 32;
            int width = 32;
            int height = 32;

            RenderData->DrawFromAtlas(
                Amy::ivec2(x * 2, y * 2),
                Amy::ivec2(width * 2, height * 2),
                Amy::ivec4(textureX, textureY, textureX + width, textureY + height),
                Amy::Color(64, 64, 64, 255)
            );
        }
    }
}

/**
 * Draws a 1 pixel (2 real pixel) wide horizontal line. Args : x, y1, y2, color
 */
void Gui::DrawHorizontalLine(int startX, int endX, int y, const Amy::Color& color) const {
    if (endX < startX) {
        int i = startX;
        startX = endX;
        endX = i;
    }

    RenderData->DrawLine(
        Amy::ivec2(startX * 2, y * 2),
        Amy::ivec2((endX + 1) * 2, y * 2),
        2,
        color
    );
}

/**
 * Draws a 1 pixel (2 real pixel) wide vertical line. Args : x, y1, y2, color
 */
void Gui::DrawVerticalLine(int x, int startY, int endY, const Amy::Color& color) const {
    if (endY < startY) {
        int i = startY;
        startY = endY;
        endY = i;
    }

    RenderData->DrawLine(
        Amy::ivec2(x * 2, startY * 2),
        Amy::ivec2(x * 2, (endY + 1) * 2),
        2,
        color
    );
}

/**
 * Draws a rectangle with the specified coordinates and color.
 */
void Gui::DrawRect(int left, int top, int right, int bottom, const Amy::Color& color) const {
    RenderData->DrawRectFilled(Amy::fvec2 (left,top), Amy::fvec2(right, bottom), color);
}