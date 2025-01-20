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

    renderer.clear(gf::Color::White);

    // views
    gf::ViewContainer views;

    gf::ExtendView mainView(ViewCenter, ViewSize);
    views.addView(mainView);

    views.setInitialFramebufferSize(ScreenSize);

    // Create world
    gf::Texture characterTexture("../assets/character_placeholder.png");
    platformer::BlockManager blockManager(&mainView);
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

    // Game loop
    int framesBeforeStart = SAFE_FRAMES;

    while (window.isOpen()) {
        constexpr double CAMERA_EASING = 2;

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
        world.update(time);

        // Update camera
        viewPos += (character.getPosition() - viewPos) * CAMERA_EASING * time.asSeconds();
        mainView.setCenter(viewPos);

        // Safe frames
        if (framesBeforeStart > 0) {
            framesBeforeStart--;
            character.teleport(world.getSpawnPoint());
            mainView.setCenter(world.getSpawnPoint());
        }

        // 3 - render
        renderer.clear();
        renderer.setView(mainView);

        world.render(renderer);
        renderer.display();
    }

    return 0;
}
