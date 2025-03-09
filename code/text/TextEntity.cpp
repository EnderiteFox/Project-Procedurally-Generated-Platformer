#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Vector.h>
#include <gf/RenderTarget.h>
#include <gf/Font.h>
#include <text/TextEntity.h>
#include <gf/Sprite.h>
#include <gf/Coordinates.h>

#include <iostream>

namespace platformer {

    TextEntity::TextEntity(
        const std::string& string,
        gf::Font& font,
        const gf::Vector2f position,
        const float characterDimension
    )
    : Text(string, font, 30u)
    , characterDimension(characterDimension)
    {
        setTextPosition(position);
        setAlignment(gf::Alignment::Center);
        setParagraphWidth(getString().length() / 2 * getCharacterSize());
    }

    TextEntity::TextEntity(
        std::string string,
        gf::Font &font,
        const gf::Vector2f position,
        const gf::Texture& texture,
        const float characterDimension
    )
    : Text(string, font, 30u)
    , characterDimension(characterDimension)
    {
        setTextPosition(position);
        prefix.setTexture(texture);
        setAlignment(gf::Alignment::Center);
        setParagraphWidth(getString().length() * 2/3 * getCharacterSize());
        prefix.scale(getLocalBounds().getSize().y / prefix.getLocalBounds().getSize().y);
        prefix.setPosition(getPosition() - prefix.getLocalBounds().getSize() * gf::Vector2i{1,0});
        showPrefix = true;
    }

    void TextEntity::setTextPosition(const gf::Vector2f position) {
        setPosition(position);
        prefix.setPosition(getPosition() - getLocalBounds().getSize() / gf::Vector2f{0.8,4});
    }

    void TextEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        if(!isHidden){
            // Updating the character size in function of the context
            setCharacterSize(gf::Coordinates(target).getRelativeCharacterSize(characterDimension));
            // Updating the dimension in function of the context
            if (showPrefix) setParagraphWidth(getString().length() * 3/5 * getCharacterSize());
            else setParagraphWidth(getString().length() / 2 * getCharacterSize());
            // Drawing
            if (showPrefix) target.draw(prefix, states);
            target.draw(*this, states);
        }
    }

    void TextEntity::update(const gf::Time time) {}

    void TextEntity::setString(const std::string& string) {
        Text::setString(string);
    }

    void TextEntity::setPrefix(const gf::Texture& texture) {
        prefix.setTexture(texture);
        prefix.scale(1.5 * getLocalBounds().getSize().y / prefix.getLocalBounds().getSize().y);
        prefix.setPosition(getPosition());
        showPrefix = true;
    }

    gf::RectF TextEntity::getPrefixBounds() const {
        if (showPrefix) return prefix.getLocalBounds();
        return gf::RectF::fromPositionSize({0,0},{0,0});
    }

    void TextEntity::setTextAnchor(const gf::Anchor anchor) {
        setAnchor(anchor);
        prefix.setAnchor(anchor);
    }

    float TextEntity::getPrefixScale() const {
        return 1.5 * getLocalBounds().getSize().y / prefix.getLocalBounds().getSize().y;
    }

    void TextEntity::setPrefixScale(const float scale) {
        prefix.scale(scale);
    }

    void TextEntity::show(){
        isHidden = false;
    }

    void TextEntity::hide(){
        isHidden = true;
    }
}

