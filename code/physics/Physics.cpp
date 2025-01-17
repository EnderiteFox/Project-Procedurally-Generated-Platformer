/**
 * A class used to handle all physic behaviors
 */
#include <gf/Rect.h>
#include <gf/Vector.h>
#include <gf/Collision.h>
#include <vector>
#include <set>
#include <physics/Physics.h>
#include <player/Character.h>
#include <blocks/BlockTypes.h>

#include <iostream>


namespace platformer {
    collisionData Physics::collide(const Character& character, const gf::RectF& otherHitbox, const std::string type) {
        collisionData res;
        const gf::RectF charHB = character.getHitbox();
        if (
            gf::Penetration p;
            collides(charHB,otherHitbox,p)
        ) {
            BlockType otherBlock = BlockTypes::getBlockTypeByName(type);

            // Storing the data of the collision
            res.hasCollisionOccured = true;
            res.flags.insert(otherBlock.type);
            if(!otherBlock.isCollidable ||
               (otherBlock.isDirectionnal && matchDirection(otherBlock.direction, character.getDirection()))
            ){
                return res;
            }

            // Correction
            constexpr float correctionCoeff = 0.3f;
            res.correction = -std::max(p.depth-0.1f,0.0f) * correctionCoeff * p.normal;
            res.hasCorrectionOccured = true;

            // Collision resolution
            gf::Vector2f relativeVelocity = -character.getSpeed();
            const float velocityAlongNormal = dot(relativeVelocity, p.normal);
            if (velocityAlongNormal > 0) {
                return res;
            }
            const float impulseScalar = (1 + otherBlock.restitution) * velocityAlongNormal;
            res.collision = impulseScalar * p.normal;

            // Friction resolution
            relativeVelocity = -(character.getSpeed() + res.collision);
            gf::Vector2f tangent = relativeVelocity - dot(relativeVelocity, p.normal* gf::Vector2f{1,0}) * p.normal;
            if (tangent.x != 0 && tangent.y != 0) {
                tangent = normalize(tangent);
            }

            if (
                float frictionScalar = -dot(relativeVelocity,tangent);
                std::abs(frictionScalar) < std::abs(impulseScalar) * otherBlock.staticFriction
            ) {
                res.friction = frictionScalar * tangent;
            }
            else {
                res.friction = -impulseScalar * tangent * otherBlock.dynamicFriction;
            }
            // To avoid floating point issues
            if (std::abs(character.getSpeed().x) < 10 * otherBlock.staticFriction && std::abs(character.getSpeed().x) > 0) {
                res.friction.x = -character.getSpeed().x;
            }

            res.friction = res.friction * gf::Vector2f{1,0};
        }
        return res;
    }

    collisionData Physics::collide(const Character& character, const std::vector<std::pair<gf::RectF,std::string>>& otherBlocks) {
        collisionData result;
        int collisions = 0;
        int corrections = 0;
        for (auto& hb: otherBlocks) {
            collisionData collisionResult = collide(character, hb.first,hb.second);
            result += collisionResult;
            if (collisionResult.hasCollisionOccured) {
                collisions++;
            }
            if (collisionResult.hasCorrectionOccured) {
                corrections++;
            }
        }
        result.collision = result.collision / std::max(collisions,1);
        result.correction = result.correction / std::max(corrections,1);
        result.friction = result.friction / std::max(collisions,1);
        return result;
    }

    gf::Vector2f Physics::friction(const gf::Vector2f speed, const gf::Vector2f direction) {
        const gf::Vector2f resistance {
            speed.x * speed.x * AIRRESISTANCE.x,
            speed.y * speed.y * AIRRESISTANCE.y
        };
        return -direction * resistance;
    }

    bool Physics::matchDirection(const gf::Vector2i blockDirection, const gf::Vector2f characterDirection) {
        const gf::Vector2f product = blockDirection * characterDirection;
        return (product.x > 0 || product.y > 0) || (product.x == 0 && product.y == 0);
    }
}
