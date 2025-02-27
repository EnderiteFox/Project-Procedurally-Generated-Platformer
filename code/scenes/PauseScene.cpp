#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <scenes/Menu.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <iostream>
#include <gf/Monitor.h>

namespace platformer{
    PauseScene::PauseScene (gf::Vector2i initialSize, platformer::PlatformerManager* manager):
        Scene(initialSize),
        manager(manager),
        pauseText("The game is Paused!\nPress P or Escape to resume !",
                    manager->font,
                    gf::Coordinates(initialSize).getCenter(),
                    manager->charSize){
            // Parameters of the text
            pauseText.setColor(gf::Color::Green);
            pauseText.setAnchor(gf::Anchor::Center);
            addHudEntity(pauseText);

            // Adding the actions
            unpauseAction.addKeycodeKeyControl(GameScene::pauseKey1);
            unpauseAction.addKeycodeKeyControl(GameScene::pauseKey2);
            unpauseAction.setInstantaneous();
            addAction(unpauseAction);
        }

        void PauseScene::doHandleActions (gf::Window& window){
            if(unpauseAction.isActive()){
                manager->unPause();
                hide();
                return;
            }
        }
}
