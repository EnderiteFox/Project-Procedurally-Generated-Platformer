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

        // Checking if starting with a ladder is possible
        bool ladderFirstPossible = true;
        for (int y = currentPoint.y; y != nextPoint.y; y += direction.y) {
            const std::string blockType = world.getBlockManager().getBlockTypeAt(currentPoint.x, y);
            if (blockType == AIR_TYPE) continue;
            if (BlockTypes::getBlockTypeByName(blockType).isCollidable) {
                ladderFirstPossible = false;
                break;
            }
        }
        if (ladderFirstPossible) {
            for (int x = currentPoint.x; x != nextPoint.x; x += direction.x) {
                const std::string blockType = world.getBlockManager().getBlockTypeAt(x, nextPoint.y);
                if (blockType == AIR_TYPE) continue;
                if (BlockTypes::getBlockTypeByName(blockType).isCollidable) {
                    ladderFirstPossible = false;
                    break;
                }
            }
        }

        // Checking if starting with a platform is possible
        bool platformFirstPossible = true;
        for (int x = currentPoint.x; x != nextPoint.x; x += direction.x) {
            const std::string blockType = world.getBlockManager().getBlockTypeAt(x, currentPoint.y);
            if (blockType == AIR_TYPE) continue;
            if (BlockTypes::getBlockTypeByName(blockType).isCollidable) {
                platformFirstPossible = false;
                break;
            }
        }
        if (platformFirstPossible) {
            for (int y = currentPoint.y; y != nextPoint.y; y += direction.y) {
                const std::string blockType = world.getBlockManager().getBlockTypeAt(nextPoint.x, y);
                if (blockType == AIR_TYPE) continue;
                if (BlockTypes::getBlockTypeByName(blockType).isCollidable) {
                    platformFirstPossible = false;
                    break;
                }
            }
        }

        if (ladderFirstPossible && platformFirstPossible) return random.computeUniformInteger(0, 1) ? LADDER_FIRST : PLATFORM_FIRST;
        if (ladderFirstPossible && !platformFirstPossible) return LADDER_FIRST;
        if (platformFirstPossible && !ladderFirstPossible) return PLATFORM_FIRST;
        return INDIRECT;
    }

    void BasicWorldGenerator::connectPathPoints(const World& world, const gf::Vector2i currentPoint, const gf::Vector2i nextPoint) {
        const PathConnectionType connectionType = getConnectionType(world, currentPoint, nextPoint);

        const gf::Vector2i direction = sign(nextPoint - currentPoint);

        switch (connectionType) {
        case LADDER_FIRST:
            // Connect with ladder first
            if (currentPoint.y != nextPoint.y) {
                for (int y = currentPoint.y; y != nextPoint.y + direction.y * (direction.y > 0 ? 0 : 1); y += direction.y) {
                    world.getBlockManager().setBlockTypeAt(currentPoint.x, y, LADDER_BLOCK);
                }
            }
            if (currentPoint.x != nextPoint.x) {
                for (int x = currentPoint.x + direction.x * (direction.y > 0 ? 0 : 1); x != nextPoint.x + direction.x; x += direction.x) {
                    world.getBlockManager().setBlockTypeAt(x, nextPoint.y, PLATFORM_BLOCK);
                }
            }
            break;
        case PLATFORM_FIRST:
            // Connect with platform first
            if (currentPoint.x != nextPoint.y) {
                for (int x = currentPoint.x + direction.x; x != nextPoint.x + direction.x * (direction.y > 0 ? 0 : 1); x += direction.x) {
                    world.getBlockManager().setBlockTypeAt(x, currentPoint.y, PLATFORM_BLOCK);
                }
            }
            if (currentPoint.y != nextPoint.y) {
                for (int y = currentPoint.y + (direction.y < 0 ? direction.y : 0); y != nextPoint.y; y += direction.y) {
                    world.getBlockManager().setBlockTypeAt(nextPoint.x, y, LADDER_BLOCK);
                }
            }
            break;
        case INDIRECT:
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
