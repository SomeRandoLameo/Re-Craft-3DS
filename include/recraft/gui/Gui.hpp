#pragma once

#include <amethyst/iron.hpp>
#include <stdint.h>
#include "amethyst/color.hpp"

// TODO: COreUI
class Gui {
public:
    enum { SliceSize = 8 };

    Gui();
    virtual ~Gui();

    void SetDimensions(int width, int height);

    void Frame();

    int RelativeWidth(float x) const;
    int RelativeHeight(float y) const;

    void Offset(int x, int y);
    void BeginRowCenter(int width, int count);
    void VerticalSpace(int y);

    void Space(float space);

    void BeginRow(int width, int count);
    void EndRow();

    void Label(float size, bool shadow, Amy::Color color, bool center, const char* text, ...);

    bool Button(float size, const char* label, ...);

    static bool IsCursorInside(int x, int y, int w, int h);
    static bool WasCursorInside(int x, int y, int w, int h);
    static void GetCursorMovement(int* x, int* y);
    static bool EnteredCursorInside(int x, int y, int w, int h);

    void DrawTexturedModalRect(int x, int y, int textureX, int textureY, int width, int height) const;

    void DrawDefaultBackground() const;

    void DrawString(const std::string& text, int x, int y, const Amy::Color& color) const;
    void DrawStringWithShadow(const std::string& text, int x, int y, const Amy::Color& color) const;
    void DrawCenteredString(const std::string& text, int x, int y, const Amy::Color& color, bool shadow = true) const;

    void DrawHorizontalLine(int startX, int endX, int y, const Amy::Color& color) const;
    void DrawVerticalLine(int x, int startY, int endY, const Amy::Color& color) const;
    void DrawRect(int left, int top, int right, int bottom, const Amy::Color& color) const;

    Amy::Iron::Drawlist* RenderData = nullptr;

    static const int BUTTON_HEIGHT = 20;
    static const int CHAR_HEIGHT = 8;
    static const int BUTTON_TEXT_PADDING = ((BUTTON_HEIGHT - CHAR_HEIGHT) / 2);

    int viewWidth = 0, viewHeight = 0;
private:
    int relativeToAbsoluteSize(float s) const;
    struct Row {
        int width;
        int highestElement;
        int unpaddedWidth;
    };

    Row currentRow{};
    int relativeX, relativeY;
    int windowX, windowY;
    int paddingX, paddingY;

};
