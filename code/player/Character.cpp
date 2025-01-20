#include <physics/Physics.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <gf/Rect.h>
#include <gf/Shapes.h>

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

    // Returns a vector describing the direction the player is currently moving towards
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
        // Calculating acceleration
        acceleration = gravity + Physics::friction(speed, getDirection());
        speed += acceleration * time.asSeconds();

        //Adding the user's main inputs
        processInput();

        // Adding other impulse such as jump/dash
        processImpulse();

        // Getting blocks the character may collide with
        std::vector<std::pair<gf::RectF, std::string>> collisionBlocks = blockManager.getNearbyHitboxes(position, this->size);

        // Removing platforms from nearby collisions computing if the player is currently trying to go go through platforms
        if (goThroughPlatforms) collisionBlocks.erase(std::remove_if(
            collisionBlocks.begin(),
            collisionBlocks.end(),
            [](const std::pair<gf::RectF, std::string>& pair) {
                return pair.second == BlockTypes::PLATFORM_BLOCK;
            }
        ), collisionBlocks.end());

        // Collisions computing
        const collisionData collisionVector = Physics::collide(*this, collisionBlocks);
        speed += collisionVector.collision + collisionVector.friction;

        // Update last time we touched the ground
        if (collisionVector.collision.y < 0) {
            lastGroundTouchTime = 0;
            jumpCount=0;
        } else {
            lastGroundTouchTime += time.asSeconds();
        }

        // Checking if we touched a wall
        if (collisionVector.collision.x !=0){
            onWall=true;
        }else {
            onWall=false;
        }

        // Checking if we touched a ladder or a hasard
        isOnLadder = collisionVector.flags.find("ladder") != collisionVector.flags.end();
        isDead = collisionVector.flags.find("hazard") != collisionVector.flags.end();
        canJump = !isOnLadder;

        // Update dash time
        if (dash){
            dashStart += time.asSeconds();
        }
        dashDelay-=time.asSeconds();

        // Adding the speed to the position
        position += speed * time.asSeconds() + collisionVector.correction;

        // Resetting the actions
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

    // A hitbox for collisions with directionnal platforms
    // The hitbox's size is equal to ratio*currentHitbox, and is placed on the side of the collision
    // This function is horrendous, but i couldn't find a logic between the direction and the resulting hitbox
    gf::RectF Character::getSidedHitbox(const gf::Vector2f direction) const
        constexpr float ratio = 0.2f;
        if(direction == gf::Vector2f{0,-1}) return gf::RectF::fromPositionSize(
                                                     this->position+this->size*gf::Vector2f{0.0f,1-ratio},
                                                     this->size * gf::Vector2f{1.0f,ratio});
        if(direction == gf::Vector2f{0,1})  return gf::RectF::fromPositionSize(
                                                     this->position,
                                                     this->size* gf::Vector2f{1.0f,ratio});
        if(direction == gf::Vector2f{1,0})  return gf::RectF::fromPositionSize(
                                                     this->position,
                                                     this->size* gf::Vector2f{ratio,1.0f});
        return gf::RectF::fromPositionSize(
                this->position+this->size*gf::Vector2f{0.0f,0.75f},
                this->size*gf::Vector2f{ratio,1.0f});
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
        jumpAction.setInstantaneous();
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
        dashAction.setInstantaneous();
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
        if (downAction.isActive() && !isOnGround() /*&& !jumping*/) {
            charSpeed.y += 1;
        }

        goThroughPlatforms = downAction.isActive();

        // Lowering speed to prevent the character from going above maximum speed
        if (std::abs(speed.x + charSpeed.x * ACCELERATION) > maxSpeed.x){
            charSpeed.x = getDirection().x * maxSpeed.x - speed.x;
        }
        if (std::abs(speed.y + charSpeed.y * ACCELERATION) > maxSpeed.y){
            charSpeed.y = getDirection().y * maxSpeed.y - speed.y;
        }

        // Initial speed determination
        speed += (charSpeed.x != 0 || charSpeed.y != 0 ? normalize(charSpeed) : charSpeed) * ACCELERATION;

        // Climbing ladders (if the character is colliding with one)
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

        // Computing jump
        if (jumpAction.isActive() && canJump) {
            if (isOnGround()){
                speed.y = -JUMP_FACTOR;
                lastGroundTouchTime = COYOTE_JUMP_TIME + 1;
                jumpCount++;
            }else if (jumpCount<2){
                speed.y = -JUMP_FACTOR;
                jumpCount++;
            }
        }

        // Computing dash
        if (dashAction.isActive() && dashDelay<=0 && !dash){
            if (rightAction.isActive() && !leftAction.isActive() ) {
                dashStart=0;
                dash=true;
                jumpSpeed.x += DASH_FACTOR;
                progress=DASH_FACTOR;
            } else if (leftAction.isActive() && !rightAction.isActive()) {
                dashStart=0;
                dash=true;
                jumpSpeed.x -= DASH_FACTOR;
                progress=-DASH_FACTOR;
            }
        }else if (dashStart<MAX_DASH_TIME && dash){
            speed.x+=progress;
        }else if(dashStart>=DELAY_BETWEEN_DASH){
            dashDelay=DELAY_BETWEEN_DASH;
            dash=false;
            dashStart=0;
        }

        // Computing wall jumps
        if (onWall){
            jumpCount=1;
            speed.y = 0;
            speed.x = 0;
        }

        // Speed resolution
        speed += jumpSpeed;
    }
}

