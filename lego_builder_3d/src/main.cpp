#include "Window.h"
#include "Renderer.h"
#include "Camera.h"
#include "LegoSystem.h"
#include "../external/glad/glad.h"
#include <iostream>
#include <memory>
#include <chrono>

using namespace LegoEngine;

// Global variables for input handling
Camera camera(Vec3(0, 5, 10));
float lastX = 400, lastY = 300;
bool firstMouse = true;
BrickType selectedBrickType = BrickType::BRICK_2x4;

// Callback functions
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    // Brick selection
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_1: selectedBrickType = BrickType::BRICK_1x1; break;
            case GLFW_KEY_2: selectedBrickType = BrickType::BRICK_1x2; break;
            case GLFW_KEY_3: selectedBrickType = BrickType::BRICK_2x2; break;
            case GLFW_KEY_4: selectedBrickType = BrickType::BRICK_2x4; break;
            case GLFW_KEY_5: selectedBrickType = BrickType::BRICK_2x6; break;
            case GLFW_KEY_6: selectedBrickType = BrickType::BRICK_2x8; break;
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    
    // Only process mouse look when right button is pressed
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        camera.processMouseMovement(xoffset, yoffset);
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        // TODO: Implement brick placement logic
        std::cout << "Left click - place brick\n";
    }
}

void processInput(GLFWwindow* window, float deltaTime) {
    int direction = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) direction |= 1; // Forward
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) direction |= 2; // Backward
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) direction |= 4; // Left
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) direction |= 8; // Right
    
    camera.processKeyboard(direction, deltaTime);
}

int main() {
    try {
        // Initialize window
        Window window(1200, 800, "LEGO Builder 3D");
        
        // Set callbacks
        window.setKeyCallback(keyCallback);
        window.setMouseCallback(mouseCallback);
        window.setScrollCallback(scrollCallback);
        window.setMouseButtonCallback(mouseButtonCallback);
        
        // Initialize OpenGL
        if (!gladLoadGL()) {
            std::cerr << "Failed to initialize GLAD\n";
            return -1;
        }
        
        // Initialize renderer
        Renderer renderer;
        renderer.setViewport(0, 0, window.getWidth(), window.getHeight());
        
        // Initialize LEGO system
        BrickSystem brickSystem;
        
        // Add some test bricks
        brickSystem.addBrick(BrickInstance(Vec3(0, 0, 0), BrickType::BRICK_2x4, BrickColor::RED));
        brickSystem.addBrick(BrickInstance(Vec3(2, 0, 0), BrickType::BRICK_2x2, BrickColor::BLUE));
        brickSystem.addBrick(BrickInstance(Vec3(0, 1, 0), BrickType::BRICK_1x2, BrickColor::GREEN));
        
        // Create shader
        std::string vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main() {
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
)";
        
        std::string fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";
        
        Shader shader(vertexShaderSource, fragmentShaderSource);
        
        // Timing
        auto lastFrame = std::chrono::high_resolution_clock::now();
        
        // Main render loop
        while (!window.shouldClose()) {
            // Calculate delta time
            auto currentFrame = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
            lastFrame = currentFrame;
            
            // Process input
            processInput(window.getHandle(), deltaTime);
            window.pollEvents();
            
            // Render
            renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);
            
            // Set up matrices
            Mat4 view = camera.getViewMatrix();
            Mat4 projection = camera.getProjectionMatrix(
                (float)window.getWidth() / (float)window.getHeight());
            
            // Set shader uniforms
            shader.use();
            shader.setVec3("lightColor", Vec3(1.0f, 1.0f, 1.0f));
            shader.setVec3("lightPos", Vec3(5.0f, 10.0f, 5.0f));
            shader.setVec3("viewPos", camera.getPosition());
            
            // Render LEGO bricks
            brickSystem.render(shader, view, projection);
            
            window.swapBuffers();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}