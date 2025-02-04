/**
 * A scene used as the main menu of the game.
 * Currently only starts the game, it will hopefully do more in the future
 */

#include <gf/Scene.h>
#include <text/TextEntity.h>
#include <gf/Text.h>
#include <gf/Font.h>

namespace platformer{
    class Menu : public gf::Scene{
    private:
        static constexpr auto gameStartKey = gf::Keycode::Space;
        gf::SceneManager& manager;
        gf::Font font;
        static constexpr uint charSize = 30;

        gf::Action startAction{"Start"};

    protected:
        virtual void doUpdate(gf::Time& time);

    public :
        Menu (gf::Vector2i initialSize) = delete;
        Menu (gf::Vector2i initialSize, gf::SceneManager& manager, std::string gameName);


    };
}
