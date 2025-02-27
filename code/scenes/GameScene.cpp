#include "scenes/GameScene.h"

#include <iostream>
#include <gf/Action.h>
#include <gf/Scene.h>
#include <gf/SceneManager.h>
#include <gf/Vector.h>

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
    , blockManager(initialSize)
    , character({0.0f, 0.0f}, characterTexture, blockManager, this)
    , generator()//(22715912219893775u) // forced seed
    , world(character, blockManager, generator)
    , camera(this,character,blockManager)
    {
        setWorldViewSize(initialSize);
        init();
    }

    void GameScene::init() {
        // Creating the pause actions
        pauseAction.addKeycodeKeyControl(pauseKey1);
        pauseAction.addKeycodeKeyControl(pauseKey2);
        pauseAction.setInstantaneous();
        addAction(pauseAction);

        // Loading textures
        blockManager.loadTextures();

        // Generate world
        world.generate();

        // Character's actions
        character.initInput();

        // Adding entities
        addWorldEntity(blockManager);
        addWorldEntity(character);
        addWorldEntity(world);
        addHudEntity(camera);
    }

    void GameScene::reset() {
        world.generate();
        character.resetScore();
    }

    void GameScene::doHandleActions(gf::Window& window) {
        if(pauseAction.isActive() && !isPaused()){
            manager->loadPause();
            pause();
            return;
        }

        character.processAcceleration();

        //Adding the user's main inputs
        character.processInput();

        // Adding other impulse such as jump/dash
        character.processImpulse();
    }

    void GameScene::endGame(){
        manager->loadEndScreen(character.getScore(),true);
    }
}
