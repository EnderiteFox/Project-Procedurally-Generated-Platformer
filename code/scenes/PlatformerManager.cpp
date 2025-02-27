#include "scenes/PlatformerManager.h"

#include <gf/Coordinates.h>
#include <gf/Monitor.h>
#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <gf/Vector.h>

namespace platformer{

    void PlatformerManager::loadGame() {
        replaceScene(gameScene);
    }

    void PlatformerManager::loadMenu(const bool replace) {
        if (replace) replaceScene(menu);
        else pushScene(menu);
    }

    void PlatformerManager::loadPause() {
        pushScene(pause);
    }

    void PlatformerManager::loadEndScreen() {}
    void PlatformerManager::loadDeathScreen() {}

    PlatformerManager::PlatformerManager(const std::string &title, const gf::Vector2i size, const gf::Flags<gf::WindowHints> hints)
    : SceneManager(title, size, hints)
    , font("../assets/fonts/Waffle Cake.otf")
    , gameTitle(title)
    , charSize(30)
    , ScreenSize(gf::Monitor::getPrimaryMonitor().getPhysicalSize())
    , gameScene(gameViewSize, this)
    , menu(gameViewSize, this)
    , pause(gameViewSize)
    {
        // Adding the text to the pause scene
        TextEntity pauseText(
            "The game is Paused!\nPress P or Escape to resume !",
            font,
            gf::Coordinates(ScreenSize).getCenter(),
            charSize);
        pauseText.setColor(gf::Color::Green);
        pauseText.setAnchor(gf::Anchor::Center);
        pause.addHudEntity(pauseText);

        loadMenu(false);
    }
}
