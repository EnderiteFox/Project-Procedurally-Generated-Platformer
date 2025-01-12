#include <blocks/BlockType.h>

#include <utility>

namespace platformer {

    BlockType::BlockType(std::string type, std::string subType, std::string texturePath):
        type(std::move(type)),
        subType(std::move(subType)),
        texturePath(std::move(texturePath)),
        isCollidable(false) {}

    BlockType::BlockType(std::string type, std::string subType ,std::string texturePath, float staticFriction, float dynamicFriction, float restitution):
        type(std::move(type)),
        subType(std::move(subType)),
        texturePath(std::move(texturePath)),
        staticFriction(staticFriction),
        dynamicFriction(dynamicFriction),
        restitution(restitution),
        isCollidable(true) {}
}
