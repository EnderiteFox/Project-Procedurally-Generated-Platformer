#include <world/World.h>
#include <player/Character.h>
#include <blocks/BlockManager.h>

namespace platformer {
    World::World(Character& player, BlockManager& blockManager, WorldGenerator& generator)
    : generator(generator)
    , blockManager(blockManager)
    , player(player)
    , playerSpawnPoint()
    {
        entityContainer.addEntity(blockManager);
        entityContainer.addEntity(player);
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

    gf::Vector2f World::getSpawnPoint() const {
        return this->playerSpawnPoint;
    }

    int World::getTotalNutCount() const {
        return totalNutCount;
    }



    void World::generate() {
        generator.generate(*this);
        player.teleport(playerSpawnPoint);
    }

    void World::update(const gf::Time time) {
        entityContainer.update(time);

        // Teleport player to spawn point if they fall in the void
        if (player.getPosition().y > voidHeight || player.died()) player.setPosition(playerSpawnPoint);
    }

    void World::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        entityContainer.render(target, states);
    }

    void World::setSpawnPoint(const gf::Vector2f spawnPoint) {
        this->playerSpawnPoint = spawnPoint;
    }

    void World::setVoidHeight(const float voidHeight) {
        this->voidHeight = voidHeight;
    }

    void World::setTotalNutCount(const int nutCount) {
        this->totalNutCount = nutCount;
    }



}
