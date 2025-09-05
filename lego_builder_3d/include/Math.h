#pragma once

#include <array>
#include <cmath>

namespace LegoEngine {

// Simple 3D vector class
struct Vec3 {
    float x, y, z;
    
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }
    
    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }
    
    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }
    
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vec3 cross(const Vec3& other) const {
        return Vec3(y * other.z - z * other.y,
                   z * other.x - x * other.z,
                   x * other.y - y * other.x);
    }
    
    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    Vec3 normalize() const {
        float len = length();
        if (len > 0) return *this * (1.0f / len);
        return *this;
    }
};

// Simple 4x4 matrix class
struct Mat4 {
    std::array<std::array<float, 4>, 4> data;
    
    Mat4() {
        // Initialize as identity matrix
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                data[i][j] = (i == j) ? 1.0f : 0.0f;
            }
        }
    }
    
    static Mat4 perspective(float fov, float aspect, float near, float far);
    static Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up);
    static Mat4 translate(const Vec3& translation);
    static Mat4 rotate(float angle, const Vec3& axis);
    static Mat4 scale(const Vec3& scale);
    
    Mat4 operator*(const Mat4& other) const;
    
    const float* ptr() const { return &data[0][0]; }
};

} // namespace LegoEngine