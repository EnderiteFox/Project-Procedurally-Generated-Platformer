#include <blocks/BlockManager.h>
#include <blocks/BlockTypes.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <vector>
#include <gf/Rect.h>
#include <gf/Shapes.h>

#include <iostream>

#define DRAWHITBOXES

namespace platformer {
    BlockManager::BlockManager(const gf::Vector2f ViewSize): ViewSize(ViewSize)  {}

    const float BlockManager::BLOCK_SIZE = 8.0f;

    std::string BlockManager::getBlockTypeAt(const int x, const int y) const {
        const auto found = blockMap.find(std::make_pair(x, y));
        if (found == blockMap.cend()) return "empty";
        return found->second;
    }

    std::string BlockManager::getBlockTypeAt(const gf::Vector2i pos) const {
        return getBlockTypeAt(pos.x, pos.y);
    }

    void BlockManager::setBlockTypeAt(const int x, const int y, const BlockType& blockType) {
        if (blockType.type == BlockTypes::EMPTY_BLOCK) return removeBlockAt(x, y);
        blockMap.insert_or_assign(std::make_pair(x, y), blockType.subType);
    }

    void BlockManager::setBlockTypeAt(const gf::Vector2i pos, const BlockType& blockType) {
        setBlockTypeAt(pos.x, pos.y, blockType);
    }

    void BlockManager::removeBlockAt(int x, int y) {
        blockMap.erase({x, y});
    }

    void BlockManager::removeBlockAt(const gf::Vector2i pos) {
        removeBlockAt(pos.x, pos.y);
    }

    bool BlockManager::isEmptyBlock(const int x, const int y) const {
        return getBlockTypeAt(x, y) == "empty";
    }

    bool BlockManager::isEmptyBlock(const gf::Vector2i pos) const {
        return isEmptyBlock(pos.x, pos.y);
    }


    void BlockManager::loadTextures() {
        for (const BlockType& blockType : BlockTypes::getAllTypes()) {
            if(blockType.texturePath == "") continue; // Skipping untextured blocks
            textureMap.insert(std::make_pair(blockType.subType, gf::Texture(blockType.texturePath)));
        }
    }

    void BlockManager::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        const gf::Vector2f center = ViewPosition;
        const gf::Vector2f size = ViewSize;

        // Iteration on the blocks within the view
        for (int x = toBlockSpace(center.x) - toBlockSpace(size.width) - 2; x <= toBlockSpace(center.x) + toBlockSpace(size.width) + 2; ++x) {
            for (int y = toBlockSpace(center.y) - toBlockSpace(size.height / 2) - 2; y <= toBlockSpace(center.y) + toBlockSpace(size.height) + 2; ++y) {
                auto found = blockMap.find({x, y});
                if (found == blockMap.cend()) continue; // Skipping if the block at the position does not exist on the map
                std::string blockType = found->second;
                auto textureFound = textureMap.find(blockType);
                if (textureFound == textureMap.cend()) continue; //Skipping untextured blocks
                gf::Sprite sprite;
                sprite.setPosition(toWorldSpace(gf::Vector2i(x, y)));
                sprite.setTexture(textureFound->second, gf::RectF::fromSize({1.0f, 1.0f}));
                sprite.scale(BlockTypes::getBlockTypeByName(blockType).scale);
                target.draw(sprite, states);
#ifdef DRAWHITBOXES
                gf::RectangleShape hitbox;
                hitbox.setSize(BlockTypes::getBlockTypeByName(found->second).hitboxSize);
                hitbox.setPosition(gf::Vector2f(toWorldSpace(x), toWorldSpace(y)) + BlockTypes::getBlockTypeByName(found->second).hitboxOffset);
                hitbox.setColor(gf::Color::Opaque(0));
                hitbox.setOutlineColor(gf::Color::Blue);
                hitbox.setOutlineThickness(0.2f);
                target.draw(hitbox);
#endif
            }
        }
    }

    void BlockManager::setViewPosition(gf::Vector2f ViewPosition){
        this->ViewPosition = ViewPosition;
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
                if (found->second == "empty") continue;


                BlockType blockType = BlockTypes::getBlockTypeByName(found->second);
                res.push_back({
                    gf::RectF::fromPositionSize(
                        gf::Vector2f(toWorldSpace(x), toWorldSpace(y)) + blockType.hitboxOffset,
                        blockType.hitboxSize),
                    found->second});
            }
        }

        return res;
    }
}
