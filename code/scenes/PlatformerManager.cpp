#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <map>
#include <scenes/PlatformerManager.h>
#include <gf/Vector.h>

#include <iostream>

namespace platformer{
    void PlatformerManager::loadScene(std::string sceneName, bool replace){
        if(replace){
            replaceScene(*(storedScenes[sceneName]));
        }
        else{
            pushScene(*(storedScenes[sceneName]));
        }
    }

    void PlatformerManager::storeScene(std::string sceneName, gf::Scene* scene){
        storedScenes.emplace(sceneName,scene);
    }

    PlatformerManager::PlatformerManager(const std::string &title, gf::Vector2i size, gf::Flags<gf::WindowHints> hints):
        gf::SceneManager(title,size,hints){}
}
