#ifndef TERRACOTTA_RENDER_CHUNKMESH_H_
#define TERRACOTTA_RENDER_CHUNKMESH_H_

#include "Shader.h"
#include <cstddef>

namespace terra {
namespace render {

class ChunkMesh {
public:
    ChunkMesh(unsigned int vao, unsigned int vbo, GLsizei vertex_count);
    ChunkMesh(const ChunkMesh& other);
    ChunkMesh& operator=(const ChunkMesh& other);

    void Render(unsigned int model_uniform);
    void Destroy();

private:
    unsigned int m_VAO;
    unsigned int m_VBO;
    GLsizei m_VertexCount;
};

} // ns render
} // ns terra

#endif
