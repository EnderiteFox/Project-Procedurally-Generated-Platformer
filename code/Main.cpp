#include <iostream>
#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Texture.h>
#include <gf/ViewContainer.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <player/Character.h>
#include <world/World.h>
#include <gf/Scene.h>

#include "world/generators/basic_generator/BasicWorldGenerator.h"
#include "world/generators/TestGenerator.h"

int main() {

    // Defining useful constants for later
    static constexpr gf::Vector2i ScreenSize(1024, 576);
    static constexpr gf::Vector2f ViewSize(100.0f, 100.0f);
    static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);
    static constexpr int SAFE_FRAMES = 5;

    // Creating window and renderer
    gf::Window window("Platformer", ScreenSize);
    gf::RenderWindow renderer(window);

    gf::ExtendView WorldView(ViewCenter,ViewSize);

    renderer.clear(gf::Color::White);

    // Game scene creation
    gf::Scene gameScene(ScreenSize);
    gameScene.addView(WorldView);
    gameScene.setWorldViewCenter(ViewCenter);
    gameScene.setWorldViewSize(ViewSize);

    // Create world
    gf::Texture characterTexture("../assets/character_placeholder.png");
    platformer::BlockManager blockManager(ViewSize);
    blockManager.setViewPosition(ViewCenter);
    platformer::Character character({0.0f, 0.0f}, characterTexture, blockManager, gameScene);
    platformer::BasicWorldGenerator generator;
    //platformer::TestGenerator generator;
    platformer::World world(character, blockManager, generator);

    // Loading textures
    world.getBlockManager().loadTextures();

    // Generate world
    world.generate();

    // actions
    character.initInput();

    // clock
    gf::Clock clock;

    // View center position
    gf::Vector2f viewPos = character.getPosition();
    blockManager.setViewPosition(viewPos);

    // Adding elements to the game scene
    gameScene.addWorldEntity(blockManager);
    gameScene.addWorldEntity(character);
    gameScene.addWorldEntity(world);
    gameScene.setActive();

    // Game loop
    int framesBeforeStart = SAFE_FRAMES;
    constexpr double CAMERA_EASING = 3.5;

    while (window.isOpen()) {
        if(gameScene.isActive()){

            // 1 - inputs
            gf::Event event{};

            while (window.pollEvent(event)) {
                switch (event.type) {
                    case gf::EventType::Closed:
                        window.close();
                        break;
                    default:
                        gameScene.processEvent(event);
                }
            }

            // 2 - update
            gf::Time time = clock.restart();
            gameScene.update(time);

            // Update camera
            viewPos += (character.getPosition() - viewPos) * CAMERA_EASING * time.asSeconds();
            gameScene.setWorldViewCenter(viewPos);
            blockManager.setViewPosition(viewPos);

            // Safe frames
            if (framesBeforeStart > 0) {
                framesBeforeStart--;
                character.teleport(world.getSpawnPoint());
                gameScene.setWorldViewCenter(world.getSpawnPoint());
            }

            // 3 - render
            renderer.clear();

            gameScene.render(renderer);
            renderer.display();
        }
    }

    return 0;
}
