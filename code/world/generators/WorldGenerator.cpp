#include "WorldGenerator.h"

namespace platformer {
    void WorldGenerator::generate(World& world)
    {
        (void) world;
    }

    WorldGenerator::WorldGenerator(const uint64_t forcedSeed)
    : seed(forcedSeed)
    , random(forcedSeed)
    {}
}
