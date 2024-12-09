/**
 * A class used to handle all physic behaviors
 */
#include <cassert>
#include <iostream>
#include <gf/Rect.h>
#include <gf/Vector.h>
#include <gf/Collision.h>
#include <vector>
#include <Physics.h>
#include <player/Character.h>


namespace platformer{
    std::pair<bool,gf::Vector2f> Physics::collide(const platformer::Character& character, const gf::RectF& otherHitbox){
        gf::RectF charHB = character.getHitbox();
        gf::Penetration p;
        if(gf::collides(charHB,otherHitbox,p)){
            // Get the player's direction
            gf::Vector2f direction = character.getSpeed();
            direction.x = direction.x/(std::abs(direction.x)<0.001?1:std::abs(direction.x));
            direction.y = direction.y/(std::abs(direction.y)<0.001?1:std::abs(direction.y));

            // Get the player's direction relative to the other hitbox
            gf::Vector2f collideDirection = charHB.getCenter() - otherHitbox.getCenter();
            collideDirection.x = collideDirection.x/(std::abs(collideDirection.x)<0.001?1:std::abs(collideDirection.x));
            collideDirection.y = collideDirection.y/(std::abs(collideDirection.y)<0.001?1:std::abs(collideDirection.y));

            gf::Vector2f result{-direction.x+collideDirection.x*p.depth,-direction.y+collideDirection.y*p.depth};
            return {true,result};
        }
        return {false,{0,0}};
    }

    std::pair<bool,gf::Vector2f> Physics::collide(const platformer::Character& character, const std::vector<gf::RectF>& otherHitboxes){
        gf::Vector2f result{0,0};
        bool occured = false;
        for(gf::RectF hb: otherHitboxes){
            std::pair<bool,gf::Vector2f> collision = collide(character,hb);
            if(collision.first){
                occured = true;
                result += collision.second;
            }
        }
        return {occured,result};
    }
}
