#include "TestGenerator.h"

#include <blocks/BlockTypes.h>
#include <world/World.h>

namespace platformer {
    void TestGenerator::generate(World& world) {
        BlockManager& blockManager = world.getBlockManager();
        for (int i = -5; i <= 5; ++i) {
            blockManager.setBlockTypeAt(i, 0, BlockTypes::getBlockTypeByName("testBlock"));
        }
        for (int i = -5; i <= 5; ++i) {
            blockManager.setBlockTypeAt(i, -3, BlockTypes::getBlockTypeByName("iceBlock"));
        }
        for (int i = -9; i <= -6; ++i) {
            blockManager.setBlockTypeAt(i, -1, BlockTypes::getBlockTypeByName("testBlock"));
        }
        for (int i = 6; i <= 10; ++i) {
            blockManager.setBlockTypeAt(i, 0, BlockTypes::getBlockTypeByName("jellyBlock"));
        }

        world.setSpawnPoint(gf::Vector2f{5.0f, -20.0f});
    }
}
