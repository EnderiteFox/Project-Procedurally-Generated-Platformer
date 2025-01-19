#include <physics/Physics.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <gf/Rect.h>

#include <iostream>
#include <blocks/BlockTypes.h>

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
        return lastGroundTouchTime <= COYOTE_JUMP_TIME || isOnLadder;
    }

    bool Character::died() const{
        return isDead;
    }

    void Character::update(const gf::Time time) {
        // We will need to change this if we want the character to accelerate in other ways than just falling
        acceleration = gravity + Physics::friction(speed, getDirection());
        speed += acceleration * time.asSeconds();

        processInput();

        // Adding other impulse such as jump/dash
        processImpulse();

        // Calculating collisions
        std::vector<std::pair<gf::RectF, std::string>> collisionBlocks = blockManager.getNearbyHitboxes(position, this->size);
        if (goThroughPlatforms) collisionBlocks.erase(std::remove_if(
            collisionBlocks.begin(),
            collisionBlocks.end(),
            [](const std::pair<gf::RectF, std::string>& pair) {
                return pair.second == BlockTypes::PLATFORM_BLOCK;
            }
        ), collisionBlocks.end());
        const collisionData collisionVector = Physics::collide(*this, collisionBlocks);
        speed += collisionVector.collision + collisionVector.friction;

        // Update last time we touched the ground
        if (collisionVector.collision.y < 0) {
            lastGroundTouchTime = 0;
            airjumps = 0;
            jumping = false;
            canDoubleJump = true;

        } else {
            lastGroundTouchTime += time.asSeconds();
        }

        // Checking if we touched a ladder
        isOnLadder = collisionVector.flags.find("ladder") != collisionVector.flags.end();
        isDead = collisionVector.flags.find("hazard") != collisionVector.flags.end();

        // Update jump time
        jumpStartTime += time.asSeconds();

        // Update dash time
        dashStart += time.asSeconds();
        dashStartContinuous-=time.asSeconds();

        // Adding the speed to the position
        position += speed * time.asSeconds() + collisionVector.correction;

        //Resetting the actions
        actionContainer.reset();
    }

    void Character::setSpeed(const gf::Vector2f speed) {
        this->speed = speed;
    }

    void Character::setPosition(const gf::Vector2f position) {
        this->position = position;
    }

    gf::Vector2f Character::getPosition() const {
        return this->position;
    }

    gf::Vector2f Character::getSpeed() const {
        return this->speed;
    }

    gf::RectF Character::getHitbox() const {
        return gf::RectF::fromPositionSize(this->position, this->size);
    }

    void Character::initInput() {
        leftAction.addScancodeKeyControl(gf::Scancode::A);
        leftAction.addScancodeKeyControl(gf::Scancode::Left);
        leftAction.setContinuous();
        actionContainer.addAction(leftAction);

        rightAction.addScancodeKeyControl(gf::Scancode::D);
        rightAction.addScancodeKeyControl(gf::Scancode::Right);
        rightAction.setContinuous();
        actionContainer.addAction(rightAction);

        jumpAction.addScancodeKeyControl(gf::Scancode::Space);
        jumpAction.setContinuous();
        actionContainer.addAction(jumpAction);

        upAction.addScancodeKeyControl(gf::Scancode::W);
        upAction.addScancodeKeyControl(gf::Scancode::Up);
        actionContainer.addAction(upAction);
        upAction.setContinuous();

        downAction.addScancodeKeyControl(gf::Scancode::S);
        downAction.addScancodeKeyControl(gf::Scancode::Down);
        downAction.setContinuous();
        actionContainer.addAction(downAction);

        dashAction.addScancodeKeyControl(gf::Scancode::Return);
        dashAction.addScancodeKeyControl(gf::Scancode::RightShift);
        dashAction.setContinuous();
        actionContainer.addAction(dashAction);
    }

    void Character::teleport(const gf::Vector2f newPosition){
        position = newPosition;
        speed = gf::Vector2f{0.0f,0.0f};
        actionContainer.reset();
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
        if (downAction.isActive() && !isOnGround() && !jumping) {
            charSpeed.y += 1;
        }

        goThroughPlatforms = downAction.isActive();

        if (std::abs(speed.x + charSpeed.x * ACCELERATION) > maxSpeed.x){
            charSpeed.x = getDirection().x * maxSpeed.x - speed.x;
        }
        if (std::abs(speed.y + charSpeed.y * ACCELERATION) > maxSpeed.y){
            charSpeed.y = getDirection().y * maxSpeed.y - speed.y;
        }

        //Initial speed determination
        speed += (charSpeed.x != 0 || charSpeed.y != 0 ? normalize(charSpeed) : charSpeed) * ACCELERATION;

        if(isOnLadder){
            if(upAction.isActive()){
                speed.y = -CLIMBSPEED;
            }
            else if(downAction.isActive()){
                speed.y = CLIMBSPEED;
            }
            else{
                speed.y = 0;
            }
        }

        //actionContainer.reset(); // This prevents dash from being processed, please don't add it back. It's already present at the end of the update method
                                   // And it's useless here since our input actions (except dash) aren't Instantaneous.
    }

    void Character::processImpulse() {
        gf::Vector2f jumpSpeed{0.0f,0.0f};

        // If jumping and can jump
        if (jumpAction.isActive() && (jumping || isOnGround() || (airjumps < maxAirJumpCount && canDoubleJump)) && jumpStartTime <= MAX_JUMP_TIME) {
            // When starting to jump
            if (!jumping) {
                if (!isOnGround()) airjumps++;
                lastGroundTouchTime = COYOTE_JUMP_TIME + 1;
                canDoubleJump = false;
                speed.y = -INITIAL_JUMP_FACTOR;
            }

            // Is jumping
            jumping = true;
            jumpSpeed.y = -JUMP_FACTOR;
        }
        // When not jumping
        else {
            jumping = false;
            jumpStartTime = 0;
            if (isOnGround()) airjumps = 0;

            // Can double jump once we finished jumping and we released the button (so when keeping the button pressed it doesn't keep jumping)
            if (!jumpAction.isActive()) canDoubleJump = true;
        }


        if (dashAction.isActive() && dashStart<MAX_DASH_TIME && dashStartContinuous<=0) {
            if (rightAction.isActive() && !leftAction.isActive() ) {
                jumpSpeed.x += DASH_FACTOR;
                dashStartContinuous=DELAY_BETWEEN_DASH;
            } else if (leftAction.isActive() && !rightAction.isActive()) {
                jumpSpeed.x -= DASH_FACTOR;
                dashStartContinuous=DELAY_BETWEEN_DASH;
            }
        }else {
            if (dashStart>=MAX_DASH_TIME) {
                dashStart = 0;
            }
        }


        // Speed resolution
        speed += jumpSpeed;
    }
}
