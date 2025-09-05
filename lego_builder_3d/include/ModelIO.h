#pragma once

#include "LegoSystem.h"
#include <string>

namespace LegoEngine {

class ModelIO {
public:
    static bool saveModel(const std::vector<BrickInstance>& bricks, const std::string& filename);
    static bool loadModel(std::vector<BrickInstance>& bricks, const std::string& filename);
    
private:
    static std::string brickTypeToString(BrickType type);
    static BrickType stringToBrickType(const std::string& str);
    static std::string brickColorToString(BrickColor color);
    static BrickColor stringToBrickColor(const std::string& str);
};

} // namespace LegoEngine