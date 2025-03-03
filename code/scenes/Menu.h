/**
 * A scene used as the main menu of the game.
 * Currently only starts the game, it will hopefully do more in the future
 */
#pragma once
#include <gf/Scene.h>
#include <text/TextEntity.h>

namespace platformer {

    class PlatformerManager; // It fixes issues with cyclic dependancies

    class Menu final : public gf::Scene {
        // Main attributes
        static constexpr auto gameStartKey = gf::Scancode::Space;
        static constexpr auto quitKey = gf::Scancode::Escape;
        PlatformerManager* manager;
        static constexpr uint charSize = 30;
        bool do_start = false;

        gf::Action startAction{"Start"};
        gf::Action quitAction{"Quit"};
        gf::Action FullScreenAction{"FullScreen"};

        // Texts
        TextEntity tooltipText;
        TextEntity titleText;


    protected:
        void doHandleActions (gf::Window& window) override;
        void doUpdate (gf::Time time) override;

    public:
        static constexpr auto FullScreenKey = gf::Scancode::F;

        explicit Menu (gf::Vector2i initialSize) = delete;
        Menu (gf::Vector2i initialSize, PlatformerManager* manager);
    };
}
