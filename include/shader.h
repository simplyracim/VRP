#pragma once
#include <string>
#include <glm/mat4x4.hpp>

class Shader {
public:
    Shader(const char* vertexSrc, const char* fragmentSrc);
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void setMat4(const char* name, const glm::mat4& m) const;
    void use() const;

private:
    unsigned int m_id = 0;
};