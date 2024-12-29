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
        const gf::Vector2f size {8.0f, 8.0f};
        const gf::Vector2f gravity {0.0f, 40.0f};
        const gf::Vector2f drag {0.1f, 0.0f};
        const float ACCELERATION = 3.0f;
        const float JUMP_FACTOR = 20.0f;
        const gf::Vector2f maxSpeed {40.0f, 100.0f};
        const float COYOTE_JUMP_TIME = 0.1f;

        BlockManager& blockManager;
        gf::ActionContainer& actionContainer;

        gf::Vector2f position;
        gf::Vector2f speed;
        gf::Vector2f acceleration;
        gf::Sprite sprite;

        gf::Action leftAction {"Left"};
        gf::Action rightAction {"Right"};
        gf::Action jumpAction {"Jump"};
        gf::Action downAction {"Down"};

        bool groundCollision = false;
        float lastGroundTouchTime = COYOTE_JUMP_TIME + 1;


    public:
        Character(gf::Vector2f position, const gf::Texture& texture, BlockManager& blockManager, gf::ActionContainer& actionContainer);

        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        void update(gf::Time time) override;

        // Setters and getters
        void setSpeed(gf::Vector2f speed);
        gf::Vector2f getSpeed() const;

        gf::Vector2f getPosition() const;

        gf::RectF getHitbox() const;

        gf::Vector2f getDirection() const;

        bool isOnGround() const;

        void initInput();
        void processInput();
        void processImpulse();
    };
}
