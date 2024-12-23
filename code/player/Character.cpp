#include <physics/Physics.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <gf/Rect.h>

#include <iostream>

namespace platformer {
    Character::Character(const gf::Vector2f position, const gf::Texture& texture, BlockManager& blockManager):
        blockManager(blockManager),
        position(position),
        speed(),
        acceleration()
    {
        this->sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void Character::render(gf::RenderTarget& target, const gf::RenderStates& states) {
        sprite.setPosition(this->position);
        target.draw(this->sprite, states);
    }

    gf::Vector2f Character::getDirection() const{
        gf::Vector2f direction = {0.0f,0.0f};
        direction.x = speed.x / (std::abs(speed.x) < 0.001 ? 1 : std::abs(speed.x));
        direction.y = speed.y / (std::abs(speed.y) < 0.001 ? 1 : std::abs(speed.y));
        return direction;
    }

    void Character::update(const gf::Time time) {
        processInput();

        // We will need to change this if we want the character to accelerate in other ways than just falling
        acceleration = gravity;
        acceleration += Physics::friction(speed,getDirection());
        const gf::Vector2f currentAcceleration = acceleration * time.asSeconds();

        // Adding the current acceleration to the speed (if it doesn't exceeds the maximum possible speed)
        speed.x = (std::abs(speed.x+currentAcceleration.x) > std::abs(maxSpeed.x))?maxSpeed.x*getDirection().x:speed.x+currentAcceleration.x;
        speed.y = (std::abs(speed.y+currentAcceleration.y) > std::abs(maxSpeed.y))?maxSpeed.y*getDirection().y:speed.y+currentAcceleration.y;

        // Calculating collisions
        std::pair<bool,gf::Vector2f> collisions = Physics::collide(*this, blockManager.getNearbyHitboxes(position));
        speed += collisions.second;

        // Adding the speed to the position
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

    void Character::initInput(gf::ActionContainer& actionContainer) {
        leftAction.addScancodeKeyControl(gf::Scancode::Q);
        leftAction.addScancodeKeyControl(gf::Scancode::Left);
        leftAction.setContinuous();
        actionContainer.addAction(leftAction);

        rightAction.addScancodeKeyControl(gf::Scancode::D);
        rightAction.addScancodeKeyControl(gf::Scancode::Right);
        rightAction.setContinuous();
        actionContainer.addAction(rightAction);

        //Will probably be removed in profit of a "jump" button once we add gravity and collisions
        upAction.addScancodeKeyControl(gf::Scancode::Z);
        upAction.addScancodeKeyControl(gf::Scancode::Up);
        upAction.setContinuous();
        actionContainer.addAction(upAction);

        downAction.addScancodeKeyControl(gf::Scancode::S);
        downAction.addScancodeKeyControl(gf::Scancode::Down);
        downAction.setContinuous();
        actionContainer.addAction(downAction);
    }


    void Character::processInput() {
        gf::Vector2f charSpeed{0.0f,0.0f};
        if (rightAction.isActive()) {
            charSpeed.x += 1;
        } else if (leftAction.isActive()) {
            charSpeed.x -= 1;
        }

        if (upAction.isActive()) {
            charSpeed.y -= 1;
        } else if (downAction.isActive()) {
            charSpeed.y += 1;
        }

        //Initial speed determination
        speed += (charSpeed.x != 0 || charSpeed.y != 0 ? normalize(charSpeed) : charSpeed);
    }
}
