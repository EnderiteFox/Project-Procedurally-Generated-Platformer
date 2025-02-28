#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <scenes/EndScene.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <iostream>
#include <gf/Monitor.h>

namespace platformer{
    EndScene::EndScene (gf::Vector2i initialSize, platformer::PlatformerManager* manager):
        Scene(initialSize),
        manager(manager),
        victoryText("You win !",
                    manager->font,
                    gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,-1.8f},
                    manager->charSize*3),
        deathText("You died",
                    manager->font,
                    gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,-1.8f},
                    manager->charSize*3),
        scoreText("Your score : 0",
                  manager->font,
                  gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,1.0f}, // Don't ask me why it works, i don't fucking know
                  manager->charSize),
        tooltipText("Press space to play again",
                  manager->font,
                  gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,3.0f},
                  manager->charSize*1.2){
            setClearColor(gf::Color::Black);

            // Adding the Actions
            menuAction.addScancodeKeyControl(gotoMenuKey);
            menuAction.setInstantaneous();
            addAction(menuAction);

            // Parameter of the texts
            victoryText.setColor(gf::Color::Green);
            victoryText.setAnchor(gf::Anchor::Center);

            deathText.setColor(gf::Color::Green);
            deathText.setAnchor(gf::Anchor::Center);

            scoreText.setColor(gf::Color::Green);
            scoreText.setAnchor(gf::Anchor::Center);

            tooltipText.setColor(gf::Color::Green);
            tooltipText.setAnchor(gf::Anchor::Center);

            addWorldEntity(victoryText);
            addWorldEntity(scoreText);
            addWorldEntity(deathText);
            addWorldEntity(tooltipText);
    }

    void EndScene::load(int score, bool isVictory){
        scoreText.setString("Your score : " + std::to_string(score));
        if(isVictory){
            deathText.kill();
            victoryText.setAlive();
        }
        else{
            deathText.setAlive();
            victoryText.kill();
        }
    }

    void EndScene::doHandleActions(gf::Window& window){
        if(menuAction.isActive()){
            manager->loadMenu();
        }
    }
}
