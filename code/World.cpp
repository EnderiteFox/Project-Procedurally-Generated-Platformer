#include "World.h"

#include <Block.h>

namespace platformer {
    World::World(Character& player): player(player) {
        entity_container.addEntity(player);
    }

    Character& World::getPlayer() {
        return this->player;
    }

    gf::EntityContainer& World::getEntityContainer() {
        return this->entity_container;
    }

    void World::generate(const gf::Texture& blockTexture) {
        for (int i = 0; i < 10; ++i) {
            Block block({(i - 5) * 10.0f, 10.0f}, blockTexture);
            this->addEntity(block);
        }
    }
}
