#include "Renderer.h"
#include "../external/glad/glad.h"
#include <GL/gl.h>
#include <iostream>
#include <sstream>

#ifndef GL_ELEMENT_ARRAY_BUFFER
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#endif

namespace LegoEngine {

// Mesh implementation
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) 
    : indexCount_(indices.size()) {
    setupMesh(vertices, indices);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteBuffers(1, &EBO_);
}

void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);
    
    glBindVertexArray(VAO_);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    
    // Position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    
    // Normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    
    // Texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
    
    glBindVertexArray(0);
}

void Mesh::render() const {
    glBindVertexArray(VAO_);
    glDrawElements(GL_TRIANGLES, indexCount_, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// Shader implementation
Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    unsigned int vertex = compileShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragment = compileShader(fragmentSource, GL_FRAGMENT_SHADER);
    
    program_ = glCreateProgram();
    glAttachShader(program_, vertex);
    glAttachShader(program_, fragment);
    glLinkProgram(program_);
    
    int success;
    char infoLog[512];
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program_, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {
    glDeleteProgram(program_);
}

void Shader::use() const {
    glUseProgram(program_);
}

void Shader::setMat4(const char* name, const Mat4& matrix) const {
    unsigned int location = glGetUniformLocation(program_, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, matrix.ptr());
}

void Shader::setVec3(const char* name, const Vec3& vector) const {
    unsigned int location = glGetUniformLocation(program_, name);
    glUniform3fv(location, 1, &vector.x);
}

void Shader::setFloat(const char* name, float value) const {
    unsigned int location = glGetUniformLocation(program_, name);
    glUniform1f(location, value);
}

void Shader::setInt(const char* name, int value) const {
    unsigned int location = glGetUniformLocation(program_, name);
    glUniform1i(location, value);
}

unsigned int Shader::compileShader(const std::string& source, unsigned int type) {
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    
    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

void Shader::checkCompileErrors(unsigned int shader, const std::string& type) {
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation error (" << type << "): " << infoLog << std::endl;
    }
}

// Renderer implementation
Renderer::Renderer() {
    enableDepthTesting();
}

Renderer::~Renderer() = default;

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::renderMesh(const Mesh& mesh, const Shader& shader, const Mat4& model, const Mat4& view, const Mat4& projection) {
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    
    mesh.render();
}

void Renderer::enableDepthTesting() {
    glEnable(GL_DEPTH_TEST);
}

} // namespace LegoEngine