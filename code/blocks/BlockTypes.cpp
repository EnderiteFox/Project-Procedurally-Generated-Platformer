#include <blocks/BlockTypes.h>
#include <string>
#include <vector>
#include <map>
#include <pugixml.hpp>

#include <cassert>

namespace platformer {
    std::map<std::string,BlockType> BlockTypes::cache;

    std::string BlockTypes::EMPTY_BLOCK = "empty";
    std::string BlockTypes::TEST_BLOCK = "testBlock";
    std::string BlockTypes::TEST_LADDER = "testLadder";
    std::string BlockTypes::PATH = "pathBlock";
    std::string BlockTypes::ICE = "iceBlock";
    std::string BlockTypes::JELLY = "jellyBlock";
    std::string BlockTypes::PLATFORM_BLOCK = "testPlatform";

    std::vector<BlockType> BlockTypes::getAllTypes() {
        if(cache.empty()){
            parseXML();
        }

        std::vector<BlockType> res;
        for(auto it = cache.begin(); it != cache.end(); ++it){
            res.push_back(it->second);
        }

        return res;
    }

    void BlockTypes::parseXML(){
        pugi::xml_document doc;
        assert(doc.load_file("../assets/tiles.xml"));
        const std::string gfxpath = doc.child("tiles").attribute("gfxpath").value();

        for (auto it = doc.child("tiles").begin(); it != doc.child("tiles").end(); ++it) {
            if (it->attribute("collidable").as_bool()) {
                if(it->attribute("direction")){
                    auto type = BlockType(
                        it->name(),
                        it->attribute("type").value(),
                        "../"+gfxpath+"/"+it->attribute("texture").value(),
                        it->attribute("staticFriction").as_float(),
                        it->attribute("dynamicFriction").as_float(),
                        it->attribute("restitution").as_float(),
                        gf::Vector2f(
                            (it->attribute("hitboxWidth") ? it->attribute("hitboxWidth").as_float() : 1) * BlockManager::BLOCK_SIZE,
                            (it->attribute("hitboxHeight") ? it->attribute("hitboxHeight").as_float() : 1) * BlockManager::BLOCK_SIZE
                        ),
                        gf::Vector2f(
                            (it->attribute("hitboxOffsetX") ? it->attribute("hitboxOffsetX").as_float() : 0) * BlockManager::BLOCK_SIZE,
                            (it->attribute("hitboxOffsetY") ? it->attribute("hitboxOffsetY").as_float() : 0) * BlockManager::BLOCK_SIZE
                        ),
                        it->attribute("direction").value()
                    );
                    cache.emplace(it->attribute("type").value(),type);
                }
                else{
                    auto type = BlockType(
                        it->name(),
                        it->attribute("type").value(),
                        "../"+gfxpath+"/"+it->attribute("texture").value(),
                        it->attribute("staticFriction").as_float(),
                        it->attribute("dynamicFriction").as_float(),
                        it->attribute("restitution").as_float(),
                        gf::Vector2f(
                            (it->attribute("hitboxWidth") ? it->attribute("hitboxWidth").as_float() : 1) * BlockManager::BLOCK_SIZE,
                            (it->attribute("hitboxHeight") ? it->attribute("hitboxHeight").as_float() : 1) * BlockManager::BLOCK_SIZE
                        ),
                        gf::Vector2f(
                            (it->attribute("hitboxOffsetX") ? it->attribute("hitboxOffsetX").as_float() : 0) * BlockManager::BLOCK_SIZE,
                            (it->attribute("hitboxOffsetY") ? it->attribute("hitboxOffsetY").as_float() : 0) * BlockManager::BLOCK_SIZE
                        )
                    );
                    cache.emplace(it->attribute("type").value(),type);
                }
            }
            else{
                auto type = BlockType(
                    it->name(),
                    it->attribute("type").value(),
                    "../" + gfxpath + "/" + it->attribute("texture").value()
                );
                cache.emplace(it->attribute("type").value(),type);
            }
        }

        BlockType empty = BlockType(
            "empty",
            "empty",
            "../" + gfxpath + "/"
        );
        cache.emplace("empty",empty);
    }

    BlockType BlockTypes::getBlockTypeByName(const std::string& name) {
        if (cache.empty()) {
            parseXML();
        }
        return cache.find(name)->second;
    }
}
