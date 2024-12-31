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
    std::pair<gf::Vector2f,gf::Vector2f> Physics::collide(const Character& character, const gf::RectF& otherHitbox) {
        const gf::RectF charHB = character.getHitbox();
        if (
            gf::Penetration p;
            collides(charHB,otherHitbox,p)
        ) {
            constexpr float correctionCoeff = 0.2f;
            const gf::Vector2f correction = -std::max(p.depth-0.1f,0.0f) * correctionCoeff * p.normal;

            const gf::Vector2f relativeVelocity = -character.getSpeed();
            const float velocityAlongNormal = dot(relativeVelocity, p.normal);

            if (velocityAlongNormal > 0) {
                return {correction, {0,0}};
            }

            const float impulseScalar = (1 + RESTITUTION) * velocityAlongNormal;
            return {correction, impulseScalar * p.normal + correction};
        }
        return {{0,0}, {0,0}};
    }

    std::pair<gf::Vector2f,gf::Vector2f> Physics::collide(const Character& character, const std::vector<gf::RectF>& otherHitboxes) {
        gf::Vector2f result {0, 0};
        gf::Vector2f correction {0, 0};
        int collisions = 0;
        int corrections = 0;
        for (gf::RectF hb: otherHitboxes) {
            auto [correctionResult, collisionResult] = collide(character, hb);
            result += collisionResult;
            correction += correctionResult;
            if(collisionResult.x != 0 || collisionResult.y != 0){
                collisions++;
            }
            if(correctionResult.x != 0  || correctionResult.y != 0){
                corrections++;
            }
        }
        return {(correction/std::max(corrections,1)), (result /std::max(collisions,1))};
    }

    gf::Vector2f Physics::friction(const gf::Vector2f speed,const gf::Vector2f direction) {
        const gf::Vector2f resistance {
            speed.x * speed.x,
            speed.y * speed.y
        };
        return -direction * AIRRESISTANCE * resistance;
    }
}
