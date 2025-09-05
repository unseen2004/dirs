#pragma once

#include "Math.h"
#include "Renderer.h"
#include <memory>
#include <vector>

namespace LegoEngine {

enum class BrickType {
    BRICK_1x1,
    BRICK_1x2,
    BRICK_2x2,
    BRICK_2x4,
    BRICK_2x6,
    BRICK_2x8
};

enum class BrickColor {
    RED,
    BLUE,
    GREEN,
    YELLOW,
    WHITE,
    BLACK,
    GRAY
};

struct BrickInstance {
    Vec3 position;
    Vec3 rotation;
    BrickType type;
    BrickColor color;
    
    BrickInstance(const Vec3& pos, BrickType t, BrickColor c = BrickColor::RED)
        : position(pos), rotation(0, 0, 0), type(t), color(c) {}
};

class Brick {
public:
    Brick(BrickType type);
    ~Brick();
    
    void render(const Shader& shader, const Mat4& model, const Mat4& view, const Mat4& projection) const;
    
    BrickType getType() const { return type_; }
    Vec3 getSize() const { return size_; }
    
    // Check if this brick can connect to another at the given relative position
    bool canConnectTo(const Vec3& relativePosition) const;
    
    static std::unique_ptr<Mesh> createBrickMesh(BrickType type);
    
private:
    BrickType type_;
    Vec3 size_;
    std::unique_ptr<Mesh> mesh_;
    
    void calculateSize();
};

class BrickSystem {
public:
    BrickSystem();
    ~BrickSystem();
    
    void addBrick(const BrickInstance& brick);
    void removeBrick(size_t index);
    void clear();
    
    // Undo/Redo functionality
    void undo();
    void redo();
    bool canUndo() const;
    bool canRedo() const;
    
    // Save/Load functionality
    bool saveModel(const std::string& filename) const;
    bool loadModel(const std::string& filename);
    
    void render(const Shader& shader, const Mat4& view, const Mat4& projection) const;
    
    // Grid-based placement system
    Vec3 snapToGrid(const Vec3& position) const;
    bool isValidPlacement(const BrickInstance& brick) const;
    
    const std::vector<BrickInstance>& getBricks() const { return bricks_; }
    
private:
    std::vector<BrickInstance> bricks_;
    std::vector<std::unique_ptr<Brick>> brickPrototypes_;
    float gridSize_;
    
    // Undo/Redo system
    std::vector<std::vector<BrickInstance>> history_;
    size_t historyIndex_;
    static const size_t MAX_HISTORY = 50;
    
    void initializeBrickPrototypes();
    bool checkCollision(const BrickInstance& newBrick) const;
    void saveToHistory();
};

Vec3 getBrickColorRGB(BrickColor color);

} // namespace LegoEngine