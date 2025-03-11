#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Vector.h>
#include <gf/RenderTarget.h>
#include <gf/Font.h>
#include <text/TextEntity.h>
#include <gf/Sprite.h>
#include <gf/Coordinates.h>
#include <gf/Rect.h>
#include <gf/Shapes.h>

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
    , scalePosition(position)
    {}

    TextEntity::TextEntity(
        std::string string,
        gf::Font &font,
        const gf::Vector2f position,
        const gf::Texture& texture,
        const float characterDimension
    )
    : Text(string, font, 30u)
    , prefix(texture)
    , characterDimension(characterDimension)
    , scalePosition(position)
    {
        showPrefix = true;
    }

    void TextEntity::setTextPosition(const gf::Vector2f position) {
        scalePosition = position;
    }

    void TextEntity::render(gf::RenderTarget &target, const gf::RenderStates &states) {
        if(!isHidden){
            gf::Coordinates coordinatesSystem(target);

            // Calculating the relative character size
            setCharacterSize(coordinatesSystem.getRelativeCharacterSize(characterDimension));

            // Computing the size of the paragraph with the new relative position and size
            if (showPrefix) setParagraphWidth(getString().length() * 3/5 * getCharacterSize());
            else setParagraphWidth(getString().length() /2 * getCharacterSize());

            // Calculating the relative position
            setPosition(coordinatesSystem.getRelativePoint(scalePosition));

            // Calculating the prefix's position if necessary
            if(showPrefix){
                prefix.setPosition(getPosition() - getLocalBounds().getSize() / gf::Vector2f{0.8,4});
            }

            // Drawing everything
            if(showPrefix){
                target.draw(prefix,states);
            }
            target.draw(*this,states);

        }
    }

    void TextEntity::update(const gf::Time time) {}

    void TextEntity::setString(const std::string& string) {
        Text::setString(string);
    }

    void TextEntity::setPrefix(const gf::Texture& texture) {
        prefix.setTexture(texture);
        showPrefix = true;
    }

    gf::RectF TextEntity::getPrefixBounds() const {
        if(showPrefix) return prefix.getLocalBounds();
    }

    void TextEntity::setTextAnchor(const gf::Anchor anchor) {
        setAnchor(anchor);
    }

    float TextEntity::getPrefixScale() const {
        return prefix.getScale().x;
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

