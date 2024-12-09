#include <Physics.h>
#include <gf/RenderTarget.h>
#include <gf/Sprite.h>
#include <gf/Time.h>
#include <gf/Vector.h>
#include <player/Character.h>
#include <gf/Rect.h>

namespace platformer{
    Character::Character(const gf::Vector2f position, const gf::Texture& texture):
    world(),
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
        std::pair<bool,gf::Vector2f> collisions = Physics::collide(*this, world.getBlockManager().getNearbyHitboxes(position));
        speed += collisions.second * speed;
    }

    void Character::setSpeed(const gf::Vector2f speed) {
        this->speed = speed;
    }

    void Character::setWorld(const World& world) {
        this->world = world;
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
        speed += (charSpeed.x != 0 || charSpeed.y != 0 ? normalize(charSpeed) : charSpeed) * ACCELERATION;
    }

}
