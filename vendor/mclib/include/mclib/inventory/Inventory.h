#pragma once

#include "../core/Connection.h"
#include "Slot.h"
#include "../protocol/packets/PacketHandler.h"

#include <memory>

namespace mc {
    namespace inventory {

        class Inventory {
        public:
            static const  s32 HOTBAR_SLOT_START;
            static const  s32 PLAYER_INVENTORY_ID;

            using ItemMap = std::map<s32, Slot>;

        private:
            ItemMap m_Items;
            int m_WindowId;
            s16 m_CurrentAction;
            Slot m_Cursor;

             void HandleTransaction(core::Connection& conn, u16 action, bool accepted);

        public:
             Inventory(int windowId);

             Slot GetItem(s32 index) const;
             const ItemMap& GetItems() const noexcept { return m_Items; }

            const Slot& GetCursorItem() const { return m_Cursor; }

            // Returns item slot index. Returns -1 if none are found.
             s32 FindItemById(s32 itemId) const;

             bool Contains(s32 itemId) const;
             bool Contains(Slot item) const;
             bool ContainsAtLeast(s32 itemId, s32 amount) const;
             bool ContainsAtLeast(Slot item, s32 amount) const;

            // Moves an item to the cursor. It will fail if something is already on cursor or if target slot is empty.
             bool PickUp(core::Connection& conn, s32 index);
            // Place the current cursor item into a slot.
            // The server may set the new cursor to the item in the target slot.
             bool Place(core::Connection& conn, s32 index);

            friend class InventoryManager;
        };

        class InventoryManager : public protocol::packets::PacketHandler {
        private:
            core::Connection* m_Connection;
            std::map<s32, std::unique_ptr<Inventory>> m_Inventories;

            void SetSlot(s32 windowId, s32 slotIndex, const Slot& slot);

        public:
             InventoryManager(protocol::packets::PacketDispatcher* dispatcher, core::Connection* connection);
             ~InventoryManager();

             void HandlePacket(protocol::packets::in::SetSlotPacket* packet);
             void HandlePacket(protocol::packets::in::WindowItemsPacket* packet);
             void HandlePacket(protocol::packets::in::OpenWindowPacket* packet);
             void HandlePacket(protocol::packets::in::ConfirmTransactionPacket* packet);

             Inventory* GetInventory(s32 windowId);
             Inventory* GetPlayerInventory();
        };

    } // ns inventory
} // ns mc