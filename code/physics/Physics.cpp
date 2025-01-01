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
    platformer::collisionData Physics::collide(const Character& character, const gf::RectF& otherHitbox) {
        platformer::collisionData res;
        const gf::RectF charHB = character.getHitbox();
        if (
            gf::Penetration p;
            collides(charHB,otherHitbox,p)
        ) {
            // Correction
            constexpr float correctionCoeff = 0.2f;
            res.correction = -std::max(p.depth-0.1f,0.0f) * correctionCoeff * p.normal;
            res.hasCorrectionOccured = true;

            // Collision
            gf::Vector2f relativeVelocity = -character.getSpeed();
            const float velocityAlongNormal = dot(relativeVelocity, p.normal);
            if (velocityAlongNormal > 0) {
                return res;
            }
            const float impulseScalar = (1 + RESTITUTION) * velocityAlongNormal;
            res.collision = impulseScalar * p.normal;
            res.hasCollisionOccured = true;

            //Friction
            relativeVelocity = -(character.getSpeed() + res.collision);
            gf::Vector2f tangent = relativeVelocity - dot(relativeVelocity, p.normal) * p.normal;
            if(tangent.x != 0 && tangent.y != 0){
                tangent = normalize(tangent);
            }
            float frictionScalar = -dot(relativeVelocity,tangent);
            if ( std::abs(frictionScalar) < std::abs(impulseScalar) * STATICFRICTION){
                res.friction = frictionScalar * tangent;
            }
            else {
                res.friction = -impulseScalar * tangent * KINETICFRICTION;
            }
            // To avoid floating point issues
            std::cout << res.friction.x << " " << res.friction.y << std::endl;
            if(std::abs(res.friction.x) < 0.05) res.friction.x = ((res.friction.x > 0) - (res.friction.x < 0)) * 0.05;
            if(std::abs(res.friction.y) < 0.05) res.friction.y = ((res.friction.y > 0) - (res.friction.y < 0)) * 0.05;
        }
        return res;
    }

   platformer::collisionData Physics::collide(const Character& character, const std::vector<gf::RectF>& otherHitboxes) {
        platformer::collisionData result;
        int collisions = 0;
        int corrections = 0;
        for (gf::RectF hb: otherHitboxes) {
            platformer::collisionData collisionResult = collide(character, hb);
            result += collisionResult;
            if(collisionResult.hasCollisionOccured){
                collisions++;
            }
            if(collisionResult.hasCorrectionOccured){
                corrections++;
            }
        }
        result.collision = result.collision / std::max(collisions,1);
        result.correction = result.correction / std::max(corrections,1);
        result.friction = result.friction / std::max(collisions,1);
        return result;
    }

    gf::Vector2f Physics::friction(const gf::Vector2f speed,const gf::Vector2f direction) {
        const gf::Vector2f resistance {
            speed.x * speed.x,
            speed.y * speed.y
        };
        return -direction * AIRRESISTANCE * resistance;
    }
}
