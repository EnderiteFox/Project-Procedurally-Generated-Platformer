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
            blockManager.setBlockTypeAt(i, -3, BlockTypes::getBlockTypeByName("testBlock"));
        }
        for (int i = -9; i <= -6; ++i) {
            blockManager.setBlockTypeAt(i, -1, BlockTypes::getBlockTypeByName("testBlock"));
        }

        world.setSpawnPoint(gf::Vector2f{5.0f, -20.0f});
    }
}
