#include <gf/Entity.h>
#include <camera/Camera.h>
#include <player/Character.h>
#include <blocks/BlockManager.h>

namespace platformer{
    Camera::Camera(gf::Scene* scene, Character& character, BlockManager& blockManager)
    : scene(scene)
    , character(character)
    , blockManager(blockManager)
    {
        viewPos = character.getPosition();
        blockManager.setViewPosition(viewPos);
    }

    void Camera::update(const gf::Time time) {
        viewPos += (character.getPosition() - viewPos) * EASING * time.asSeconds();
        scene->setWorldViewCenter(viewPos);
        blockManager.setViewPosition(viewPos);
    }

}
