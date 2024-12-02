/**
 * A basic block that will be used for testing, and maybe build maps later
 */
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>
#include "Block.h"

namespace platformer{
    Block::Block(gf::Vector2f position, const gf::Texture& texture) {
        this->sprite.setTexture(texture);
        sprite.setPosition(position);
        this->position = position;
    }

    void Block::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        target.draw(this->sprite, states);
    }
}
