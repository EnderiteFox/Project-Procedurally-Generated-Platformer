#ifndef BASICWORLDGENERATOR_H
#define BASICWORLDGENERATOR_H
#include <vector>
#include <gf/Vector.h>

#include "WorldGenerator.h"

namespace platformer {
    class BasicWorldGenerator final : public WorldGenerator {
        // Placeholder values, will be defined later
        const int ROOM_WIDTH = 10;
        const int ROOM_HEIGHT = 10;
        const int ROOM_COUNT = 10;

        std::vector<gf::Vector2i> rooms;

        void generateRooms();

    public:
        void generate(World& world) override;
    };
}

#endif //BASICWORLDGENERATOR_H
