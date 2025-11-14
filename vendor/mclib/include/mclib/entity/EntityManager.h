#pragma once

#include "../mclib.h"
#include "Entity.h"
#include "Player.h"
#include "../protocol/packets/Packet.h"
#include "../protocol/packets/PacketHandler.h"
#include "../util/ObserverSubject.h"

#include <array>
#include <unordered_map>

namespace mc {
    namespace entity {

        class EntityListener {
        public:
            virtual ~EntityListener() { }

            virtual void OnPlayerSpawn(PlayerEntityPtr entity, UUID uuid) { }
            virtual void OnEntitySpawn(EntityPtr entity) { }
            virtual void OnObjectSpawn(EntityPtr entity) { }
            virtual void OnEntityDestroy(EntityPtr entity) { }
            virtual void OnEntityMove(EntityPtr entity, Vector3d oldPos, Vector3d newPos) { }
        };

        class EntityManager : public protocol::packets::PacketHandler, public util::ObserverSubject<EntityListener> {
        public:
            using EntityMap = std::unordered_map<EntityId, EntityPtr>;
            using iterator = EntityMap::iterator;
            using const_iterator = EntityMap::const_iterator;

        private:
            std::unordered_map<EntityId, EntityPtr> m_Entities;
            // Entity Id for the client player
            EntityId m_EntityId;
            protocol::Version m_ProtocolVersion;

        public:
             EntityManager(protocol::packets::PacketDispatcher* dispatcher, protocol::Version protocolVersion);
             ~EntityManager();

            EntityManager(const EntityManager& rhs) = delete;
            EntityManager& operator=(const EntityManager& rhs) = delete;
            EntityManager(EntityManager&& rhs) = delete;
            EntityManager& operator=(EntityManager&& rhs) = delete;

            std::shared_ptr<PlayerEntity> GetPlayerEntity() const {
                auto iter = m_Entities.find(m_EntityId);
                if (iter == m_Entities.end()) return nullptr;

                return std::dynamic_pointer_cast<PlayerEntity>(iter->second);
            }

            EntityPtr GetEntity(EntityId eid) const {
                auto iter = m_Entities.find(eid);
                if (iter == m_Entities.end()) return nullptr;
                return iter->second;
            }

            iterator begin() { return m_Entities.begin(); }
            iterator end() { return m_Entities.end(); }

            const_iterator begin() const { return m_Entities.begin(); }
            const_iterator end() const { return m_Entities.end(); }

            void  HandlePacket(protocol::packets::in::JoinGamePacket* packet);
            void  HandlePacket(protocol::packets::in::PlayerPositionAndLookPacket* packet);
            void  HandlePacket(protocol::packets::in::SpawnPlayerPacket* packet);
            void  HandlePacket(protocol::packets::in::SpawnPaintingPacket* packet);
            void  HandlePacket(protocol::packets::in::SpawnExperienceOrbPacket* packet);
            void  HandlePacket(protocol::packets::in::SpawnGlobalEntityPacket* packet);
            void  HandlePacket(protocol::packets::in::SpawnMobPacket* packet);
            void  HandlePacket(protocol::packets::in::SpawnObjectPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityRelativeMovePacket* packet);
            void  HandlePacket(protocol::packets::in::EntityLookAndRelativeMovePacket* packet);
            void  HandlePacket(protocol::packets::in::EntityTeleportPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityLookPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityHeadLookPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityVelocityPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityMetadataPacket* packet);
            void  HandlePacket(protocol::packets::in::EntityPropertiesPacket* packet);
            void  HandlePacket(protocol::packets::in::DestroyEntitiesPacket* packet);
            void  HandlePacket(protocol::packets::in::AttachEntityPacket* packet);
        };

    } // ns entity
} // ns mc


