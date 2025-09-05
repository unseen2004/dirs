#include "ModelIO.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace LegoEngine {

bool ModelIO::saveModel(const std::vector<BrickInstance>& bricks, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return false;
    }
    
    file << "# LEGO Builder 3D Model File\n";
    file << "# Format: x y z type color\n";
    file << "version 1.0\n\n";
    
    for (const auto& brick : bricks) {
        file << brick.position.x << " " 
             << brick.position.y << " " 
             << brick.position.z << " "
             << brickTypeToString(brick.type) << " "
             << brickColorToString(brick.color) << "\n";
    }
    
    file.close();
    std::cout << "Model saved: " << filename << " (" << bricks.size() << " bricks)\n";
    return true;
}

bool ModelIO::loadModel(std::vector<BrickInstance>& bricks, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << filename << std::endl;
        return false;
    }
    
    bricks.clear();
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        // Skip comments and empty lines
        if (line.empty() || line[0] == '#' || line.find("version") == 0) {
            continue;
        }
        
        std::istringstream iss(line);
        float x, y, z;
        std::string typeStr, colorStr;
        
        if (!(iss >> x >> y >> z >> typeStr >> colorStr)) {
            std::cerr << "Error parsing line " << lineNumber << ": " << line << std::endl;
            continue;
        }
        
        BrickType type = stringToBrickType(typeStr);
        BrickColor color = stringToBrickColor(colorStr);
        
        bricks.emplace_back(Vec3(x, y, z), type, color);
    }
    
    file.close();
    std::cout << "Model loaded: " << filename << " (" << bricks.size() << " bricks)\n";
    return true;
}

std::string ModelIO::brickTypeToString(BrickType type) {
    switch (type) {
        case BrickType::BRICK_1x1: return "1x1";
        case BrickType::BRICK_1x2: return "1x2";
        case BrickType::BRICK_2x2: return "2x2";
        case BrickType::BRICK_2x4: return "2x4";
        case BrickType::BRICK_2x6: return "2x6";
        case BrickType::BRICK_2x8: return "2x8";
        default: return "unknown";
    }
}

BrickType ModelIO::stringToBrickType(const std::string& str) {
    if (str == "1x1") return BrickType::BRICK_1x1;
    if (str == "1x2") return BrickType::BRICK_1x2;
    if (str == "2x2") return BrickType::BRICK_2x2;
    if (str == "2x4") return BrickType::BRICK_2x4;
    if (str == "2x6") return BrickType::BRICK_2x6;
    if (str == "2x8") return BrickType::BRICK_2x8;
    return BrickType::BRICK_2x4; // Default
}

std::string ModelIO::brickColorToString(BrickColor color) {
    switch (color) {
        case BrickColor::RED: return "red";
        case BrickColor::BLUE: return "blue";
        case BrickColor::GREEN: return "green";
        case BrickColor::YELLOW: return "yellow";
        case BrickColor::WHITE: return "white";
        case BrickColor::BLACK: return "black";
        case BrickColor::GRAY: return "gray";
        default: return "red";
    }
}

BrickColor ModelIO::stringToBrickColor(const std::string& str) {
    if (str == "red") return BrickColor::RED;
    if (str == "blue") return BrickColor::BLUE;
    if (str == "green") return BrickColor::GREEN;
    if (str == "yellow") return BrickColor::YELLOW;
    if (str == "white") return BrickColor::WHITE;
    if (str == "black") return BrickColor::BLACK;
    if (str == "gray") return BrickColor::GRAY;
    return BrickColor::RED; // Default
}

} // namespace LegoEngine