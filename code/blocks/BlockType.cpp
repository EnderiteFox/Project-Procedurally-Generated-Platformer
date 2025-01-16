#include <blocks/BlockType.h>

#include <utility>

namespace platformer {

    BlockType::BlockType(std::string type, std::string subType, std::string texturePath):
        type(std::move(type)),
        subType(std::move(subType)),
        texturePath(std::move(texturePath)),
        isCollidable(false) {}

    BlockType::BlockType(std::string type, std::string subType ,std::string texturePath, float staticFriction, float dynamicFriction, float restitution, gf::Vector2f hitboxSize, gf::Vector2f hitboxOffset):
        type(std::move(type)),
        subType(std::move(subType)),
        texturePath(std::move(texturePath)),
        isCollidable(true),
        staticFriction(staticFriction),
        dynamicFriction(dynamicFriction),
        restitution(restitution),
        hitboxSize(hitboxSize),
        hitboxOffset(hitboxOffset) {}

    BlockType::BlockType(std::string type, std::string subType ,std::string texturePath, float staticFriction, float dynamicFriction, float restitution, gf::Vector2f hitboxSize, gf::Vector2f hitboxOffset, std::string direction):
        type(std::move(type)),
        subType(std::move(subType)),
        texturePath(std::move(texturePath)),
        isCollidable(true),
        staticFriction(staticFriction),
        dynamicFriction(dynamicFriction),
        restitution(restitution),
        isDirectionnal(true),
        hitboxSize(hitboxSize),
        hitboxOffset(hitboxOffset),
        direction(directionMap[direction]) {}
}
