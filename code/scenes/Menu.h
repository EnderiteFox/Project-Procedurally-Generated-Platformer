/**
 * A scene used as the main menu of the game.
 * Currently only starts the game, it will hopefully do more in the future
 */
#pragma once
#include <gf/Scene.h>
#include <text/TextEntity.h>
#include <gf/Text.h>
#include <gf/Font.h>
#include <scenes/PlatformerManager.h>
#include <text/TextEntity.h>
#include <gf/Window.h>

namespace platformer{

    class PlatformerManager; // It fixes issues with cyclic dependancies

    class Menu : public gf::Scene{
    private:
        // Main attributes
        static constexpr auto gameStartKey = gf::Scancode::Space;
        static constexpr auto quitKey = gf::Scancode::Escape;
        platformer::PlatformerManager* manager;
        static constexpr uint charSize = 30;
        bool do_start=false;

        gf::Action startAction{"Start"};
        gf::Action quitAction{"Quit"};
        gf::Action FullScreenAction{"FullScreen"};

        // Texts
        platformer::TextEntity tooltipText;
        platformer::TextEntity titleText;


    protected:
        virtual void doHandleActions (gf::Window& window) override;
        virtual void doUpdate (gf::Time time) override;
    public :
        static constexpr auto FullScreenKey = gf::Scancode::F;

        Menu (gf::Vector2i initialSize) = delete;
        Menu (gf::Vector2i initialSize, platformer::PlatformerManager* manager);


    };
}
