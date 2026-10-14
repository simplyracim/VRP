#include "ui.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

UI::UI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::draw(UIState& s) {
    ImGui::Begin("Controls");
    ImGui::SliderFloat("Rotation speed", &s.rotationSpeed, 0.0f, 5.0f);
    ImGui::Checkbox("Paused", &s.paused);
    ImGui::Separator();
    ImGui::ColorEdit3("Background", s.background);
    ImGui::Checkbox("Wireframe", &s.wireframe);
    ImGui::Checkbox("Cull back faces", &s.cullFaces);
    ImGui::Separator();
    ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Separator();
    ImGui::SliderInt("Nodes", &s.nodeCount, 3, 100);
    ImGui::SliderFloat("Edge probability", &s.edgeProbability, 0.0f, 1.0f);
    if (ImGui::Button("Randomize graph"))
        s.randomizeRequested = true;
}

void UI::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool UI::wantsMouse() const    { return ImGui::GetIO().WantCaptureMouse; }
bool UI::wantsKeyboard() const { return ImGui::GetIO().WantCaptureKeyboard; }