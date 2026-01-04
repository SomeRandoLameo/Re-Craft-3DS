#include "gui/SpriteBatch.h"
#include "rendering/VertexFmt.h"

#include "gui/DebugUI.h"
#undef CHAR_HEIGHT
#include "ReCraftCore.h"

#include <stdarg.h>
#include <stdlib.h>

#define STATUS_LINES (240 / 8)
#define LOG_LINES 40
#define LOG_LINE_LENGTH 128
#define STATUS_LINE_LENGTH 128

static char* statusLines[STATUS_LINES];
static char* logLines[LOG_LINES];
static int currentStatusLine = 0;

DebugUI::DebugUI() {
    for (auto& logLine : logLines) {
        logLine = (char*)Amy::Malloc(LOG_LINE_LENGTH);
        memset(logLine, 0x0, LOG_LINE_LENGTH);
    }
    for (auto& statusLine : statusLines) {
        statusLine = (char*)Amy::Malloc(STATUS_LINE_LENGTH);
        memset(statusLine, 0x0, STATUS_LINE_LENGTH);
    }
    RenderData = Iron::Drawlist::New();
    RenderData->SetFont(ReCraftCore::GetInstance()->GetAssetManager()->Get<Iron::Font>("font"));
}

DebugUI::~DebugUI() {
    for (int i = 0; i < LOG_LINES; i++) {
        Amy::Free(logLines[i]);
    }
    for (int i = 0; i < STATUS_LINES; i++) {
        Amy::Free(statusLines[i]);
    }

    RenderData.reset();
}

void DebugUI::Text(const char* text, ...) {
    if (currentStatusLine >= STATUS_LINES) {
        return;
    }

    va_list args;
    va_start(args, text);

    vsprintf(statusLines[currentStatusLine++], text, args);

    va_end(args);
}

void DebugUI::Log(const char* text, ...) {
    char* lastLine = logLines[LOG_LINES - 1];
    for (int i = LOG_LINES - 1; i > 0; i--) {
        logLines[i] = logLines[i - 1];
    }

    logLines[0] = lastLine;

    va_list args;
    va_start(args, text);

    vsprintf(logLines[0], text, args);

    va_end(args);
}

void DebugUI::Draw() {
    RenderData->SetFontScale(0.5);
    int yOffset = (240 / 3) * 2;
    for (auto& logLine : logLines) {
        int step = 12;
        RenderData->DrawText(Amy::fvec2(0, yOffset), logLine, 0xffffffff);
        yOffset += step;
        if (yOffset >= 240)
            break;
    }
    yOffset = 0;
    for (auto& statusLine : statusLines) {
        int step = 12;
        RenderData->DrawText(Amy::fvec2(0, yOffset), statusLine, 0xffffffff);
        yOffset += step;

        memset(statusLine, 0x0, STATUS_LINE_LENGTH);
    }
    currentStatusLine = 0;
}
