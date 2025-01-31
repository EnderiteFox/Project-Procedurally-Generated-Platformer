#pragma once
#include <vector>
#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <gf/Font.h>
/**
 * An entity created to be able to add texts to entity containers
 * Also, allows the text to be prefixed by a picture
 */

namespace platformer{
    class TextEntity : public gf::Entity, public gf::Text{
    public:

        TextEntity(std::string string, gf::Font &font, gf::Vector2f baseScreenSize ,unsigned characterSize=30);

        void render (gf::RenderTarget &target, const gf::RenderStates &states) override;
    };
}
