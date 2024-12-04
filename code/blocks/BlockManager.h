#pragma once
#include <map>
#include <string>
#include <gf/Entity.h>
#include <gf/Texture.h>

namespace platformer {
    class BlockManager final : public gf::Entity {
        std::map<std::string, gf::Texture> textureMap;
        std::map<std::pair<int, int>, std::string> blockMap;

    public:
        const std::string EMPTY_BLOCK = "empty";
        const float BLOCK_SIZE = 8.0;

        std::string getBlockTypeAt(int x, int y) const;
        void setBlockTypeAt(int x, int y, std::string blockType);

        void loadTextures();

        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    };
}
