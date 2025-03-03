#pragma once
#include <gf/Entity.h>
#include <player/Character.h>
#include <blocks/BlockManager.h>
#include <gf/Vector.h>

/**
 * Simple class to handle the camera movements to avoid having it in the Main.cpp file
 */
namespace platformer{
    class Camera final : public gf::Entity {
        const double EASING = 3.5;
        gf::Scene* scene;
        Character& character;
        BlockManager& blockManager;

        gf::Vector2f viewPos;

    public :
        Camera(gf::Scene* scene, Character& character, BlockManager& blockManager);

        void update(gf::Time time) override;
    };
}
