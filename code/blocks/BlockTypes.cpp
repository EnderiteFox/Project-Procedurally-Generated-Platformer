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

    void BlockTypes::parseXML(){
        pugi::xml_document doc;
        assert(doc.load_file("../assets/tiles.xml"));
        std::string gfxpath = doc.child("tiles").attribute("gfxpath").value();

        for (auto it = doc.child("tiles").begin(); it != doc.child("tiles").end(); it++){
            if(it->attribute("collidable")){
                BlockType type = BlockType(
                    it->name(),
                    it->attribute("type").value(),
                    "../"+gfxpath+"/"+it->attribute("texture").value(),
                    it->attribute("staticFriction").as_float(),
                    it->attribute("dynamicFriction").as_float(),
                    it->attribute("restitution").as_float()
                );
                BlockTypes::cache.emplace(it->attribute("type").value(),type);
            }
            else{
                BlockType type = BlockType(
                    it->name(),
                    it->attribute("type").value(),
                    "../"+gfxpath+"/"+it->attribute("texture").value()
                );
                BlockTypes::cache.emplace(it->attribute("type").value(),type);
            }
        }
    }

    BlockType BlockTypes::getBlockTypeByName(std::string name){
        if(cache.empty()){
            parseXML();
        }
        return cache.find(name)->second;
    }
}
