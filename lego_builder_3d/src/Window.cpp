#include "Window.h"
#include <GL/gl.h>
#include <stdexcept>
#include <iostream>

namespace LegoEngine {

Window::Window(int width, int height, const std::string& title)
    : width_(width), height_(height), title_(title) {
    
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    
    // Set OpenGL version and profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);
    
    // Store this instance in the window user pointer for callbacks
    glfwSetWindowUserPointer(window_, this);
}

Window::~Window() {
    if (window_) {
        glfwDestroyWindow(window_);
    }
    glfwTerminate();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window_);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window_);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::setKeyCallback(GLFWkeyfun callback) {
    glfwSetKeyCallback(window_, callback);
}

void Window::setMouseCallback(GLFWcursorposfun callback) {
    glfwSetCursorPosCallback(window_, callback);
}

void Window::setMouseButtonCallback(GLFWmousebuttonfun callback) {
    glfwSetMouseButtonCallback(window_, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback) {
    glfwSetScrollCallback(window_, callback);
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    
    // Update the Window object's dimensions
    Window* windowObj = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (windowObj) {
        windowObj->width_ = width;
        windowObj->height_ = height;
    }
}

} // namespace LegoEngine