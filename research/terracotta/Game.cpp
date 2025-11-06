#include "Game.h"

#include <GLFW/glfw3.h>
#include <mclib/util/Utility.h>
#include <mclib/util/Utility.h>
#include <mclib/world/World.h>
#include <mclib/common/AABB.h>
#include <mclib/inventory/Inventory.h>
#include <mclib/protocol/packets/Packet.h>
#include "math/Plane.h"
#include "math/TypeUtil.h"
#include <iostream>
#include <limits>

namespace terra {

#ifndef M_TAU
#define M_TAU 3.14159 * 2
#endif

float clamp(float value, float min, float max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

Player::Player(terra::World* world) : m_CollisionDetector(world), m_OnGround(false), m_Sneaking(false), m_Transform({}) {
    m_Transform.bounding_box = mc::AABB(mc::Vector3d(-0.3, 0, -0.3), mc::Vector3d(0.3, 1.8, 0.3));
    m_Transform.max_speed = 14.3f;
}

bool Player::OnGround() {
    return m_OnGround;
}

void Player::Update(float dt) {
    const float kMaxAcceleration = 100.0f;
    const double kEpsilon = 0.0001;

    using vec3 = mc::Vector3d;

    vec3 horizontal_acceleration = m_Transform.acceleration;
    horizontal_acceleration.y = 0;

    horizontal_acceleration.Truncate(kMaxAcceleration);

    vec3 acceleration(horizontal_acceleration.x, -38 + m_Transform.acceleration.y, horizontal_acceleration.z);

    m_OnGround = false;

    m_CollisionDetector.ResolveCollisions(&m_Transform, dt, &m_OnGround);

    m_Transform.velocity += acceleration * dt;
    m_Transform.input_velocity += m_Transform.input_acceleration * dt;
    m_Transform.orientation += m_Transform.rotation * dt;

    if (m_Transform.velocity.LengthSq() < kEpsilon) m_Transform.velocity = vec3(0, 0, 0);
    if (m_Transform.input_velocity.LengthSq() < kEpsilon) m_Transform.input_velocity = vec3(0, 0, 0);

    float drag = 0.98 - (int)m_OnGround * 0.13;
    m_Transform.velocity *= drag;
    m_Transform.input_velocity *= drag;

    m_Transform.orientation = clamp(m_Transform.orientation, -M_TAU, M_TAU);

    vec3 horizontal_velocity = m_Transform.input_velocity;
    horizontal_velocity.y = 0;
    horizontal_velocity.Truncate(m_Transform.max_speed);
    
    m_Transform.input_velocity = horizontal_velocity + vec3(0, m_Transform.input_velocity.y, 0);

    m_Transform.acceleration = vec3();
    m_Transform.input_acceleration = vec3();
    m_Transform.rotation = 0.0f;
}

Game::Game(mc::protocol::packets::PacketDispatcher* dispatcher, GameWindow& window, const Camera& camera)
    : mc::protocol::packets::PacketHandler(dispatcher),
      m_NetworkClient(dispatcher, mc::protocol::Version::Minecraft_1_13_2),
      m_Window(window),
      m_Camera(camera),
      m_Sprinting(false),
      m_LastPositionTime(0)
{
    window.RegisterMouseChange(std::bind(&Game::OnMouseChange, this, std::placeholders::_1, std::placeholders::_2));
    window.RegisterMouseScroll(std::bind(&Game::OnMouseScroll, this, std::placeholders::_1, std::placeholders::_2));
    window.RegisterMouseButton(std::bind(&Game::OnMousePress, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

    m_NetworkClient.GetPlayerController()->SetHandleFall(true);
    m_NetworkClient.GetConnection()->GetSettings()
        .SetMainHand(mc::MainHand::Right)
        .SetViewDistance(4);

    m_NetworkClient.GetPlayerManager()->RegisterListener(this);

    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::UpdateHealth, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::EntityVelocity, this);
    dispatcher->RegisterHandler(mc::protocol::State::Play, mc::protocol::play::SpawnPosition, this);
}

void Game::CreatePlayer(terra::World* world) {
    m_Player = std::make_unique<terra::Player>(world);
}

mc::Vector3d Game::GetPosition() {
    return m_Player->GetTransform().position;
}

void Game::Update() {
    UpdateClient();

    float current_frame = (float)glfwGetTime();

    if (current_frame - m_LastFrame < 1.0f / 60.0f) {
        return;
    }

    m_DeltaTime = current_frame - m_LastFrame;
    m_LastFrame = current_frame;

    mc::Vector3d front(
        std::cos(m_Camera.GetYaw()) * std::cos(0),
        std::sin(0),
        std::sin(m_Camera.GetYaw()) * std::cos(0)
    );

    mc::Vector3d direction;

    if (m_Window.IsKeyDown(GLFW_KEY_W)) {
        direction += front;

        if (m_Window.IsKeyDown(GLFW_KEY_LEFT_CONTROL)) {
            m_Sprinting = true;
        }
    }

    if (m_Window.IsKeyDown(GLFW_KEY_S)) {
        direction -= front;
        m_Sprinting = false;
    }

    if (m_Window.IsKeyDown(GLFW_KEY_A)) {
        mc::Vector3d right = mc::Vector3Normalize(front.Cross(mc::Vector3d(0, 1, 0)));

        direction -= right;
    }

    if (m_Window.IsKeyDown(GLFW_KEY_D)) {
        mc::Vector3d right = mc::Vector3Normalize(front.Cross(mc::Vector3d(0, 1, 0)));

        direction += right;
    }

    if (!m_Player->OnGround()) {
        direction *= 0.2;
    }

    if (m_Sprinting) {
        if (direction.LengthSq() <= 0.0) {
            m_Sprinting = false;
        } else {
            direction *= 1.3f;
        }
        m_Camera.SetFov(glm::radians(90.0f));
    } else {
        m_Camera.SetFov(glm::radians(80.0f));
    }

    if (m_Window.IsKeyDown(GLFW_KEY_SPACE) && m_Player->OnGround()) {
        m_Player->GetTransform().input_acceleration += mc::Vector3d(0, 6 / m_DeltaTime, 0);
    }

    m_Player->GetTransform().max_speed = 4.3f + (int)m_Sprinting * 1.3f;
    m_Player->GetTransform().input_acceleration += direction * 85.0f;

    m_Player->Update(m_DeltaTime);

    glm::vec3 eye = math::VecToGLM(m_Player->GetTransform().position) + glm::vec3(0, 1.6, 0);

    m_Camera.SetPosition(eye);

    constexpr float kTickTime = 1000.0f / 20.0f / 1000.0f;

    if (current_frame > m_LastPositionTime + kTickTime && m_NetworkClient.GetConnection()->GetProtocolState() == mc::protocol::State::Play) {
        float yaw = m_Camera.GetYaw() - glm::radians(90.0f);
        float pitch = -m_Camera.GetPitch();

        mc::protocol::packets::out::PlayerPositionAndLookPacket response(m_Player->GetTransform().position, yaw * 180.0f / 3.14159f, pitch * 180.0f / 3.14159f, m_Player->OnGround());

        m_NetworkClient.GetConnection()->SendPacket(&response);

        m_LastPositionTime = current_frame;

        if (m_Player->IsSneaking() && !m_Window.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
            mc::protocol::packets::out::EntityActionPacket packet(0, mc::protocol::packets::out::EntityActionPacket::Action::StopSneak);

            m_NetworkClient.GetConnection()->SendPacket(&packet);

            m_Player->SetSneaking(false);
        } else if (!m_Player->IsSneaking() && m_Window.IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
            mc::protocol::packets::out::EntityActionPacket packet(0, mc::protocol::packets::out::EntityActionPacket::Action::StartSneak);

            m_NetworkClient.GetConnection()->SendPacket(&packet);

            m_Player->SetSneaking(true);
        }
    }
}

void Game::UpdateClient() {
    try {
        m_NetworkClient.GetConnection()->CreatePacket();
    } catch (std::exception& e) {
        std::wcout << e.what() << std::endl;
    }
}

void Game::OnMouseChange(double offset_x, double offset_y) {
    m_Camera.ProcessRotation((float)offset_x, (float)offset_y);
}

void Game::OnMouseScroll(double offset_x, double offset_y) {
    m_Camera.ProcessZoom((float)offset_y);
}

void Game::OnClientSpawn(mc::core::PlayerPtr player) {
    m_Player->GetTransform().position = player->GetEntity()->GetPosition();
    m_Player->GetTransform().velocity = mc::Vector3d();
    m_Player->GetTransform().acceleration = mc::Vector3d();
    m_Player->GetTransform().orientation = player->GetEntity()->GetYaw() * 3.14159f / 180.0f;
}

void Game::HandlePacket(mc::protocol::packets::in::UpdateHealthPacket* packet) {

}

void Game::HandlePacket(mc::protocol::packets::in::EntityVelocityPacket* packet) {
    mc::EntityId eid = packet->GetEntityId();

    auto playerEntity = m_NetworkClient.GetEntityManager()->GetPlayerEntity();
    if (playerEntity && playerEntity->GetEntityId() == eid) {
        mc::Vector3d newVelocity = ToVector3d(packet->GetVelocity()) * 20.0 / 8000.0;

        std::cout << "Applying new velocity " << newVelocity << std::endl;
        m_Player->GetTransform().velocity = newVelocity;
    }
}

void Game::HandlePacket(mc::protocol::packets::in::SpawnPositionPacket* packet) {
    s64 x = packet->GetLocation().GetX();
    s64 y = packet->GetLocation().GetY();
    s64 z = packet->GetLocation().GetZ();
}

// TODO: Temporary fun code
template <typename T>
inline T Sign(T val) {
    return std::signbit(val) ? static_cast<T>(-1) : static_cast<T>(1);
}

// TODO: Temporary fun code
inline mc::Vector3d BasisAxis(int basisIndex) {
    static const mc::Vector3d axes[3] = { mc::Vector3d(1, 0, 0), mc::Vector3d(0, 1, 0), mc::Vector3d(0, 0, 1) };
    return axes[basisIndex];
}

// TODO: Temporary fun code
std::pair<mc::Vector3d, mc::Face> GetClosestNormal(const mc::Vector3d& pos, mc::AABB bounds) {
    mc::Vector3d center = bounds.min + (bounds.max - bounds.min) / 2;
    mc::Vector3d dim = bounds.max - bounds.min;
    mc::Vector3d offset = pos - center;

    double minDist = std::numeric_limits<double>::max();
    mc::Vector3d normal;

    for (int i = 0; i < 3; ++i) {
        double dist = dim[i] - std::abs(offset[i]);
        if (dist < minDist) {
            minDist = dist;
            normal = BasisAxis(i) * Sign(offset[i]);
        }
    }

    mc::Face face = mc::Face::North;

    if (normal.x == 1) {
        face = mc::Face::East;
    } else if (normal.x == -1) {
        face = mc::Face::West;
    } else if (normal.y == 1) {
        face = mc::Face::Top;
    } else if (normal.y == -1) {
        face = mc::Face::Bottom;
    } else if (normal.z == 1) {
        face = mc::Face::South;
    }

    return std::make_pair<>(normal, face);
}

// TODO: Temporary fun code
bool RayCast(mc::world::World& world, mc::Vector3d from, mc::Vector3d direction, double range, mc::Vector3d& hit, mc::Vector3d& normal, mc::Face& face) {
    static const std::vector<mc::Vector3d> directions = {
        mc::Vector3d(0, 0, 0), 
        mc::Vector3d(1, 0, 0), mc::Vector3d(-1, 0, 0), 
        mc::Vector3d(0, 1, 0), mc::Vector3d(0, -1, 0), 
        mc::Vector3d(0, 0, 1), mc::Vector3d(0, 0, -1),
        mc::Vector3d(0, 1, 1), mc::Vector3d(0, 1, -1),
        mc::Vector3d(1, 1, 0), mc::Vector3d(1, 1, 1), mc::Vector3d(1, 1, -1),
        mc::Vector3d(-1, 1, 0), mc::Vector3d(-1, 1, 1), mc::Vector3d(-1, 1, -1),

        mc::Vector3d(0, -1, 1), mc::Vector3d(0, -1, -1),
        mc::Vector3d(1, -1, 0), mc::Vector3d(1, -1, 1), mc::Vector3d(1, -1, -1),
        mc::Vector3d(-1, -1, 0), mc::Vector3d(-1, -1, 1), mc::Vector3d(-1, -1, -1)
    };

    mc::Ray ray(from, direction);

    double closest_distance = std::numeric_limits<double>::max();
    mc::Vector3d closest_pos;
    mc::AABB closest_aabb;
    bool collided = false;

    for (double i = 0; i < range + 1; ++i) {
        mc::Vector3d position = from + direction * i;

        for (mc::Vector3d checkDirection : directions) {
            mc::Vector3d checkPos = position + checkDirection;
            mc::block::BlockPtr block = world.GetBlock(checkPos);

            if (block->IsOpaque()) {
                mc::AABB bounds = block->GetBoundingBox(checkPos);
                double distance;

                if (bounds.Intersects(ray, &distance)) {
                    if (distance < 0 || distance > range) continue;

                    if (distance < closest_distance) {
                        closest_distance = distance;
                        closest_pos = from + direction * closest_distance;
                        closest_aabb = bounds;
                        collided = true;
                    }
                }
            }
        }
    }

    if (collided) {
        hit = closest_pos;
        std::tie(normal, face) = GetClosestNormal(hit, closest_aabb);
        return true;
    }

    return false;
}

// TODO: Temporary fun code
void Game::OnMousePress(int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS) {
        using namespace mc::protocol::packets::out;

        auto& world = *m_NetworkClient.GetWorld();
        mc::Vector3d position(m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
        mc::Vector3d forward(m_Camera.GetFront().x, m_Camera.GetFront().y, m_Camera.GetFront().z);
        mc::Vector3d hit;
        mc::Vector3d normal;
        mc::Face face;

        if (RayCast(world, position, forward, 5.0, hit, normal, face)) {

            {
                PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::StartedDigging;
                PlayerDiggingPacket packet(status, mc::ToVector3i(hit + forward * 0.1), mc::Face::West);

                m_NetworkClient.GetConnection()->SendPacket(&packet);
            }

            {
                PlayerDiggingPacket::Status status = PlayerDiggingPacket::Status::FinishedDigging;
                PlayerDiggingPacket packet(status, mc::ToVector3i(hit + forward * 0.1), mc::Face::West);

                m_NetworkClient.GetConnection()->SendPacket(&packet);
            }
        }

        AnimationPacket animation;
        m_NetworkClient.GetConnection()->SendPacket(&animation);
        
    } else if (button == GLFW_MOUSE_BUTTON_2 && action == GLFW_PRESS) {
        auto& world = *m_NetworkClient.GetWorld();
        mc::Vector3d position(m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
        mc::Vector3d forward(m_Camera.GetFront().x, m_Camera.GetFront().y, m_Camera.GetFront().z);
        mc::Vector3d hit;
        mc::Vector3d normal;
        mc::Face face;

        if (RayCast(world, position, forward, 5.0, hit, normal, face)) {
            mc::inventory::Inventory& inventory = *m_NetworkClient.GetInventoryManager()->GetPlayerInventory();

            auto& hotbar = m_NetworkClient.GetHotbar();
            s32 slot_id = hotbar.GetSelectedSlot() + mc::inventory::Inventory::HOTBAR_SLOT_START;

            mc::inventory::Slot slot = inventory.GetItem(slot_id);

            // Item ids are separate from block ids.
            const u32 draw_block = 9;

            if (slot.GetItemId() != draw_block) {
                mc::protocol::packets::out::CreativeInventoryActionPacket packet(slot_id, mc::inventory::Slot(draw_block, 1, 0));
                m_NetworkClient.GetConnection()->SendPacket(&packet);
            }

            mc::Vector3i target = mc::ToVector3i(hit + forward * 0.1);

            mc::protocol::packets::out::PlayerBlockPlacementPacket packet(target, face, mc::Hand::Main, mc::Vector3f(1.0f, 0.0f, 0.0f));
            m_NetworkClient.GetConnection()->SendPacket(&packet);
        }
    }
}

} // ns terra
