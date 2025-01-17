/**
 * This class define what a world generator is.
 * It's more or less an interface, destined to be inherited and not used as it is.
 *
 * If the generator is random, it uses a seed. The seed can be random or given as a parameter of the constructor.
 */

#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <gf/Random.h>


namespace platformer {
    class World;

    class WorldGenerator {
        gf::Random seedRandom;
    protected:
        explicit WorldGenerator() = default;
        explicit WorldGenerator(uint64_t forcedSeed);

        uint64_t seed = seedRandom.computeId();
        gf::Random random{seed};

    public:
        virtual ~WorldGenerator() = default;
        virtual void generate(World& world);
    };
}



#endif //WORLDGENERATOR_H
