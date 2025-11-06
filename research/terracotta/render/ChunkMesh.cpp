#include "ChunkMesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include "../assets/AssetCache.h"
#include <iostream>

namespace terra {
namespace render {

ChunkMesh::ChunkMesh(unsigned int vao, unsigned int vbo, GLsizei vertex_count) 
    : m_VAO(vao), 
      m_VBO(vbo), 
      m_VertexCount(vertex_count) 
{

}

ChunkMesh::ChunkMesh(const ChunkMesh& other) {
    this->m_VAO = other.m_VAO;
    this->m_VBO = other.m_VBO;
    this->m_VertexCount = other.m_VertexCount;
}

ChunkMesh& ChunkMesh::operator=(const ChunkMesh& other) {
    this->m_VAO = other.m_VAO;
    this->m_VBO = other.m_VBO;
    this->m_VertexCount = other.m_VertexCount;

    return *this;
}

void ChunkMesh::Render(unsigned int model_uniform) {
    static const glm::mat4 modelMatrix(1.0);

    glBindVertexArray(m_VAO);
    g_AssetCache->GetTextures().Bind();

    glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);

    GLenum error;

    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error rendering: " << error << std::endl;
    }
}

void ChunkMesh::Destroy() {
    glBindVertexArray(m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
}

} // ns render
} // ns terra
