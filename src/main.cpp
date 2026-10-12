#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "shaders_embedded.h"
#include <iostream>

int main() {
    try {
        Window window(800, 600, "Rotating Cube");
        Shader shader(shaders::vertex, shaders::fragment);

        // 6 faces x 4 vertices: x, y, z,  r, g, b
        Mesh cube({
            // front (red)
            -0.5f,-0.5f, 0.5f,  1,0,0,   0.5f,-0.5f, 0.5f,  1,0,0,
             0.5f, 0.5f, 0.5f,  1,0,0,  -0.5f, 0.5f, 0.5f,  1,0,0,
            // back (green)
             0.5f,-0.5f,-0.5f,  0,1,0,  -0.5f,-0.5f,-0.5f,  0,1,0,
            -0.5f, 0.5f,-0.5f,  0,1,0,   0.5f, 0.5f,-0.5f,  0,1,0,
            // right (blue)
             0.5f,-0.5f, 0.5f,  0,0,1,   0.5f,-0.5f,-0.5f,  0,0,1,
             0.5f, 0.5f,-0.5f,  0,0,1,   0.5f, 0.5f, 0.5f,  0,0,1,
            // left (yellow)
            -0.5f,-0.5f,-0.5f,  1,1,0,  -0.5f,-0.5f, 0.5f,  1,1,0,
            -0.5f, 0.5f, 0.5f,  1,1,0,  -0.5f, 0.5f,-0.5f,  1,1,0,
            // top (magenta)
            -0.5f, 0.5f, 0.5f,  1,0,1,   0.5f, 0.5f, 0.5f,  1,0,1,
             0.5f, 0.5f,-0.5f,  1,0,1,  -0.5f, 0.5f,-0.5f,  1,0,1,
            // bottom (cyan)
            -0.5f,-0.5f,-0.5f,  0,1,1,   0.5f,-0.5f,-0.5f,  0,1,1,
             0.5f,-0.5f, 0.5f,  0,1,1,  -0.5f,-0.5f, 0.5f,  0,1,1,
        }, {
             0, 1, 2,  2, 3, 0,
             4, 5, 6,  6, 7, 4,
             8, 9,10, 10,11, 8,
            12,13,14, 14,15,12,
            16,17,18, 18,19,16,
            20,21,22, 22,23,20,
        });

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);   // optional: skip faces pointing away

        while (!window.shouldClose()) {
            int w, h;
            window.getFramebufferSize(w, h);
            if (w == 0 || h == 0) { window.pollEvents(); continue; } // minimized
            glViewport(0, 0, w, h);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float t = static_cast<float>(window.time());
            glm::mat4 model = glm::rotate(glm::mat4(1.0f), t,
                                          glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f)));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));
            glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                              static_cast<float>(w) / h, 0.1f, 100.0f);

            shader.use();
            shader.setMat4("uMVP", proj * view * model);
            cube.draw();

            window.swapBuffers();
            window.pollEvents();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
    return 0;
}