#pragma once

#include "../mclib/common/Common.h"
#include "../mclib/core/Client.h"
#include "../mclib/util/Forge.h"
#include "../mclib/util/Hash.h"
#include "../mclib/util/Utility.h"
#include "../mclib/util/VersionFetcher.h"

class Core {
private:
    void startGame();
    static Core* coreInstance;
public:
    Core();
    void run();
};

