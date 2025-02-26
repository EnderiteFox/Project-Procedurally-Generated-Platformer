#pragma once
#include <vector>
#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <gf/Font.h>
#include <gf/Sprite.h>
/**
 * An entity created to be able to add texts to entity containers
 * Also, allows the text to be prefixed by a picture (only works for single lined texts)
 */

namespace platformer{
    class TextEntity : public gf::Entity, public gf::Text{
        gf::Sprite prefix;
    public:

        TextEntity(std::string string, gf::Font &font, gf::Vector2f position ,unsigned characterSize=30);

        TextEntity(std::string string, gf::Font &font, gf::Vector2f position ,const gf::Texture& texture ,unsigned characterSize=30);

        void setPosition(gf::Vector2f position);

        void render (gf::RenderTarget &target, const gf::RenderStates &states) override;

        void update(gf::Time time) override;
    };
}
