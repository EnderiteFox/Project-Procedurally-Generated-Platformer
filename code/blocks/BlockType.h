/**
 * A type of block used by the blockmanager
 * A type of block is defined by a string and a texture
 */
#pragma once
#include <string>

namespace platformer {
    class BlockType {
    public:
        // Constructor
        BlockType(std::string  type, std::string subType,std::string  texturePath); // Simple constructor for non-collidable blocks
        BlockType(std::string  type, std::string subType,std::string  texturePath, float staticFriction, float dynamicFriction, float restitution);

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
    };
}
