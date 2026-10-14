#include "graph/layout.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <random>

void randomizePositions(Graph& g, float radius, unsigned seed) {
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> dist(-1.0f, 1.0f);
    for (Node& n : g.nodes())
        n.position = radius * glm::vec3(dist(rng), dist(rng), dist(rng));
}

void forceLayout(Graph& g, int iterations, float k) {
    auto& nodes = g.nodes();
    const size_t n = nodes.size();
    if (n < 2) return;

    std::vector<glm::vec3> disp(n);
    float temperature = k * 2.0f;
    const float gravity = 0.05f;   // keeps disconnected components from drifting away

    for (int it = 0; it < iterations; ++it) {
        std::fill(disp.begin(), disp.end(), glm::vec3(0.0f));

        // repulsion between every pair
        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                glm::vec3 delta = nodes[i].position - nodes[j].position;
                float d = std::max(glm::length(delta), 0.01f);
                glm::vec3 force = (delta / d) * (k * k / d);
                disp[i] += force;
                disp[j] -= force;
            }
        }

        // attraction along edges
        for (const Edge& e : g.edges()) {
            glm::vec3 delta = nodes[e.from].position - nodes[e.to].position;
            float d = std::max(glm::length(delta), 0.01f);
            glm::vec3 force = (delta / d) * (d * d / k);
            disp[e.from] -= force;
            disp[e.to]   += force;
        }

        // move, capped by the temperature
        for (size_t i = 0; i < n; ++i) {
            disp[i] -= nodes[i].position * gravity;
            float len = glm::length(disp[i]);
            if (len > 0.0f)
                nodes[i].position += (disp[i] / len) * std::min(len, temperature);
        }
        temperature = std::max(temperature * 0.98f, 0.01f);
    }

    // center on the origin so the rotation pivots around the middle
    glm::vec3 centroid(0.0f);
    for (const Node& node : nodes) centroid += node.position;
    centroid /= static_cast<float>(n);
    for (Node& node : nodes) node.position -= centroid;
}