#include <gf/Action.h>
#include <gf/SceneManager.h>
#include <scenes/Menu.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>

namespace platformer{
    Menu::Menu (gf::Vector2i initialSize, gf::SceneManager& manager, std::string gameName):
        gf::Scene(initialSize),
        manager(manager),
        font("../assets/fonts/Waffle Cake.otf"){

            // Adding the texts
            gf::Coordinates coordinates(initialSize);
            platformer::TextEntity titleText(gameName,
                                             font,
                                             coordinates.getCenter() * gf::Vector2f{1.0f,0.5f},
                                             charSize*2);
            addHudEntity(titleText);
            platformer::TextEntity tooltipText(std::string("Press space to start"),
                                               font,
                                               coordinates.getCenter() * gf::Vector2f{1.0f,1.5f},
                                               charSize);
            addHudEntity(titleText);

            // Adding the Actions
            startAction.addKeycodeKeyControl(gameStartKey);
            startAction.setInstantaneous();
            addAction(startAction);
    }

    void Menu::doUpdate(gf::Time& time){
        if(startAction.isActive()){
            // Creating the game scene, initializing it and replacing the menu by it
            platformer::GameScene gameScene(getWorldView().getSize(),manager);

            platformer::TextEntity pauseText("The game is Paused!\nPress P or Escape to resume !",
                                             font,
                                             gf::Coordinates(getWorldView().getSize()).getCenter(),
                                             charSize);
            gameScene.init(pauseText);
            manager.replaceAllScenes(gameScene);
        }
    }
}
