#include "Camera.h"
#include <algorithm>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace LegoEngine {

Camera::Camera(const Vec3& position, const Vec3& target, const Vec3& up)
    : position_(position), worldUp_(up), yaw_(-90.0f), pitch_(0.0f),
      movementSpeed_(2.5f), mouseSensitivity_(0.1f), zoom_(45.0f) {
    
    // Calculate front vector from target
    front_ = (target - position_).normalize();
    updateCameraVectors();
}

Mat4 Camera::getViewMatrix() const {
    return Mat4::lookAt(position_, position_ + front_, up_);
}

Mat4 Camera::getProjectionMatrix(float aspect, float fov, float near, float far) const {
    return Mat4::perspective(zoom_, aspect, near, far);
}

void Camera::processKeyboard(int direction, float deltaTime) {
    float velocity = movementSpeed_ * deltaTime;
    
    if (direction & 1) // Forward (W)
        position_ = position_ + front_ * velocity;
    if (direction & 2) // Backward (S)
        position_ = position_ - front_ * velocity;
    if (direction & 4) // Left (A)
        position_ = position_ - right_ * velocity;
    if (direction & 8) // Right (D)
        position_ = position_ + right_ * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity_;
    yoffset *= mouseSensitivity_;
    
    yaw_ += xoffset;
    pitch_ += yoffset;
    
    if (constrainPitch) {
        pitch_ = std::clamp(pitch_, -89.0f, 89.0f);
    }
    
    updateCameraVectors();
}

void Camera::processMouseScroll(float yoffset) {
    zoom_ -= yoffset;
    zoom_ = std::clamp(zoom_, 1.0f, 45.0f);
}

void Camera::updateCameraVectors() {
    // Calculate the new front vector
    Vec3 front;
    front.x = std::cos(yaw_ * M_PI / 180.0f) * std::cos(pitch_ * M_PI / 180.0f);
    front.y = std::sin(pitch_ * M_PI / 180.0f);
    front.z = std::sin(yaw_ * M_PI / 180.0f) * std::cos(pitch_ * M_PI / 180.0f);
    front_ = front.normalize();
    
    // Re-calculate the right and up vector
    right_ = front_.cross(worldUp_).normalize();
    up_ = right_.cross(front_).normalize();
}

} // namespace LegoEngine