#include "scenes/GameScene.h"

#include <iostream>
#include <gf/Action.h>
#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <gf/Vector.h>
#include <gf/Coordinates.h>

#include "camera/Camera.h"
#include "player/Character.h"
#include "scenes/PlatformerManager.h"
#include "world/World.h"
#include "world/generators/basic_generator/BasicWorldGenerator.h"


namespace platformer {
    GameScene::GameScene(const gf::Vector2i initialSize, PlatformerManager* manager)
    : Scene(initialSize)
    , manager(manager)
    , characterTexture("../assets/character_placeholder.png")
    , scoreTexture("../assets/tiles/nut.png")
    , blockManager(initialSize)
    , character({0.0f, 0.0f}, characterTexture, blockManager, this)
    , generator()
    , world(character, blockManager, generator)
    , camera(this,character,blockManager)
    , scoreDisplay(
        "x0",
        manager->font,
        gf::Vector2f{0.015f,0.0070f},
        manager->charSize
    )
    , lifeDisplay(
        "x0",
        manager->font,
        gf::Vector2f{0.015f,0.040f},
        characterTexture,
        manager->charSize
    )
    {
        setWorldViewSize(initialSize);

        // Parameters of the texts
        scoreDisplay.setColor(gf::Color::Yellow);
        scoreDisplay.updateAnchor(gf::Anchor::TopLeft);

        lifeDisplay.setColor(gf::Color::Yellow);
        lifeDisplay.updateAnchor(gf::Anchor::TopLeft);

        init();
    }

    void GameScene::init() {
        // Creating the pause actions
        pauseAction.addKeycodeKeyControl(pauseKey1);
        pauseAction.addKeycodeKeyControl(pauseKey2);
        pauseAction.setInstantaneous();
        addAction(pauseAction);

        // Creating the fullscreen action
        FullScreenAction.addScancodeKeyControl(Menu::FullScreenKey);
        FullScreenAction.setInstantaneous();
        addAction(FullScreenAction);

        // Loading textures
        blockManager.loadTextures();

        // Generate world
        world.generate();

        // Character's actions
        character.initInput();

        // Texts displayed
        scoreDisplay.setPrefix(scoreTexture);
        scoreDisplay.setPrefixScale(0.12f);
        lifeDisplay.setPrefixScale(3);
        lifeDisplay.setPrefixRelativePosition(gf::Vector2f{0.002f,0.0375f});

        // Adding entities
        addWorldEntity(blockManager);
        addWorldEntity(character);
        addWorldEntity(world);
        addHudEntity(camera);
        addHudEntity(scoreDisplay);
        addHudEntity(lifeDisplay);
    }

    void GameScene::reset() {
        world.generate();
        character.resetScore();
        character.resetLives();
    }

    void GameScene::doHandleActions(gf::Window& window) {
        if (pauseAction.isActive() && !isPaused()) {
            manager->loadPause();
            pause();
            return;
        }

        if (FullScreenAction.isActive()) {
            window.toggleFullscreen();
        }

        character.processAcceleration();

        //Adding the user's main inputs
        character.processInput();

        // Adding other impulse such as jump/dash
        character.processImpulse();
    }

    void GameScene::doUpdate(gf::Time time) {
        scoreDisplay.setString("x" + std::to_string(character.getScore()));
        lifeDisplay.setString("x" + std::to_string(character.getLives()));
    }

    void GameScene::endGame() const {
        manager->loadEndScreen(character.getScore(),character.getLives()>0);
    }
}
