/**
 * The BlockManager class acts as a container for blocks
 * It's made of a map containing the position and the subtype of each block
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

namespace platformer {

    class BlockType;

    struct internalBlockData{
        std::string blockType;
        uint8_t offset;
    };

    class BlockManager final : public gf::Entity {
        std::map<std::string, gf::Texture> textureMap;             // Associate a block's subtype to it's texture
        std::map<std::pair<int, int>, internalBlockData> blockMap; // Associate a position to the subtype of a block

        gf::Vector2f ViewPosition;
        const gf::Vector2f ViewSize;

    public:
        explicit BlockManager(gf::Vector2f ViewSize);

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
        const gf::Texture& getBlockTextureByName(const std::string& name) const;

        // Updating stored view position
        void setViewPosition(gf::Vector2f ViewPosition);

        // Loads all available blocks textures
        void loadTextures();

        // Updates the texture offset based on the other blocks stored in the blockmap
        void updateTextureOffset();

        // GF render function. Draws the texture corresponding to all the blocks on the map
        // Only the blocks within the current view will be rendered
        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        /**
         * Gets the hitboxes near the given position. All hitboxes outside of the size parameter will be ignored.
         * @param position The world position to get the nearby hitboxes of
         * @param size The size of the selection
         * @return A vector corresponding to the hiboxes of the blocks, made of pairs of :
         *           - a rectangle corresponding to the block's hitbox
         *           - The subtype of the block, defined by a string
         */
        std::vector<std::pair<gf::RectF, std::string>> getNearbyHitboxes(gf::Vector2f position, gf::Vector2f size) const;

        // Converts coordinates data to data usable by the block manager
        static gf::Vector2f toWorldSpace(gf::Vector2i vector);
        static float toWorldSpace(int number);
        static gf::Vector2i toBlockSpace(gf::Vector2f vector);
        static int toBlockSpace(float number);
    };
}
