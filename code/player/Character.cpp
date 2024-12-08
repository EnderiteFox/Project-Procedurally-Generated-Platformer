#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <iostream>
#include <gf/Rect.h>

namespace platformer{
    Character::Character(gf::Vector2f position, const gf::Texture& texture): position(position), maxSpeed(), speed() {
        this->sprite.setTexture(texture);
        sprite.setPosition(position);
        this->acceleration = gf::Vector2f(0.0f,100.0f);
        this->maxSpeed = gf::Vector2f(40.0f,40.0f);
    }

    void Character::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        sprite.setPosition(this->position);
        target.draw(this->sprite, states);
    }

    void Character::update(const gf::Time time) {
        //Acceleration
        this->speed += this->acceleration*time.asSeconds();
        if(abs(this->speed.x) > this->maxSpeed.x){
            this->speed.x = this->maxSpeed.x * this->speed.x/abs(this->speed.x);
        }
        if(abs(this->speed.y) > this->maxSpeed.y){
            this->speed.y = this->maxSpeed.y * this->speed.y/abs(this->speed.y);
        }
        //Speed
        position += speed * time.asSeconds();
    }

    void Character::setSpeed(const gf::Vector2f speed) {
        this->speed = speed;
    }

    gf::Vector2f Character::getPosition() const {
        return this->position;
    }

    gf::Vector2f Character::getSpeed() const{
        return this->speed;
    }

    gf::RectF Character::getHitbox() const{
        return gf::RectF::fromPositionSize(this->position, this->size);
    }

    gf::Vector2f Character::getAcceleration() const{
        return this->acceleration;
    }

    void Character::setAcceleration(gf::Vector2f acceleration){
        this->acceleration = acceleration;
    }
}
