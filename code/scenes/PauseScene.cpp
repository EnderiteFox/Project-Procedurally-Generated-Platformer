#include <gf/Action.h>
#include <scenes/PlatformerManager.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>
#include <gf/Vector.h>
#include <scenes/GameScene.h>
#include <gf/Monitor.h>

namespace platformer {
    PauseScene::PauseScene(const gf::Vector2i initialSize, PlatformerManager* manager)
    : Scene(initialSize)
    , manager(manager)
    , pauseText(
        "The game is Paused!\nPress P or Escape to resume !",
        manager->font,
        gf::Coordinates(initialSize).getCenter(),
        manager->charSize
    )
    {
        // Parameters of the text
        pauseText.setColor(gf::Color::Yellow);
        pauseText.setTextAnchor(gf::Anchor::Center);
        addHudEntity(pauseText);

        // Adding the actions
        unpauseAction.addKeycodeKeyControl(GameScene::pauseKey1);
        unpauseAction.addKeycodeKeyControl(GameScene::pauseKey2);
        unpauseAction.setInstantaneous();
        addAction(unpauseAction);
    }

    void PauseScene::doHandleActions(gf::Window& window) {
        if (unpauseAction.isActive()) {
            manager->unpause();
            hide();
        }
    }
}
