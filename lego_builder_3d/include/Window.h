#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <string>

namespace LegoEngine {

class Window {
public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool shouldClose() const;
    void swapBuffers();
    void pollEvents();
    
    GLFWwindow* getHandle() const { return window_; }
    
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    
    void setKeyCallback(GLFWkeyfun callback);
    void setMouseCallback(GLFWcursorposfun callback);
    void setMouseButtonCallback(GLFWmousebuttonfun callback);
    void setScrollCallback(GLFWscrollfun callback);

private:
    GLFWwindow* window_;
    int width_, height_;
    std::string title_;
    
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};

} // namespace LegoEngine