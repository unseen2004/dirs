#include "LegoSystem.h"
#include <algorithm>
#include <cmath>

namespace LegoEngine {

// Brick implementation
Brick::Brick(BrickType type) : type_(type) {
    calculateSize();
    mesh_ = createBrickMesh(type);
}

Brick::~Brick() = default;

void Brick::calculateSize() {
    switch (type_) {
        case BrickType::BRICK_1x1: size_ = Vec3(1, 1, 1); break;
        case BrickType::BRICK_1x2: size_ = Vec3(1, 1, 2); break;
        case BrickType::BRICK_2x2: size_ = Vec3(2, 1, 2); break;
        case BrickType::BRICK_2x4: size_ = Vec3(2, 1, 4); break;
        case BrickType::BRICK_2x6: size_ = Vec3(2, 1, 6); break;
        case BrickType::BRICK_2x8: size_ = Vec3(2, 1, 8); break;
    }
}

void Brick::render(const Shader& shader, const Mat4& model, const Mat4& view, const Mat4& projection) const {
    if (mesh_) {
        shader.use();
        shader.setMat4("model", model);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        mesh_->render();
    }
}

bool Brick::canConnectTo(const Vec3& relativePosition) const {
    // Simple connection logic - bricks can connect if they're adjacent
    float distance = relativePosition.length();
    return distance > 0.5f && distance < 2.0f;
}

std::unique_ptr<Mesh> Brick::createBrickMesh(BrickType type) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Get brick dimensions
    Vec3 size;
    switch (type) {
        case BrickType::BRICK_1x1: size = Vec3(0.8f, 0.96f, 0.8f); break;
        case BrickType::BRICK_1x2: size = Vec3(0.8f, 0.96f, 1.6f); break;
        case BrickType::BRICK_2x2: size = Vec3(1.6f, 0.96f, 1.6f); break;
        case BrickType::BRICK_2x4: size = Vec3(1.6f, 0.96f, 3.2f); break;
        case BrickType::BRICK_2x6: size = Vec3(1.6f, 0.96f, 4.8f); break;
        case BrickType::BRICK_2x8: size = Vec3(1.6f, 0.96f, 6.4f); break;
    }
    
    float hx = size.x * 0.5f;
    float hy = size.y * 0.5f;
    float hz = size.z * 0.5f;
    
    // Create a simple box mesh
    // Front face
    vertices.emplace_back(Vec3(-hx, -hy,  hz), Vec3(0, 0, 1), 0, 0);
    vertices.emplace_back(Vec3( hx, -hy,  hz), Vec3(0, 0, 1), 1, 0);
    vertices.emplace_back(Vec3( hx,  hy,  hz), Vec3(0, 0, 1), 1, 1);
    vertices.emplace_back(Vec3(-hx,  hy,  hz), Vec3(0, 0, 1), 0, 1);
    
    // Back face
    vertices.emplace_back(Vec3(-hx, -hy, -hz), Vec3(0, 0, -1), 0, 0);
    vertices.emplace_back(Vec3(-hx,  hy, -hz), Vec3(0, 0, -1), 1, 0);
    vertices.emplace_back(Vec3( hx,  hy, -hz), Vec3(0, 0, -1), 1, 1);
    vertices.emplace_back(Vec3( hx, -hy, -hz), Vec3(0, 0, -1), 0, 1);
    
    // Left face
    vertices.emplace_back(Vec3(-hx,  hy,  hz), Vec3(-1, 0, 0), 1, 0);
    vertices.emplace_back(Vec3(-hx,  hy, -hz), Vec3(-1, 0, 0), 1, 1);
    vertices.emplace_back(Vec3(-hx, -hy, -hz), Vec3(-1, 0, 0), 0, 1);
    vertices.emplace_back(Vec3(-hx, -hy,  hz), Vec3(-1, 0, 0), 0, 0);
    
    // Right face
    vertices.emplace_back(Vec3( hx,  hy,  hz), Vec3(1, 0, 0), 1, 0);
    vertices.emplace_back(Vec3( hx, -hy,  hz), Vec3(1, 0, 0), 0, 0);
    vertices.emplace_back(Vec3( hx, -hy, -hz), Vec3(1, 0, 0), 0, 1);
    vertices.emplace_back(Vec3( hx,  hy, -hz), Vec3(1, 0, 0), 1, 1);
    
    // Bottom face
    vertices.emplace_back(Vec3(-hx, -hy, -hz), Vec3(0, -1, 0), 0, 1);
    vertices.emplace_back(Vec3( hx, -hy, -hz), Vec3(0, -1, 0), 1, 1);
    vertices.emplace_back(Vec3( hx, -hy,  hz), Vec3(0, -1, 0), 1, 0);
    vertices.emplace_back(Vec3(-hx, -hy,  hz), Vec3(0, -1, 0), 0, 0);
    
