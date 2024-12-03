#pragma once
#include <gf/Vector.h>
#include <gf/Rect.h>
#include "Character.h"
#include "Collidable.h"

namespace platformer{
    class Physics{
        Physics() = delete; //Prevent users from creating an instance of physics, as it only has static functions

    public:
        /**
         * Tests the collision between a character and a collidable object
         * @param character : The moving character
         * @param object : The object the character can collide with
         *
         * @return a pair of:
         *                  boolean indicating if the collision occured
         *                  Vector2f giving a vector of the collision that occured. If the collision didn't occur, the result is the null vector
         */
        static std::pair<bool,gf::Vector2f> collide(const platformer::Character& character, const platformer::Collidable& object);
    };
}
