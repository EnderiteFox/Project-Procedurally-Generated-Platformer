/**
 * A scene for when the game is paused
 */
#pragma once
#include <gf/Scene.h>
#include <text/TextEntity.h>

namespace platformer {

    class PlatformerManager; // It fixes issues with cyclic dependancies

    class PauseScene final : public gf::Scene  {

        PlatformerManager* manager;
        TextEntity pauseText;
        gf::Action unpauseAction{"Unpause"};

    protected:
        void doHandleActions (gf::Window& window) override;

    public :
        explicit PauseScene (gf::Vector2i initialSize) = delete;
        PauseScene(gf::Vector2i initialSize, PlatformerManager* manager);

        ~PauseScene() override = default;
    };
}
