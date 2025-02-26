/**
 * The main game Scene for the platformer game
 */
#pragma once
#include <gf/Scene.h>
#include <text/TextEntity.h>
#include <player/Character.h>
#include <world/World.h>
#include <map>
#include <camera/Camera.h>
#include "world/generators/basic_generator/BasicWorldGenerator.h"

namespace platformer{

    class PlatformerManager; // It fixes issues with cyclic dependancies

    class GameScene : public gf::Scene{
    private :
        static constexpr auto pauseKey1 = gf::Keycode::P;
        static constexpr auto pauseKey2 = gf::Keycode::Escape;
        platformer::PlatformerManager* manager;
        gf::Action pauseAction {"Pause"};

        // Textures
        gf::Texture characterTexture;

        // Entities
        platformer::BlockManager blockManager;
        platformer::Character character;
        platformer::BasicWorldGenerator generator;
        platformer::World world;
        platformer::Camera camera;

        // Initialize the scene and it's main entities
        void init();

    protected:
        virtual void doUpdate(gf::Time& time);
        virtual void doHandleActions(gf::Window&  window);

    public :

        GameScene (gf::Vector2i initialSize) = delete;
        GameScene (gf::Vector2i initialSize, platformer::PlatformerManager* manager);

        // Create a new world and reset the main entities
        void reset();

    };

}
