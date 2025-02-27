/**
 * A scene for when the game is paused
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

    class PauseScene : public gf::Scene{

        platformer::PlatformerManager* manager;
        platformer::TextEntity pauseText;
        gf::Action unpauseAction{"Unpause"};

    protected:
        virtual void doHandleActions (gf::Window& window) override;
    public :
        PauseScene (gf::Vector2i initialSize) = delete;
        PauseScene(gf::Vector2i initialSize, platformer::PlatformerManager* manager);

        virtual ~PauseScene() = default;
    };
}
