#ifndef BASICWORLDGENERATOR_H
#define BASICWORLDGENERATOR_H
#include <optional>
#include <vector>
#include <blocks/BlockType.h>
#include <blocks/BlockTypes.h>
#include <gf/Vector.h>

#include "WorldGenerator.h"

namespace platformer {
    class BasicWorldGenerator final : public WorldGenerator {
        // Placeholder values, will be defined later
        const int ROOM_WIDTH = 3;
        const int ROOM_HEIGHT = 3;
        const int ROOM_COUNT = 10;

        const std::string WALL_BLOCK_NAME = "testBlock";

        const BlockType WALL_BLOCK = BlockTypes::getBlockTypeByName(WALL_BLOCK_NAME);

        std::vector<gf::Vector2i> rooms;
        std::vector<gf::Vector2i> path;


        void generateRooms();
        void fillWorld(World& world);
        void carveRooms(const World& world);

        void generatePath();
        void debugPath(const World& world);

        std::optional<gf::Vector2f> findValidSpawnpoint(const World& world, gf::Vector2i room) const;

    public:
        BasicWorldGenerator();
        explicit BasicWorldGenerator(uint64_t forcedSeed);

        void generate(World& world) override;
    };
}

#endif //BASICWORLDGENERATOR_H
