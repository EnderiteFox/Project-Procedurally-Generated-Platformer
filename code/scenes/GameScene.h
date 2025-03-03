/**
 * The main game Scene for the platformer game
 */
#pragma once
#include <gf/Scene.h>

#include "camera/Camera.h"
#include "player/Character.h"
#include "text/TextEntity.h"
#include "world/World.h"
#include "world/generators/basic_generator/BasicWorldGenerator.h"

namespace platformer {
    class PlatformerManager; // It fixes issues with cyclic dependancies

    class GameScene final : public gf::Scene {
        PlatformerManager* manager;
        gf::Action pauseAction{"Pause"};
        gf::Action FullScreenAction{"FullScreen"};

        // Textures
        gf::Texture characterTexture;
        gf::Texture scoreTexture;

        // Entities
        BlockManager blockManager;
        Character character;
        BasicWorldGenerator generator;
        World world;
        Camera camera;

        // Texts
        TextEntity scoreDisplay;
        TextEntity lifeDisplay;

        // Initialize the scene and it's main entities
        void init();

    protected:
        void doHandleActions(gf::Window& window) override;
        void doUpdate(gf::Time time) override;

    public:
        static constexpr auto pauseKey1 = gf::Keycode::P;
        static constexpr auto pauseKey2 = gf::Keycode::Escape;

        explicit GameScene(gf::Vector2i initialSize) = delete;
        GameScene(gf::Vector2i initialSize, PlatformerManager* manager);

        // Create a new world and reset the main entities
        void reset();

        void endGame() const;
    };
}
