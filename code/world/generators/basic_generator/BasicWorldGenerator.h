/**
 * A world generator using a random seed to create a World
 *
 * The world generator work as the following:
 * - A list of rooms are generated, with a random size, each room being generated right next to the previous room
 *     - To generate a room, a size is chosen, then a direction is chosen, and finally a position in the chosen direction is randomly chosen
 *     - If the room is intersecting another room, another direction and position is chosen
 *     - If placing the room fails too much times, another size of room is chosen
 *     - If even after changing size multiple times, the room still can't generate, then the previous room is removed and re-generated
 * - After rooms are generated, the world is filled with blocks. The type of block is determined using Perlin Noise
 * - The rooms are then carved into the blocks
 * - After that, a path is generated
 * - Then all points from the path are connected, going in zig-zags if necessary
 * - Fake platforms are generated in all rooms. Fake platforms also generate with ladders and collectibles
 * - Some wall blocks and platforms are turned into their ice and jelly counterparts, based on a perlin noise
 * - Some rooms are chosen to be dangerous, and spikes are generated to replace the floor of the room
 * - The exit block is placed at the last path point
 */

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
        // The amount of rooms to generate
        static constexpr int ROOM_COUNT = 20;

        // The minimum and maximum width of a room
        static constexpr int MIN_ROOM_WIDTH = 7;
        static constexpr int MAX_ROOM_WIDTH = 15;

        // The minimum and maximum height of a room
        static constexpr int MIN_ROOM_HEIGHT = 5;
        static constexpr int MAX_ROOM_HEIGHT = 13;

        // The number of times the generator is allowed to change the size of a room before backtracking
        static constexpr int MAX_ROOMGEN_SIZE_TRIES = 10;
        // The amount of positions the generator is allowed to place a room at before trying to change size
        static constexpr int MAX_ROOMGEN_DIRECTION_TRIES = 2 * MAX_ROOM_WIDTH + 2 * MAX_ROOM_HEIGHT;

        // The minimum size for the entrance of a room
        static constexpr int MIN_ROOM_ENTRANCE_SIZE = 2;

        // The minimum and maximum size of a fake platform
        static constexpr int MIN_FAKE_PLATFORM_SIZE = 4;
        static constexpr int MAX_FAKE_PLATFORM_SIZE = 10;

        // The amount tries the generator has to place a fake platform, before skipping the platform
        static constexpr int MAX_FAKE_PLATFORM_GEN_TRIES = 3;

        // The minimum and maximum amount of ladders on a fake platform
        static constexpr int MIN_FAKE_PLATFORM_LADDER_COUNT = 0;
        static constexpr int MAX_FAKE_PLATFORM_LADDER_COUNT = 1;

        // The max size of a fake platform ladder
        static constexpr int MAX_FAKE_PLATFORM_LADDER_SIZE = 10;

        // The minimum and maximum amount of fake platforms per room
        static constexpr int MIN_FAKE_PLATFORM_AMOUNT = 3;
        static constexpr int MAX_FAKE_PLATFORM_AMOUNT = 6;

        // The perlin noise threshold to make a block an ice block
        static constexpr double ICE_BLOCK_THRESHOLD = -0.15;

        // The perlin noise threshold to make a block a jelly block
        static constexpr double JELLY_BLOCK_THRESHOLD = 0.35;

        // The chance that a room spawns with spikes at the bottom
        static constexpr double DANGEROUS_ROOM_CHANCE = 0.3;

        // The minimum and maximum amount of collectibles to generate in each room
        static constexpr int MIN_COLLECTIBLE_AMOUNT = 0;
        static constexpr int MAX_COLLECTIBLE_AMOUNT = 1;

        // The minimum consecutive ice blocks required before placing a spike at the end
        static constexpr int ICE_TRAP_THRESHOLD = 3;

        // The block type used to generate walls
        const BlockType WALL_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::TEST_BLOCK);

        // The ladder block type
        const BlockType LADDER_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::TEST_LADDER);

        // The platform block types
        const BlockType PLATFORM_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::PLATFORM_BLOCK);
        const BlockType ICE_PLATFORM = BlockTypes::getBlockTypeByName(BlockTypes::ICE_PLATFORM);
        const BlockType JELLY_PLATFORM = BlockTypes::getBlockTypeByName(BlockTypes::JELLY_PLATFORM);

        // The block used to debug the path
        const BlockType PATH = BlockTypes::getBlockTypeByName(BlockTypes::PATH);

        // The ice block
        const BlockType ICE_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::ICE);

        // The jelly block
        const BlockType JELLY_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::JELLY);

        // The spike block
        const BlockType SPIKE_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::SPIKE);

        // The exit block
        const BlockType EXIT_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::EXIT);

        // The collectible block
        const BlockType COLLECTIBLE_BLOCK = BlockTypes::getBlockTypeByName(BlockTypes::NUT);

        // Blocks spikes can spawn on
        const std::vector<BlockType> SPIKE_SUPPORTS{
            WALL_BLOCK,
            ICE_BLOCK,
            JELLY_BLOCK
        };

        /*
         * The list of rooms
         * The coordinate of the room is at the top left of the room
         * x: The x room coordinate
         * y: The y room coordinate
         * z: The height of the room
         * w: The width of the room
         */
        std::vector<gf::Vector4i> rooms;

        // The points of the path
        std::vector<gf::Vector2i> path;

        /**
         * Generates the list of rooms
         */
        void generateRooms();

        /**
         * Generates a room next to another room
         * @param previousRoom The room to generate a room next to
         * @return true if the room was successfully generated, false otherwise
         */
        bool generateRoomNextToRoom(gf::Vector4i previousRoom);

        /**
         * Fills the world with wall blocks
         * @param world The world to fill
         */
        void fillWorld(World& world) const;

        /**
         * Chooses which block type to place at the given position using Perlin Noise.
         * Walls and platforms may be transformed into their ice or jelly counterpart, other block types are simply returned
         * @param blockType The type of the block
         * @param pos The position of the block
         * @param worldDimensions The dimensions of the world
         * @return The block type to place at the position
         */
        BlockType getBlockCounterpart(const std::string& blockType, gf::Vector2i pos, gf::Vector<gf::Vector2i, 2> worldDimensions);

        /**
         * Carves all rooms into the blocks
         * @param world The world to carve rooms into
         */
        void carveRooms(const World& world);

        /**
         * Generates the path for the level
         * @param world The world to generate the path in
         */
        void generatePath(const World& world);

        /**
         * Places blocks where the path points are for debugging purposes
         * @param world The world to place the blocks in
         */
        void debugPath(const World& world);

        /**
         * Connects two points, with platforms and ladders
         * @param world The world to connect the path in
         * @param currentPoint The point to start from
         * @param nextPoint The destination point
         * @return The list of intermediary points to connect the two points
         */
        std::vector<gf::Vector2i> getConnectionPath(const World& world, gf::Vector2i currentPoint, gf::Vector2i nextPoint);

        /**
         * Calculates and generates a connection between two points, using ladders and platforms
         * @param world The world to place blocks in
         * @param currentPoint The point to start from
         * @param nextPoint The destination point
         */
        void connectPathPoints(const World& world, gf::Vector2i currentPoint, gf::Vector2i nextPoint);

        /**
         * Tries to find a valid spawnpoint inside a room
         * @param world The world the room is in
         * @param room The room to find a spawnpoint in
         * @return An optional with the coordinates of the spawnpoint if found, an empty optional otherwise
         */
        static std::optional<gf::Vector2f> findValidSpawnpoint(const World& world, gf::Vector4i room);

        /**
         * Checks if two rooms are intersecting, using classic rectangle-to-rectangle collision calculation
         * @param room1 The first room
         * @param room2 The second room
         * @return true if the rooms are instersecting, false otherwise
         */
        static bool areIntersecting(gf::Vector4i room1, gf::Vector4i room2);

        /**
         * Calculates the minimum and maximum points of the world. Used to fill the world with blocks using the list of rooms
         * @return The dimensions of the world
         */
        gf::Vector<gf::Vector2i, 2> getWorldDimensions() const;

        /**
         * Generates fake platforms inside the room
         * @param world The world to place blocks in
         * @param room The rooms to generate the platforms in
         */
        void generateFakePlatformsInRoom(const World& world, gf::Vector4i room);

        /**
         * Grows a platform from a position, until either it reaches a predetermined size, or the platform hits something
         * @param world The world to place blocks in
         * @param startPos The start position of the platform
         * @param direction The direction where the platform will grow
         * @param collectibleAmount The amount of collectibles to generate on the platform
         */
        void growFakePlatform(const World& world, gf::Vector2i startPos, gf::Vector2i direction, int collectibleAmount);

        /**
         * Grows a ladder, from top to bottom, until either the ladder reaches a maximum size, or the ladder hits something
         * @param world The world to place blocks in
         * @param startPos The start position of the ladder
         */
        void growLadder(const World& world, gf::Vector2i startPos) const;

        /**
         * Turns some blocks into their ice and jelly counterparts, based on a Perlin Noise
         * @param world The world containing the blocks
         */
        void transformBlocks(const World& world);

        /**
         * Makes some random rooms dangerous by calling the makeRoomDangerous method.
         * Is also responsible for the creation of small traps with placeSmallTraps.
         * @param world The world to place blocks in
         */
        void makeSomeRoomsDangerous(const World& world);

        /**
         * Places some small traps, like spikes at the end of an ice floor
         * @param world The world to place blocks in
         * @param room The room to place the traps in
         */
        void placeSmallTraps(const World& world, gf::Vector4i room) const;

        /**
         * Makes a room dangerous, placing spikes at the bottom of the room
         * @param world The world to place blocks in
         * @param room The room to make dangerous
         */
        void makeRoomDangerous(const World& world, gf::Vector4i room) const;

        /**
         * Checks if a block can support a spike
         * @param blockType The block type to check
         * @return true if the block type supports spikes, false otherwise
         */
        bool blockSupportsSpike(const std::string& blockType) const;

    public:
        /**
         * A constructor to initialize the generator with a random seed
         */
        BasicWorldGenerator();

        /**
         * A constructor to initialize the generator with the chosen seed
         * @param forcedSeed The seed to use
         */
        explicit BasicWorldGenerator(uint64_t forcedSeed);

        /**
         * Generates the world
         * @param world The world to generate
         */
        void generate(World& world) override;
    };
}

#endif //BASICWORLDGENERATOR_H
