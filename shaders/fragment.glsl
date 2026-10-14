#version 330 core
in vec3 vNormal;
in vec3 vColor;
out vec4 FragColor;

void main() {
    vec3 n = normalize(vNormal);
    vec3 lightDir = normalize(vec3(0.4, 0.7, 1.0));
    float diffuse = max(dot(n, lightDir), 0.0);
    FragColor = vec4(vColor * (0.25 + 0.75 * diffuse), 1.0);
}