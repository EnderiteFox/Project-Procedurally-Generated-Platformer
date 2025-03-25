#include "BasicWorldGenerator.h"

#include <iostream>
#include <blocks/BlockTypes.h>
#include <world/World.h>

namespace platformer {
    BasicWorldGenerator::BasicWorldGenerator()
    : WorldGenerator()
    {}

    BasicWorldGenerator::BasicWorldGenerator(const uint64_t forcedSeed)
    : WorldGenerator(forcedSeed)
    {}


    void BasicWorldGenerator::generate(World& world) {
        std::cout << "Generating seed " << seed << "\n";

        // Generate the list of rooms
        generateRooms();
        if (rooms.empty()) return;

        // Fill the world with blocks
        fillWorld(world);

        // Carve the rooms into the walls
        carveRooms(world);

        // Generate the list of path points
        generatePath(world);

        // Generate a ladder from the first path point
        growLadder(world, path.at(0));

        // Connect path points
        for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i) {
            connectPathPoints(world, path.at(i), path.at(i + 1));
        }

        // Generate fake platforms
        for (const gf::Vector4i room : rooms) {
            generateFakePlatformsInRoom(world, room);
        }

        // Transform blocks into ice or jelly
        transformBlocks(world);

        // Place spikes
        makeSomeRoomsDangerous(world);

        // Find spawn point
        for (const gf::Vector4i room : rooms) {
            if (const std::optional<gf::Vector2f> spawnpoint = findValidSpawnpoint(world, room)) {
                world.setSpawnPoint(spawnpoint.value());
                break;
            }
        }

        // Place the exit at the last path point
        world.getBlockManager().setBlockTypeAt(path.back(), EXIT_BLOCK);

        // Store the amount of nuts in the world
        world.setTotalNutCount(nutCount);

