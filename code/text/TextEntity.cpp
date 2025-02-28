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
            prefix.setPosition(getPosition() - prefix.getLocalBounds().getSize()*gf::Vector2i{1,0});
            showPrefix=true;
    }

    void TextEntity::setPosition(gf::Vector2f position){
        Text::setPosition(position);
        prefix.setPosition(getPosition() - getLocalBounds().getSize()/gf::Vector2f{0.8,4});
    }

    void TextEntity::render (gf::RenderTarget &target, const gf::RenderStates &states){
        if(showPrefix) target.draw(prefix,states);
        target.draw(*this,states);
    }

    void TextEntity::update(const gf::Time time) {
        return;
    }

    void TextEntity::setString(std::string string){
        Text::setString(string);
        if(showPrefix) Text::setParagraphWidth ((getString().length()*3/5)*getCharacterSize());
        else Text::setParagraphWidth ((getString().length()/2)*getCharacterSize());
    }

    void TextEntity::setPrefix(const gf::Texture& texture){
        prefix.setTexture(texture);
        prefix.scale(1.5*getLocalBounds().getSize().y / prefix.getLocalBounds().getSize().y);
        prefix.setPosition(getPosition());
        showPrefix=true;
    }

    gf::RectF TextEntity::getPrefixBounds() const{
        if(showPrefix) return prefix.getLocalBounds();
        return gf::RectF::fromPositionSize({0,0},{0,0});
    }

    void TextEntity::setAnchor(gf::Anchor anchor){
        Text::setAnchor(anchor);
        prefix.setAnchor(anchor);
    }

    float TextEntity::getPrefixScale() const{
        return (1.5*getLocalBounds().getSize().y / prefix.getLocalBounds().getSize().y);
    }

    void TextEntity::setPrefixScale(float scale){
        prefix.scale(scale);
    }
}
