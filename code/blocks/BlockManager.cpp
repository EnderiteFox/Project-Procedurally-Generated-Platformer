#include <blocks/BlockManager.h>
#include <blocks/BlockType.h>
#include <blocks/BlockTypes.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>

namespace platformer {
    std::string BlockManager::getBlockTypeAt(const int x, const int y) const {
        const auto found = blockMap.find(std::make_pair(x, y));
        if (found == blockMap.cend()) return EMPTY_BLOCK;
        return found->second;
    }

    void BlockManager::setBlockTypeAt(const int x, const int y, const BlockType& blockType) {
        blockMap.insert(std::make_pair(std::make_pair(x, y), blockType.getTypeName()));
    }

    void BlockManager::loadTextures() {
        for (const BlockType& blockType : BlockTypes::getAllTypes()) {
            textureMap.insert(std::make_pair(blockType.getTypeName(), gf::Texture(blockType.getTexturePath())));
        }
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