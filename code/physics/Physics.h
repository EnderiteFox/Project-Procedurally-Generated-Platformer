/**
 * Static class dedicated to physics and interactions between objects
 * Currently handles :
 *  - Collisions between character and a rectangle
 *      TODO : Update this function so it works between 2 rectangles with velocity
 *  - Collisions between character and multiple rectangles
 *      TODO : Same as above
 *  - Friction : Determines the air resistance that would affect an object as a function of it's speed and direction
 */

#pragma once
#include <gf/Vector.h>
#include <gf/Rect.h>
#include <vector>
#include "player/Character.h"

namespace platformer {

    struct collisionData{
        gf::Vector2f collision{0,0};
        gf::Vector2f correction{0,0};
        gf::Vector2f friction{0,0};
        bool hasCollisionOccured = false;
        bool hasCorrectionOccured = false;

        collisionData& operator+=(const collisionData& other) {
            collision += other.collision;
            correction += other.correction;
            friction += other.friction;
            hasCollisionOccured = hasCollisionOccured || other.hasCollisionOccured;
            hasCorrectionOccured = hasCorrectionOccured || other.hasCorrectionOccured;
            return *this;
        }
    };

    class Physics {
        Physics() = delete; //Prevent users from creating an instance of physics, as it only has static functions

    public:
        static constexpr gf::Vector2f AIRRESISTANCE{0.125f,0.0f}; // Coefficient affecting the air direction
        static constexpr float RESTITUTION = 0.1f; // Coefficient giving the strength of the rebound in case one occurs
        static constexpr float STATICFRICTION = 0.2f;
        static constexpr float KINETICFRICTION = 0.02f;

        /**
         * Tests the collision between a character and a collidable object, symbolised by it's hitbox
         * @param character : The moving character
         * @param otherHitbox : The hitbox of the object the character can collide with
         *
         * @return a pair of:
         *                  Vector2f giving a vector corresponding to a correction to prevent the character from sinking in other objects
         *                  Vector2f giving a vector of the collision that occured. If the collision didn't occur, the result is the null vector
         */
        static collisionData collide(const Character& character, const gf::RectF& otherHitbox);

        /**
         * Tests the collision between a character and multiple objects/hitboxes
         * @params same as above
         *
         * @return a pair of:
         *                  Vector2f giving a vector corresponding to a correction to prevent the character from sinking in other objects
         *                  Vector2f giving a vector of the sum of the vector of the collision that occured.
         */
        static collisionData collide(const Character& character, const std::vector<gf::RectF>& otherHitboxes);

        /**
         *  Determines a moving object's friction with the air in fuction of their current speed
         *
         *  @param speed : Speed of the moving object
         *  @param direction : The direction of the friction
         *
         *  @return vector corresponding to the current friction
         */
        static gf::Vector2f friction(gf::Vector2f speed, gf::Vector2f direction);
    };
}
