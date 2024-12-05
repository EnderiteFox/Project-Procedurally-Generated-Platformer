#include <world/World.h>
#include <player/Character.h>
#include <blocks/BlockManager.h>
#include <blocks/BlockTypes.h>

namespace platformer {
    World::World(Character& player): blockManager(BlockManager()), player(player) {
        entityContainer.addEntity(player);
        entityContainer.addEntity(blockManager);
    }

    Character& World::getPlayer() {
        return this->player;
    }

    BlockManager& World::getBlockManager() {
        return blockManager;
    }

    gf::EntityContainer& World::getEntityContainer() {
        return this->entityContainer;
    }

    void World::generate() {
        for (int i = -5; i <= 5; ++i) {
            blockManager.setBlockTypeAt(i, 0, BlockTypes::TEST_BLOCK);
        }
    }
}
