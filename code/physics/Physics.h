#pragma once
#include <gf/Vector.h>
#include <gf/Rect.h>
#include <vector>
#include "player/Character.h"

namespace platformer {
    class Physics {
        Physics() = delete; //Prevent users from creating an instance of physics, as it only has static functions

    public:
        static constexpr gf::Vector2f AIRRESISTANCE{0.125f,0.125f};
        static constexpr float RESTITUTION = 0.1; // Coefficient giving the strength of the rebound in case one occurs

        /**
         * Tests the collision between a character and a collidable object, symbolised by it's hitbox
         * @param character : The moving character
         * @param otherHitbox : The hitbox of the object the character can collide with
         *
         * @return a pair of:
         *                  boolean indicating if the collision occured
         *                  Vector2f giving a vector of the collision that occured. If the collision didn't occur, the result is the null vector
         */
        static std::pair<bool,gf::Vector2f> collide(const Character& character, const gf::RectF& otherHitbox);

        /**
         * Tests the collision between a character and multiple objects/hitboxes
         * @params same as above
         *
         * @return a pair of:
         *                  boolean indicating if the collision occured
         *                  Vector2f giving a vector of the sum of the vector of the collision that occured.
         */
        static std::pair<bool,gf::Vector2f> collide(const Character& character, const std::vector<gf::RectF>& otherHitboxes);

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
