/**
 * The Scene for when the game ends
 * Should be able to handle both loss and wins
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

    class EndScene : public gf::Scene{
        PlatformerManager* manager;
        gf::Action menuAction {"Menu"};

        // Text
        platformer::TextEntity victoryText;
        platformer::TextEntity deathText;
        platformer::TextEntity scoreText;
        platformer::TextEntity tooltipText;

    protected:
        virtual void doHandleActions(gf::Window& window) override;

    public:
        static constexpr auto gotoMenuKey = gf::Scancode::Space;

        EndScene (gf::Vector2i initialSize) = delete;
        EndScene (gf::Vector2i initialSize, platformer::PlatformerManager* manager);

        void load(int score, bool isVictory);
    };
}
