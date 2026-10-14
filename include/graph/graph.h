#pragma once
#include <glm/vec3.hpp>
#include <vector>

struct Node {
    glm::vec3 position{0.0f};
    glm::vec3 color{0.4f, 0.7f, 1.0f};
    float radius = 0.2f;
};

struct Edge {
    int from = 0, to = 0;
    glm::vec3 color{0.6f};
    float radius = 0.04f;
};

class Graph {
public:
    int  addNode(glm::vec3 position = glm::vec3(0.0f));  // returns the node's index
    void addEdge(int from, int to);

    std::vector<Node>&       nodes()       { return m_nodes; }
    const std::vector<Node>& nodes() const { return m_nodes; }
    const std::vector<Edge>& edges() const { return m_edges; }

    float boundingRadius() const;   // distance from origin to the farthest node surface
    void  clear();

private:
    std::vector<Node> m_nodes;
    std::vector<Edge> m_edges;
};