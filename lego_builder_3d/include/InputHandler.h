#pragma once

#include "Math.h"
#include "LegoSystem.h"
#include <GLFW/glfw3.h>

namespace LegoEngine {

class InputHandler {
public:
    InputHandler();
    ~InputHandler();
    
    void processKeyboard(GLFWwindow* window, int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseButton(GLFWwindow* window, int button, int action);
    void processKey(int key, int action);
    void processScroll(float yoffset);
    
    // Ray casting for 3D picking
    Vec3 screenToWorldRay(GLFWwindow* window, const Mat4& view, const Mat4& projection);
    bool rayIntersectPlane(const Vec3& rayOrigin, const Vec3& rayDirection, 
                          const Vec3& planePoint, const Vec3& planeNormal, Vec3& intersection);
    
    BrickType getSelectedBrickType() const { return selectedBrickType_; }
    BrickColor getSelectedBrickColor() const { return selectedBrickColor_; }
    
    bool isPlacementMode() const { return placementMode_; }
    Vec3 getPreviewPosition() const { return previewPosition_; }
    
    void updatePreviewPosition(GLFWwindow* window, const Mat4& view, const Mat4& projection);

private:
    BrickType selectedBrickType_;
    BrickColor selectedBrickColor_;
    bool placementMode_;
    Vec3 previewPosition_;
    bool firstMouse_;
    float lastX_, lastY_;
    
    void cycleBrickType();
    void cycleBrickColor();
};

} // namespace LegoEngine