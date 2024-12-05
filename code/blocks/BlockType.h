#pragma once
#include <string>

namespace platformer {
    class BlockType {
        const std::string typeName;
        const std::string texturePath;
    public:
        BlockType(std::string  typeName, std::string  texturePath);
        std::string getTypeName() const;
        std::string getTexturePath() const;
    };
}
