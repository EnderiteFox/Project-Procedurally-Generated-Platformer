#ifndef WORLDGENERATOR_H
#define WORLDGENERATOR_H


namespace platformer {
    class World;

    class WorldGenerator {
    public:
        virtual ~WorldGenerator() = default;
        virtual void generate(World& world);
    };
}



#endif //WORLDGENERATOR_H
