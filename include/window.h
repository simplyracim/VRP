#pragma once
#include <string>

struct GLFWwindow;

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    double time() const;
    void getFramebufferSize(int& w, int& h) const;
    GLFWwindow* handle() const { return m_window; }

private:
    GLFWwindow* m_window = nullptr;
};