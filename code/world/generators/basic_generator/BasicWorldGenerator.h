#ifndef BASICWORLDGENERATOR_H
#define BASICWORLDGENERATOR_H
#include <optional>
#include <vector>
#include <blocks/BlockType.h>
#include <blocks/BlockTypes.h>
#include <gf/Vector.h>

#include "../WorldGenerator.h"

namespace platformer {
    class BasicWorldGenerator final : public WorldGenerator {
        const int ROOM_COUNT = 20;

        const int MIN_ROOM_WIDTH = 7;
        const int MAX_ROOM_WIDTH = 15;

        const int MIN_ROOM_HEIGHT = 5;
        const int MAX_ROOM_HEIGHT = 13;

        const int MAX_ROOMGEN_SIZE_TRIES = 10;
        const int MAX_ROOMGEN_DIRECTION_TRIES = 2 * MAX_ROOM_WIDTH + 2 * MAX_ROOM_HEIGHT;

        const int MIN_ROOM_ENTRANCE_SIZE = 2;

        const std::string AIR_TYPE = BlockTypes::EMPTY_BLOCK;
        const BlockType WALL_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::TEST_BLOCK);
        const BlockType LADDER_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::TEST_LADDER);
        const BlockType PLATFORM_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::PLATFORM_BLOCK);
        const BlockType PATH = BlockTypes::getBlockTypeByName(BlockTypes::PATH);

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
        void fillWorld(World& world) const;
        void carveRooms(const World& world);

        void generatePath();
        void debugPath(const World& world);
        std::vector<gf::Vector2i> getConnectionPath(const World& world, gf::Vector2i currentPoint, gf::Vector2i nextPoint);
        void connectPathPoints(const World& world, gf::Vector2i currentPoint, gf::Vector2i nextPoint);

        static std::optional<gf::Vector2f> findValidSpawnpoint(const World& world, gf::Vector4i room);

        static bool areIntersecting(gf::Vector4i room1, gf::Vector4i room2);
        gf::Vector<gf::Vector2i, 2> getWorldDimensions() const;

    public:
        BasicWorldGenerator();
        explicit BasicWorldGenerator(uint64_t forcedSeed);

        void generate(World& world) override;
    };
}

#endif //BASICWORLDGENERATOR_H
