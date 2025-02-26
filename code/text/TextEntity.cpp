#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Views.h>
#include <gf/Vector.h>
#include <gf/RenderTarget.h>
#include <gf/Font.h>
#include <text/TextEntity.h>
#include <gf/Sprite.h>

#include <iostream>

namespace platformer{

    TextEntity::TextEntity(std::string string, gf::Font &font, gf::Vector2f position, unsigned characterSize):
        Text(std::move(string),font,characterSize){
            Text::setPosition(position);
            Text::setAlignment(gf::Alignment::Center);
            Text::setParagraphWidth ((getString().length()/2)*characterSize);
    }

    TextEntity::TextEntity(std::string string, gf::Font &font, gf::Vector2f position ,const gf::Texture& texture ,unsigned characterSize):
        Text(std::move(string),font,characterSize){
            Text::setPosition(position);
            prefix.setTexture(texture);
            Text::setAlignment(gf::Alignment::Center);
            Text::setParagraphWidth ((getString().length()*2/3)*characterSize);
            prefix.scale(getLocalBounds().getSize().y / prefix.getLocalBounds().getSize().y);
            prefix.setPosition(getPosition() - getLocalBounds().getSize()/2 - prefix.getLocalBounds().getSize()*gf::Vector2i{1,0});
    }

    void TextEntity::setPosition(gf::Vector2f position){
        Text::setPosition(position);
        prefix.setPosition(getPosition() - getLocalBounds().getSize()/2 - prefix.getLocalBounds().getSize()*gf::Vector2i{1,0});
    }

    void TextEntity::render (gf::RenderTarget &target, const gf::RenderStates &states){
        target.draw(prefix,states);
        target.draw(*this,states);
    }

    void TextEntity::update(const gf::Time time) {
        return;
    }
}
