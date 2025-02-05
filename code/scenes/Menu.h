/**
 * A scene used as the main menu of the game.
 * Currently only starts the game, it will hopefully do more in the future
 */

#include <gf/Scene.h>
#include <text/TextEntity.h>
#include <gf/Text.h>
#include <gf/Font.h>
#include <scenes/PlatformerManager.h>
#include <text/TextEntity.h>
#include <gf/Window.h>
#include <scenes/GameScene.h>

namespace platformer{
    class Menu : public gf::Scene{
    private:
        static constexpr auto gameStartKey = gf::Scancode::Space;
        PlatformerManager& manager;
        static constexpr uint charSize = 30;
        bool do_start=false;

        gf::Action startAction{"Start"};
    protected:
        virtual void doHandleActions (gf::Window& window) override;
        virtual void doUpdate (gf::Time time) override;
    public :
        Menu (gf::Vector2i initialSize) = delete;
        Menu (gf::Vector2i initialSize, PlatformerManager& manager);


    };
}
