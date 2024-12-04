#include "Character.h"

#include <gf/Vector.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>

namespace platformer{
    Character::Character(gf::Vector2f position, const gf::Texture& texture): position(position), maxSpeed(), speed(), acceleration() {
        this->sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void Character::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        sprite.setPosition(this->position);
        target.draw(this->sprite, states);
    }

    void Character::update(const gf::Time time) {
        //Not handling acceleration for now
        position += speed * time.asSeconds();
    }

    void Character::setSpeed(const gf::Vector2f speed) {
        this->speed = speed;
    }

    gf::Vector2f Character::getPosition() const {
        return this->position;
    }

}
