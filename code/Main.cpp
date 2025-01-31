#include <iostream>
#include <gf/Action.h>
#include <gf/Clock.h>
#include <gf/Color.h>
#include <gf/Event.h>
#include <gf/RenderWindow.h>
#include <gf/Texture.h>
#include <gf/Views.h>
#include <gf/Window.h>
#include <player/Character.h>
#include <world/World.h>
#include <gf/Scene.h>
#include <gf/Text.h>
#include <gf/Font.h>
#include <camera/Camera.h>
#include <text/TextEntity.h>
#include <gf/Coordinates.h>

#include "world/generators/basic_generator/BasicWorldGenerator.h"
#include "world/generators/TestGenerator.h"
int main() {

    // Defining useful constants for later
    static constexpr gf::Vector2i ScreenSize(1920, 1080);
    static constexpr gf::Vector2f ViewSize(100.0f, 100.0f);
    static constexpr gf::Vector2f ViewCenter(0.0f, 0.0f);
    static constexpr int SAFE_FRAMES = 5;
    static constexpr auto pauseKey1 = gf::Keycode::P;
    static constexpr auto pauseKey2 = gf::Keycode::Escape;
    static constexpr int charSize = 30;
    static gf::Font font("../assets/fonts/Waffle Cake.otf");

    // Window and renderer
    gf::Window window("Platformer", ScreenSize);
    gf::RenderWindow renderer(window);

    gf::ExtendView WorldView(ViewCenter,ViewSize);

    renderer.clear(gf::Color::White);

    /**************
     *   Scenes   *
     **************/

    gf::Action pauseAction {"Pause"};
    pauseAction.addKeycodeKeyControl(pauseKey1);
    pauseAction.addKeycodeKeyControl(pauseKey2);
    pauseAction.setInstantaneous();

    // Game scene creation
    gf::Scene gameScene(ScreenSize);
    gameScene.addView(WorldView);
    gameScene.setWorldViewCenter(ViewCenter);
    gameScene.setWorldViewSize(ViewSize);
    gameScene.addAction(pauseAction);

    // Pause scene creation
    gf::Scene pauseScene(ScreenSize);
    pauseScene.addView(WorldView);
    pauseScene.hide();

    /**************
     *    World   *
     **************/

    gf::Texture characterTexture("../assets/character_placeholder.png");
    platformer::BlockManager blockManager(ViewSize);
    platformer::Character character({0.0f, 0.0f}, characterTexture, blockManager, gameScene);
    platformer::BasicWorldGenerator generator;
    //platformer::TestGenerator generator;
    platformer::World world(character, blockManager, generator);
    platformer::Camera camera(gameScene,character,blockManager);

    // Loading textures
    world.getBlockManager().loadTextures();

    // Generate world
    world.generate();

    // actions
    character.initInput();

    // clock
    gf::Clock clock;

    // Adding elements to the game scene
    gameScene.addWorldEntity(blockManager);
    gameScene.addWorldEntity(character);
    gameScene.addWorldEntity(world);
    gameScene.addHudEntity(camera);
    gameScene.setActive();

    // Adding elements to the pause scene
    platformer::TextEntity pauseText("The game is Paused!\nPress P or Escape to resume !", font, gf::Coordinates(renderer).getCenter(),charSize);
    pauseText.setColor(gf::Color::Green);
    pauseText.setAnchor(gf::Anchor::Center);
    pauseScene.addHudEntity(pauseText);

    /**************
     *  Game Loop *
     **************/

    int framesBeforeStart = SAFE_FRAMES;

    while (window.isOpen()) {
        if (gameScene.isActive()) {

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

            // Pause
            if (pauseAction.isActive()) {
                if (pauseScene.isHidden()) {
                    pauseScene.show();
                    gameScene.pause();
                    pauseText.setPosition(gf::Coordinates(renderer).getCenter());
                }
                else {
                    pauseScene.hide();
                    gameScene.resume();
                }
            }

            // Game scene updating
            gf::Time time = clock.restart();
            gameScene.update(time);

            if (character.died()) {
                framesBeforeStart = SAFE_FRAMES;
            }

            // Safe frames
            if (framesBeforeStart > 0) {
                framesBeforeStart--;
                character.teleport(world.getSpawnPoint());
                gameScene.setWorldViewCenter(world.getSpawnPoint());
            }

            // 3 - render
            renderer.clear();

            gameScene.render(renderer);
            pauseScene.render(renderer);
            renderer.display();
            pauseAction.reset();
        }
    }

    return 0;
}
