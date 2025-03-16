#include <blocks/BlockType.h>

#include <utility>

namespace platformer {

    BlockType::BlockType()
    : type("empty")
    , subType("empty")
    , texturePath("")
    , isCollidable(false)
    {}

    BlockType::BlockType(
        std::string type,
        std::string subType,
        std::string texturePath,
        gf::Vector2f hitboxSize,
        gf::Vector2f hitboxOffset,
        float scale,
        bool isConnected
    )
    : type(std::move(type))
    , subType(std::move(subType))
    , texturePath(std::move(texturePath))
    , scale(scale)
    , isCollidable(false)
    , hitboxSize(hitboxSize)
    , hitboxOffset(hitboxOffset)
    , isConnected(isConnected)
    {}

    BlockType::BlockType(
        std::string type,
        std::string subType,
        std::string texturePath,
        float staticFriction,
        float dynamicFriction,
        float restitution,
        gf::Vector2f hitboxSize,
        gf::Vector2f hitboxOffset,
        float scale,
        bool isConnected
    )
    : type(std::move(type))
    , subType(std::move(subType))
    , texturePath(std::move(texturePath))
    , scale(scale)
    , isCollidable(true)
    , staticFriction(staticFriction)
    , dynamicFriction(dynamicFriction)
    , restitution(restitution)
    , hitboxSize(hitboxSize)
    , hitboxOffset(hitboxOffset)
    , isConnected(isConnected)
    {}

    BlockType::BlockType(
        std::string type,
        std::string subType,
        std::string texturePath,
        float staticFriction,
        float dynamicFriction,
        float restitution,
        gf::Vector2f hitboxSize,
        gf::Vector2f hitboxOffset,
        std::string direction,
        float scale,
        bool isConnected
    )
    : type(std::move(type))
    , subType(std::move(subType))
    , texturePath(std::move(texturePath))
    , scale(scale)
    , isCollidable(true)
    , staticFriction(staticFriction)
    , dynamicFriction(dynamicFriction)
    , restitution(restitution)
    , hitboxSize(hitboxSize)
    , hitboxOffset(hitboxOffset)
    , isDirectional(true)
    , direction(directionMap[direction])
    , isConnected(isConnected)
    {}
}
