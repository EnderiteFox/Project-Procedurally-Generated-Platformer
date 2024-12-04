#pragma once
#include <Character.h>
#include <gf/EntityContainer.h>

#include "BlockManager.h"

namespace platformer {
    class World {
        gf::EntityContainer entityContainer;
        BlockManager blockManager;
        Character player;

    public:
        explicit World(Character& player);
        Character& getPlayer();
        BlockManager& getBlockManager();
        gf::EntityContainer& getEntityContainer();

        /**
         * Generates the world
         * For the time being, will generate a line of blocks
         */
        void generate();
    };
}
