/**
 * A character, a basic movable entity that should collide with other entities
 * Hopefully, we'll add some acceleration to it
 */
#pragma once
#include <gf/Action.h>
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/Vector.h>
#include <gf/Rect.h>
#include <world/World.h>

namespace platformer {
    class Character final : public gf::Entity {
        const gf::Vector2f size{8.0f,8.0f};
        const gf::Vector2f gravity{0.0f, 40.0f};
        const gf::Vector2f drag{0.1, 0};
        const float ACCELERATION = 3.0f;
        const float JUMP_FACTOR = 20.0f;
        const gf::Vector2f maxSpeed{40.0f,100.0f};

        BlockManager& blockManager;

        gf::Vector2f position;
        gf::Vector2f speed;
        gf::Vector2f acceleration;
        gf::Sprite sprite;

        gf::Action leftAction{"Left"};
        gf::Action rightAction{"Right"};
        gf::Action jumpAction{"Jump"};
        gf::Action downAction{"Down"};


    public:
        Character(gf::Vector2f position, const gf::Texture& texture, BlockManager& blockManager);

        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        void update(gf::Time time) override;

        void setSpeed(gf::Vector2f speed);
        gf::Vector2f getSpeed() const;
        gf::Vector2f getPosition() const;
        gf::RectF getHitbox() const;
        gf::Vector2f getDirection() const;

        void initInput(gf::ActionContainer& actionContainer);
        void processInput();
        void processImpulse();
    };
}
