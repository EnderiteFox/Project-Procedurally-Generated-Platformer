#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Views.h>
#include <gf/Vector.h>
#include <gf/RenderTarget.h>
#include <gf/Font.h>
#include <text/TextEntity.h>

#include <iostream>

namespace platformer{

    TextEntity::TextEntity(std::string string, gf::Font &font, gf::Vector2f position, unsigned characterSize):
        Text(std::move(string),font,characterSize){
            Text::setPosition(position);
            Text::setAlignment(gf::Alignment::Center);
            Text::setParagraphWidth (1000.0f);
    }

    void TextEntity::render (gf::RenderTarget &target, const gf::RenderStates &states){
        target.draw(*this,states);
    }
}
