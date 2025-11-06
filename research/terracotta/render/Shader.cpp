#include "Shader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool CreateShader(GLenum type, const char* source, GLuint* shaderOut) {
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    GLchar info_log[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
        std::cout << "Shader error: " << std::endl << info_log << std::endl;
        return false;
    }

    *shaderOut = shader;
    return true;
}

bool CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint* programOut) {
    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    GLchar info_log[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    if (!success) {
        glGetProgramInfoLog(program, sizeof(info_log), nullptr, info_log);
        std::cout << "Link error: " << std::endl << info_log << std::endl;
        return false;
    }

    *programOut = program;
    return true;
}

namespace terra {
namespace render {

Shader::Shader() {

}

bool Shader::Initialize(const char* vPath, const char* fPath) {
    std::string vertex_source, fragment_source;
    std::ifstream vertex_file, fragment_file;

    vertex_file.exceptions(std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::badbit);
    try {
        vertex_file.open(vPath);
        fragment_file.open(fPath);

        std::stringstream vertex_stream, fragment_stream;
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_source = vertex_stream.str();
        fragment_source = fragment_stream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Read error: " << e.what() << std::endl;
        return false;
    }

    GLuint vertex_shader, fragment_shader;
    if (!CreateShader(GL_VERTEX_SHADER, vertex_source.c_str(), &vertex_shader)) {
        return false;
    }

    if (!CreateShader(GL_FRAGMENT_SHADER, fragment_source.c_str(), &fragment_shader)) {
        glDeleteShader(vertex_shader);
        return false;
    }

    return CreateProgram(vertex_shader, fragment_shader, &m_Program);
}

GLuint Shader::GetUniform(const char* name) {
    return glGetUniformLocation(m_Program, name);
}

void Shader::Use() {
    glUseProgram(m_Program);
}

void Shader::Stop() {
    glUseProgram(0);
}

} // ns render
} // ns terra
