#pragma once
#include <vector>

class Mesh {
public:
    // vertex layout: x, y, z, r, g, b
    Mesh(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw() const;

private:
    unsigned int m_vao = 0, m_vbo = 0, m_ebo = 0;
    int m_indexCount = 0;
};