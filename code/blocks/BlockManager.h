/**
 * The BlockManager class acts as a container for blocks
 * It's made of a map containing the position and the data of each block
 * It contains multiple methods to place or access blocks with their positions
 *
 * The method "getNearbyHitboxes" returns the hitboxes of all the nearby blocks near a position
 * This method is made to avoid checking collisions with all the existing blocks
 */

#pragma once
#include <map>
#include <string>
#include <gf/Entity.h>
#include <gf/Texture.h>
#include <vector>
#include <gf/Rect.h>

#include "BlockType.h"

namespace platformer {
    class BlockManager final : public gf::Entity {
        std::map<std::string, gf::Texture> textureMap;
        std::map<std::pair<int, int>, std::string> blockMap; // Associate a position to a block, described by a string

    public:
        const std::string EMPTY_BLOCK = "empty";
        // Size of a block sprite (and hitbox)
        const float BLOCK_SIZE = 8.0;

        const int COLLISION_CHECK_MARGIN = 1;

        // Getters and setter
        std::string getBlockTypeAt(int x, int y) const;
        void setBlockTypeAt(int x, int y, const BlockType& blockType);
        void removeBlockAt(int x, int y);
        bool isEmptyBlock(int x, int y) const;

        // Loads all available blocks textures
        void loadTextures();

        // GF render function. Shows the texture corresponding to all the blocks on the map
        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        // Get and return rectangles symbolising the hitboxes of the blocks located nearby the position passed in parameter
        /**
         * Gets the hitboxes near the given position. All hitboxes outside of the size parameter will be ignored.
         * @param position The world position to get the nearby hitboxes of
         * @param size The size of the selection
         * @return A vector of rectangles
         */
        std::vector<std::pair<gf::RectF,std::string>> getNearbyHitboxes(gf::Vector2f position, gf::Vector2f size) const;

        gf::Vector2f toWorldSpace(gf::Vector2i vector) const;
        float toWorldSpace(int number) const;
        gf::Vector2i toBlockSpace(gf::Vector2f vector) const;
        int toBlockSpace(float number) const;
    };
}
