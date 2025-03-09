#pragma once
#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/RenderTarget.h>
#include <gf/Vector.h>
#include <gf/Sprite.h>

/**
 * An entity created to be able to add texts to entity containers
 * Also, allows the text to be prefixed by a picture (only works for single lined texts)
 */
namespace platformer {
    class TextEntity final : public gf::Entity, public gf::Text {
        gf::Sprite prefix;

        bool showPrefix = false;

        bool isHidden = false;

    public:
        TextEntity(const std::string& string, gf::Font &font, gf::Vector2f position, unsigned characterSize=30);

        TextEntity(std::string string, gf::Font &font, gf::Vector2f position, const gf::Texture& texture, unsigned characterSize=30);

        void setTextPosition(gf::Vector2f position);

        void render(gf::RenderTarget &target, const gf::RenderStates &states) override;

        void update(gf::Time time) override;

        void setPrefix(const gf::Texture& texture);

        void setTextAnchor(gf::Anchor anchor);

        void setString(const std::string& string);

        gf::RectF getPrefixBounds() const;

        void setPrefixScale(float scale);

        float getPrefixScale() const;

        void show();

        void hide();
    };
}
