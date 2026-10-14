#include "graph/graph.h"
#include <glm/glm.hpp>
#include <algorithm>
#include <stdexcept>

int Graph::addNode(glm::vec3 position) {
    Node n;
    n.position = position;
    m_nodes.push_back(n);
    return static_cast<int>(m_nodes.size()) - 1;
}

void Graph::addEdge(int from, int to) {
    const int n = static_cast<int>(m_nodes.size());
    if (from < 0 || from >= n || to < 0 || to >= n)
        throw std::out_of_range("Graph::addEdge: node index out of range");
    Edge e;
    e.from = from;
    e.to = to;
    m_edges.push_back(e);
}

float Graph::boundingRadius() const {
    float r = 1.0f;
    for (const Node& n : m_nodes)
        r = std::max(r, glm::length(n.position) + n.radius);
    return r;
}

void Graph::clear() {
    m_nodes.clear();
    m_edges.clear();
}