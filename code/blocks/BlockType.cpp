#include <blocks/BlockType.h>

#include <utility>

namespace platformer {
    BlockType::BlockType(std::string  typeName, std::string  texturePath):
    typeName(std::move(typeName)),
    texturePath(std::move(texturePath)) {}

    std::string BlockType::getTypeName() const {
        return typeName;
    }

    std::string BlockType::getTexturePath() const {
        return texturePath;
    }
}
