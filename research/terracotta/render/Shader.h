#ifndef TERRACOTTA_RENDER_SHADER_H_
#define TERRACOTTA_RENDER_SHADER_H_

#include <GL/glew.h>

namespace terra {
namespace render {

class Shader {
public:
    Shader();
    Shader(const Shader& other) = delete;
    Shader(Shader&& other) = delete;
    Shader& operator=(const Shader& other) = delete;
    Shader& operator=(Shader&& other) = delete;

    bool Initialize(const char* vertexPath, const char* fragmentPath);

    void Use();
    void Stop();

    GLuint GetUniform(const char* name);

private:
    GLuint m_Program;
};

} // ns render
} // ns terra

#endif
