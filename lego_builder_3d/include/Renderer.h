#pragma once

#include "Math.h"
#include <memory>
#include <vector>

namespace LegoEngine {

struct Vertex {
    Vec3 position;
    Vec3 normal;
    float u, v; // texture coordinates
    
    Vertex(const Vec3& pos, const Vec3& norm, float u = 0, float v = 0)
        : position(pos), normal(norm), u(u), v(v) {}
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();
    
    void render() const;
    
private:
    unsigned int VAO_, VBO_, EBO_;
    size_t indexCount_;
    
    void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
};

class Shader {
public:
    Shader(const std::string& vertexSource, const std::string& fragmentSource);
    ~Shader();
    
    void use() const;
    void setMat4(const char* name, const Mat4& matrix) const;
    void setVec3(const char* name, const Vec3& vector) const;
    void setFloat(const char* name, float value) const;
    void setInt(const char* name, int value) const;
    
private:
    unsigned int program_;
    
    unsigned int compileShader(const std::string& source, unsigned int type);
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

class Renderer {
public:
    Renderer();
    ~Renderer();
    
    void clear(float r = 0.2f, float g = 0.3f, float b = 0.3f, float a = 1.0f);
    void setViewport(int x, int y, int width, int height);
    void renderMesh(const Mesh& mesh, const Shader& shader, const Mat4& model, const Mat4& view, const Mat4& projection);
    
private:
    void enableDepthTesting();
};

} // namespace LegoEngine