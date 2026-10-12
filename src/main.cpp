#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"
#include "shader.h"
#include "mesh.h"
#include "ui.h"
#include "shaders_embedded.h"
#include <iostream>

int main() {
    try {
        Window window(800, 600, "Rotating Cube");
        UI ui(window.handle());          // declared after Window, so destroyed before it
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

        UIState state;
        glEnable(GL_DEPTH_TEST);

        float angle = 0.0f;
        double lastTime = window.time();

        while (!window.shouldClose()) {
            window.pollEvents();

            double now = window.time();
            float dt = static_cast<float>(now - lastTime);
            lastTime = now;

            int w, h;
            window.getFramebufferSize(w, h);
            if (w == 0 || h == 0) continue;   // minimized
            glViewport(0, 0, w, h);

            // --- UI ---
            ui.beginFrame();
            ui.draw(state);

            // --- apply state ---
            if (!state.paused) angle += dt * state.rotationSpeed;

            if (state.cullFaces) glEnable(GL_CULL_FACE);
            else                 glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);

            // --- 3D ---
            glClearColor(state.background[0], state.background[1],
                         state.background[2], 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model = glm::rotate(glm::mat4(1.0f), angle,
                                          glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f)));
            glm::mat4 view  = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3));
            glm::mat4 proj  = glm::perspective(glm::radians(45.0f),
                                               static_cast<float>(w) / h, 0.1f, 100.0f);

            shader.use();
            shader.setMat4("uMVP", proj * view * model);
            cube.draw();

            // --- UI on top ---
            // ImGui draws filled triangles, so make sure wireframe is off for it
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            ui.render();

            window.swapBuffers();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
    return 0;
}