#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <scenes/EndScene.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <gf/Monitor.h>
#include <iostream>

namespace platformer{
    EndScene::EndScene (const gf::Vector2i initialSize, PlatformerManager* manager):
        Scene(initialSize),
        manager(manager),
        victoryText(
            "You win !",
            manager->font,
            gf::Vector2f{0.5f,0.4f},
            manager->charSize * 3
        ),
        deathText(
            "You died",
            manager->font,
            gf::Vector2f{0.5f,0.4f},
            manager->charSize * 3
        ),
        scoreText(
            "Your score : 0",
            manager->font,
            gf::Vector2f{0.5f,0.6f},
            manager->charSize
        ),
        tooltipText(
            "Press space to play again",
            manager->font,
            gf::Vector2f{0.5f,0.65f},
            manager->charSize*1.2
        )
    {
        setClearColor(gf::Color::Black);

        // Adding the Actions
        menuAction.addScancodeKeyControl(gotoMenuKey);
        menuAction.setInstantaneous();
        addAction(menuAction);

        // Parameter of the texts
        victoryText.setColor(gf::Color::Yellow);

        deathText.setColor(gf::Color::Yellow);

        scoreText.setColor(gf::Color::Yellow);

        tooltipText.setColor(gf::Color::Yellow);

        addHudEntity(victoryText);
        addHudEntity(scoreText);
        addHudEntity(deathText);
        addHudEntity(tooltipText);
    }

    void EndScene::load(const int score, const bool isVictory) {
        scoreText.setString("Your score : " + std::to_string(score));
        if (isVictory) {
            deathText.hide();
            victoryText.show();
        }
        else {
            deathText.show();
            victoryText.hide();
        }
    }

    void EndScene::doHandleActions(gf::Window& window) {
        (void) window;
        if (menuAction.isActive()) {
            manager->loadMenu();
        }
    }
}
