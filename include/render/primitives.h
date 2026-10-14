#pragma once
#include <vector>

// interleaved: x, y, z, nx, ny, nz
struct MeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

MeshData sphereData(int stacks, int sectors);   // unit radius, centered at origin
MeshData cylinderData(int sectors);             // unit radius, height 1, along Y, centered