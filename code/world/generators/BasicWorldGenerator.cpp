#include "BasicWorldGenerator.h"

#include <iostream>
#include <blocks/BlockTypes.h>
#include <world/World.h>

namespace platformer {
    void BasicWorldGenerator::generate(World& world) {
        std::cout << "Generating seed " << seed << "\n";
        generateRooms();
        fillWorld(world);
        carveRooms(world);
        generatePath();
        debugPath(world);
        for (const gf::Vector2i room : rooms) {
            if (const std::optional<gf::Vector2f> spawnpoint = findValidSpawnpoint(world, room)) {
                world.setSpawnPoint(spawnpoint.value());
                break;
            }
        }
        world.setSpawnPoint(gf::Vector2f(
            (rooms.front().x + ROOM_WIDTH / 2) * world.getBlockManager().BLOCK_SIZE,
            (rooms.front().y + ROOM_HEIGHT) * world.getBlockManager().BLOCK_SIZE
        ));
    }

    void BasicWorldGenerator::generateRooms() {
        rooms.push_back(gf::Vector2i{0, 0});

        for (int i = 1; i < ROOM_COUNT; ++i) {
            gf::Vector2i top = {rooms.back().x, rooms.back().y - ROOM_HEIGHT};
            gf::Vector2i right = {rooms.back().x + ROOM_WIDTH, rooms.back().y};
            gf::Vector2i bottom = {rooms.back().x, rooms.back().y + ROOM_HEIGHT};
            gf::Vector2i left = {rooms.back().x - ROOM_WIDTH, rooms.back().y};

            std::vector<gf::Vector2i> possibleDirections;
            if (std::find(rooms.cbegin(), rooms.cend(), top) == rooms.cend()) {
                possibleDirections.push_back(top);
            }
            if (std::find(rooms.cbegin(), rooms.cend(), right) == rooms.cend()) {
                possibleDirections.push_back(right);
            }
            if (std::find(rooms.cbegin(), rooms.cend(), bottom) == rooms.cend()) {
                possibleDirections.push_back(bottom);
            }
            if (std::find(rooms.cbegin(), rooms.cend(), left) == rooms.cend()) {
                possibleDirections.push_back(left);
            }

            rooms.push_back(
                possibleDirections.at(
                    random.computeUniformInteger(0, static_cast<int>(possibleDirections.size() - 1))
                )
            );
        }
    }

    void BasicWorldGenerator::fillWorld(World& world) {
        int minX = 0;
        int maxX = 0;
        int minY = 0;
        int maxY = 0;

        for (const gf::Vector2i room : rooms) {
            if (room.x < minX) minX = room.x;
            if (room.x + ROOM_WIDTH > maxX) maxX = room.x + ROOM_WIDTH;
            if (room.y < minY) minY = room.y;
            if (room.y + ROOM_HEIGHT > maxY) maxY = room.y + ROOM_HEIGHT;
        }

        for (int x = minX - 1; x <= maxX; ++x) {
            for (int y = minY - 1; y <= maxY; ++y) {
                world.getBlockManager().setBlockTypeAt(x, y, WALL_BLOCK);
            }
        }
        world.setVoidHeight((maxY + 2) * world.getBlockManager().BLOCK_SIZE);
    }

    void BasicWorldGenerator::carveRooms(const World& world) {
        for (const gf::Vector2i room : rooms) {
            for (int x = room.x; x < room.x + ROOM_WIDTH; ++x) {
                for (int y = room.y; y < room.y + ROOM_HEIGHT; ++y) {
                    world.getBlockManager().removeBlockAt(x, y);
                }
            }
        }
    }


    void BasicWorldGenerator::generatePath() {
        for (const gf::Vector2i room : rooms) {
            path.push_back(gf::Vector2i{
                room.x + random.computeUniformInteger(0, ROOM_WIDTH - 1),
                room.y + random.computeUniformInteger(0, ROOM_HEIGHT - 1)
            });
        }
    }

    void BasicWorldGenerator::debugPath(const World& world) {
        for (const gf::Vector2i pathPoint : path) {
            world.getBlockManager().setBlockTypeAt(pathPoint.x, pathPoint.y, BlockTypes::getBlockTypeByName("testBlock"));
        }
    }

    std::optional<gf::Vector2f> BasicWorldGenerator::findValidSpawnpoint(const World& world, const gf::Vector2i room) const {
        for (int y = room.y + ROOM_HEIGHT - 1; y >= room.y; --y) {
            for (int x = room.x + ROOM_WIDTH / 2; x < room.x + ROOM_WIDTH; ++x) {
                if (world.getBlockManager().getBlockTypeAt(x, y) == world.getBlockManager().EMPTY_BLOCK) {
                    return std::make_optional(
                        gf::Vector2f{static_cast<float>(x), static_cast<float>(y)} * world.getBlockManager().BLOCK_SIZE
                    );
                }
            }
            for (int x = room.x + ROOM_WIDTH / 2 - 1; x >= room.x; --x) {
                if (world.getBlockManager().getBlockTypeAt(x, y) == world.getBlockManager().EMPTY_BLOCK) {
                    return std::make_optional(
                        gf::Vector2f{static_cast<float>(x), static_cast<float>(y)} * world.getBlockManager().BLOCK_SIZE
                    );
                }
            }
        }
        return std::nullopt;
    }

} // platformer
