#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H

#include <gf/Random.h>


namespace platformer {
    class World;

    class WorldGenerator {
    protected:
        gf::Random random;

    public:
        virtual ~WorldGenerator() = default;
        virtual void generate(World& world);
    };
}



#endif //WORLDGENERATOR_H
