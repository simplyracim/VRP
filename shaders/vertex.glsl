#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in mat4 aModel;    // uses locations 2, 3, 4, 5
layout (location = 6) in vec3 aColor;

uniform mat4 uViewProj;
uniform mat4 uWorld;                     // whole-graph rotation

out vec3 vNormal;
out vec3 vColor;

void main() {
    mat4 m = uWorld * aModel;
    // inverse-transpose: edges are stretched cylinders (non-uniform scale)
    vNormal = transpose(inverse(mat3(m))) * aNormal;
    vColor = aColor;
    gl_Position = uViewProj * m * vec4(aPos, 1.0);
}