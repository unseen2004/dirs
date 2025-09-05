#include "Math.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace LegoEngine {

Mat4 Mat4::perspective(float fov, float aspect, float near, float far) {
    Mat4 result;
    float tanHalfFov = std::tan(fov * 0.5f * M_PI / 180.0f);
    
    result.data[0][0] = 1.0f / (aspect * tanHalfFov);
    result.data[1][1] = 1.0f / tanHalfFov;
    result.data[2][2] = -(far + near) / (far - near);
    result.data[2][3] = -1.0f;
    result.data[3][2] = -(2.0f * far * near) / (far - near);
    result.data[3][3] = 0.0f;
    
    return result;
}

Mat4 Mat4::lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    Vec3 f = (center - eye).normalize();
    Vec3 s = f.cross(up).normalize();
    Vec3 u = s.cross(f);
    
    Mat4 result;
    result.data[0][0] = s.x;
    result.data[1][0] = s.y;
    result.data[2][0] = s.z;
    result.data[0][1] = u.x;
    result.data[1][1] = u.y;
    result.data[2][1] = u.z;
    result.data[0][2] = -f.x;
    result.data[1][2] = -f.y;
    result.data[2][2] = -f.z;
    result.data[3][0] = -s.dot(eye);
    result.data[3][1] = -u.dot(eye);
    result.data[3][2] = f.dot(eye);
    
    return result;
}

Mat4 Mat4::translate(const Vec3& translation) {
    Mat4 result;
    result.data[3][0] = translation.x;
    result.data[3][1] = translation.y;
    result.data[3][2] = translation.z;
    return result;
}

Mat4 Mat4::rotate(float angle, const Vec3& axis) {
    Mat4 result;
    float c = std::cos(angle);
    float s = std::sin(angle);
    float omc = 1.0f - c;
    
    Vec3 a = axis.normalize();
    
    result.data[0][0] = c + a.x * a.x * omc;
    result.data[0][1] = a.x * a.y * omc + a.z * s;
    result.data[0][2] = a.x * a.z * omc - a.y * s;
    result.data[1][0] = a.y * a.x * omc - a.z * s;
    result.data[1][1] = c + a.y * a.y * omc;
    result.data[1][2] = a.y * a.z * omc + a.x * s;
    result.data[2][0] = a.z * a.x * omc + a.y * s;
    result.data[2][1] = a.z * a.y * omc - a.x * s;
    result.data[2][2] = c + a.z * a.z * omc;
    
    return result;
}

Mat4 Mat4::scale(const Vec3& scale) {
    Mat4 result;
    result.data[0][0] = scale.x;
    result.data[1][1] = scale.y;
    result.data[2][2] = scale.z;
    return result;
}

Mat4 Mat4::operator*(const Mat4& other) const {
    Mat4 result;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = 0;
            for (int k = 0; k < 4; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }
    return result;
}

} // namespace LegoEngine