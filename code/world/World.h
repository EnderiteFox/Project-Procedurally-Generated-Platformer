/**
 * The World class handles the world generation
 * It acts as a container for the game's main elements (Main block manager and Character)
 *
 * It's final purpose is to generate the grid, giving the initial position of the player and creating the randomly generated world
 * The generator used by the world is given in the constructor
 */

#pragma once
#include <blocks/BlockManager.h>
#include <gf/EntityContainer.h>

#include "generators/WorldGenerator.h"

namespace platformer {

    class Character;

    class World final : public gf::Entity {
        // The height at which the void is located
        // If the player falls in the void, they are teleported at their spawn point
        const float DEFAULT_VOID_HEIGHT = 100.0f;

        WorldGenerator& generator;
        gf::EntityContainer entityContainer;
        BlockManager& blockManager;
        Character& player;
        gf::Vector2f playerSpawnPoint;
        float voidHeight = DEFAULT_VOID_HEIGHT;
        int totalNutCount = 0;

    public:
        //Constructor
        World(Character& player, BlockManager& blockManager, WorldGenerator& generator);

        //Getters
        Character& getPlayer() const;
        BlockManager& getBlockManager() const;
        gf::EntityContainer& getEntityContainer();
        gf::Vector2f getSpawnPoint() const;
        int getTotalNutCount() const;

        // Setters
        void setSpawnPoint(gf::Vector2f spawnPoint);
        void setVoidHeight(float voidHeight);
        void setTotalNutCount(int nutCount);

        // GF's render and update methods
        void render(gf::RenderTarget& target, const gf::RenderStates& states = gf::RenderStates()) override;
        void update(gf::Time time) override;

        /**
         * Generates the world using the generator given in parameters
         */
        void generate();
    };
}
