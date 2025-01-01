#include <world/World.h>
#include <player/Character.h>
#include <blocks/BlockManager.h>
#include <blocks/BlockTypes.h>
#include <vector>

namespace platformer {
    World::World(Character& player, BlockManager& blockManager): blockManager(blockManager), player(player) {
        entityContainer.addEntity(player);
        entityContainer.addEntity(blockManager);
    }

    Character& World::getPlayer() const {
        return this->player;
    }

    BlockManager& World::getBlockManager() const {
        return blockManager;
    }

    gf::EntityContainer& World::getEntityContainer() {
        return this->entityContainer;
    }

    void World::generate() {
        for (int i = -5; i <= 5; ++i) {
            blockManager.setBlockTypeAt(i, 0, BlockTypes::TEST_BLOCK);
        }
        playerSpawnPoint = gf::Vector2f{5.0f,-20.0f};
        player.teleport(playerSpawnPoint);
    }
}
