#include <blocks/BlockManager.h>
#include <blocks/BlockType.h>
#include <blocks/BlockTypes.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <vector>
#include <gf/Rect.h>

namespace platformer {
    BlockManager::BlockManager(const gf::View* view): view(view) {}

    const float BlockManager::BLOCK_SIZE = 8.0f;

    std::string BlockManager::getBlockTypeAt(const int x, const int y) const {
        const auto found = blockMap.find(std::make_pair(x, y));
        if (found == blockMap.cend()) return BlockTypes::EMPTY_BLOCK;
        return found->second;
    }

    void BlockManager::setBlockTypeAt(const int x, const int y, const BlockType& blockType) {
        blockMap.insert(std::make_pair(std::make_pair(x, y), blockType.subType));
    }

    void BlockManager::removeBlockAt(int x, int y) {
        blockMap.erase(std::make_pair(x, y));
    }

    bool BlockManager::isEmptyBlock(const int x, const int y) const {
        return getBlockTypeAt(x, y) == BlockTypes::EMPTY_BLOCK;
    }



    void BlockManager::loadTextures() {
        for (const BlockType& blockType : BlockTypes::getAllTypes()) {
            textureMap.insert(std::make_pair(blockType.subType, gf::Texture(blockType.texturePath)));
        }
    }

    void BlockManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        const gf::Vector2f center = view->getCenter();
        const gf::Vector2f size = view->getSize();

        for (int x = toBlockSpace(center.x) - toBlockSpace(size.width) - 2; x <= toBlockSpace(center.x) + toBlockSpace(size.width) + 2; ++x) {
            for (int y = toBlockSpace(center.y) - toBlockSpace(size.height / 2) - 2; y <= toBlockSpace(center.y) + toBlockSpace(size.height) + 2; ++y) {
                auto found = blockMap.find(std::make_pair(x, y));
                if (found == blockMap.cend()) continue;
                std::string blockType = found->second;
                if (blockType == BlockTypes::EMPTY_BLOCK) continue;
                auto textureFound = textureMap.find(blockType);
                if (textureFound == textureMap.cend()) continue;
                gf::Sprite sprite;
                sprite.setPosition(toWorldSpace(gf::Vector2i(x, y)));
                sprite.setTexture(textureFound->second, gf::RectF::fromSize({1.0f, 1.0f}));
                target.draw(sprite, states);
            }
        }
    }

    int BlockManager::toBlockSpace(const float number) {
        return static_cast<int>(number / BLOCK_SIZE);
    }


    gf::Vector2i BlockManager::toBlockSpace(const gf::Vector2f vector) {
        return gf::Vector2i(toBlockSpace(vector.x), toBlockSpace(vector.y));
    }

    float BlockManager::toWorldSpace(const int number) {
        return number * BLOCK_SIZE;
    }


    gf::Vector2f BlockManager::toWorldSpace(const gf::Vector2i vector) {
        return gf::Vector2f(toWorldSpace(vector.x), toWorldSpace(vector.y));
    }



    std::vector<std::pair<gf::RectF,std::string>> BlockManager::getNearbyHitboxes(const gf::Vector2f position, const gf::Vector2f size) const {
        std::vector<std::pair<gf::RectF,std::string>> res;

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
                if (found->second == BlockTypes::EMPTY_BLOCK) continue;

                res.push_back({
                    gf::RectF::fromPositionSize(
                        gf::Vector2f(toWorldSpace(x), toWorldSpace(y)),
                        gf::Vector2f(BLOCK_SIZE)),
                    found->second});
            }
        }

        return res;
    }
}
