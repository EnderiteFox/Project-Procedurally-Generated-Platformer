#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H
#include "WorldGenerator.h"


namespace platformer {
    class TestGenerator final : public WorldGenerator {
        // Placeholder values, will be defined later
        const int ROOM_WIDTH = 10;
        const int ROOM_HEIGHT = 10;

    public:
        void generate(World& world) override;
    };
}



#endif //TESTGENERATOR_H
