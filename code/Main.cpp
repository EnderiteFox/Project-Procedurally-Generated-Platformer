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

#include "world/generators/basic_generator/BasicWorldGenerator.h"
#include "world/generators/TestGenerator.h"

void drawPauseText(gf::RenderWindow& renderer, const int charSize, gf::Font& font, const gf::ExtendView& WorldView, const gf::Vector2i ScreenSize) {
    gf::Text firstLine("The game is paused !",font);
    firstLine.setCharacterSize(charSize);
    firstLine.setColor(gf::Color::Green);
    firstLine.setPosition(WorldView.getCenter() + ScreenSize / 2 -
        gf::Vector2f{static_cast<float>(firstLine.getString().size() / 4 * charSize),static_cast<float>(charSize)}
    );
    renderer.draw(firstLine);

    gf::Text secondLine("Press P or Escape to resume !",font);
    secondLine.setColor(gf::Color::Green);
    secondLine.setPosition(WorldView.getCenter() + ScreenSize / 2 -
        gf::Vector2f{static_cast<float>(secondLine.getString().size() / 4 * charSize),0}
    );
    renderer.draw(secondLine);
}

int main() {

    // Defining useful constants for later
    static constexpr gf::Vector2i ScreenSize(1024, 576);
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

    /**************
     *  Game Loop *
     **************/

    int framesBeforeStart = SAFE_FRAMES;
    constexpr double CAMERA_EASING = 3.5;

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
                }
                else {
                    pauseScene.hide();
                    gameScene.resume();
                }
            }

            // Game scene updating
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
            if (!pauseScene.isHidden()) {
                drawPauseText(renderer, charSize, font, WorldView, ScreenSize);
            }
            renderer.display();
            pauseAction.reset();
        }
    }

    return 0;
}
