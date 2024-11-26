#include <iostream>
#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Event.h>
#include <gf/EntityContainer.h>
#include <gf/Entity.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Action.h>
#include <gf/Views.h>
#include <gf/ViewContainer.h>
#include <gf/Texture.h>
#include "Block.h"
#include "Character.h"

int main() {
    // Speed added to a character by default
    const float speed = 10.0f;

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

    // Loading textures
    gf::Texture blockTexture("../assets/tile_placeholder.png");
    gf::Texture characterTexture("../assets/character_placeholder.png");

    // entities
    gf::EntityContainer mainEntities;

    platformer::Block block({10.0f,10.0f},blockTexture);
    mainEntities.addEntity(block);

    platformer::Character character({5.0f,5.0f},characterTexture);
    mainEntities.addEntity(character);

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

        gf::Vector2f charSpeed{0.0f,0.0f};
        if (rightAction.isActive()) {
            charSpeed.x+= speed;
        }else if (leftAction.isActive()) {
            charSpeed.x-= speed;
        }

        if (upAction.isActive()) {
            charSpeed.y-= speed;
        } else if (downAction.isActive()) {
            charSpeed.y+= speed;
        }
        character.setSpeed(charSpeed);

        // 2 - update
        gf::Time time = clock.restart();
        mainEntities.update(time);

        // 3 - render
        renderer.clear();
        renderer.setView(mainView);

        mainEntities.render(renderer);
        renderer.display();
    }

    return 0;
}
