/**
 * The Scene for when the game ends
 * Should be able to handle both loss and wins
 */
#pragma once
#include <gf/Scene.h>
#include <text/TextEntity.h>

namespace platformer {
    class PlatformerManager; // It fixes issues with cyclic dependancies

    class EndScene final : public gf::Scene {
        PlatformerManager* manager;
        gf::Action menuAction {"Menu"};

        // Text
        TextEntity victoryText;
        TextEntity deathText;
        TextEntity scoreText;
        TextEntity tooltipText;

    protected:
        void doHandleActions(gf::Window& window) override;

    public:
        static constexpr auto gotoMenuKey = gf::Scancode::Space;

        explicit EndScene (gf::Vector2i initialSize) = delete;
        EndScene (gf::Vector2i initialSize, PlatformerManager* manager);

        void load(int score, bool isVictory);
    };
}
