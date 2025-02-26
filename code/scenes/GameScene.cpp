#include <gf/Vector.h>
#include <gf/Action.h>
#include <gf/SceneManager.h>
#include <scenes/GameScene.h>
#include <player/Character.h>
#include <world/World.h>
#include <gf/Scene.h>
#include <camera/Camera.h>
#include <gf/Coordinates.h>
#include <text/TextEntity.h>
#include "world/generators/basic_generator/BasicWorldGenerator.h"
#include <scenes/PlatformerManager.h>
#include <gf/Window.h>

#include <iostream>


namespace platformer{

    GameScene::GameScene(gf::Vector2i initialSize, platformer::PlatformerManager* manager):
        gf::Scene(initialSize),
        manager(manager),
        characterTexture("../assets/character_placeholder.png"),
        blockManager(initialSize),
        character({0.0f, 0.0f}, characterTexture, blockManager, this),
        world(character, blockManager, generator),
        camera(this,character,blockManager)
        {
            setWorldViewSize(initialSize);
            init();
        }

    void GameScene::init(){
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

    void GameScene::reset(){
        world.generate();
        character.resetScore();
    }

    void GameScene::doUpdate(gf::Time& time) {
        // Pause
        if (pauseAction.isActive()) {
            if(isActive()){
                manager->loadPause();
            }
            else {
                manager->popScene();
            }
        }/*
        if (isActive()) {
            //Rest of the calls
            gf::Scene::doUpdate(time);
        }*/
    }

    void GameScene::doHandleActions(gf::Window&  window){
        character.processAcceleration();

        //Adding the user's main inputs
        character.processInput();

        // Adding other impulse such as jump/dash
        character.processImpulse();
    }
}
