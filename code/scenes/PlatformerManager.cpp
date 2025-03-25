#include "scenes/PlatformerManager.h"

#include <gf/Monitor.h>
#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <gf/Vector.h>

namespace platformer {

    void PlatformerManager::loadGame() {
        replaceScene(gameScene);
    }

    void PlatformerManager::loadMenu(const bool replace) {
        if (replace) replaceScene(menu);
        else pushScene(menu);
    }

    void PlatformerManager::loadPause() {
        pushScene(pause);
        pause.show();
    }

    void PlatformerManager::unpause() {
        gameScene.resume();
        popScene();
    }

    void PlatformerManager::loadEndScreen(const int score, const bool isVictory) {
        gameScene.reset();
        endScreen.load(score,isVictory);
        replaceScene(endScreen);
    }

    PlatformerManager::PlatformerManager(
        const std::string &title,
        const gf::Vector2i size,
        const std::optional<uint64_t> forcedSeed,
        const gf::Flags<gf::WindowHints> hints
    )
    : SceneManager(title, size, hints)
    , font("../assets/fonts/Waffle Cake.otf")
    , gameTitle(title)
    , charSize(0.025f)
    , ScreenSize(gf::Monitor::getPrimaryMonitor().getPhysicalSize())
    , forcedSeed(forcedSeed)
    , gameScene(gameViewSize, this)
    , menu(gameViewSize, this)
    , pause(ScreenSize, this)
    , endScreen(gameViewSize,this)
    {
        loadMenu(false);
    }

    void PlatformerManager::setForcedSeed(uint64_t forcedSeed)
    {
        this->forcedSeed = std::optional(forcedSeed);
    }

    std::optional<uint64_t> PlatformerManager::getForcedSeed() const
    {
        return this->forcedSeed;
    }


}
