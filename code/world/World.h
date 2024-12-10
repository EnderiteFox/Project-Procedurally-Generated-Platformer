#pragma once
#include <blocks/BlockManager.h>
#include <gf/EntityContainer.h>

namespace platformer {
    class Character;

    class World {
        gf::EntityContainer entityContainer;
        BlockManager& blockManager;
        Character& player;

    public:
        World(Character& player, BlockManager& blockManager);
        Character& getPlayer() const;
        BlockManager& getBlockManager() const;
        gf::EntityContainer& getEntityContainer();

        /**
         * Generates the world
         * For the time being, will generate a line of blocks
         */
        void generate() const;
    };
}