    // Top face
    vertices.emplace_back(Vec3(-hx,  hy, -hz), Vec3(0, 1, 0), 0, 1);
    vertices.emplace_back(Vec3(-hx,  hy,  hz), Vec3(0, 1, 0), 0, 0);
    vertices.emplace_back(Vec3( hx,  hy,  hz), Vec3(0, 1, 0), 1, 0);
    vertices.emplace_back(Vec3( hx,  hy, -hz), Vec3(0, 1, 0), 1, 1);
    
    // Add indices for all faces
    for (int i = 0; i < 6; ++i) {
        int base = i * 4;
        // First triangle
        indices.push_back(base + 0);
        indices.push_back(base + 1);
        indices.push_back(base + 2);
        // Second triangle
        indices.push_back(base + 2);
        indices.push_back(base + 3);
        indices.push_back(base + 0);
    }
    
    return std::make_unique<Mesh>(vertices, indices);
}

// BrickSystem implementation
BrickSystem::BrickSystem() : gridSize_(0.8f) {
    initializeBrickPrototypes();
}

BrickSystem::~BrickSystem() = default;

void BrickSystem::initializeBrickPrototypes() {
    brickPrototypes_.push_back(std::make_unique<Brick>(BrickType::BRICK_1x1));
    brickPrototypes_.push_back(std::make_unique<Brick>(BrickType::BRICK_1x2));
    brickPrototypes_.push_back(std::make_unique<Brick>(BrickType::BRICK_2x2));
    brickPrototypes_.push_back(std::make_unique<Brick>(BrickType::BRICK_2x4));
    brickPrototypes_.push_back(std::make_unique<Brick>(BrickType::BRICK_2x6));
    brickPrototypes_.push_back(std::make_unique<Brick>(BrickType::BRICK_2x8));
}

void BrickSystem::addBrick(const BrickInstance& brick) {
    if (isValidPlacement(brick)) {
        bricks_.push_back(brick);
    }
}

void BrickSystem::removeBrick(size_t index) {
    if (index < bricks_.size()) {
        bricks_.erase(bricks_.begin() + index);
    }
}

void BrickSystem::clear() {
    bricks_.clear();
}

void BrickSystem::render(const Shader& shader, const Mat4& view, const Mat4& projection) const {
    for (const auto& brickInstance : bricks_) {
        // Create model matrix for this brick
        Mat4 model = Mat4::translate(brickInstance.position);
        
        // Set brick color
        Vec3 color = getBrickColorRGB(brickInstance.color);
        shader.setVec3("objectColor", color);
        
        // Get the appropriate brick prototype
        size_t typeIndex = static_cast<size_t>(brickInstance.type);
        if (typeIndex < brickPrototypes_.size()) {
            brickPrototypes_[typeIndex]->render(shader, model, view, projection);
        }
    }
}

Vec3 BrickSystem::snapToGrid(const Vec3& position) const {
    return Vec3(
        std::round(position.x / gridSize_) * gridSize_,
        std::round(position.y / gridSize_) * gridSize_,
        std::round(position.z / gridSize_) * gridSize_
    );
}

bool BrickSystem::isValidPlacement(const BrickInstance& brick) const {
    return !checkCollision(brick);
}

bool BrickSystem::checkCollision(const BrickInstance& newBrick) const {
    for (const auto& existingBrick : bricks_) {
        // Simple bounding box collision check
        Vec3 distance = newBrick.position - existingBrick.position;
        if (std::abs(distance.x) < gridSize_ && 
            std::abs(distance.y) < gridSize_ && 
            std::abs(distance.z) < gridSize_) {
            return true; // Collision detected
        }
    }
    return false;
}

Vec3 getBrickColorRGB(BrickColor color) {
    switch (color) {
        case BrickColor::RED:    return Vec3(0.8f, 0.2f, 0.2f);
        case BrickColor::BLUE:   return Vec3(0.2f, 0.2f, 0.8f);
        case BrickColor::GREEN:  return Vec3(0.2f, 0.8f, 0.2f);
        case BrickColor::YELLOW: return Vec3(0.8f, 0.8f, 0.2f);
        case BrickColor::WHITE:  return Vec3(0.9f, 0.9f, 0.9f);
        case BrickColor::BLACK:  return Vec3(0.1f, 0.1f, 0.1f);
        case BrickColor::GRAY:   return Vec3(0.5f, 0.5f, 0.5f);
        default:                 return Vec3(0.8f, 0.2f, 0.2f);
    }
}

} // namespace LegoEngine