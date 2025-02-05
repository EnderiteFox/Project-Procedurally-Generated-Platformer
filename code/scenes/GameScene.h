/**
 * The main game Scene for the platformer game
 */
#pragma once
#include <gf/Scene.h>
#include <text/TextEntity.h>
#include <player/Character.h>
#include <scenes/PlatformerManager.h>
#include <world/World.h>
#include <map>

namespace platformer{
    struct MainEntities{
        platformer::World& world;
        platformer::Character& character;
    };

    class GameScene : public gf::Scene{
    private :
        static constexpr auto pauseKey1 = gf::Keycode::P;
        static constexpr auto pauseKey2 = gf::Keycode::Escape;
        PlatformerManager& manager;
        gf::Action pauseAction {"Pause"};

        // Storing the scene's main entities
        std::map<std::string,gf::Entity*> mainEntities;

        // Initialize the scene and it's main entities
        void init();

    protected:
        virtual void doUpdate(gf::Time& time);

    public :

        GameScene (gf::Vector2i initialSize) = delete;
        GameScene (gf::Vector2i initialSize, PlatformerManager& manager);

        // Create a new world and reset the main entities
        void reset();

    };

}
