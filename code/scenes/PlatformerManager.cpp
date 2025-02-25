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

    void PlatformerManager::loadMenu(){
        replaceScene(menu);
    }

    void PlatformerManager::loadPause(){
        pushScene(pause);
    }

    void PlatformerManager::loadEndScreen(){}
    void PlatformerManager::loadDeathScreen(){}

    PlatformerManager::PlatformerManager(const std::string &title, gf::Vector2i size, gf::Flags<gf::WindowHints> hints):
        charSize(30),
        font("../assets/fonts/Waffle Cake.otf"),
        gameTitle(title),
        gf::SceneManager(title,size,hints),
        gameScene(size, this),
        menu(size, this),
        pause(size){
            gf::Vector2i ScreenSize = gf::Monitor::getPrimaryMonitor().getPhysicalSize();

            // Adding the text to the pause scene
            platformer::TextEntity pauseText("The game is Paused!\nPress P or Escape to resume !",
                                     font,
                                     gf::Coordinates(ScreenSize).getCenter(),
                                     charSize);
            pauseText.setColor(gf::Color::Green);
            pauseText.setAnchor(gf::Anchor::Center);
            pause.addHudEntity(pauseText);

            pushScene(gameScene);
        }
}
