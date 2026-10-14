#include "render/primitives.h"
#include <cmath>

static constexpr float kPi = 3.14159265358979f;

MeshData sphereData(int stacks, int sectors) {
    MeshData d;
    for (int i = 0; i <= stacks; ++i) {
        float theta = kPi * i / stacks;
        for (int j = 0; j <= sectors; ++j) {
            float phi = 2.0f * kPi * j / sectors;
            float x = std::sin(theta) * std::cos(phi);
            float y = std::cos(theta);
            float z = std::sin(theta) * std::sin(phi);
            d.vertices.insert(d.vertices.end(), {x, y, z, x, y, z});  // normal == position
        }
    }
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < sectors; ++j) {
            unsigned a = i * (sectors + 1) + j;
            unsigned b = a + sectors + 1;
            d.indices.insert(d.indices.end(), {a, a + 1, b,  a + 1, b + 1, b});  // CCW from outside
        }
    }
    return d;
}

MeshData cylinderData(int sectors) {
    MeshData d;
    for (int j = 0; j <= sectors; ++j) {
        float phi = 2.0f * kPi * j / sectors;
        float x = std::cos(phi), z = std::sin(phi);
        d.vertices.insert(d.vertices.end(), {x, -0.5f, z, x, 0.0f, z});  // bottom (index 2j)
        d.vertices.insert(d.vertices.end(), {x,  0.5f, z, x, 0.0f, z});  // top    (index 2j+1)
    }
    for (int j = 0; j < sectors; ++j) {
        unsigned b0 = 2 * j, t0 = 2 * j + 1, b1 = 2 * (j + 1), t1 = 2 * (j + 1) + 1;
        d.indices.insert(d.indices.end(), {b0, t0, b1,  t0, t1, b1});
    }
    return d;   // open-ended: the node spheres cover the ends
}