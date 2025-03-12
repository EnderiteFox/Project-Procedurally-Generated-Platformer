#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <scenes/Menu.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <iostream>
#include <gf/Monitor.h>
#include <gf/Rect.h>
#include <gf/Shapes.h>


namespace platformer{
    Menu::Menu(const gf::Vector2i initialSize, PlatformerManager* manager)
    : Scene(initialSize)
    , manager(manager)
    , tooltipText(
        "Press space to start",
        manager->font,
        gf::Vector2f{0.5f,0.6f},
        manager->charSize
        )
    , titleText(
        manager->gameTitle,
        manager->font,
        gf::Vector2f{0.5f,0.4f},
        manager->charSize * 3
    )
    {
        setClearColor(gf::Color::Black);

        // Adding the Actions
        startAction.addScancodeKeyControl(gameStartKey);
        startAction.setInstantaneous();
        addAction(startAction);

        quitAction.addScancodeKeyControl(quitKey);
        quitAction.setInstantaneous();
        addAction(quitAction);

        FullScreenAction.addScancodeKeyControl(FullScreenKey);
        FullScreenAction.setInstantaneous();
        addAction(FullScreenAction);

        // Parameters of the texts
        tooltipText.setColor(gf::Color::Yellow);

        titleText.setColor(gf::Color::Yellow);

        addHudEntity(titleText);
        addHudEntity(tooltipText);
    }

    void Menu::doHandleActions(gf::Window& window) {
        if (startAction.isActive()) {
            do_start = true;
        }
        if (quitAction.isActive()) {
            window.close();
        }
        if (FullScreenAction.isActive()) {
            window.toggleFullscreen();
        }
    }

    void Menu::doUpdate(gf::Time time) {
        if (do_start) {
            do_start = false;
            manager->loadGame();
        }
    }
}
