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

int main() {
    // Speed added to a character by default


    // Defining useful constants for later
    static constexpr gf::Vector2i ScreenSize(1024, 576);
    static constexpr gf::Vector2f ViewSize(100.0f, 100.0f);
    static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);

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
    platformer::BlockManager blockManager;
    gf::ActionContainer actions;
    platformer::Character character({0.0f, 0.0f}, characterTexture, blockManager, actions);
    platformer::World world(character, blockManager);

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

        // 3 - render
        renderer.clear();
        renderer.setView(mainView);

        world.render(renderer);
        renderer.display();
    }

    return 0;
}
