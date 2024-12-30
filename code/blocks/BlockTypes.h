/**
 * A container for all the existing block types
 * Destined to be used by the block manager
 */
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
