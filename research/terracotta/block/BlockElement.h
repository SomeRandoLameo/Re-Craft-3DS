#pragma once

#ifndef TERRACOTTA_BLOCK_BLOCKELEMENT_H_
#define TERRACOTTA_BLOCK_BLOCKELEMENT_H_

#include "BlockFace.h"
#include <vector>
#include <glm/glm.hpp>
#include "../assets/TextureArray.h"

namespace terra {
namespace block {

struct RenderableFace {
    assets::TextureHandle texture;
    int tint_index;
    BlockFace face;
    BlockFace cull_face;
    glm::vec2 uv_from;
    glm::vec2 uv_to;
};

struct ElementRotation {
    glm::vec3 origin;
    glm::vec3 axis;
    float angle;
    bool rescale;

    ElementRotation() : origin(0, 0, 0), axis(0, 0, 0), angle(0), rescale(false) { }
};

class BlockElement {
public:
    BlockElement(glm::vec3 from, glm::vec3 to) : m_From(from), m_To(to), m_Shade(true) {
        m_Faces.resize(6);

        for (RenderableFace& renderable : m_Faces) {
            renderable.face = BlockFace::None;
        }

        m_FullExtent = m_From == glm::vec3(0, 0, 0) && m_To == glm::vec3(1, 1, 1);
    }

    std::vector<RenderableFace>& GetFaces() { return m_Faces; }

    const RenderableFace& GetFace(BlockFace face) const { return m_Faces[static_cast<std::size_t>(face)]; }
    void AddFace(RenderableFace face) { m_Faces[static_cast<std::size_t>(face.face)] = face; }

    const glm::vec3& GetFrom() const { return m_From; }
    const glm::vec3& GetTo() const { return m_To; }
    bool IsFullExtent() const { return m_FullExtent; }

    void SetShouldShade(bool shade) { m_Shade = shade; }
    bool ShouldShade() const { return m_Shade; }

    ElementRotation& GetRotation() { return m_Rotation; }
    const ElementRotation& GetRotation() const { return m_Rotation; }

private:
    std::vector<RenderableFace> m_Faces;
    glm::vec3 m_From;
    glm::vec3 m_To;
    bool m_FullExtent;
    bool m_Shade;
    ElementRotation m_Rotation;
};

} // ns block
} // ns terra

#endif

