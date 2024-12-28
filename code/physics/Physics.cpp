/**
 * A class used to handle all physic behaviors
 */
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
            constexpr float correctionCoeff = 0.8f;
            const gf::Vector2f relativeVelocity = -character.getSpeed();
            const float velocityAlongNormal = dot(relativeVelocity, p.normal);
            if (velocityAlongNormal > 0) {
                return {false, {0,0}};
            }
            const gf::Vector2f correction = -p.depth / correctionCoeff * p.normal;

            const float impulseScalar = (1 + RESTITUTION) * velocityAlongNormal;
            return {true, impulseScalar * p.normal + correction};
        }
        return {false, {0,0}};
    }

    std::pair<bool,gf::Vector2f> Physics::collide(const Character& character, const std::vector<gf::RectF>& otherHitboxes) {
        gf::Vector2f result {0, 0};
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
        return {occured, (result / (collisions == 0 ? 1 : collisions))};
    }

    gf::Vector2f Physics::friction(const gf::Vector2f speed,const gf::Vector2f direction) {
        const gf::Vector2f resistance {
            speed.x * speed.x,
            speed.y * speed.y
        };
        return -direction * AIRRESISTANCE * resistance;
    }
}
