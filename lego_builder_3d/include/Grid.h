#pragma once

#include "Renderer.h"
#include "Math.h"
#include <memory>

namespace LegoEngine {

class Grid {
public:
    Grid(float size = 20.0f, int divisions = 25);
    ~Grid();
    
    void render(const Shader& shader, const Mat4& view, const Mat4& projection);

private:
    std::unique_ptr<Mesh> gridMesh_;
    float size_;
    int divisions_;
    
    void generateGridMesh();
};

} // namespace LegoEngine