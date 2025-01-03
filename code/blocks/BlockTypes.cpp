#include <blocks/BlockTypes.h>

namespace platformer {
    std::vector<BlockType> BlockTypes::getAllTypes() {
        return {
            TEST_BLOCK
        };
    }

    BlockType BlockTypes::TEST_BLOCK = BlockType("testBlock", "../assets/tiles/tile_placeholder.png");
}
