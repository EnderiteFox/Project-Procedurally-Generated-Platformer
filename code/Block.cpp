/**
 * A basic block that will be used for testing, and maybe build maps later
 */
#include <Block.h>
#include <gf/RenderTarget.h>
#include <gf/Shapes.h>

namespace platformer{
    Block::Block(gf::Vector2f position, const gf::Texture& texture): position(position) {
        this->sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void Block::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        target.draw(this->sprite, states);
    }
}
