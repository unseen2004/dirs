#pragma once

#include "Math.h"

namespace LegoEngine {

class Camera {
public:
    Camera(const Vec3& position = Vec3(0, 5, 10), 
           const Vec3& target = Vec3(0, 0, 0),
           const Vec3& up = Vec3(0, 1, 0));
    
    Mat4 getViewMatrix() const;
    Mat4 getProjectionMatrix(float aspect, float fov = 45.0f, float near = 0.1f, float far = 100.0f) const;
    
    void processKeyboard(int direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void processMouseScroll(float yoffset);
    
    Vec3 getPosition() const { return position_; }
    Vec3 getFront() const { return front_; }
    
    void setPosition(const Vec3& position) { position_ = position; }
    
private:
    Vec3 position_;
    Vec3 front_;
    Vec3 up_;
    Vec3 right_;
    Vec3 worldUp_;
    
    float yaw_;
    float pitch_;
    float movementSpeed_;
    float mouseSensitivity_;
    float zoom_;
    
    void updateCameraVectors();
};

} // namespace LegoEngine