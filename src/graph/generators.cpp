#include "graph/generators.h"
#include <random>

Graph makeCycle(int n) {
    Graph g;
    for (int i = 0; i < n; ++i) g.addNode();
    for (int i = 0; i < n; ++i) g.addEdge(i, (i + 1) % n);
    return g;
}

Graph makeComplete(int n) {
    Graph g;
    for (int i = 0; i < n; ++i) g.addNode();
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) g.addEdge(i, j);
    return g;
}

Graph makePetersen() {
    Graph g;
    for (int i = 0; i < 10; ++i) g.addNode();
    for (int i = 0; i < 5; ++i) {
        g.addEdge(i, (i + 1) % 5);          // outer pentagon
        g.addEdge(5 + i, 5 + (i + 2) % 5);  // inner pentagram
        g.addEdge(i, 5 + i);                // spokes
    }
    return g;
}

Graph makeRandom(int n, float p, unsigned seed) {
    Graph g;
    for (int i = 0; i < n; ++i) g.addNode();
    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(rng) < p) g.addEdge(i, j);
    return g;
}