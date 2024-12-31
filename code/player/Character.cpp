#include <physics/Physics.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <gf/Rect.h>

#include <iostream>

namespace platformer {
    Character::Character(const gf::Vector2f position, const gf::Texture& texture, BlockManager& blockManager, gf::ActionContainer& actionContainer):
        blockManager(blockManager),
        actionContainer(actionContainer),
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

    gf::Vector2f Character::getDirection() const {
        gf::Vector2f direction = {0.0f,0.0f};
        direction.x = speed.x / (std::abs(speed.x) < 0.001 ? 1 : std::abs(speed.x));
        direction.y = speed.y / (std::abs(speed.y) < 0.001 ? 1 : std::abs(speed.y));
        return direction;
    }

    bool Character::isOnGround() const {
        return lastGroundTouchTime <= COYOTE_JUMP_TIME;
    }

    void Character::update(const gf::Time time) {
        processInput();

        // We will need to change this if we want the character to accelerate in other ways than just falling
        acceleration = gravity + Physics::friction(speed, getDirection());
        const gf::Vector2f currentAcceleration = acceleration * time.asSeconds();

        // Adding the current acceleration to the speed (if it doesn't exceeds the maximum possible speed)
        speed.x = std::abs(speed.x + currentAcceleration.x) > std::abs(maxSpeed.x) ? maxSpeed.x * getDirection().x : speed.x + currentAcceleration.x;
        speed.y = std::abs(speed.y + currentAcceleration.y) > std::abs(maxSpeed.y) ? maxSpeed.y * getDirection().y : speed.y + currentAcceleration.y;

        // Adding other impulse such as jump/dash
        processImpulse();

        // Calculating collisions
        auto [collisionOccured, collisionVector] = Physics::collide(*this, blockManager.getNearbyHitboxes(position));
        speed += collisionVector;

        // Update last time we touched the ground
        if (collisionVector.y < 0){
            lastGroundTouchTime = 0;
            jumpCount=0;
        } else {
            lastGroundTouchTime += time.asSeconds();
        }

        // Adding the speed to the position
        position += speed * time.asSeconds();

        //Resetting the actions
        actionContainer.reset();
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

    void Character::initInput() {
        leftAction.addScancodeKeyControl(gf::Scancode::Q);
        leftAction.addScancodeKeyControl(gf::Scancode::A);
        leftAction.addScancodeKeyControl(gf::Scancode::Left);
        leftAction.setContinuous();
        actionContainer.addAction(leftAction);

        rightAction.addScancodeKeyControl(gf::Scancode::D);
        rightAction.addScancodeKeyControl(gf::Scancode::Right);
        rightAction.setContinuous();
        actionContainer.addAction(rightAction);

        jumpAction.addScancodeKeyControl(gf::Scancode::Z);
        jumpAction.addScancodeKeyControl(gf::Scancode::W);
        jumpAction.addScancodeKeyControl(gf::Scancode::Up);
        jumpAction.addScancodeKeyControl(gf::Scancode::Space);
        jumpAction.setInstantaneous();
        actionContainer.addAction(jumpAction);

        downAction.addScancodeKeyControl(gf::Scancode::S);
        downAction.addScancodeKeyControl(gf::Scancode::Down);
        downAction.setContinuous();
        actionContainer.addAction(downAction);
    }

    // Determining initial basic inputs (left/right/down)
    // "Immediate" inputs such as dash or jump are treated in "processImpulse"
    void Character::processInput() {
        gf::Vector2f charSpeed{0.0f,0.0f};
        if (rightAction.isActive()) {
            charSpeed.x += 1;
        } else if (leftAction.isActive()) {
            charSpeed.x -= 1;
        }

        if (downAction.isActive()) {
            charSpeed.y += 1;
        }

        //Initial speed determination
        speed += charSpeed * ACCELERATION;
    }

    void Character::processImpulse() {
        gf::Vector2f jumpSpeed{0.0f,0.0f};
        if (jumpAction.isActive() && (isOnGround() || jumpCount < maxJumpCount)) {
            jumpSpeed.y = -JUMP_FACTOR;
            jumpCount++;
            if (isOnGround()) {
                lastGroundTouchTime = COYOTE_JUMP_TIME + 1;
            }
        }

        // if(dashAction.isActive()){
        //      jumpSpeed.x += DASH_FACTOR;
        // }

        // Speed resolution
        speed += jumpSpeed;
    }
}
