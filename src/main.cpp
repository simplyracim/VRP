#include <glad/glad.h>
#include "window.h"
#include "shader.h"
#include "mesh.h"
#include <iostream>
#include "shaders_embedded.h"

int main() {
    try {
        Window window(800, 600, "OpenGL Shape");
        Shader shader(shaders::vertex, shaders::fragment);
        Mesh triangle({
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.0f,  0.5f
        });

        while (!window.shouldClose()) {
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.use();
            triangle.draw();

            window.swapBuffers();
            window.pollEvents();
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return -1;
    }
    return 0;
}