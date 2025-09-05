#include "InputHandler.h"
#include <algorithm>
#include <iostream>

namespace LegoEngine {

InputHandler::InputHandler() 
    : selectedBrickType_(BrickType::BRICK_2x4), selectedBrickColor_(BrickColor::RED),
      placementMode_(true), previewPosition_(0, 0, 0), firstMouse_(true),
      lastX_(400), lastY_(300) {
}

InputHandler::~InputHandler() = default;

void InputHandler::processKeyboard(GLFWwindow* window, int direction, float deltaTime) {
    // This will be handled by the camera directly
}

void InputHandler::processMouseMovement(float xoffset, float yoffset) {
    // Mouse movement for camera is handled elsewhere
}

void InputHandler::processMouseButton(GLFWwindow* window, int button, int action) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (placementMode_) {
            std::cout << "Placing brick at position: (" 
                     << previewPosition_.x << ", " 
                     << previewPosition_.y << ", " 
                     << previewPosition_.z << ")\n";
            // This will be handled by the main application
        }
    }
}

void InputHandler::processKey(int key, int action) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_1: selectedBrickType_ = BrickType::BRICK_1x1; break;
            case GLFW_KEY_2: selectedBrickType_ = BrickType::BRICK_1x2; break;
            case GLFW_KEY_3: selectedBrickType_ = BrickType::BRICK_2x2; break;
            case GLFW_KEY_4: selectedBrickType_ = BrickType::BRICK_2x4; break;
            case GLFW_KEY_5: selectedBrickType_ = BrickType::BRICK_2x6; break;
            case GLFW_KEY_6: selectedBrickType_ = BrickType::BRICK_2x8; break;
            case GLFW_KEY_TAB: cycleBrickType(); break;
            case GLFW_KEY_C: cycleBrickColor(); break;
            case GLFW_KEY_SPACE: placementMode_ = !placementMode_; break;
        }
        
        std::cout << "Selected brick: " << static_cast<int>(selectedBrickType_) 
                 << ", Color: " << static_cast<int>(selectedBrickColor_) << std::endl;
    }
}

void InputHandler::processScroll(float yoffset) {
    // Scroll is handled by camera
}

Vec3 InputHandler::screenToWorldRay(GLFWwindow* window, const Mat4& view, const Mat4& projection) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    // Convert screen coordinates to normalized device coordinates
    float x = (2.0f * mouseX) / width - 1.0f;
    float y = 1.0f - (2.0f * mouseY) / height;
    
    // Create ray direction (simplified - in a real implementation, 
    // you'd need to properly unproject the coordinates)
    return Vec3(x, 0, y).normalize();
}

bool InputHandler::rayIntersectPlane(const Vec3& rayOrigin, const Vec3& rayDirection,
                                   const Vec3& planePoint, const Vec3& planeNormal, Vec3& intersection) {
    float denom = planeNormal.dot(rayDirection);
    if (std::abs(denom) < 0.0001f) {
        return false; // Ray is parallel to plane
    }
    
    Vec3 diff = planePoint - rayOrigin;
    float t = diff.dot(planeNormal) / denom;
    
    if (t < 0) {
        return false; // Intersection is behind ray origin
    }
    
    intersection = rayOrigin + rayDirection * t;
    return true;
}

void InputHandler::updatePreviewPosition(GLFWwindow* window, const Mat4& view, const Mat4& projection) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);
    
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    
    // Simple placement on ground plane for now
    float x = (mouseX / width - 0.5f) * 20.0f;  // Scale to world coordinates
    float z = (mouseY / height - 0.5f) * 20.0f;
    
    // Snap to grid
    const float gridSize = 0.8f;
    previewPosition_.x = std::round(x / gridSize) * gridSize;
    previewPosition_.y = 0.0f;
    previewPosition_.z = std::round(z / gridSize) * gridSize;
}

void InputHandler::cycleBrickType() {
    int currentType = static_cast<int>(selectedBrickType_);
    currentType = (currentType + 1) % 6; // We have 6 brick types
    selectedBrickType_ = static_cast<BrickType>(currentType);
}

void InputHandler::cycleBrickColor() {
    int currentColor = static_cast<int>(selectedBrickColor_);
    currentColor = (currentColor + 1) % 7; // We have 7 colors
    selectedBrickColor_ = static_cast<BrickColor>(currentColor);
}

} // namespace LegoEngine