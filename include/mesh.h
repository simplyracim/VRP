#pragma once
#include <vector>

class Mesh {
public:
    // vertex layout: x, y, z, nx, ny, nz
    Mesh(const std::vector<float>& vertices,
         const std::vector<unsigned int>& indices);
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void drawInstanced(int instanceCount) const;
    unsigned int vao() const { return m_vao; }

    void draw() const;

private:
    unsigned int m_vao = 0, m_vbo = 0, m_ebo = 0;
    int m_indexCount = 0;
};