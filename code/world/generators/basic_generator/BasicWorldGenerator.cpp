#include "BasicWorldGenerator.h"

#include <iostream>
#include <blocks/BlockTypes.h>
#include <world/World.h>

namespace platformer {
    BasicWorldGenerator::BasicWorldGenerator(): WorldGenerator() {
    }

    BasicWorldGenerator::BasicWorldGenerator(const uint64_t forcedSeed): WorldGenerator(forcedSeed) {
    }


    void BasicWorldGenerator::generate(World& world) {
        std::cout << "Generating seed " << seed << "\n";
        generateRooms();
        if (rooms.empty()) return;
        fillWorld(world);
        carveRooms(world);
        generatePath();

        // Connect path points
        for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i) {
            connectPathPoints(world, path.at(i), path.at(i + 1));
        }
        //debugPath(world);

        // Generate fake platforms
        for (const gf::Vector4i room : rooms) {
            generateFakePlatformsInRoom(world, room);
        }

        // Find spawn point
        for (const gf::Vector4i room : rooms) {
            if (const std::optional<gf::Vector2f> spawnpoint = findValidSpawnpoint(world, room)) {
                world.setSpawnPoint(spawnpoint.value());
                break;
            }
        }
    }

    void BasicWorldGenerator::generateRooms() {
        rooms.push_back(gf::Vector4i{
            0,
            0,
            random.computeUniformInteger(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT),
            random.computeUniformInteger(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH)
        });


        for (int i = 1; i < ROOM_COUNT; ++i) {
            if (rooms.empty()) {
                std::cerr << "Unable to generate any room\n";
                return;
            }

            if (!generateRoomNextToRoom(rooms.back())) {
                rooms.pop_back();
                i--;
            }
        }
    }

    bool BasicWorldGenerator::generateRoomNextToRoom(const gf::Vector4i previousRoom) {
        for (int sizeTries = 0; sizeTries < MAX_ROOMGEN_SIZE_TRIES; ++sizeTries) {
            gf::Vector4i room{
                0,
                0,
                random.computeUniformInteger(MIN_ROOM_HEIGHT, MAX_ROOM_HEIGHT),
                random.computeUniformInteger(MIN_ROOM_WIDTH, MAX_ROOM_WIDTH)
            };
            for (int posTries = 0; posTries < MAX_ROOMGEN_DIRECTION_TRIES; ++posTries) {
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

                if (
                    std::none_of(
                        rooms.cbegin(),
                        rooms.cend(),
                        [room](const gf::Vector4i otherRoom) {
                                return areIntersecting(room, otherRoom);
                        }
                    )
                ) {
                    rooms.push_back(room);
                    return true;
                }
            }
        }
        return false;
    }


    void BasicWorldGenerator::fillWorld(World& world) const {
        const gf::Vector<gf::Vector2i, 2> worldDimensions = getWorldDimensions();

        for (int x = worldDimensions.x.x; x < worldDimensions.y.x; ++x) {
            for (int y = worldDimensions.x.y; y < worldDimensions.y.y; ++y) {
                world.getBlockManager().setBlockTypeAt(x, y, WALL_BLOCK);
            }
        }
        world.setVoidHeight(BlockManager::toWorldSpace(worldDimensions.y.y + 1));
    }

    void BasicWorldGenerator::carveRooms(const World& world) {
        for (const gf::Vector4i room : rooms) {
            for (int x = room.x; x < room.x + room.w; ++x) {
                for (int y = room.y; y < room.y + room.z; ++y) {
                    world.getBlockManager().removeBlockAt(x, y);
                }
            }
        }
    }


    void BasicWorldGenerator::generatePath() {
        for (const gf::Vector4i room : rooms) {
            path.push_back(gf::Vector2i{
                room.x + random.computeUniformInteger(0, room.w - 1),
                room.y + random.computeUniformInteger(1, room.z - 1)
            });
        }
    }

    void BasicWorldGenerator::debugPath(const World& world) {
        for (const gf::Vector2i pathPoint : path) {
            world.getBlockManager().setBlockTypeAt(pathPoint.x, pathPoint.y, PATH);
        }
    }

    std::vector<gf::Vector2i> BasicWorldGenerator::getConnectionPath(const World& world, gf::Vector2i currentPoint, gf::Vector2i nextPoint) {
        gf::Vector2i direction = sign(nextPoint - currentPoint);
        if (direction.x == 0 && direction.y == 0) return std::vector<gf::Vector2i>();

        // Checking if starting with a ladder is possible
        std::vector<gf::Vector2i> ladderFirstPath;
        gf::Vector2i checkPos = currentPoint;
        while (checkPos != nextPoint) {
            bool firstCheck = true;
            gf::Vector2i prevCheckPos = checkPos;
            if (direction.y != 0) {
                for (; checkPos.y != nextPoint.y; checkPos.y += direction.y) {
                    if (
                        (!world.getBlockManager().isEmptyBlock(checkPos)
                        || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                        || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType)
                        && !firstCheck
                    ) {
                        checkPos.y -= direction.y;
                        break;
                    }
                    firstCheck = false;
                }
                ladderFirstPath.push_back(checkPos);
                if (checkPos == nextPoint) break;
            }
            if (direction.x != 0) {
                for (; checkPos.x != nextPoint.x; checkPos.x += direction.x) {
                    if (
                        (!world.getBlockManager().isEmptyBlock(checkPos)
                        || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                        || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType)
                        && !firstCheck
                    ) {
                        checkPos.x -= direction.x;
                        break;
                    }
                    firstCheck = false;
                }
                ladderFirstPath.push_back(checkPos);
            }
            if (prevCheckPos == checkPos) {
                ladderFirstPath.clear();
                break;
            }
        }

        // Checking if starting with a platform is possible
        std::vector<gf::Vector2i> platformFirstPath;
        checkPos = currentPoint;
        while (checkPos != nextPoint) {
            bool firstCheck = true;
            gf::Vector2i prevCheckPoint = checkPos;
            if (direction.x != 0) {
                for (; checkPos.x != nextPoint.x; checkPos.x += direction.x) {
                    if (
                        (!world.getBlockManager().isEmptyBlock(checkPos)
                        || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                        || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType)
                        && !firstCheck
                    ) {
                        checkPos.x -= direction.x;
                        break;
                    }
                    firstCheck = false;
                }
                platformFirstPath.push_back(checkPos);
                if (checkPos == nextPoint) break;
            }
            if (direction.y != 0) {
                for (; checkPos.y != nextPoint.y; checkPos.y += direction.y) {
                    if (
                        (!world.getBlockManager().isEmptyBlock(checkPos)
                        || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y - 1)).isCollidable
                        || world.getBlockManager().getBlockTypeAt(checkPos.x, checkPos.y + 1) == LADDER_BLOCK.subType)
                        && !firstCheck
                    ) {
                        checkPos.y -= direction.y;
                        break;
                    }
                    firstCheck = false;
                }
                platformFirstPath.push_back(checkPos);
            }
            if (prevCheckPoint == checkPos) {
                platformFirstPath.clear();
                break;
            }
        }

        if (!ladderFirstPath.empty() && !platformFirstPath.empty()) {
            if (random.computeUniformInteger(0, 1) == 1) return ladderFirstPath;
            return platformFirstPath;
        }
        if (!ladderFirstPath.empty()) return ladderFirstPath;
        if (!platformFirstPath.empty()) return platformFirstPath;
        return std::vector<gf::Vector2i>();
    }

    void BasicWorldGenerator::connectPathPoints(const World& world, const gf::Vector2i currentPoint, const gf::Vector2i nextPoint) {
        const std::vector<gf::Vector2i> connectionPath = getConnectionPath(world, currentPoint, nextPoint);

        gf::Vector2i placePos = currentPoint;
        for (const gf::Vector2i nextPlacePos : connectionPath) {
            if (
                const gf::Vector2i direction = sign(nextPlacePos - placePos);
                direction.y != 0
            ) {
                if (world.getBlockManager().getBlockTypeAt(placePos) == PLATFORM_BLOCK.subType && direction.y < 0) {
                    placePos.y += direction.y;
                }
                for (; placePos.y != nextPlacePos.y; placePos.y += direction.y) {
                    world.getBlockManager().setBlockTypeAt(placePos, LADDER_BLOCK);
                }
                world.getBlockManager().setBlockTypeAt(placePos, LADDER_BLOCK);
            }
            else if (direction.x != 0) {
                if (world.getBlockManager().getBlockTypeAt(placePos) == LADDER_BLOCK.subType
                    && world.getBlockManager().getBlockTypeAt(placePos.x, placePos.y - 1) != LADDER_BLOCK.subType) {
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
        return blockManager.isEmptyBlock(x, y) && BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(x, y + 1)).isCollidable;
    }

    std::optional<gf::Vector2f> BasicWorldGenerator::findValidSpawnpoint(const World& world, const gf::Vector4i room) {
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

        for (const gf::Vector4i room : rooms) {
            if (room.x < minX) minX = room.x;
            if (room.x + room.w > maxX) maxX = room.x + room.w;
            if (room.y < minY) minY = room.y;
            if (room.y + room.z > maxY) maxY = room.y + room.z;
        }
        return gf::Vector<gf::Vector2i, 2>(
            gf::Vector2i(minX - 1, minY - 1),
            gf::Vector2i(maxX + 1, maxY + 1)
        );
    }

    void BasicWorldGenerator::generateFakePlatformsInRoom(const World& world, const gf::Vector4i room) {
        const int platformsCount = random.computeUniformInteger(MIN_FAKE_PLATFORM_AMOUNT, MAX_FAKE_PLATFORM_AMOUNT);

        for (int i = 0; i < platformsCount; ++i) {
            for (int tries = 0; tries < MAX_FAKE_PLATFORM_GEN_TRIES; ++tries) {
                const int side = random.computeUniformInteger(0, 1);
                const int height = random.computeUniformInteger(room.y + MIN_ROOM_ENTRANCE_SIZE, room.y + room.z - 1 - MIN_ROOM_ENTRANCE_SIZE);
                const gf::Vector2i platformPos(
                    side == 0 ? room.x : room.x + room.w - 1,
                    height
                );
                if (
                    !world.getBlockManager().isEmptyBlock(platformPos)
                    || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(platformPos.x, platformPos.y - 1)).isCollidable
                    || world.getBlockManager().getBlockTypeAt(platformPos.x, platformPos.y + 1) == PLATFORM_BLOCK.subType
                ) continue;
                growFakePlatform(world, platformPos, gf::Vector2i(side == 0 ? 1 : -1, 0));
                break;
            }
        }
    }

    void BasicWorldGenerator::growFakePlatform(const World& world, const gf::Vector2i startPos, const gf::Vector2i direction) {
        const int platformLength = random.computeUniformInteger(MIN_FAKE_PLATFORM_SIZE, MAX_FAKE_PLATFORM_SIZE);

        gf::Vector2i placePos = startPos;
        int generatedPlatformLength = 0;
        for (; generatedPlatformLength < platformLength; generatedPlatformLength++) {
            if (
                !world.getBlockManager().isEmptyBlock(placePos)
                || BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(placePos.x, placePos.y - 1)).isCollidable
                || world.getBlockManager().getBlockTypeAt(placePos.x, placePos.y + 1) == PLATFORM_BLOCK.subType
            ) break;
            world.getBlockManager().setBlockTypeAt(placePos, PLATFORM_BLOCK);
            placePos += direction;
        }

        if (generatedPlatformLength == 0) return;

        const int laddersCount = random.computeUniformInteger(MIN_FAKE_PLATFORM_LADDER_COUNT, MAX_FAKE_PLATFORM_LADDER_COUNT);
        for (int i = 0; i < laddersCount; ++i) {
            const gf::Vector2i ladderPos = startPos + direction * random.computeUniformInteger(0, generatedPlatformLength);
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
        while (world.getBlockManager().isEmptyBlock(placePos)) {
            world.getBlockManager().setBlockTypeAt(placePos, LADDER_BLOCK);
            placePos.y++;
            ladder_size++;
            if (ladder_size >= MAX_FAKE_PLATFORM_LADDER_SIZE) break;
        }
    }
} // platformer
