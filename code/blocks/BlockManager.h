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
        std::map<std::pair<int, int>, std::string> blockMap;

    public:
        const std::string EMPTY_BLOCK = "empty";
        const float BLOCK_SIZE = 8.0;

        std::string getBlockTypeAt(int x, int y) const;
        void setBlockTypeAt(int x, int y, const BlockType& blockType);

        void loadTextures();

        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        // Get and return rectangles symbolising the hitboxes of the blocks located nearby the position passed in parameter
        std::vector<gf::RectF> getNearbyHitboxes(const gf::Vector2f position) const;
    };
}
