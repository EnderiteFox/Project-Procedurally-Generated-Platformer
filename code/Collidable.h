/**
 * A class that defines a behavior of an entity that can collide with another one
 */
#pragma once
#include <gf/Rect.h>

namespace platformer{
    class Collidable{
    public:
        Collidable() {};
        virtual ~Collidable(){}; // Must be defined for some reason (c.f the gcc FAQ)

        /**
         * Returns a float rectangle defining the hitbox the collidable object will use
         */
        virtual gf::RectF getHitbox() const;

        virtual gf::Vector2f getPosition() const;
    };
}
