/**
 * Generator used for tests. Will be removed when the final game is out
 */
#ifndef TESTGENERATOR_H
#define TESTGENERATOR_H
#include "WorldGenerator.h"


namespace platformer {
    class TestGenerator final : public WorldGenerator {
    public:
        void generate(World& world) override;
    };
}



#endif //TESTGENERATOR_H
