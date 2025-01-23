#include <gf/Text.h>
#include <gf/Entity.h>
#include <gf/Views.h>
#include <gf/RenderTarget.h>
#include <gf/Font.h>
#include <text/TextEntity.h>

#include <iostream>

namespace platformer{
    std::vector<std::string> TextEntity::parseLines(std::string string) const{
        std::vector<std::string> res;
        std::string current = "";
        for(char c : string){
            if(c=='\n'){
                res.push_back(current);
                current = "";
            }
            else{
                current += c;
            }
        }
        if(current!=""){
            res.push_back(current);
        }
        return res;
    }

    TextEntity::TextEntity(std::string string, gf::Font &font, gf::Vector2f baseScreenSize, unsigned characterSize):
        Text(),
        lines(parseLines(string)),
        font(font),
        baseScreenSize(baseScreenSize){
            setCharacterSize(characterSize);
            setFont(font);
    }

    const std::string TextEntity::getString () const{
        std::string res ="";
        for(std::string string : lines){
            if(res!=""){
                res += "\n";
            }
            res += string;
        }
        return res;
    }

    void TextEntity::setString (std::string string){
        lines = parseLines(string);
    }

    void TextEntity::addLine(std::string string){
        std::vector<std::string> parsedLines = parseLines(string);
        for(std::string s : parsedLines){
            lines.push_back(s);
        }
    }

    void TextEntity::render (gf::RenderTarget &target, const gf::RenderStates &states){
        unsigned int charSize = ((float)target.getSize().x / baseScreenSize.x) * getCharacterSize();
        gf::Vector2f center = getPosition();
        float middle = lines.size()/2;
        int linecount = 0;
        gf::View WorldView = target.getView();

        for(std::string& text : lines){
            linecount++;
            gf::Text line = gf::Text(text,font);
            line.setColor(getColor());
            line.setPosition(
                center -
                gf::Vector2f{static_cast<float>(text.size() / 4 * charSize),static_cast<float>(charSize) * (linecount - middle)}
            );
            target.draw(line);
        }
    }
}
