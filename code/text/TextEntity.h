#pragma once
#include <vector>
#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/RenderTarget.h>
#include <gf/Font.h>
/**
 * An entity created to be able to add texts to entity containers
 * Also allows to have centered multi-lined text
 */

namespace platformer{
    class TextEntity : public gf::Entity, public gf::Text{
    private :
        std::vector<std::string> lines;
        gf::Font& font;
        gf::Vector2f baseScreenSize;

        std::vector<std::string> parseLines(std::string string) const;
    public:

        TextEntity(std::string string, gf::Font &font, gf::Vector2f baseScreenSize ,unsigned characterSize=30);

        const std::string getString () const;
        void setString (std::string string);

        void addLine(std::string string);

        void render (gf::RenderTarget &target, const gf::RenderStates &states) override;
    };
}
