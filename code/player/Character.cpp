#include <physics/Physics.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <gf/Rect.h>

namespace platformer {
    Character::Character(const gf::Vector2f position, const gf::Texture& texture, BlockManager& blockManager):
        blockManager(blockManager),
        position(position),
        maxSpeed(),
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

    void Character::update(const gf::Time time) {
        processInput();

        speed += gravity;
        // speed /= drag;

        //Not handling acceleration for now
        position += speed * time.asSeconds();

        // Calculate collisions
        auto [collided, collisionVector] = Physics::collide(*this, blockManager.getNearbyHitboxes(position));
        position.y += collisionVector.y * time.asSeconds();
        if (collided) speed.y = 0;
        auto [collided1, collisionVector1] = Physics::collide(*this, blockManager.getNearbyHitboxes(position));
        position.x += collisionVector1.x * time.asSeconds();
        if (collided1) speed.x = 0;
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
        leftAction.addScancodeKeyControl(gf::Scancode::A);
        leftAction.addScancodeKeyControl(gf::Scancode::Left);
        leftAction.setContinuous();
        actionContainer.addAction(leftAction);

        rightAction.addScancodeKeyControl(gf::Scancode::D);
        rightAction.addScancodeKeyControl(gf::Scancode::Right);
        rightAction.setContinuous();
        actionContainer.addAction(rightAction);

        jumpAction.addScancodeKeyControl(gf::Scancode::Space);
        jumpAction.addScancodeKeyControl(gf::Scancode::Z);
        actionContainer.addAction(jumpAction);

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

        if (jumpAction.isActive()) {
            charSpeed.y = -JUMP_FACTOR;
            jumpAction.reset();
        }

        if (downAction.isActive()) {
            charSpeed.y += 1;
        }

        //Initial speed determination
        speed += charSpeed * ACCELERATION;
    }
}
