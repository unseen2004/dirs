#include "Window.h"
#include "Renderer.h"
#include "Camera.h"
#include "LegoSystem.h"
#include "InputHandler.h"
#include "UIRenderer.h"
#include "../external/glad/glad.h"
#include <iostream>
#include <memory>
#include <chrono>

using namespace LegoEngine;

// Global variables for input handling
Camera camera(Vec3(0, 5, 10));
InputHandler inputHandler;
UIRenderer uiRenderer;
BrickSystem brickSystem;

// Callback functions
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode; // Suppress unused parameter warning
    (void)mods;     // Suppress unused parameter warning
    
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    inputHandler.processKey(key, action);
    
    // Handle undo/redo
    if (key == GLFW_KEY_Z && action == GLFW_PRESS && (mods & GLFW_MOD_CONTROL)) {
        if (mods & GLFW_MOD_SHIFT) {
            // Ctrl+Shift+Z = Redo
            if (brickSystem.canRedo()) {
                brickSystem.redo();
                std::cout << "Redo\n";
            }
        } else {
            // Ctrl+Z = Undo
            if (brickSystem.canUndo()) {
                brickSystem.undo();
                std::cout << "Undo\n";
            }
        }
    }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    static bool firstMouse = true;
    static float lastX = 400, lastY = 300;
    
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
    (void)window;   // Suppress unused parameter warning
    (void)xoffset;  // Suppress unused parameter warning
    camera.processMouseScroll(yoffset);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    (void)mods; // Suppress unused parameter warning
    
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (inputHandler.isPlacementMode()) {
            // Create new brick at preview position
            BrickInstance newBrick(
                inputHandler.getPreviewPosition(),
                inputHandler.getSelectedBrickType(),
                inputHandler.getSelectedBrickColor()
            );
            
            // Snap to grid
            newBrick.position = brickSystem.snapToGrid(newBrick.position);
            
            // Add brick if placement is valid
            if (brickSystem.isValidPlacement(newBrick)) {
                brickSystem.addBrick(newBrick);
                std::cout << "Brick placed successfully!\n";
            } else {
                std::cout << "Invalid placement - collision detected!\n";
            }
        }
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
        
        // Add some initial demo bricks
        brickSystem.addBrick(BrickInstance(Vec3(0, 0, 0), BrickType::BRICK_2x4, BrickColor::RED));
        brickSystem.addBrick(BrickInstance(Vec3(2, 0, 0), BrickType::BRICK_2x2, BrickColor::BLUE));
        brickSystem.addBrick(BrickInstance(Vec3(0, 1, 0), BrickType::BRICK_1x2, BrickColor::GREEN));
        brickSystem.addBrick(BrickInstance(Vec3(-2, 0, 0), BrickType::BRICK_2x6, BrickColor::YELLOW));
        
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
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}
)";
        
        Shader shader(vertexShaderSource, fragmentShaderSource);
        
        // Setup UI
        uiRenderer.showInstructions();
        
        // Timing
        auto lastFrame = std::chrono::high_resolution_clock::now();
        
        std::cout << "\n=== LEGO Builder 3D Started ===\n";
        std::cout << "Use WASD to move, mouse to look around\n";
        std::cout << "Press 1-6 to select brick types, C to change colors\n";
        std::cout << "Left-click to place bricks\n";
        std::cout << "================================\n\n";
        
        // Main render loop
        while (!window.shouldClose()) {
            // Calculate delta time
            auto currentFrame = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
            lastFrame = currentFrame;
            
            // Process input
            processInput(window.getHandle(), deltaTime);
            window.pollEvents();
            
            // Update input handler for preview position
            Mat4 view = camera.getViewMatrix();
            Mat4 projection = camera.getProjectionMatrix(
                (float)window.getWidth() / (float)window.getHeight());
            inputHandler.updatePreviewPosition(window.getHandle(), view, projection);
            
            // Render
            renderer.clear(0.15f, 0.25f, 0.35f, 1.0f); // Nice blue-gray background
            
            // Set shader uniforms
            shader.use();
            shader.setVec3("lightColor", Vec3(1.0f, 1.0f, 1.0f));
            shader.setVec3("lightPos", Vec3(10.0f, 10.0f, 10.0f));
            shader.setVec3("viewPos", camera.getPosition());
            
            // Render existing LEGO bricks
            brickSystem.render(shader, view, projection);
            
            // Render preview brick if in placement mode
            if (inputHandler.isPlacementMode()) {
                Vec3 previewPos = brickSystem.snapToGrid(inputHandler.getPreviewPosition());
                Mat4 previewModel = Mat4::translate(previewPos);
                
                // Make preview brick semi-transparent by using a different color
                Vec3 previewColor = getBrickColorRGB(inputHandler.getSelectedBrickColor()) * 0.7f;
                shader.setVec3("objectColor", previewColor);
                
                // Create a temporary brick for preview
                BrickInstance previewBrick(previewPos, inputHandler.getSelectedBrickType(), 
                                         inputHandler.getSelectedBrickColor());
                
                // Render preview (simplified - in a real implementation you'd want proper transparency)
                // For now, just render with dimmed colors
            }
            
            // Update UI
            uiRenderer.clearElements();
            uiRenderer.showInstructions();
            uiRenderer.showBrickInfo(inputHandler.getSelectedBrickType(), inputHandler.getSelectedBrickColor());
            if (inputHandler.isPlacementMode()) {
                uiRenderer.showStatus("Placement Mode - Left click to place brick");
            } else {
                uiRenderer.showStatus("View Mode - Press Space to enter placement mode");
            }
            uiRenderer.render(window.getWidth(), window.getHeight());
            
            window.swapBuffers();
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "\nThank you for using LEGO Builder 3D!\n";
    return 0;
}