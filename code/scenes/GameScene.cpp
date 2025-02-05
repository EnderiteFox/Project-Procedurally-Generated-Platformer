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

#include <iostream>


namespace platformer{

    GameScene::GameScene(gf::Vector2i initialSize, PlatformerManager& manager):
        gf::Scene(initialSize),
        manager(manager){
            init();
        }

    void GameScene::init(){
        // Creating the pause actions
        pauseAction.addKeycodeKeyControl(pauseKey1);
        pauseAction.addKeycodeKeyControl(pauseKey2);
        pauseAction.setInstantaneous();
        addAction(pauseAction);

        // Loading the textures
        gf::Texture characterTexture("../assets/character_placeholder.png");

        // Creating the main entities
        platformer::BlockManager blockManager(getWorldView().getSize());
        platformer::Character character({0.0f, 0.0f}, characterTexture, blockManager, *this);
        platformer::BasicWorldGenerator generator;
        platformer::World world(character, blockManager, generator);
        platformer::Camera camera(*this,character,blockManager);

        // Adding the main entities to the map
        mainEntities.emplace("world",&world);
        mainEntities.emplace("character",&character);

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
        dynamic_cast<platformer::World*>(mainEntities["world"])->generate();
        dynamic_cast<platformer::Character*>(mainEntities["character"])->resetScore();
    }

    void GameScene::doUpdate(gf::Time& time) {

        // Pause
        if (pauseAction.isActive()) {
            if(isActive()){
                manager.loadScene("pause");
            }
            else {
                manager.popScene();
            }
        }
        if (isActive()) {
            //Rest of the calls
            gf::Scene::doUpdate(time);
        }
    }
}
