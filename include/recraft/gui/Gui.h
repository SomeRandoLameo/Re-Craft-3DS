#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "../misc/InputData.h"
#include "amethyst/iron.hpp"

//TODO: COreUI
class Gui {
public:
    static void Init();
    static void Deinit();

    static void InputData(InputData data);

    static void Frame();

    static int RelativeWidth(float x);
    static int RelativeHeight(float y);

    static void Offset(int x, int y);
    static void BeginRowCenter(int width, int count);
    static void VerticalSpace(int y);

    static void Space(float space);

    static void BeginRow(int width, int count);
    static void EndRow();

    static void Label(float size, bool shadow, int16_t color, bool center, const char* text, ...);

    static bool Button(float size, const char* label, ...);

    static bool IsCursorInside(int x, int y, int w, int h);
    static bool WasCursorInside(int x, int y, int w, int h);
    static void GetCursorMovement(int* x, int* y);
    static bool EnteredCursorInside(int x, int y, int w, int h);

    static Amy::Iron::Drawlist* RenderData;

    static const int BUTTON_HEIGHT = 20;
    static const int CHAR_HEIGHT = 8;
    static const int BUTTON_TEXT_PADDING = ((BUTTON_HEIGHT - CHAR_HEIGHT) / 2);
};
