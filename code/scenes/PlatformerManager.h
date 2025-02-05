#pragma once
#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <map>
#include <gf/Vector.h>

namespace platformer{
    class PlatformerManager : public gf::SceneManager{
    private:
        std::map<std::string,gf::Scene*> storedScenes;
    public:
        void loadScene(std::string sceneName, bool replace=false);

        void storeScene(std::string sceneName, gf::Scene* scene);

        PlatformerManager(const std::string &title, gf::Vector2i size, gf::Flags<gf::WindowHints> hints=gf::All);
    };
}
