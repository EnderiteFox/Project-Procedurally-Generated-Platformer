#include "TestGenerator.h"

#include <blocks/BlockTypes.h>
#include <world/World.h>
#include <iostream>

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
        for (int i=-2; i>=-4; --i){
            assert(BlockTypes::getBlockTypeByName("testLadder").type != "");
            blockManager.setBlockTypeAt(-6, i, BlockTypes::getBlockTypeByName("testLadder"));
        }

        world.setSpawnPoint(gf::Vector2f{5.0f, -20.0f});
    }
}
