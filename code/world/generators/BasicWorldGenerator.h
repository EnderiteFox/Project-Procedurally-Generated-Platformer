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
        const int ROOM_COUNT = 10;

        const int MIN_ROOM_WIDTH = 3;
        const int MAX_ROOM_WIDTH = 7;

        const int MIN_ROOM_HEIGHT = 3;
        const int MAX_ROOM_HEIGHT = 7;

        const int MAX_ROOMGEN_SIZE_TRIES = 10;
        const int MAX_ROOMGEN_DIRECTION_TRIES = 2 * MAX_ROOM_WIDTH + 2 * MAX_ROOM_HEIGHT;

        const int MIN_ROOM_ENTRANCE_SIZE = 2;

        const std::string WALL_BLOCK_NAME = "testBlock";

        const BlockType WALL_BLOCK = BlockTypes::getBlockTypeByName(WALL_BLOCK_NAME);

        /*
         * The list of rooms
         * The coordinate of the room is at the top left of the room
         * x: The x room coordinate
         * y: The y room coordinate
         * z: The height of the room
         * w: The width of the room
         */
        std::vector<gf::Vector4i> rooms;
        std::vector<gf::Vector2i> path;


        void generateRooms();
        /**
         * Generates a room next to another room
         * @param previousRoom The room to generate a room next to
         * @return true if the room was successfully generated, false otherwise
         */
        bool generateRoomNextToRoom(gf::Vector4i previousRoom);
        void fillWorld(World& world);
        void carveRooms(const World& world);

        void generatePath();
        void debugPath(const World& world);

        static std::optional<gf::Vector2f> findValidSpawnpoint(const World& world, gf::Vector4i room);

        static bool areIntersecting(gf::Vector4i room1, gf::Vector4i room2);

    public:
        BasicWorldGenerator();
        explicit BasicWorldGenerator(uint64_t forcedSeed);

        void generate(World& world) override;
    };
}

#endif //BASICWORLDGENERATOR_H
