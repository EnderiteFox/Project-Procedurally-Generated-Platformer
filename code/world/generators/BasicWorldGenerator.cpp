#include "BasicWorldGenerator.h"

#include <iostream>

namespace platformer {
    void BasicWorldGenerator::generate(World& world) {
        generateRooms();
        for (const gf::Vector2i room : rooms) std::cout << room.x << ", " << room.y << "\n";
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


} // platformer