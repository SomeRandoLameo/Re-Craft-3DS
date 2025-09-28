#pragma once

#include "../common/AABB.h"
#include "../common/Json.h"
#include "../common/Vector.h"
#include "../core/AuthToken.h"
#include "../core/Client.h"
#include "../core/Connection.h"
#include "../core/PlayerManager.h"
#include "../world/World.h"

#include <fstream>
#include <string>
#include <queue>
#include <utility>

namespace mc {

    inline std::string to_string(const std::string& str) {
        return str;
    }

    inline std::string to_string(const std::wstring& str) {
        return std::string(str.begin(), str.end());
    }

    namespace util {

        bool GetProfileToken(const std::string& username, core::AuthToken* token);
        std::string ParseChatNode(const json& node);
        std::string StripChatMessage(const std::string& message);
        s64 GetTime();

        class PlayerController : public core::PlayerListener {
        private:
            core::PlayerManager& m_PlayerManager;
            core::Connection* m_Connection;
            world::World& m_World;
            Vector3d m_Position;
            float m_Yaw;
            float m_Pitch;
            AABB m_BoundingBox;
            EntityId m_EntityId;
            u64 m_LastUpdate;
            Vector3d m_TargetPos;
            bool m_Sprinting;
            bool m_LoadedIn;
            bool m_HandleFall;

            double m_MoveSpeed;

            std::queue<Vector3d> m_DigQueue;

            // todo: gravity
            const float FallSpeed = 8.3f * (50.0f / 1000.0f);

            std::vector<std::pair<block::BlockPtr, mc::Vector3i>> GetNearbyBlocks(const s32 radius);

        public:
            PlayerController(core::Connection* connection, world::World& world, core::PlayerManager& playerManager);
            ~PlayerController();

            void OnClientSpawn(core::PlayerPtr player);
            bool ClearPath(Vector3d target);

            void Dig(Vector3d target);
            void Attack(EntityId id);
            void Move(Vector3d delta);

            bool HandleJump();
            bool HandleFall();
            void UpdateDigging();
            void UpdatePosition();
            void Update();

            bool InLoadedChunk() const;
            Vector3d GetPosition() const;
            Vector3d GetHeading() const;
            float GetYaw() const;
            float GetPitch() const;
            AABB GetBoundingBox() const;

            void SetYaw(float yaw);
            void SetPitch(float pitch);
            void LookAt(Vector3d target);
            void SetMoveSpeed(double speed);
            void SetTargetPosition(Vector3d target);
            void SetHandleFall(bool handle);
        };

        class PlayerFollower : public core::PlayerListener, public core::ClientListener {
        private:
            core::Client* m_Client;
            core::PlayerManager& m_PlayerManager;
            entity::EntityManager& m_EntityManager;
            core::PlayerPtr m_Following;
            PlayerController& m_PlayerController;
            std::wstring m_Target;
            u64 m_LastUpdate;

        public:
            PlayerFollower(protocol::packets::PacketDispatcher* dispatcher, core::Client* client);

            ~PlayerFollower();

            void UpdateRotation();

            void OnTick() override;
            bool IsIgnored(const std::wstring& name);

            void FindClosestPlayer();

            void OnPlayerJoin(core::PlayerPtr player) override;
            void OnPlayerLeave(core::PlayerPtr player) override;
            void OnPlayerSpawn(core::PlayerPtr player) override;
            void OnPlayerDestroy(core::PlayerPtr player, EntityId eid) override;
            void OnPlayerMove(core::PlayerPtr player, Vector3d oldPos, Vector3d newPos) override;
        };

        class IConsole {
        public:
            virtual void Output(const std::string& str) = 0;
            virtual void Output(const std::wstring& str) = 0;

            virtual IConsole& operator<<(const std::string& str) = 0;
            virtual IConsole& operator<<(const std::wstring& str) = 0;

            template <typename T>
            IConsole& operator<<(T data) {
                using std::to_string;
                using mc::to_string;

                Output(to_string(data));
                return *this;
            }

            IConsole& operator<<(const char* data) {
                Output(std::string(data));
                return *this;
            }

            IConsole& operator<<(const wchar_t* data) {
                Output(std::wstring(data));
                return *this;
            }
        };


        class Console {
        private:
            IConsole* m_Impl;

        public:
            Console() : m_Impl(nullptr) { }

            // Doesn't take control of impl
            void SetImpl(IConsole* impl) {
                m_Impl = impl;
            }

            IConsole* GetImpl() const {
                return m_Impl;
            }

            void Output(const std::string& str) {
                if (m_Impl)
                    m_Impl->Output(str);
            }

            void Output(const std::wstring& str) {
                if (m_Impl)
                    m_Impl->Output(str);
            }

            template <typename T>
            Console& operator<<(const T& data) {
                using std::to_string;
                using mc::to_string;
                Output(to_string(data));
                return *this;
            }

            template <typename T>
            Console& operator<<(const std::string& str) {
                Output(str);
                return *this;
            }

            template <typename T>
            Console& operator<<(const std::wstring& str) {
                Output(str);
                return *this;
            }

            Console& operator<<(const char* str) {
                Output(std::string(str));
                return *this;
            }

            Console& operator<<(const wchar_t* str) {
                Output(std::wstring(str));
                return *this;
            }
        };

        class LoggerConsole : public IConsole {
        private:
            std::ofstream m_Out;
            std::string m_Filename;

        public:
            LoggerConsole(const std::string& filename) {
                m_Out.open(filename.c_str(), std::ios::out);
            }

            ~LoggerConsole() {
                m_Out.close();
            }

            void Output(const std::string& str) override {
                m_Out << str << std::endl;
            }

            void Output(const std::wstring& str) override {
                m_Out << std::string(str.begin(), str.end()) << std::endl;
            }

            IConsole& operator<<(const std::string& str) override {
                Output(str);
                return *this;
            }
            IConsole& operator<<(const std::wstring& str) override {
                Output(str);
                return *this;
            }
        };

    } // ns util
} // ns mc
