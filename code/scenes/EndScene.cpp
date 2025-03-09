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
            gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,-1.8f},
            manager->charSize * 3
        ),
        deathText(
            "You died",
            manager->font,
            gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,-1.8f},
            manager->charSize * 3
        ),
        scoreText(
            "Your score : 0",
            manager->font,
            gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,1.0f}, // Don't ask me why it works, i don't fucking know
            manager->charSize
        ),
        tooltipText(
            "Press space to play again",
            manager->font,
            gf::Coordinates(initialSize).getCenter() * gf::Vector2f{1.0f,3.0f},
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
        victoryText.setTextAnchor(gf::Anchor::Center);

        deathText.setColor(gf::Color::Yellow);
        deathText.setTextAnchor(gf::Anchor::Center);

        scoreText.setColor(gf::Color::Yellow);
        scoreText.setTextAnchor(gf::Anchor::Center);

        tooltipText.setColor(gf::Color::Yellow);
        tooltipText.setTextAnchor(gf::Anchor::Center);

        addWorldEntity(victoryText);
        addWorldEntity(scoreText);
        addWorldEntity(deathText);
        addWorldEntity(tooltipText);
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
        if (menuAction.isActive()) {
            manager->loadMenu();
        }
    }
}
