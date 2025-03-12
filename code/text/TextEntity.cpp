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

            // Calculating the relative position
            setPosition(coordinatesSystem.getRelativePoint(scalePosition));

            // Resetting the anchor
            setAnchor(anchor);

            // Calculating the prefix's position if necessary
            if(showPrefix){
                prefix.setPosition(coordinatesSystem.getRelativePoint(prefixRelativePosition));
            }

            // Drawing everything
            if(showPrefix){
                target.draw(prefix,states);
            }
            target.draw(*this,states);

        }
    }

    void TextEntity::setString(const std::string& string) {
        Text::setString(string);
    }

    void TextEntity::updateAnchor(gf::Anchor anchor){
        this->anchor = anchor;
    }

    void TextEntity::setPrefix(const gf::Texture& texture) {
        prefix.setTexture(texture);
        showPrefix = true;
    }

    void TextEntity::setPrefixRelativePosition(const gf::Vector2f position){
        prefixRelativePosition = position;
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

