#include <random>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "window.h"
#include "ui.h"
#include "graph/graph.h"
#include "graph/layout.h"
#include "graph/generators.h"
#include "render/graph_renderer.h"
#include <cmath>
#include <iostream>

int main() {
    try {
        Window window(800, 600, "Graph Visualizer");
        UI ui(window.handle());          // after Window: destroyed before it
        GraphRenderer renderer;          // after Window: needs the GL context

        const float fov = glm::radians(45.0f);

        // --- build the graph and compute a 3D layout ---
        Graph graph = makePetersen();    // try makeComplete(8), makeCycle(12), makeRandom(40, 0.08f)
        float radius = 1.0f, distance = 3.0f;

        // lay out the current graph and frame the camera around it
        auto layoutAndFrame = [&]() {
            randomizePositions(graph, 2.0f, std::random_device{}());
            forceLayout(graph, 300);
            radius = graph.boundingRadius();
            distance = radius / std::sin(fov * 0.5f) * 1.1f;
        };
        layoutAndFrame();

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

            // --- handle "Randomize graph" ---
            if (state.randomizeRequested) {
                state.randomizeRequested = false;
                graph = makeRandom(state.nodeCount, state.edgeProbability,
                                   std::random_device{}());
                layoutAndFrame();
            }

            // --- apply state ---
            if (!state.paused) angle += dt * state.rotationSpeed;

            if (state.cullFaces) glEnable(GL_CULL_FACE);
            else                 glDisable(GL_CULL_FACE);
            glPolygonMode(GL_FRONT_AND_BACK, state.wireframe ? GL_LINE : GL_FILL);

            // --- 3D ---
            glClearColor(state.background[0], state.background[1],
                         state.background[2], 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 world = glm::rotate(glm::mat4(1.0f), angle,
                                          glm::normalize(glm::vec3(0.5f, 1.0f, 0.0f)));
            glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -distance));
            glm::mat4 proj = glm::perspective(fov, static_cast<float>(w) / h,
                                              0.1f, distance + radius * 2.0f);

            renderer.draw(graph, proj * view, world);

            // --- UI on top ---
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