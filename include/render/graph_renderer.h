#pragma once
#include "graph/graph.h"
#include "mesh.h"
#include "shader.h"
#include "render/primitives.h"
#include <glm/glm.hpp>
#include <vector>

struct Instance {
    glm::mat4 model;
    glm::vec3 color;
};

class GraphRenderer {
public:
    GraphRenderer();   // needs a live GL context, so create it after Window

    void draw(const Graph& graph, const glm::mat4& viewProj, const glm::mat4& world);

private:
    // One mesh plus a per-instance buffer attached to its VAO.
    struct Batch {
        explicit Batch(const MeshData& data);
        ~Batch();
        Batch(const Batch&) = delete;
        Batch& operator=(const Batch&) = delete;

        void draw();

        Mesh mesh;
        unsigned int vbo = 0;
        std::vector<Instance> instances;
    };

    Shader m_shader;
    Batch  m_nodes;   // spheres
    Batch  m_edges;   // cylinders
};