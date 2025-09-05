#include "Grid.h"

namespace LegoEngine {

Grid::Grid(float size, int divisions) : size_(size), divisions_(divisions) {
    generateGridMesh();
}

Grid::~Grid() = default;

void Grid::generateGridMesh() {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float step = size_ / divisions_;
    float halfSize = size_ * 0.5f;
    
    // Generate grid lines along X axis
    for (int i = 0; i <= divisions_; ++i) {
        float z = -halfSize + i * step;
        vertices.emplace_back(Vec3(-halfSize, 0, z), Vec3(0, 1, 0));
        vertices.emplace_back(Vec3(halfSize, 0, z), Vec3(0, 1, 0));
    }
    
    // Generate grid lines along Z axis
    for (int i = 0; i <= divisions_; ++i) {
        float x = -halfSize + i * step;
        vertices.emplace_back(Vec3(x, 0, -halfSize), Vec3(0, 1, 0));
        vertices.emplace_back(Vec3(x, 0, halfSize), Vec3(0, 1, 0));
    }
    
    // Generate indices for lines
    for (size_t i = 0; i < vertices.size(); ++i) {
        indices.push_back(i);
    }
    
    gridMesh_ = std::make_unique<Mesh>(vertices, indices);
}

void Grid::render(const Shader& shader, const Mat4& view, const Mat4& projection) {
    Mat4 model; // Identity matrix (grid is at origin)
    
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setVec3("objectColor", Vec3(0.3f, 0.3f, 0.3f)); // Dark gray grid
    
    // Enable line mode for grid rendering
    // Note: This would require additional OpenGL calls in a full implementation
    if (gridMesh_) {
        gridMesh_->render();
    }
}

} // namespace LegoEngine