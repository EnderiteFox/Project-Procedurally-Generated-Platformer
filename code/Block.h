#pragma once
#include <gf/Entity.h>
#include <gf/Vector.h>
#include <gf/Sprite.h>
#include "Collidable.h"

namespace platformer{
    class Block : public gf::Entity, public platformer::Collidable {
    public:
        Block(gf::Vector2f position, const gf::Texture& texture);
        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;

        gf::RectF getHitbox() const;
        gf::Vector2f getPosition() const;
    private:
        gf::Vector2f position;
        gf::Sprite sprite;
    };
}