        // Debug the path
        //debugPath(world);
    }

    void BasicWorldGenerator::generateRooms() {
        // Generate first room
        rooms.push_back(gf::Vector4i{
            0,
            0,
            random.computeUniformInteger(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT),
            random.computeUniformInteger(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH)
        });


        for (int i = 1; i < ROOM_COUNT; ++i) {
            // If, after going back, the list of rooms is empty, that means that the first room is unable to generate
            // for some reason
            if (rooms.empty()) {
                std::cerr << "Unable to generate any room\n";
                return;
            }

            // If failed to generate a room, go back
            if (!generateRoomNextToRoom(rooms.back())) {
                rooms.pop_back();
                i--;
            }
        }
    }

    bool BasicWorldGenerator::generateRoomNextToRoom(const gf::Vector4i previousRoom) {
        // Try to place the room multiple times, with different room sizes
        for (int sizeTries = 0; sizeTries < MAX_ROOMGEN_SIZE_TRIES; ++sizeTries) {
            // Generate a room of random size
            gf::Vector4i room{
                0,
                0,
                random.computeUniformInteger(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT),
                random.computeUniformInteger(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH)
            };
            // Try to place the room multiple times, with different directions
            for (int posTries = 0; posTries < MAX_ROOMGEN_DIRECTION_TRIES; ++posTries) {
                // Place room randomly along the previous room
                // An integer represents the direction
                // 0: left
                // 1: up
                // 2: right
                // 3: down
                switch (random.computeUniformInteger(0, 3)) {
                case 0:
                    room.x = previousRoom.x - room.w;
                    room.y = random.computeUniformInteger(
                        previousRoom.y - room.z + MIN_ROOM_ENTRANCE_SIZE,
                        previousRoom.y + previousRoom.z - MIN_ROOM_ENTRANCE_SIZE
                    );
                    break;
                case 1:
                    room.x = random.computeUniformInteger(
                        previousRoom.x - room.w + MIN_ROOM_ENTRANCE_SIZE,
                        previousRoom.x + previousRoom.w - MIN_ROOM_ENTRANCE_SIZE
                    );
                    room.y = previousRoom.y - room.z;
                    break;
                case 2:
                    room.x = previousRoom.x + previousRoom.w;
                    room.y = random.computeUniformInteger(
                        previousRoom.y - room.z + MIN_ROOM_ENTRANCE_SIZE,
                        previousRoom.y + previousRoom.z - MIN_ROOM_ENTRANCE_SIZE
                    );
                    break;
                default:
                    room.x = random.computeUniformInteger(
                        previousRoom.x - room.w + MIN_ROOM_ENTRANCE_SIZE,
                        previousRoom.x + previousRoom.w - MIN_ROOM_ENTRANCE_SIZE
                    );
                    room.y = previousRoom.y + previousRoom.z;
                }

                // Check if any room is intersecting with the newly placed room
                if (
                    std::none_of(
                        rooms.cbegin(),
                        rooms.cend(),
                        [room](const gf::Vector4i otherRoom) {
                                return areIntersecting(room, otherRoom);
                        }
                    )
                ) {
                    // No room is intersecting, place the room
                    rooms.push_back(room);
                    return true;
                }
            }
        }
        // Max number of tries reached, failed to generate the room
        return false;
    }


    void BasicWorldGenerator::fillWorld(World& world) const {
        // Get world size
        const gf::Vector<gf::Vector2i, 2> worldDimensions = getWorldDimensions();

        // Iterate over each block in the world, placing wall blocks everywhere
        for (int x = worldDimensions.x.x; x < worldDimensions.y.x; ++x) {
            for (int y = worldDimensions.x.y; y < worldDimensions.y.y; ++y) {
                world.getBlockManager().setBlockTypeAt(
                    x,
                    y,
                    WALL_BLOCK
                );
            }
        }

        // Set the void height to the lowest coordinate
        world.setVoidHeight(BlockManager::toWorldSpace(worldDimensions.y.y + 1));
    }

    BlockType BasicWorldGenerator::getBlockCounterpart(
        const std::string& blockType,
        gf::Vector2i pos,
        gf::Vector<gf::Vector2i, 2> worldDimensions
    ) {
        // Get position in perlin noise
        double perlinX = (pos.x - worldDimensions.x.x) / static_cast<double>(worldDimensions.y.x);
        double perlinY = (pos.y - worldDimensions.x.y) / static_cast<double>(worldDimensions.y.y);

        // Get perlin noise value
        double perlinValue = perlinNoise.getValue(perlinX, perlinY);

        // Select block counterpart if the perlin noise value passes a threshold
        if (blockType == WALL_BLOCK.subType) {
            return perlinValue < ICE_BLOCK_THRESHOLD ? ICE_BLOCK : perlinValue > JELLY_BLOCK_THRESHOLD ? JELLY_BLOCK : WALL_BLOCK;
        }
        if (blockType == PLATFORM_BLOCK.subType) {
            return perlinValue < ICE_BLOCK_THRESHOLD ? ICE_PLATFORM : perlinValue > JELLY_BLOCK_THRESHOLD ? JELLY_PLATFORM : PLATFORM_BLOCK;
        }

        // Return the block type
        return BlockTypes::getBlockTypeByName(blockType);
    }


    void BasicWorldGenerator::carveRooms(const World& world) {
        // Iterate over each room, and remove block in the room
        for (const gf::Vector4i room : rooms) {
            for (int x = room.x; x < room.x + room.w; ++x) {
                for (int y = room.y; y < room.y + room.z; ++y) {
                    world.getBlockManager().removeBlockAt(x, y);
                }
            }
        }
    }


    void BasicWorldGenerator::generatePath(const World& world) {
        // Place a path point in each room
        for (const gf::Vector4i room : rooms) {
            // Make the last path point generate on the ground
            if (room == rooms.back()) {
                // To generate the path on the ground, find a valid spawnpoint
                // The point will generate on the ground if possible, otherwise it will find another safe spot to place the point on
                // If no safe spot can be found, the room will be skipped and no path point will be added
                // In the case of generating the exit in the last room, the new last room will be the previous room
                if (
                    std::optional<gf::Vector2f> posOpt = findValidSpawnpoint(world, room);
                    posOpt.has_value()
                ) {
                    path.push_back(BlockManager::toBlockSpace(posOpt.value()));
                }
            }
            else {
                // Place the point at a random position in the room
                path.push_back(gf::Vector2i{
                    room.x + random.computeUniformInteger(0, room.w - 1),
                    room.y + random.computeUniformInteger(1, room.z - 1)
                });
            }
        }
    }

    void BasicWorldGenerator::debugPath(const World& world) {
        // Place a PATH block at each path point
        for (const gf::Vector2i pathPoint : path) {
            world.getBlockManager().setBlockTypeAt(pathPoint.x, pathPoint.y, PATH);
        }
    }

    std::vector<gf::Vector2i> BasicWorldGenerator::getConnectionPath(const World& world, gf::Vector2i currentPoint, gf::Vector2i nextPoint) {
        // Get the direction the path point is in
        gf::Vector2i direction = sign(nextPoint - currentPoint);

        // Make sure we are not already at the destination
        if (direction.x == 0 && direction.y == 0) return std::vector<gf::Vector2i>();

        // Checking if starting with a ladder is possible
        std::vector<gf::Vector2i> ladderFirstPath;
        gf::Vector2i checkPos = currentPoint;
        // While destination not reached
        while (checkPos != nextPoint) {
            bool firstCheck = true;
            gf::Vector2i prevCheckPos = checkPos;
            // Progress vertically if needed
            if (direction.y != 0) {
                for (; checkPos.y != nextPoint.y; checkPos.y += direction.y) {
                    // If not blocked
                    if (
                        (
                            !world.getBlockManager().isEmptyBlock(checkPos)
                            || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                            || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType
                        )
                        && !firstCheck
                    ) {
                        // Progress
                        checkPos.y -= direction.y;
                        break;
                    }
                    firstCheck = false;
                }
                ladderFirstPath.push_back(checkPos);
                if (checkPos == nextPoint) break;
            }
            // Progress horizontally if needed
            if (direction.x != 0) {
                for (; checkPos.x != nextPoint.x; checkPos.x += direction.x) {
                    // If not blocked
                    if (
                        (
                            !world.getBlockManager().isEmptyBlock(checkPos)
                            || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                            || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType
                        )
                        && !firstCheck
                    ) {
                        // Progress
                        checkPos.x -= direction.x;
                        break;
                    }
                    firstCheck = false;
                }
                ladderFirstPath.push_back(checkPos);
            }

            // If unable to progress (didn't move in this iteration), fail to connect
            if (prevCheckPos == checkPos) {
                ladderFirstPath.clear();
                break;
            }
        }

        // Checking if starting with a platform is possible
        std::vector<gf::Vector2i> platformFirstPath;
        checkPos = currentPoint;
        // While destination not reached
        while (checkPos != nextPoint) {
            bool firstCheck = true;
            gf::Vector2i prevCheckPoint = checkPos;
            // Progress horizontally if needed
            if (direction.x != 0) {
                for (; checkPos.x != nextPoint.x; checkPos.x += direction.x) {
                    // If not blocked
                    if (
                        (
                            !world.getBlockManager().isEmptyBlock(checkPos)
                            || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                            || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType
                        )
                        && !firstCheck
                    ) {
                        // Progress
                        checkPos.x -= direction.x;
                        break;
                    }
                    firstCheck = false;
                }
                platformFirstPath.push_back(checkPos);
                if (checkPos == nextPoint) break;
            }
            // Progress vertically if needed
            if (direction.y != 0) {
                for (; checkPos.y != nextPoint.y; checkPos.y += direction.y) {
                    // If not blocked
                    if (
                        (
                            !world.getBlockManager().isEmptyBlock(checkPos)
                            || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                            || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType
                        )
                        && !firstCheck
                    ) {
                        // Progress
                        checkPos.y -= direction.y;
                        break;
                    }
                    firstCheck = false;
                }
                platformFirstPath.push_back(checkPos);
            }

            // If unable to progress (didn't move this iteration), fail to connect
            if (prevCheckPoint == checkPos) {
                platformFirstPath.clear();
                break;
            }
        }

        // If able to connect by starting by a ladder and by starting by a platform, choose one randomly
        if (!ladderFirstPath.empty() && !platformFirstPath.empty()) {
            if (random.computeUniformInteger(0, 1) == 1) return ladderFirstPath;
            return platformFirstPath;
        }

        // If only one way to connect succeded, return it
        if (!ladderFirstPath.empty()) return ladderFirstPath;
        if (!platformFirstPath.empty()) return platformFirstPath;

        // Failed to connect
        return std::vector<gf::Vector2i>();
    }

    void BasicWorldGenerator::connectPathPoints(const World& world, const gf::Vector2i currentPoint, const gf::Vector2i nextPoint) {
        // Calculate path connection
        const std::vector<gf::Vector2i> connectionPath = getConnectionPath(world, currentPoint, nextPoint);

        gf::Vector2i placePos = currentPoint;
        for (const gf::Vector2i nextPlacePos : connectionPath) {
            if (
                const gf::Vector2i direction = sign(nextPlacePos - placePos);
                direction.y != 0
            ) {
                // Progress vertically
                if (world.getBlockManager().getBlockTypeAt(placePos) == PLATFORM_BLOCK.subType && direction.y < 0) {
                    placePos.y += direction.y;
                }
                for (; placePos.y != nextPlacePos.y; placePos.y += direction.y) {
                    world.getBlockManager().setBlockTypeAt(placePos, LADDER_BLOCK);
                }
                world.getBlockManager().setBlockTypeAt(placePos, LADDER_BLOCK);
            }
            else if (direction.x != 0) {
                // Progress horizontally
                if (
                    world.getBlockManager().getBlockTypeAt(placePos) == LADDER_BLOCK.subType
                    && world.getBlockManager().getBlockTypeAt(placePos.x, placePos.y - 1) != LADDER_BLOCK.subType
                ) {
                    placePos.x += direction.x;
                }
                for (; placePos.x != nextPlacePos.x; placePos.x += direction.x) {
                    world.getBlockManager().setBlockTypeAt(placePos, PLATFORM_BLOCK);
                }
                world.getBlockManager().setBlockTypeAt(placePos, PLATFORM_BLOCK);
            }
            placePos = nextPlacePos;
        }
    }

    /**
     * Checks if the given point is a valid spawnpoint
     * @param world The world position is in
     * @param x The x coordinate of the point
     * @param y The y coordinate of the point
     * @return true if the given position is a valid spawnpoint, false otherwise
     */
    bool isValidSpawnpoint(const World& world, const int x, const int y) {
        const BlockManager& blockManager = world.getBlockManager();

        // Return true if block is empty, and block below is solid
        return blockManager.isEmptyBlock(x, y) && BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(x, y + 1)).isCollidable;
    }

    std::optional<gf::Vector2f> BasicWorldGenerator::findValidSpawnpoint(const World& world, const gf::Vector4i room) {
        // Check each block in the room, starting at the middle at the bottom,
        // going left and right, and up
        for (int y = room.y + room.z - 1; y >= room.y; --y) {
            for (int x = room.x + room.w / 2; x < room.x + room.w; ++x) {
                if (isValidSpawnpoint(world, x, y)) {
                    return std::make_optional(BlockManager::toWorldSpace(gf::Vector2i(x, y)));
                }
            }
            for (int x = room.x + room.w / 2 - 1; x >= room.x; --x) {
                if (isValidSpawnpoint(world, x, y)) {
                    return std::make_optional(BlockManager::toWorldSpace(gf::Vector2i(x, y)));
                }
            }
        }

        // Didn't find any valid spawn point
        return std::nullopt;
    }

    bool BasicWorldGenerator::areIntersecting(const gf::Vector4i room1, const gf::Vector4i room2) {
        return room1.x + room1.w > room2.x
            && room1.x < room2.x + room2.w
            && room1.y + room1.z > room2.y
            && room1.y < room2.y + room2.z;
    }

    gf::Vector<gf::Vector2i, 2> BasicWorldGenerator::getWorldDimensions() const {
        int minX = 0;
        int maxX = 0;
        int minY = 0;
        int maxY = 0;


        // Get the minimum and maximum room positions in each axis
        for (const gf::Vector4i room : rooms) {
            if (room.x < minX) minX = room.x;
            if (room.x + room.w > maxX) maxX = room.x + room.w;
            if (room.y < minY) minY = room.y;
            if (room.y + room.z > maxY) maxY = room.y + room.z;
        }

        // Return the min and max position, plus a margin
        return gf::Vector<gf::Vector2i, 2>(
            gf::Vector2i(minX - 2, minY - 2),
            gf::Vector2i(maxX + 2, maxY + 2)
        );
    }

    void BasicWorldGenerator::generateFakePlatformsInRoom(const World& world, const gf::Vector4i room) {
        // Choose a random platform count in the room
        const int platformsCount = random.computeUniformInteger(MIN_FAKE_PLATFORM_AMOUNT, MAX_FAKE_PLATFORM_AMOUNT);

        for (int i = 0; i < platformsCount; ++i) {
            // Try to generate the platform multiple times
            for (int tries = 0; tries < MAX_FAKE_PLATFORM_GEN_TRIES; ++tries) {
                // Choose random starting point, starting from the side of a wall
                const int side = random.computeUniformInteger(0, 1);
                const int height = random.computeUniformInteger(room.y + MIN_ROOM_ENTRANCE_SIZE, room.y + room.z - 1 - MIN_ROOM_ENTRANCE_SIZE);
                const gf::Vector2i platformPos(
                    side == 0 ? room.x : room.x + room.w - 1,
                    height
                );
                // If the platform starting point is blocked, the try fails
                if (
                    !world.getBlockManager().isEmptyBlock(platformPos)
                    || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(platformPos.x, platformPos.y - 1)).isCollidable
                    || world.getBlockManager().getBlockTypeAt(platformPos.x, platformPos.y + 1) == PLATFORM_BLOCK.subType
                ) continue;

                // Make the platform grow from the starting point
                growFakePlatform(
                    world,
                    platformPos,
                    gf::Vector2i(side == 0 ? 1 : -1, 0),
                    random.computeUniformInteger(MIN_COLLECTIBLE_AMOUNT, MAX_COLLECTIBLE_AMOUNT)
                );
                break;
            }
        }
    }

    void BasicWorldGenerator::growFakePlatform(const World& world, const gf::Vector2i startPos, const gf::Vector2i direction, const int collectibleAmount) {
        // Choose random length
        const int platformLength = random.computeUniformInteger(MIN_FAKE_PLATFORM_SIZE, MAX_FAKE_PLATFORM_SIZE);

        gf::Vector2i placePos = startPos;
        int generatedPlatformLength = 0;

        // While length not reached and not blocked, grow the platform
        for (; generatedPlatformLength < platformLength; generatedPlatformLength++) {
            if (
                !world.getBlockManager().isEmptyBlock(placePos)
                || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(placePos.x, placePos.y - 1)).isCollidable
                || world.getBlockManager().getBlockTypeAt(placePos.x, placePos.y + 1) == PLATFORM_BLOCK.subType
            ) break;
            world.getBlockManager().setBlockTypeAt(placePos, PLATFORM_BLOCK);
            placePos += direction;
        }

        // Failed to grow the platform even a single block
        if (generatedPlatformLength == 0) return;


        if (collectibleAmount >= generatedPlatformLength) {
            // More collectibles to place than space on the platform
            for (int i = 0; i < generatedPlatformLength; ++i) {
                world.getBlockManager().setBlockTypeAt(startPos.x + direction.x * i, startPos.y - 1, COLLECTIBLE_BLOCK);
                ++nutCount;
            }
        }
        else {
            // Place the given amount of collectibles randomly on the platform
            for (int i = 0; i < collectibleAmount; ++i) {
                const int xPos = random.computeUniformInteger(0, generatedPlatformLength - 1);
                if (world.getBlockManager().getBlockTypeAt(startPos.x + direction.x * xPos, startPos.y - 1) == COLLECTIBLE_BLOCK.subType) {
                    --i;
                    continue;
                }
                world.getBlockManager().setBlockTypeAt(startPos.x + direction.x * xPos, startPos.y - 1, COLLECTIBLE_BLOCK);
                ++nutCount;
            }
        }

        // Choose random ladder count
        const int laddersCount = random.computeUniformInteger(MIN_FAKE_PLATFORM_LADDER_COUNT, MAX_FAKE_PLATFORM_LADDER_COUNT);

        // Place ladders on the platform
        for (int i = 0; i < laddersCount; ++i) {
            const gf::Vector2i ladderPos = startPos + direction * random.computeUniformInteger(0, generatedPlatformLength);
            // Do not place ladder next to another ladder
            if (
                world.getBlockManager().getBlockTypeAt(ladderPos) == LADDER_BLOCK.subType
                || world.getBlockManager().getBlockTypeAt(ladderPos.x - 1, ladderPos.y) == LADDER_BLOCK.subType
                || world.getBlockManager().getBlockTypeAt(ladderPos.x + 1, ladderPos.y) == LADDER_BLOCK.subType
            ) continue;
            growLadder(world, ladderPos);
        }
    }

    void BasicWorldGenerator::growLadder(const World& world, const gf::Vector2i startPos) const {
        world.getBlockManager().setBlockTypeAt(startPos, LADDER_BLOCK);
        gf::Vector2i placePos = startPos;
        placePos.y++;
        int ladder_size = 0;
        // While not blocked, grow ladder
        while (world.getBlockManager().isEmptyBlock(placePos)) {
            world.getBlockManager().setBlockTypeAt(placePos, LADDER_BLOCK);
            placePos.y++;
            ladder_size++;
            if (ladder_size >= MAX_FAKE_PLATFORM_LADDER_SIZE) break;
        }
    }

    void BasicWorldGenerator::transformBlocks(const World& world) {
        const gf::Vector<gf::Vector2i, 2> worldDimensions = getWorldDimensions();

        // Apply their ice and jelly counterpart to each block in the world
        for (int x = worldDimensions.x.x; x < worldDimensions.y.x; ++x) {
            for (int y = worldDimensions.x.y; y < worldDimensions.y.y; ++y) {
                world.getBlockManager().setBlockTypeAt(
                    x,
                    y,
                    getBlockCounterpart(
                        world.getBlockManager().getBlockTypeAt(x, y),
                        gf::Vector2i(x, y),
                        worldDimensions
                    )
                );
            }
        }
    }

    void BasicWorldGenerator::makeSomeRoomsDangerous(const World& world) {
        for (const gf::Vector4i room : rooms) {
            // Skip last room to make it always safe
            if (room == rooms.back()) continue;

            if (random.computeUniformFloat(0.0, 1.0) <= DANGEROUS_ROOM_CHANCE) makeRoomDangerous(world, room);
            placeSmallTraps(world, room);
        }
    }

    void BasicWorldGenerator::placeSmallTraps(const World& world, const gf::Vector4i room) const {
        gf::Vector2i pos{room.x, room.y + room.z - 1};

        int consecutiveIceBlocks = 0;
        int direction = 1;

        // Scan the ground once in each direction
        for (int i = 0; i < 2; ++i) {
            while (direction == 1 ? pos.x < room.x + room.w : pos.x >= room.x) {
                // Don't handle non-empty blocks
                if (!world.getBlockManager().isEmptyBlock(pos)) {
                    consecutiveIceBlocks = 0;
                    pos.x += direction;
                    continue;
                }

                if (
                    std::string blockType = world.getBlockManager().getBlockTypeAt(pos.x, pos.y + 1);
                    blockType == ICE_BLOCK.subType
                ) {
                    consecutiveIceBlocks++;
                }
                else if (consecutiveIceBlocks >= ICE_TRAP_THRESHOLD && blockType != BlockTypes::EMPTY_BLOCK) {
                    // Reached the end of the ice line, place spike
                    world.getBlockManager().setBlockTypeAt(pos, SPIKE_BLOCK);
                    break;
                }
                else consecutiveIceBlocks = 0;

                // Move position
                pos.x += direction;
            }

            // Change direction
            direction *= -1;
            consecutiveIceBlocks = 0;
            pos.x = room.x + room.w - 1;
        }
    }


    bool BasicWorldGenerator::blockSupportsSpike(const std::string& blockType) const {
        return std::any_of(
            SPIKE_SUPPORTS.cbegin(),
            SPIKE_SUPPORTS.cend(),
            [blockType](const BlockType& support) {
                return support.subType == blockType;
            }
        );
    }


    void BasicWorldGenerator::makeRoomDangerous(const World& world, const gf::Vector4i room) const {
        const int y = room.y + room.z;

        // Iterate over each block on the ground, and place a spike if possible
        for (int x = room.x; x < room.x + room.w; ++x) {
            if (!blockSupportsSpike(world.getBlockManager().getBlockTypeAt(x, y))) continue;
            if (blockSupportsSpike(world.getBlockManager().getBlockTypeAt(x, y + 1))) {
                world.getBlockManager().setBlockTypeAt(x, y, SPIKE_BLOCK);
            }
            else if (
                world.getBlockManager().isEmptyBlock(x, y - 1)
                && !blockSupportsSpike(world.getBlockManager().getBlockTypeAt(x, y - 2))
            ) {
                world.getBlockManager().setBlockTypeAt(x, y - 1, SPIKE_BLOCK);
            }
        }
    }

} // platformer
