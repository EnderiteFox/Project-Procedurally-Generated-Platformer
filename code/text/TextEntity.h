#pragma once
#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <gf/Sprite.h>
#include <gf/Vector.h>

/**
 * An entity created to be able to add texts to entity containers
 * Also, allows the text to be prefixed by a picture (only works for single lined texts)
 */
namespace platformer {
    class TextEntity final : public gf::Entity, public gf::Text {
        gf::Sprite prefix;

        bool showPrefix = false;

        bool isHidden = false;

        float characterDimension;

        gf::Vector2f scalePosition;

        gf::Vector2f prefixRelativePosition{0.0f,0.0f};

        gf::Anchor anchor = gf::Anchor::Center;

    public:
        TextEntity(const std::string& string, gf::Font &font, gf::Vector2f position, float characterDimension=5.0f);

        TextEntity(std::string string, gf::Font &font, gf::Vector2f position, const gf::Texture& texture, float characterDimension=5.0f);

        void setTextPosition(gf::Vector2f position);

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

        void updateAnchor(gf::Anchor anchor);

        void setPrefix(const gf::Texture& texture);

        void setPrefixRelativePosition(const gf::Vector2f position);

        void setString(const std::string& string);

        void setPrefixScale(float scale);

        float getPrefixScale() const;

        void show();

        void hide();
    };
}
