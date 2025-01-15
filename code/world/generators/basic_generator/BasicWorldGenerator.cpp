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
        debugPath(world);

        // Connect path points
        for (int i = 0; i < static_cast<int>(path.size()) - 1; ++i) {
            connectPathPoints(world, path.at(i), path.at(i + 1));
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


    void BasicWorldGenerator::fillWorld(World& world) {
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

        for (int x = minX - 1; x <= maxX; ++x) {
            for (int y = minY - 1; y <= maxY; ++y) {
                world.getBlockManager().setBlockTypeAt(x, y, WALL_BLOCK);
            }
        }
        world.setVoidHeight(BlockManager::toWorldSpace(maxY + 2));
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
                room.y + random.computeUniformInteger(0, room.z - 1)
            });
        }
    }

    void BasicWorldGenerator::debugPath(const World& world) {
        for (const gf::Vector2i pathPoint : path) {
            world.getBlockManager().setBlockTypeAt(pathPoint.x, pathPoint.y, PATH);
        }
    }

    PathConnectionType BasicWorldGenerator::getConnectionType(const World& world, const gf::Vector2i currentPoint, const gf::Vector2i nextPoint) {
        const gf::Vector2i direction = sign(nextPoint - currentPoint);
        if (direction.x == 0 && direction.y == 0) return CANT_CONNECT;

        // Checking if starting with a ladder is possible
        bool ladderFirstPossible = true;
        gf::Vector2i checkPos = currentPoint;
        while (checkPos != nextPoint) {
            gf::Vector2i prevCheckPos = checkPos;
            if (direction.y != 0) {
                for (; checkPos.y != nextPoint.y; checkPos.y += direction.y) {
                    if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos)).isCollidable) {
                        checkPos.y -= direction.y;
                        break;
                    }
                }
                if (checkPos == nextPoint) break;
            }
            if (direction.x != 0) {
                for (; checkPos.x != nextPoint.x; checkPos.x += direction.x) {
                    if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos)).isCollidable) {
                        checkPos.x -= direction.x;
                        break;
                    }
                }
            }
            if (prevCheckPos == checkPos) {
                ladderFirstPossible = false;
                break;
            }
        }

        // Checking if starting with a platform is possible
        bool platformFirstPossible = true;
        checkPos = currentPoint;
        while (checkPos != nextPoint) {
            gf::Vector2i prevCheckPoint = checkPos;
            if (direction.x != 0) {
                for (; checkPos.x != nextPoint.x; checkPos.x += direction.x) {
                    if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos)).isCollidable) {
                        checkPos.x -= direction.x;
                        break;
                    }
                }
                if (checkPos == nextPoint) break;
            }
            if (direction.y != 0) {
                for (; checkPos.y != nextPoint.y; checkPos.y += direction.y) {
                    if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(checkPos)).isCollidable) {
                        checkPos.y -= direction.y;
                        break;
                    }
                }
            }
            if (prevCheckPoint == checkPos) {
                platformFirstPossible = false;
                break;
            }
        }

        if (ladderFirstPossible && platformFirstPossible) return random.computeUniformInteger(0, 1) ? LADDER_FIRST : PLATFORM_FIRST;
        if (ladderFirstPossible) return LADDER_FIRST;
        if (platformFirstPossible) return PLATFORM_FIRST;
        return CANT_CONNECT;
    }

    void BasicWorldGenerator::connectPathPoints(const World& world, const gf::Vector2i currentPoint, const gf::Vector2i nextPoint) {
        const PathConnectionType connectionType = getConnectionType(world, currentPoint, nextPoint);

        const gf::Vector2i direction = sign(nextPoint - currentPoint);
        if (direction.x == 0 && direction.y == 0) return;

        switch (connectionType) {
        case LADDER_FIRST: {
            gf::Vector2i placePos = currentPoint;
            std::vector<gf::Vector2i> platformPos;
            std::vector<gf::Vector2i> ladderPos;
            while (placePos != nextPoint) {
                if (direction.y != 0) {
                    for (; placePos.y != nextPoint.y; placePos.y += direction.y) {
                        if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(placePos)).isCollidable) {
                            placePos.y -= direction.y;
                            break;
                        }
                        ladderPos.push_back(placePos);
                    }
                    if (placePos == nextPoint) break;
                }
                if (direction.x != 0) {
                    for (; placePos.x != nextPoint.x; placePos.x += direction.x) {
                        if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(placePos)).isCollidable) {
                            placePos.x -= direction.x;
                            break;
                        }
                        platformPos.push_back(placePos);
                    }
                }
            }
            for (gf::Vector2i pos : platformPos) world.getBlockManager().setBlockTypeAt(pos, WALL_BLOCK);
            for (gf::Vector2i pos : ladderPos) world.getBlockManager().setBlockTypeAt(pos, LADDER_BLOCK);
            break;
        }
        case PLATFORM_FIRST: {
            gf::Vector2i placePos = currentPoint;
            std::vector<gf::Vector2i> platformPos;
            std::vector<gf::Vector2i> ladderPos;
            while (placePos != nextPoint) {
                if (direction.x != 0) {
                    for (; placePos.x != nextPoint.x; placePos.x += direction.x) {
                        if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(placePos)).isCollidable) {
                            placePos.x -= direction.x;
                            break;
                        }
                        platformPos.push_back(placePos);
                    }
                    if (placePos == nextPoint) break;
                }
                if (direction.y != 0) {
                    for (; placePos.y != nextPoint.y; placePos.y += direction.y) {
                        if (BlockTypes::getBlockTypeByName(world.getBlockManager().getBlockTypeAt(placePos)).isCollidable) {
                            placePos.y -= direction.y;
                            break;
                        }
                        ladderPos.push_back(placePos);
                    }
                }
            }
            for (gf::Vector2i pos : platformPos) world.getBlockManager().setBlockTypeAt(pos, WALL_BLOCK);
            for (gf::Vector2i pos : ladderPos) world.getBlockManager().setBlockTypeAt(pos, LADDER_BLOCK);
            break;
        }
        case CANT_CONNECT:
            // Will do something later, currently fails to connect points
            break;
        }
    }

    bool isValidSpawnpoint(const World& world, const int x, const int y) {
        const BlockManager& blockManager = world.getBlockManager();
        return blockManager.isEmptyBlock(x, y) && !blockManager.isEmptyBlock(x, y + 1);
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


} // platformer
