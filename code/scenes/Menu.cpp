#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <scenes/Menu.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <iostream>
#include <gf/Monitor.h>
#include <gf/Window.h>

namespace platformer{
    Menu::Menu (gf::Vector2i initialSize, PlatformerManager* manager):
        gf::Scene(initialSize),
        manager(manager){
            // Adding the Actions
            startAction.addScancodeKeyControl(gameStartKey);
            startAction.setInstantaneous();
            addAction(startAction);

            gf::Vector2i ScreenSize = gf::Monitor::getPrimaryMonitor().getPhysicalSize();

            // Adding the Texts as HUD entities
            platformer::TextEntity tooltipText("Press space to start",
                                       manager->font,
                                       gf::Coordinates(ScreenSize).getCenter() * gf::Vector2f{1.0f,1.2f},
                                       manager->charSize);
            tooltipText.setColor(gf::Color::Green);
            tooltipText.setAnchor(gf::Anchor::Center);

            platformer::TextEntity titleText(manager->gameTitle,
                                     manager->font,
                                     gf::Coordinates(ScreenSize).getCenter() * gf::Vector2f{1.0f,0.5f},
                                     manager->charSize*3);
            titleText.setColor(gf::Color::Green);
            titleText.setAnchor(gf::Anchor::Center);
            addHudEntity(titleText);
            addHudEntity(tooltipText);

            // Setting the clear color
            setClearColor(gf::Color::Black);
    }

    void Menu::doHandleActions (gf::Window& window){
        if(startAction.isActive()){
            do_start=true;
        }
    }

    void Menu::doUpdate (gf::Time time){
        if(do_start){
            do_start=false;
            manager->loadGame();
        }
    }
}
