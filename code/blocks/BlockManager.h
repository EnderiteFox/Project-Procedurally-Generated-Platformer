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
#include <gf/View.h>

#include "BlockType.h"

namespace platformer {
    class BlockManager final : public gf::Entity {
        std::map<std::string, gf::Texture> textureMap;
        std::map<std::pair<int, int>, std::string> blockMap; // Associate a position to a block, described by a string

        const gf::View* view;

    public:
        explicit BlockManager(const gf::View* view);

        // Size of a block sprite (and hitbox)
        static const float BLOCK_SIZE;

        const int COLLISION_CHECK_MARGIN = 1;

        // Getters and setter
        std::string getBlockTypeAt(int x, int y) const;
        std::string getBlockTypeAt(gf::Vector2i pos) const;
        void setBlockTypeAt(int x, int y, const BlockType& blockType);
        void setBlockTypeAt(gf::Vector2i pos, const BlockType& blockType);
        void removeBlockAt(int x, int y);
        void removeBlockAt(gf::Vector2i pos);
        bool isEmptyBlock(int x, int y) const;
        bool isEmptyBlock(gf::Vector2i pos) const;

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

        static gf::Vector2f toWorldSpace(gf::Vector2i vector);
        static float toWorldSpace(int number);
        static gf::Vector2i toBlockSpace(gf::Vector2f vector);
        static int toBlockSpace(float number);
    };
}
