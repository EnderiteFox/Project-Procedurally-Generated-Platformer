/**
 * The World class handles the world generation
 * It acts as a container for the game's main elements (Main block manager and Character)
 *
 * It's final purpose is to generate the grid, giving the initial position of the player and creating the randomly generated world
 */

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
        //Constructor
        World(Character& player, BlockManager& blockManager);
        //Getters
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
