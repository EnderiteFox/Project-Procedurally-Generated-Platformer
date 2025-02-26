#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <map>
#include <scenes/PlatformerManager.h>
#include <gf/Vector.h>
#include <gf/Coordinates.h>
#include <gf/Monitor.h>

#include <iostream>

namespace platformer{

    void PlatformerManager::loadGame(){
        replaceScene(gameScene);
    }

    void PlatformerManager::loadMenu(bool replace){
        if (replace ) replaceScene(menu);
        else pushScene(menu);
    }

    void PlatformerManager::loadPause(){
        pushScene(pause);
    }

    void PlatformerManager::loadEndScreen(){}
    void PlatformerManager::loadDeathScreen(){}

    PlatformerManager::PlatformerManager(const std::string &title, gf::Vector2i size, gf::Flags<gf::WindowHints> hints):
        gf::SceneManager(title,size,hints),
        font("../assets/fonts/Waffle Cake.otf"),
        gameTitle(title),
        charSize(30),
        ScreenSize(gf::Monitor::getPrimaryMonitor().getPhysicalSize()),
        gameScene(gameViewSize, this),
        menu(gameViewSize, this),
        pause(gameViewSize){
            // Adding the text to the pause scene
            platformer::TextEntity pauseText("The game is Paused!\nPress P or Escape to resume !",
                                     font,
                                     gf::Coordinates(ScreenSize).getCenter(),
                                     charSize);
            pauseText.setColor(gf::Color::Green);
            pauseText.setAnchor(gf::Anchor::Center);
            pause.addHudEntity(pauseText);

            loadMenu(false);
        }
}
