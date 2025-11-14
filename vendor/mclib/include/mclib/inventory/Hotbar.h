#pragma once

#include "../core/Connection.h"
#include "Slot.h"
#include "../protocol/packets/PacketHandler.h"

namespace mc {
    namespace inventory {

        class InventoryManager;

        class Hotbar : public protocol::packets::PacketHandler {
        private:
            core::Connection* m_Connection;
            InventoryManager* m_InventoryManager;
            s32 m_SelectedSlot;

        public:
            Hotbar(protocol::packets::PacketDispatcher* dispatcher, core::Connection* connection, InventoryManager* inventoryManager);
            ~Hotbar();

            Hotbar(const Hotbar& rhs) = delete;
            Hotbar& operator=(const Hotbar& rhs) = delete;
            Hotbar(Hotbar&& rhs) = delete;
            Hotbar& operator=(Hotbar&& rhs) = delete;

            void HandlePacket(protocol::packets::in::HeldItemChangePacket* packet);

            // Sends a packet telling the server that the client is changing slots.
            // Slot should be between 0 and 8.
            void SelectSlot(s32 hotbarIndex);
            s32 GetSelectedSlot() const noexcept { return m_SelectedSlot; }

            Slot GetCurrentItem();
            // Index should be between 0 and 8.
            Slot GetItem(s32 hotbarIndex);
        };

    } // ns inventory
} // ns mc


