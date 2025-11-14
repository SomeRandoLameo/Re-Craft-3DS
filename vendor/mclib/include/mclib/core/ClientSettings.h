#pragma once

#include "../mclib.h"
#include "../common/Types.h"
#include <string>

namespace mc {
    namespace core {

        class ClientSettings {
        private:
            std::wstring m_Locale;
            ChatMode m_ChatMode;
            MainHand m_MainHand;
            u8 m_ViewDistance;
            u8 m_SkinParts;
            bool m_ChatColors;

        public:
            ClientSettings();

            ClientSettings(const ClientSettings& rhs) = default;
            ClientSettings& operator=(const ClientSettings& rhs) = default;
            ClientSettings(ClientSettings&& rhs) = default;
            ClientSettings& operator=(ClientSettings&& rhs) = default;

            ClientSettings& SetLocale(const std::wstring& locale);
            ClientSettings& SetChatMode(ChatMode mode) noexcept;
            ClientSettings& SetMainHand(MainHand main) noexcept;
            ClientSettings& SetViewDistance(u8 distance) noexcept;
            ClientSettings& SetViewDistance(s32 distance) noexcept;
            ClientSettings& SetSkinParts(u8 parts) noexcept;
            ClientSettings& SetChatColors(bool chatColors) noexcept;

            ClientSettings& SetSkinPart(SkinPart part, bool display) noexcept;

            inline const std::wstring& GetLocale() const noexcept { return m_Locale; }
            inline ChatMode GetChatMode() const noexcept { return m_ChatMode; }
            inline MainHand GetMainHand() const noexcept { return m_MainHand; }
            inline u8 GetViewDistance() const noexcept { return m_ViewDistance; }
            inline u8 GetSkinParts() const noexcept { return m_SkinParts; }
            inline bool GetChatColors() const noexcept { return m_ChatColors; }
        };

    } // ns core
} // ns mc