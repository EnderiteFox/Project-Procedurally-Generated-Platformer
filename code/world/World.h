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

    class World final : public gf::Entity {
        // The height at which the void is located
        // If the player falls in the void, they are teleported at their spawn point
        const float VOID_HEIGHT = 100.0f;

        gf::EntityContainer entityContainer;
        BlockManager& blockManager;
        Character& player;
        gf::Vector2f playerSpawnPoint;

    public:
        //Constructor
        World(Character& player, BlockManager& blockManager);

        //Getters
        Character& getPlayer() const;
        BlockManager& getBlockManager() const;
        gf::EntityContainer& getEntityContainer();

        // GF's render and update methods
        void render(gf::RenderTarget& target, const gf::RenderStates& states = gf::RenderStates()) override;
        void update(gf::Time time) override;

        /**
         * Generates the world
         * For the time being, will generate a line of blocks
         */
        void generate();
    };
}
