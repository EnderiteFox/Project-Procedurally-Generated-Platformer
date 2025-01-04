#include <blocks/BlockTypes.h>
#include <string>
#include <vector>
#include <map>
#include <fstream>

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
        std::size_t i = line.find("<")+1;
        size_t size = line.size();
        std::string tagName;
        std::map<std::string,std::string> elements;
        // Tag
        while(line[i] != '>' && line[i] != ' '){
            tagName += line[i];
            i++;
        }
        if(line[i] == '>'){
            return {tagName,elements};
        }
        i++;
        // Rest of the elements
        while(line[i] != '>' && i < size){
            while(line[i]==' ') i++;
            std::string tag;
            std::string value;
            while (line[i] != '='){
                tag += line[i];
                i++;
            }
            i+=2;
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
        std::ifstream file("../assets/tiles.xml");
        assert(file);
        std::string texturepaths = "";
        for(std::string line; std::getline(file, line); )
        {
            if(line.find("<!--") == 0) continue;


            auto[tagName,tags] = parseLine(line);

            if(texturepaths == ""){
                texturepaths = tags["gfxpath"];
                continue;
            }

            if (tagName[0] == '/') continue;

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
