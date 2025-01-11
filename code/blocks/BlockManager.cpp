#include <blocks/BlockManager.h>
#include <blocks/BlockType.h>
#include <blocks/BlockTypes.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <vector>
#include <gf/Rect.h>

namespace platformer {
    std::string BlockManager::getBlockTypeAt(const int x, const int y) const {
        const auto found = blockMap.find(std::make_pair(x, y));
        if (found == blockMap.cend()) return EMPTY_BLOCK;
        return found->second;
    }

    void BlockManager::setBlockTypeAt(const int x, const int y, const BlockType& blockType) {
        blockMap.insert(std::make_pair(std::make_pair(x, y), blockType.getTypeName()));
    }

    void BlockManager::removeBlockAt(int x, int y) {
        blockMap.erase(std::make_pair(x, y));
    }

    bool BlockManager::isEmptyBlock(const int x, const int y) const {
        return getBlockTypeAt(x, y) == EMPTY_BLOCK;
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
            sprite.setPosition(gf::Vector2f(pos.first, pos.second) * BLOCK_SIZE);
            sprite.setTexture(found->second,gf::RectF::fromSize({1.0f,1.0f}));
            target.draw(sprite, states);
        }
    }

    int BlockManager::toBlockSpace(const float number) const {
        return static_cast<int>(number / BLOCK_SIZE);
    }


    gf::Vector2i BlockManager::toBlockSpace(const gf::Vector2f vector) const {
        return gf::Vector2i(toBlockSpace(vector.x), toBlockSpace(vector.y));
    }

    float BlockManager::toWorldSpace(const int number) const {
        return number * BLOCK_SIZE;
    }


    gf::Vector2f BlockManager::toWorldSpace(const gf::Vector2i vector) const {
        return gf::Vector2f(toWorldSpace(vector.x), toWorldSpace(vector.y));
    }



    std::vector<gf::RectF> BlockManager::getNearbyHitboxes(const gf::Vector2f position, const gf::Vector2f size) const {
        std::vector<gf::RectF> res;

        for (
            int x = toBlockSpace(position.x) - COLLISION_CHECK_MARGIN;
            x < toBlockSpace(position.x + size.x) + COLLISION_CHECK_MARGIN;
            ++x
        ) {
            for (
                int y = toBlockSpace(position.y) - COLLISION_CHECK_MARGIN;
                y < toBlockSpace(position.y + size.y) + COLLISION_CHECK_MARGIN;
                ++y
            ) {
                auto found = blockMap.find(std::make_pair(x, y));
                if (found == blockMap.end()) continue;
                if (found->second == EMPTY_BLOCK) continue;

                res.push_back(gf::RectF::fromPositionSize(
                    gf::Vector2f(toWorldSpace(x), toWorldSpace(y)),
                    gf::Vector2f(BLOCK_SIZE)
                ));
            }
        }

        return res;
    }
}
