/**
 * Static class dedicated to physics and interactions between objects
 * Handles :
 *  - Collisions between character and a tile with it's type
 *  - Collisions between character and multiple tiles
 *  - Friction : Determines the air resistance that would affect an object as a function of it's speed and direction
 *               Proportionnal to v^2 * coefficient
 */

#pragma once
#include <gf/Vector.h>
#include <gf/Rect.h>
#include <vector>
#include <set>
#include "player/Character.h"

namespace platformer {

    /**
     * Data structures allowing to store the data of one or multiple collisions
     * - collision : Vector giving the impulse to apply to a character to make it go away from the block(s) it collided with
     * - correction : Vector to move the character away from a block, to avoid floating point error from the previous vector
     * - friction : Horizontal friction of a character on a block
     * - hasCollisionOccured/hasCorrectionOccured : Respectively indicates if a collision or a correction with a block has occured
     * - flags : Stores the types of all the blocks the character has collided with
     */
    struct collisionData{
        gf::Vector2f collision{0,0};
        gf::Vector2f correction{0,0};
        gf::Vector2f friction{0,0};
        bool hasCollisionOccured = false;
        bool hasCorrectionOccured = false;
        std::set<std::string> flags{};

        collisionData& operator+=(collisionData& other) {
            collision += other.collision;
            correction += other.correction;
            friction += other.friction;
            hasCollisionOccured = hasCollisionOccured || other.hasCollisionOccured;
            hasCorrectionOccured = hasCorrectionOccured || other.hasCorrectionOccured;
            flags.merge(other.flags);
            return *this;
        }
    };

    class Physics {
        Physics() = delete; //Prevent users from creating an instance of physics, as it only has static functions

    public:
        static constexpr gf::Vector2f AIRRESISTANCE{0.01f,0.0f}; // Coefficient affecting the air direction

        /**
         * Tests the collision between a character and a collidable block
         * @param character : The moving character
         * @param otherHitbox : The hitbox of the object the character can collide with
         * @param blockType : The subtype of the block the character can collide with
         *
         * @return the data of the collision (see the comment on collisionData)
         */
        static collisionData collide(const Character& character, const gf::RectF& otherHitbox, const std::string blockType);

        /**
         * Tests the collision between a character and blocks
         * @params and @return : same as above
         */
        static collisionData collide(const Character& character, const std::vector<std::pair<gf::RectF,std::string>>& otherBlocks);

        /**
         *  Determines a moving object's friction with the air in fuction of their current speed
         *
         *  @param speed : Speed of the moving object
         *  @param direction : The direction of the friction
         *
         *  @return vector corresponding to the current friction
         */
        static gf::Vector2f friction(gf::Vector2f speed, gf::Vector2f direction);

    private :
        static bool matchDirection(gf::Vector2i blockDirection, gf::Vector2f characterDirection);
    };
}
