#pragma once
#include <string>
#include <iostream>
#include <3ds.h>
#include <memory>
#include "../../serverStorage/ServerNBTStorage.hpp" 

#include "../Screen.hpp"

class ServerList; 

class ServerConnect : public Screen {
public:
    ServerConnect(ServerList* serverList, const ServerNBTStorage& server);
    ~ServerConnect();
    void drawScreen() const override;
    void updateControls(u32 kDown) override;

private:
    ServerList* serverList;
    ServerNBTStorage server;
};