/**
 * A class used to handle all physic behaviors
 */
#include <cassert>
#include <iostream>
#include <gf/Rect.h>
#include <gf/Vector.h>
#include <gf/Collision.h>
#include <vector>
#include <physics/Physics.h>
#include <player/Character.h>


namespace platformer {
    std::pair<bool,gf::Vector2f> Physics::collide(const Character& character, const gf::RectF& otherHitbox) {
        const gf::RectF charHB = character.getHitbox();
        if (
            gf::Penetration p;
            collides(charHB,otherHitbox,p)
        ) {
            gf::Vector2f relativeVelocity = -character.getSpeed();
            float velocityAlongNormal = gf::dot(relativeVelocity, p.normal);
            if(velocityAlongNormal > 0){
                return {false,{0,0}};
            }
            float impulseScalar = (1+RESTITUTION)*velocityAlongNormal;

            return {true,impulseScalar*p.normal};
        }
        return {false,{0,0}};
    }

    std::pair<bool,gf::Vector2f> Physics::collide(const Character& character, const std::vector<gf::RectF>& otherHitboxes) {
        gf::Vector2f result{0,0};
        bool occured = false;
        int collisions = 0;
        for (gf::RectF hb: otherHitboxes) {
            if (
                auto [collided, collisionResult] = collide(character, hb);
                collided
            ) {
                occured = true;
                result += collisionResult;
                collisions++;
            }
        }
        return {occured,(result/((std::abs(collisions)>0.001)?collisions:1))};
    }

    gf::Vector2f Physics::friction(const gf::Vector2f speed,const gf::Vector2f direction){
        gf::Vector2f resistance{std::abs(speed.x),std::abs(speed.y)};
        return -direction * AIRRESISTANCE  * resistance;
    }
}
