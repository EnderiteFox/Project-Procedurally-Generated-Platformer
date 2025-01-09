#include <blocks/BlockTypes.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <pugixml.hpp>

#include <iostream>
#include <cassert>

namespace platformer {
    std::map<std::string,BlockType> BlockTypes::cache;

    std::vector<BlockType> BlockTypes::getAllTypes() {
        if(cache.empty()){
            parseXML();
        }

        std::vector<BlockType> res;
        for(auto it = cache.begin(); it != cache.end(); it++){
            res.push_back(it->second);
        }

        return res;
    }

    std::pair<std::string,std::map<std::string,std::string>> BlockTypes::parseLine(std::string line){
        // Finding the beginning of the line
        std::size_t i = line.find("<")+1;
        size_t size = line.size();
        std::string tagName;
        std::map<std::string,std::string> elements;

        // Reading the tag of the line
        while(line[i] != '>' && line[i] != ' '){
            tagName += line[i];
            i++;
        }

        // If the line only contains the tag, returns it
        if(line[i] == '>'){
            return {tagName,elements};
        }

        //Reading the elements of the tag
        i++;
        while(line[i] != '>' && i < size){
            while(line[i]==' ') i++;
            std::string tag;
            std::string value;

            // First, the key (everything before the =)
            while (line[i] != '='){
                tag += line[i];
                i++;
            }
            i+=2;
            // The value (everything between two quotes, after the equal)
            while(line[i] != '"'){
                value += line[i];
                i++;
            }
            elements[tag] = value;
            i++;
        }
        return {tagName,elements};
    }

    void BlockTypes::parseXML(){
        // Opening the file
        std::ifstream file("../assets/tiles.xml");
        assert(file);
        std::string texturepaths = "";

        // Reading the file line by line
        for(std::string line; std::getline(file, line); ){
            if(line.find("<!--") == 0) continue; // Skipping the line if it's a comment

            auto[tagName,tags] = parseLine(line);

            // First line, which only elements should be gfxpath
            if(texturepaths == ""){
                texturepaths = tags["gfxpath"];
                continue;
            }

            if (tagName[0] == '/') continue; // Skipping the lines that closes tags. There should be only one : at the end

            BlockTypes::cache.emplace(tags["type"],BlockType(tags["type"],"../"+texturepaths+"/"+tags["texture"]));
        }
    }

    BlockType BlockTypes::getBlockTypeByName(std::string name){
        if(cache.empty()){
            parseXML();
        }
        return cache.find(name)->second;
    }
}
