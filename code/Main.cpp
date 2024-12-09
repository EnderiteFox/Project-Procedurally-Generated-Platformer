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
#include <physics/Physics.h>

int main() {
    // Speed added to a character by default
    const float speed = 3.0f;

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
    platformer::Character character({8.0f,-20.0f}, characterTexture);
    platformer::World world(character);

    // Loading textures
    world.getBlockManager().loadTextures();

    // Generate world
    world.generate();

    // actions
    gf::ActionContainer actions;

    gf::Action leftAction("Left");
    leftAction.addScancodeKeyControl(gf::Scancode::Q);
    leftAction.addScancodeKeyControl(gf::Scancode::Left);
    leftAction.setContinuous();
    actions.addAction(leftAction);

    gf::Action rightAction("Right");
    rightAction.addScancodeKeyControl(gf::Scancode::D);
    rightAction.addScancodeKeyControl(gf::Scancode::Right);
    rightAction.setContinuous();
    actions.addAction(rightAction);

    //Will probably be removed in profit of a "jump" button once we add gravity and collisions
    gf::Action upAction("Up");
    upAction.addScancodeKeyControl(gf::Scancode::Z);
    upAction.addScancodeKeyControl(gf::Scancode::Up);
    upAction.setContinuous();
    actions.addAction(upAction);

    gf::Action downAction("Down");
    downAction.addScancodeKeyControl(gf::Scancode::S);
    downAction.addScancodeKeyControl(gf::Scancode::Down);
    downAction.setContinuous();
    actions.addAction(downAction);

    // clock
    gf::Clock clock;

    // View center position
    const double CAMERA_EASING = 2;
    gf::Vector2f viewPos = character.getPosition();

    // Game loop

    while (window.isOpen()) {
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

        gf::Vector2f charSpeed{0.0f,0.0f};
        if (rightAction.isActive()) {
            charSpeed.x += 1;
        } else if (leftAction.isActive()) {
            charSpeed.x -= 1;
        }

        if (upAction.isActive()) {
            charSpeed.y -= 1;
        } else if (downAction.isActive()) {
            charSpeed.y += 1;
        }
        //Initial speed determination
        character.setSpeed(
            character.getSpeed() + (charSpeed.x != 0 || charSpeed.y != 0 ? normalize(charSpeed) : charSpeed) * speed
        );
        //Collision determination
        std::pair<bool,gf::Vector2f> collisions = platformer::Physics::collide(character,world.getBlockManager().getNearbyHitboxes(character.getPosition()));
        character.setSpeed(character.getSpeed() + collisions.second*speed);

        gf::Time time = clock.restart();
        world.getEntityContainer().update(time);

        // Update camera
        viewPos += (character.getPosition() - viewPos) * CAMERA_EASING * time.asSeconds();
        mainView.setCenter(viewPos);

        // 3 - render
        renderer.clear();
        renderer.setView(mainView);

        world.getEntityContainer().render(renderer);
        renderer.display();
    }

    return 0;
}
