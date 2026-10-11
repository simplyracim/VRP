#include "shader.h"
#include <glad/glad.h>
#include <stdexcept>


static unsigned int compile(GLenum type, const char* src, const char* name) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    int ok;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(shader, 512, nullptr, log);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Shader error (") + name + "):\n" + log);
    }
    return shader;
}

Shader::Shader(const char* vertexSrc, const char* fragmentSrc) {
    unsigned int vs = compile(GL_VERTEX_SHADER, vertexSrc, "vertex");
    unsigned int fs = compile(GL_FRAGMENT_SHADER, fragmentSrc, "fragment");

    m_id = glCreateProgram();
    glAttachShader(m_id, vs);
    glAttachShader(m_id, fs);
    glLinkProgram(m_id);

    glDeleteShader(vs);
    glDeleteShader(fs);

    int ok;
    glGetProgramiv(m_id, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(m_id, 512, nullptr, log);
        glDeleteProgram(m_id);
        throw std::runtime_error(std::string("Link error:\n") + log);
    }
}

Shader::~Shader() { glDeleteProgram(m_id); }
void Shader::use() const { glUseProgram(m_id); }