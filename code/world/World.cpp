#include <world/World.h>
#include <player/Character.h>
#include <blocks/BlockManager.h>
#include <blocks/BlockTypes.h>

namespace platformer {
    World::World(Character& player, BlockManager& blockManager): blockManager(blockManager), player(player), playerSpawnPoint() {
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
            blockManager.setBlockTypeAt(i, 0, BlockTypes::getBlockTypeByName("testBlock"));
        }
        for (int i = -5; i <= 5; ++i) {
            blockManager.setBlockTypeAt(i, -3, BlockTypes::getBlockTypeByName("testBlock"));
        }
        for (int i = -9; i <= -6; ++i) {
            blockManager.setBlockTypeAt(i, -1, BlockTypes::getBlockTypeByName("testBlock"));
        }


        playerSpawnPoint = gf::Vector2f{5.0f,-20.0f};
        player.teleport(playerSpawnPoint);
    }

    void World::update(const gf::Time time) {
        entityContainer.update(time);

        // Teleport player to spawn point if they fall in the void
        if (player.getPosition().y > VOID_HEIGHT) player.setPosition(playerSpawnPoint);
    }

    void World::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        entityContainer.render(target, states);
    }
}
