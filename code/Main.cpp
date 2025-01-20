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

    // Scene creation and handling
    gf::Scene defaultScene(ScreenSize);
    defaultScene.addView(WorldView);
    defaultScene.setWorldViewCenter(ViewCenter);
    defaultScene.setWorldViewSize(ViewSize);

    // Create world
    gf::Texture characterTexture("../assets/character_placeholder.png");
    platformer::BlockManager blockManager(ViewSize);
    blockManager.setViewPosition(ViewCenter);
    gf::ActionContainer actions;
    platformer::Character character({0.0f, 0.0f}, characterTexture, blockManager, actions);
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

    // Adding elements to the scene
    defaultScene.addWorldEntity(blockManager);
    defaultScene.addWorldEntity(character);
    defaultScene.addWorldEntity(world);


    // Game loop
    int framesBeforeStart = SAFE_FRAMES;

    while (window.isOpen()) {
        constexpr double CAMERA_EASING = 3.5;

        // 1 - inputs
        gf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case gf::EventType::Closed:
                    window.close();
                    break;
                default:
                    actions.processEvent(event);
            }
        }

        // 2 - update
        gf::Time time = clock.restart();
        defaultScene.update(time);

        // Update camera
        viewPos += (character.getPosition() - viewPos) * CAMERA_EASING * time.asSeconds();
        defaultScene.setWorldViewCenter(viewPos);
        blockManager.setViewPosition(viewPos);

        // Safe frames
        if (framesBeforeStart > 0) {
            framesBeforeStart--;
            character.teleport(world.getSpawnPoint());
            defaultScene.setWorldViewCenter(world.getSpawnPoint());
        }

        // 3 - render
        renderer.clear();

        defaultScene.render(renderer);
        renderer.display();
    }

    return 0;
}
