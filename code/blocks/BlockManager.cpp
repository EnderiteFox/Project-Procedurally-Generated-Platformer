#include <blocks/BlockManager.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

namespace platformer {
    std::string BlockManager::getBlockTypeAt(const int x, const int y) const {
        const auto found = blockMap.find(std::make_pair(x, y));
        if (found == blockMap.cend()) return EMPTY_BLOCK;
        return found->second;
    }

    void BlockManager::setBlockTypeAt(const int x, const int y, std::string blockType) {
        blockMap.insert(std::make_pair(std::make_pair(x, y), blockType));
    }

    void BlockManager::loadTextures() {
        textureMap.insert(std::make_pair("testBlock", gf::Texture("../assets/tile_placeholder.png")));
    }

    void BlockManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        for (const auto& [pos, blockType] : blockMap) {
            if (blockType == EMPTY_BLOCK) continue;
            auto found = textureMap.find(blockType);
            if (found == textureMap.cend()) continue;
            gf::Sprite sprite;
            sprite.setPosition(gf::Vector2f(pos.first, pos.second) * 8);
            sprite.setTexture(found->second);
            target.draw(sprite, states);
        }
    }
}
