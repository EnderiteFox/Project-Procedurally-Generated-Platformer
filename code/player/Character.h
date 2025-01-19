/**
 * The main character of the game.
 * The character is an entity who's actions are controlled by the user.
 * The character also :
 *  - Interacts with blocks, with the collision method from the physics class
 *  - Is affected by air resistance, calculated with the physics class
 *  - Is affected by gravity
 *  - Has friction with the floor
 *  - Can climb ladders
 *  - Can fall through platforms
 *
 * Every of those characteristics are processed during the "update" method, using several other methods to make the code clearer
 *
 * The character uses a sprite that's rendered during the "render" method
 */
#pragma once
#include <gf/Action.h>
#include <gf/Entity.h>
#include <gf/Sprite.h>
#include <gf/Vector.h>
#include <gf/Rect.h>
#include <world/World.h>

namespace platformer {
    class Character final : public gf::Entity {
        /**
         * Main constants relating to the physics of the character :
         * size : Size of the sprite (and hitbox) of the character
         * gravity : Value of the gravity that affects the character
         * ACCELERATION : The speed the character gains from the user's inputs
         * JUMP_FACTOR : Magnitude of the impulsion the character gains during a jump
         * maxSpeed : Maximum horizontal and vertical speed, not considering jumping
         * COYOTE_JUMP_TIME : Duration after leaving the ground when jumping is still possible
         * maxJumpCount : Number of jumps possible in a row without touching the ground
         */
        const gf::Vector2f size {8.0f, 8.0f};
        const gf::Vector2f gravity {0.0f, 200.0f};
        const float ACCELERATION = 3.0f;
        const float JUMP_FACTOR = 100.0f;
        const float DASH_FACTOR = 50.0f;
        const gf::Vector2f maxSpeed {40.0f, 50.0f};
        const float COYOTE_JUMP_TIME = 0.1f;
        const float MAX_DASH_TIME = 0.1f;
        const float DELAY_BETWEEN_DASH = 0.5f;
        const float CLIMBSPEED = 20.0f;

        BlockManager& blockManager;
        gf::ActionContainer& actionContainer;

        gf::Vector2f position;
        gf::Vector2f speed;
        gf::Vector2f acceleration;
        gf::Sprite sprite;

        gf::Action leftAction {"Left"};
        gf::Action rightAction {"Right"};
        gf::Action jumpAction {"Jump"};
        gf::Action upAction {"Up"};
        gf::Action downAction {"Down"};
        gf::Action dashAction {"Dash"};

        bool groundCollision = false;
        float lastGroundTouchTime = COYOTE_JUMP_TIME + 1;
        int jumpCount = 0;
        float jumpStartTime = 0;
        float dashStart = 0;
        float dashDelay = 0;
        bool isOnLadder = false;
        bool goThroughPlatforms = false;

        bool canJump=true;
        bool dash=false;
        float progress = 0.0f;

        // True if the character is dead within the last frame
        bool isDead;
    public:

        // Constructor
        Character(gf::Vector2f position, const gf::Texture& texture, BlockManager& blockManager, gf::ActionContainer& actionContainer);

        // GF's render and update methods
        void render(gf::RenderTarget& target, const gf::RenderStates& states) override;
        void update(gf::Time time) override;

        // Setters and getters
        void setSpeed(gf::Vector2f speed);
        gf::Vector2f getSpeed() const;
        gf::Vector2f getPosition() const;
        void setPosition(gf::Vector2f position);
        gf::RectF getHitbox() const;
        gf::RectF getSidedHitbox(gf::Vector2f direction) const;
        gf::Vector2f getDirection() const;

        // Tests if a collision has been detected between the player and the a block beneath it during the current frame
        bool isOnGround() const;

        // Tests if the player died (collided with a hasard during the current frame)
        bool died() const;

        // Initialize the input action within the player's action container
        void initInput();

        // Teleports the character to a location given in parameter
        void teleport(gf::Vector2f newPosition);

        /**
         * Processed the user's inputs.
         * Adds speed to the player depending on the actions active during the current frame
         * Action processed by this method are :
         *  - Left
         *  - Up => Climbing ladders
         *  - Right
         *  - Down => Acceleration thowards the ground on air/ going down ladders/ going through platforms
         * note : right have a priority on left, and left won't be processed if right is active
         */
        void processInput();

        /**
         * Process the immediate impulses depending of the player's input
         * Adds a constant speed to the player, defined by the constants at the top of this file
         * Actions processed by this method are :
         *  - Jump
         *  - Dash
         */
        void processImpulse();
    };
}
