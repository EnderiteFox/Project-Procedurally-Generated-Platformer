#pragma once
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/Vector.h>

namespace platformer{
    class Block final : public gf::Entity {
    public:
        Block(gf::Vector2f position, const gf::Texture& texture);
        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
    private:
        gf::Vector2f position;
        gf::Sprite sprite;
    };
}
