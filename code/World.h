#pragma once
#include <Block.h>
#include <Character.h>
#include <gf/EntityContainer.h>

namespace platformer {
    class World {
        gf::EntityContainer entityContainer;
        Character player;
        std::vector<Block> blocks;

    public:
        explicit World(Character& player);
        Character& getPlayer();
        std::vector<Block>& getBlocks();
        gf::EntityContainer& getEntityContainer();

        /**
         * Generates the world
         * For the time being, will generate a line of blocks
         */
        void generate(const gf::Texture& blockTexture);
    };
}
