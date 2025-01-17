/**
 * A type of tile used by the blockmanager
 * It is a data structure meant to store multiple informations about a tile
 * It stores the following data :
 *  - Type : The main type of the tile. tiles of the same type are intented to have a similar behaviour
 *  - subType : The name of the tile
 *  - texturePath : the path to the texture supposed to represent the tile
 *
 * isCollidable : Define if the collision with a tile should be resolved or not
 *                if the tile is not collidable, the collision will still be detected, but won't be resolved and won't affect the player
 *                if the tile is not collidable, the following attributes can be ignored.
 *  - staticFriction/dynamicFriction : Friction coefficients used by the tile.
 *  - restitution : The restitution coefficient of the tile. The player will bounce on the tile proportionally to this coefficient
 *  - hitboxSize/hitboxOffset : Define data about the hitbox of the tile
 *
 * isDirectionnal : Define if the tile is directionnal or not
 *                  A collision with a directionnal tile will only be resolved if the character colliding with it is coming from a specific direction.
 *                  Like with the collidable blocks, the collision will be detected whatsoever
 *                  A directionnal block is always collidable
 *  - direction : The direction of the block (see above for the meaning)
 *                Within the XML file, the direction is given as a string. See the directionMap at the bottom for the meaning of each string.
 */
#pragma once
#include <string>
#include <map>
#include <gf/Vector.h>

#include "BlockManager.h"

namespace platformer {
    class BlockType {
    public:
        // Default contructor, creating an empty block
        BlockType();
        // Simple constructor for non-collidable blocks
        BlockType(std::string type, std::string subType, std::string texturePath, gf::Vector2f hitboxSize, gf::Vector2f hitboxOffset);
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
        // Map indicating the direction of a directionnal block depending of a string used within the XML file
        std::map<std::string,gf::Vector2i> directionMap{
            {"up",gf::Vector2i{0,-1}},
            {"down",gf::Vector2i{0,1}},
            {"left",gf::Vector2i{-1,0}},
            {"right",gf::Vector2i{1,0}}
        };
    };
}
