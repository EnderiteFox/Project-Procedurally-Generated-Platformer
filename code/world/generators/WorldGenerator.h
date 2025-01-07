#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <gf/Random.h>


namespace platformer {
    class World;

    class WorldGenerator {
        gf::Random seedRandom;
    protected:
        uint64_t seed = seedRandom.computeId();
        gf::Random random{seed};

    public:
        virtual ~WorldGenerator() = default;
        virtual void generate(World& world);
    };
}



#endif //WORLDGENERATOR_H
