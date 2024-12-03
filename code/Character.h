/**
 * A character, a basic movable entity that should collide with other entities
 * Hopefully, we'll add some acceleration to it
 */
#pragma once
#include <gf/Entity.h>
#include <gf/Vector.h>
#include <gf/Sprite.h>

namespace platformer{
    class Character : public gf::Entity {
    public:
        Character(gf::Vector2f position, const gf::Texture& texture);

        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        void update(gf::Time time) override;

        void setSpeed(gf::Vector2f speed);
        gf::Vector2f getPosition();
    private:
        gf::Vector2f position;
        gf::Vector2f maxSpeed;
        gf::Vector2f speed;
        gf::Vector2f acceleration;
        gf::Sprite sprite;
    };
}
