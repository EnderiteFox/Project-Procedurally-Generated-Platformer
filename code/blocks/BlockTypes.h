/**
 * A container for all the existing block types
 * Destined to be used by the block manager
 */
#pragma once
#include <vector>
#include <map>

#include <blocks/BlockType.h>

namespace platformer {
    class BlockTypes {
    public:
        static std::vector<BlockType> getAllTypes();

        static BlockType getBlockTypeByName(std::string name);

    private:

        static std::map<std::string,BlockType> cache;

        static std::pair<std::string,std::map<std::string,std::string>> parseLine(std::string line);

        static void parseXML();
    };
}
