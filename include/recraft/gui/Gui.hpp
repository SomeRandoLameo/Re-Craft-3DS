#pragma once

#include <stdint.h>
#include <amethyst/iron.hpp>

//TODO: COreUI
class Gui {
public:
    enum { SliceSize = 8 };

    Gui();
    virtual ~Gui();

    void Frame();

    int RelativeWidth(float x);
    int RelativeHeight(float y);

    void Offset(int x, int y);
    void BeginRowCenter(int width, int count);
    void VerticalSpace(int y);

    void Space(float space);

    void BeginRow(int width, int count);
    void EndRow();

    void Label(float size, bool shadow, int16_t color, bool center, const char* text, ...);

    bool Button(float size, const char* label, ...);

    bool IsCursorInside(int x, int y, int w, int h);
    bool WasCursorInside(int x, int y, int w, int h);
    static void GetCursorMovement(int* x, int* y);
    bool EnteredCursorInside(int x, int y, int w, int h);

    void DrawTexturedModalRect(int x, int y, int textureX, int textureY, int width, int height);

    Amy::Iron::Drawlist* RenderData = nullptr;

    static const int BUTTON_HEIGHT = 20;
    static const int CHAR_HEIGHT = 8;
    static const int BUTTON_TEXT_PADDING = ((BUTTON_HEIGHT - CHAR_HEIGHT) / 2);
private:
    int relativeToAbsoluteSize(float s);
    struct Row {
        int width;
        int highestElement;
        int unpaddedWidth;
    };

    Row currentRow;
    int relativeX, relativeY;
    int windowX, windowY;
    int paddingX, paddingY;

};
