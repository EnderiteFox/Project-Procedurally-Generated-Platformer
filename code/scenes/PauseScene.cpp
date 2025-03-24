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
        gf::Vector2f{0.5f,0.5f},
        manager->charSize*1.5
    )
    {
        // Parameters of the text
        pauseText.setColor(gf::Color::Yellow);
        pauseText.setAlignment(gf::Alignment::Center);
        pauseText.setParagraphWidth(650);
        addHudEntity(pauseText);

        // Adding the actions
        unpauseAction.addKeycodeKeyControl(GameScene::pauseKey1);
        unpauseAction.addKeycodeKeyControl(GameScene::pauseKey2);
        unpauseAction.setInstantaneous();
        addAction(unpauseAction);
    }

    void PauseScene::doHandleActions(gf::Window& window) {
        (void) window;
        if (unpauseAction.isActive()) {
            manager->unpause();
            hide();
        }
    }
}
