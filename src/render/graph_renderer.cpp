#include "render/graph_renderer.h"
#include "shaders_embedded.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstddef>

// ---- Batch ----

GraphRenderer::Batch::Batch(const MeshData& data) : mesh(data.vertices, data.indices) {
    glGenBuffers(1, &vbo);
    glBindVertexArray(mesh.vao());
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const GLsizei stride = sizeof(Instance);
    for (int i = 0; i < 4; ++i) {                      // mat4 = 4 vec4 attributes
        glEnableVertexAttribArray(2 + i);
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, stride,
                              (void*)(i * sizeof(glm::vec4)));
        glVertexAttribDivisor(2 + i, 1);
    }
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Instance, color));
    glVertexAttribDivisor(6, 1);
}

GraphRenderer::Batch::~Batch() { glDeleteBuffers(1, &vbo); }

void GraphRenderer::Batch::draw() {
    if (instances.empty()) return;
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(Instance),
                 instances.data(), GL_DYNAMIC_DRAW);
    mesh.drawInstanced(static_cast<int>(instances.size()));
}

// ---- GraphRenderer ----

GraphRenderer::GraphRenderer()
    : m_shader(shaders::vertex, shaders::fragment),
      m_nodes(sphereData(16, 32)),
      m_edges(cylinderData(16)) {}

void GraphRenderer::draw(const Graph& graph, const glm::mat4& viewProj, const glm::mat4& world) {
    const auto& nodes = graph.nodes();

    m_nodes.instances.clear();
    m_edges.instances.clear();

    for (const Node& n : nodes) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), n.position)
                        * glm::scale(glm::mat4(1.0f), glm::vec3(n.radius));
        m_nodes.instances.push_back({model, n.color});
    }

    const glm::vec3 up(0.0f, 1.0f, 0.0f);
    for (const Edge& e : graph.edges()) {
        glm::vec3 a = nodes[e.from].position;
        glm::vec3 b = nodes[e.to].position;
        glm::vec3 delta = b - a;
        float len = glm::length(delta);
        if (len < 1e-4f) continue;
        glm::vec3 dir = delta / len;

        // rotate the cylinder's Y axis onto the edge direction
        glm::mat4 rot(1.0f);
        glm::vec3 axis = glm::cross(up, dir);
        if (glm::length(axis) > 1e-5f)
            rot = glm::rotate(glm::mat4(1.0f), std::acos(glm::clamp(glm::dot(up, dir), -1.0f, 1.0f)),
                              glm::normalize(axis));
        else if (dir.y < 0.0f)   // pointing straight down
            rot = glm::rotate(glm::mat4(1.0f), 3.14159265f, glm::vec3(1, 0, 0));

        glm::mat4 model = glm::translate(glm::mat4(1.0f), (a + b) * 0.5f)
                        * rot
                        * glm::scale(glm::mat4(1.0f), glm::vec3(e.radius, len, e.radius));
        m_edges.instances.push_back({model, e.color});
    }

    m_shader.use();
    m_shader.setMat4("uViewProj", viewProj);
    m_shader.setMat4("uWorld", world);
    m_edges.draw();
    m_nodes.draw();
}