#include "gui/SpriteBatch.h"
#include "rendering/VertexFmt.h"

#include "ReCraftCore.h"
#include "gui/DebugUI.h"


#include <stdarg.h>
#include <stdlib.h>


static char* statusLines[DebugUI::StatusLines];
static char* logLines[DebugUI::LogLines];
static int currentStatusLine = 0;

DebugUI::DebugUI() {
    for (auto& logLine : logLines) {
        logLine = (char*)malloc(LineLength);
        memset(logLine, 0x0, LineLength);
    }
    for (auto& statusLine : statusLines) {
        statusLine = (char*)malloc(LineLength);
        memset(statusLine, 0x0, LineLength);
    }
    RenderData = Iron::Drawlist::New();
    RenderData->SetFont(ReCraftCore::GetInstance()->GetAssetManager()->Get<Iron::Font>("font"));
}

DebugUI::~DebugUI() {
    for (int i = 0; i < LogLines; i++) {
        free(logLines[i]);
    }
    for (int i = 0; i < StatusLines; i++) {
        free(statusLines[i]);
    }

    RenderData.reset();
}

void DebugUI::Text(const char* text, ...) {
    if (currentStatusLine >= StatusLines) {
        return;
    }

    va_list args;
    va_start(args, text);

    vsprintf(statusLines[currentStatusLine++], text, args);

    va_end(args);
}

void DebugUI::Log(const char* text, ...) {
    char* lastLine = logLines[LogLines - 1];
    for (int i = LogLines - 1; i > 0; i--) {
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

        memset(statusLine, 0x0, LineLength);
    }
    currentStatusLine = 0;
}
