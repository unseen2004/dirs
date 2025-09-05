#include "UIRenderer.h"
#include <iostream>

namespace LegoEngine {

UIRenderer::UIRenderer() {
}

UIRenderer::~UIRenderer() = default;

void UIRenderer::render(int windowWidth, int windowHeight) {
    // For now, just output to console since we don't have text rendering
    // In a full implementation, this would render text overlays
    static int frameCount = 0;
    frameCount++;
    
    // Only output every 60 frames to avoid spam
    if (frameCount % 60 == 0) {
        std::cout << "\n=== LEGO Builder 3D ===\n";
        for (const auto& element : elements_) {
            std::cout << element.text << std::endl;
        }
        std::cout << "========================\n";
    }
}

void UIRenderer::addElement(const UIElement& element) {
    elements_.push_back(element);
}

void UIRenderer::clearElements() {
    elements_.clear();
}

void UIRenderer::showInstructions() {
    clearElements();
    addElement(UIElement("=== CONTROLS ===", 0.02f, 0.02f, Vec3(1, 1, 0)));
    addElement(UIElement("WASD: Move camera", 0.02f, 0.06f));
    addElement(UIElement("Mouse: Look around (right-click drag)", 0.02f, 0.10f));
    addElement(UIElement("Scroll: Zoom", 0.02f, 0.14f));
    addElement(UIElement("1-6: Select brick type", 0.02f, 0.18f));
    addElement(UIElement("C: Cycle brick color", 0.02f, 0.22f));
    addElement(UIElement("Tab: Cycle brick type", 0.02f, 0.26f));
    addElement(UIElement("Left Click: Place brick", 0.02f, 0.30f));
    addElement(UIElement("Space: Toggle placement mode", 0.02f, 0.34f));
    addElement(UIElement("Ctrl+Z: Undo", 0.02f, 0.38f));
    addElement(UIElement("Ctrl+Shift+Z: Redo", 0.02f, 0.42f));
    addElement(UIElement("ESC: Exit", 0.02f, 0.46f));
}

void UIRenderer::showBrickInfo(BrickType type, BrickColor color) {
    std::string info = "Selected: " + brickTypeToString(type) + " (" + brickColorToString(color) + ")";
    addElement(UIElement(info, 0.02f, 0.90f, Vec3(0, 1, 1)));
}

void UIRenderer::showStatus(const std::string& status) {
    addElement(UIElement("Status: " + status, 0.02f, 0.94f, Vec3(1, 1, 0)));
}

void UIRenderer::renderText(const std::string& text, float x, float y, const Vec3& color,
                           int windowWidth, int windowHeight) {
    // Placeholder for actual text rendering
    // In a full implementation, this would use OpenGL text rendering or a library like FreeType
}

std::string UIRenderer::brickTypeToString(BrickType type) {
    switch (type) {
        case BrickType::BRICK_1x1: return "1x1 Brick";
        case BrickType::BRICK_1x2: return "1x2 Brick";
        case BrickType::BRICK_2x2: return "2x2 Brick";
        case BrickType::BRICK_2x4: return "2x4 Brick";
        case BrickType::BRICK_2x6: return "2x6 Brick";
        case BrickType::BRICK_2x8: return "2x8 Brick";
        default: return "Unknown Brick";
    }
}

std::string UIRenderer::brickColorToString(BrickColor color) {
    switch (color) {
        case BrickColor::RED: return "Red";
        case BrickColor::BLUE: return "Blue";
        case BrickColor::GREEN: return "Green";
        case BrickColor::YELLOW: return "Yellow";
        case BrickColor::WHITE: return "White";
        case BrickColor::BLACK: return "Black";
        case BrickColor::GRAY: return "Gray";
        default: return "Unknown Color";
    }
}

} // namespace LegoEngine