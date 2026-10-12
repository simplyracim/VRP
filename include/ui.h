#pragma once

struct GLFWwindow;

// Everything the UI can change. main reads this; the UI writes to it.
struct UIState {
    float rotationSpeed = 1.0f;
    float background[3] = {0.1f, 0.1f, 0.1f};
    bool  paused    = false;
    bool  wireframe = false;
    bool  cullFaces = true;
};

class UI {
public:
    explicit UI(GLFWwindow* window);
    ~UI();
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;

    void beginFrame();          // call at the start of each frame
    void draw(UIState& state);  // builds the widgets
    void render();              // call after your 3D drawing, before swapBuffers

    bool wantsMouse() const;    // true when the cursor is over the UI
    bool wantsKeyboard() const;
};