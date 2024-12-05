#pragma once
#include <vector>

#include <blocks/BlockType.h>

namespace platformer {
    class BlockTypes {
    public:
        static std::vector<BlockType> getAllTypes();

        static BlockType TEST_BLOCK;
    };
}
