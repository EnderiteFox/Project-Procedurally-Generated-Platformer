/**
 * A static class used to manage types of blocks.
 * Types of blocks are stored in a .xml file within the assets file
 * Destined to be used by the block manager
 */
#pragma once
#include <vector>
#include <map>

#include <blocks/BlockType.h>

namespace platformer {
    class BlockTypes {
    public:
        // Returns all the existing types of block
        static std::vector<BlockType> getAllTypes();

        // Get the data of a type of block by it's name. It's name can be found within the XML file
        static BlockType getBlockTypeByName(const std::string& name);

        static std::string EMPTY_BLOCK;
        static std::string TEST_LADDER;
        static std::string PATH;
        static std::string ICE;
        static std::string JELLY;
        static std::string TEST_BLOCK;
        static std::string PLATFORM_BLOCK;
    private:
        // Contains the data stored in the XML file once it has been parsed once, to avoid reading the file again and again.
        // If the file doesn't exists, the game crashes at launch.
        static std::map<std::string,BlockType> cache;

        // Parse a line from the XML file
        // Returns a pair of :
        //  - The tag of the line
        //  - A map matching an element of the tag with it's value
        // If the file is not well formated, creates a segmentation fault.
        static std::pair<std::string,std::map<std::string,std::string>> parseLine(std::string line);

        // Parse the XML file and store it's data in the cache
        // It's not a complete XML parser, and only works with the specific format we use for this project.
        static void parseXML();
    };
}
