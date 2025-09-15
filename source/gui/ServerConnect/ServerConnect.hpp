#pragma once
#include <string>
#include <iostream>
#include <3ds.h>
#include "../../serverStorage/ServerNBTStorage.hpp" 

#include "../Screen.hpp"

class ServerList; 

class ServerConnect : public Screen {
public:
    ServerConnect(ServerList* serverList, const ServerNBTStorage& server);

    void drawScreen() const override;
    void updateControls(u32 kDown) override;

private:
    ServerList* serverList;
    ServerNBTStorage server;
};