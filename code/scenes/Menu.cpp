#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <scenes/Menu.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <iostream>
#include <gf/Window.h>

namespace platformer{
    Menu::Menu (gf::Vector2i initialSize, PlatformerManager& manager):
        gf::Scene(initialSize),
        manager(manager){
            // Adding the Actions
            startAction.addScancodeKeyControl(gameStartKey);
            startAction.setInstantaneous();
            addAction(startAction);
    }

    void Menu::doHandleActions (gf::Window& window){
        if(startAction.isActive()){
            do_start=true;
        }
    }

    void Menu::doUpdate (gf::Time time){
        if(do_start){
            do_start=false;
            manager.loadScene("game");
        }
    }
}
