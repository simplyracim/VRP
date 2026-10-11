#pragma once
#include <vector>

class Mesh {
public:
    explicit Mesh(const std::vector<float>& vertices); // 2D positions: x, y
    ~Mesh();
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw() const;

private:
    unsigned int m_vao = 0, m_vbo = 0;
    int m_vertexCount = 0;
};