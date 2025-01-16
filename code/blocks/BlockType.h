/**
 * A type of block used by the blockmanager
 * A type of block is defined by a string and a texture
 */
#pragma once
#include <string>
#include <map>
#include <gf/Vector.h>

#include "BlockManager.h"

namespace platformer {
    class BlockType {
    public:
        // Simple constructor for non-collidable blocks
        BlockType(std::string type, std::string subType, std::string texturePath);
        // Constructor for normal blocks
        BlockType(std::string type, std::string subType, std::string texturePath, float staticFriction, float dynamicFriction, float restitution, gf::Vector2f hitboxSize, gf::Vector2f hitboxOffset);
        // Constructor for directionnal blocks
        BlockType(std::string type, std::string subType, std::string texturePath, float staticFriction, float dynamicFriction, float restitution, gf::Vector2f hitboxSize, gf::Vector2f hitboxOffset, std::string direction);

        // Textures and type names
        const std::string type;
        const std::string subType;
        const std::string texturePath;

        // Set to true if the collision should be resolved or not
        const bool isCollidable;

        // Constants to resolve collisions. Can be ignored if isCollidable is set to false.
        const float staticFriction = 0.0f;
        const float dynamicFriction = 0.0f;
        const float restitution = 0.0f;
        const gf::Vector2f hitboxSize = BlockManager::toWorldSpace(gf::Vector2i(1, 1));
        const gf::Vector2f hitboxOffset = BlockManager::toWorldSpace(gf::Vector2i(0, 0));

        // Constants for directionnal blocks
        const bool isDirectionnal = false;
        const gf::Vector2i direction{0,0};

    private:
        std::map<std::string,gf::Vector2i> directionMap{
            {"up",gf::Vector2i{0,-1}},
            {"down",gf::Vector2i{0,1}},
            {"left",gf::Vector2i{-1,0}},
            {"right",gf::Vector2i{1,0}}
        };
    };
}
