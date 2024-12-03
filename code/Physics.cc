/**
 * A class used to handle all physic behaviors
 */
#include <gf/Rect.h>
#include <gf/Vector.h>
#include "Physics.h"
#include "Character.h"
#include "Collidable.h"


namespace platformer{
    std::pair<bool,gf::Vector2f> Physics::collide(const platformer::Character& character, const platformer::Collidable& object){
        gf::RectF charHB = character.getHitbox();
        gf::RectF objHB = object.getHitbox();
        if(charHB.intersects(objHB)){
            gf::Vector2f result{charHB.getCenter().x - objHB.getCenter().x ,charHB.getCenter().y - objHB.getCenter().y};
            //The following code does what the "Euclidian distance" function could do, but i can't get it to work
            //float EuLength = sqrt(result.x*result.x + result.y*result.y);
            result.x = - result.x/result.x *1.5*character.getSpeed().x;
            result.y = - result.y/result.y *1.5*character.getSpeed().y;
            return {true,result};
        }
        return {false,{0,0}};
    }
}
