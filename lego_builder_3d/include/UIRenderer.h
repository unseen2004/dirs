#pragma once

#include "LegoSystem.h"
#include "Renderer.h"
#include <string>
#include <vector>

namespace LegoEngine {

struct UIElement {
    std::string text;
    Vec3 color;
    float x, y; // Screen coordinates (0-1)
    
    UIElement(const std::string& t, float x, float y, const Vec3& c = Vec3(1, 1, 1))
        : text(t), color(c), x(x), y(y) {}
};

class UIRenderer {
public:
    UIRenderer();
    ~UIRenderer();
    
    void render(int windowWidth, int windowHeight);
    void addElement(const UIElement& element);
    void clearElements();
    
    // Helper methods for common UI elements
    void showInstructions();
    void showBrickInfo(BrickType type, BrickColor color);
    void showStatus(const std::string& status);

private:
    std::vector<UIElement> elements_;
    
    void renderText(const std::string& text, float x, float y, const Vec3& color, 
                   int windowWidth, int windowHeight);
    std::string brickTypeToString(BrickType type);
    std::string brickColorToString(BrickColor color);
};

} // namespace LegoEngine