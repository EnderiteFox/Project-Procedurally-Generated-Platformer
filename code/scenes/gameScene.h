/**
 * The main game Scene for the platformer game
 */
#include <gf/Scene.h>


namespace platformer{
    class GameScene : public gf::Scene{
    private :
        static constexpr auto pauseKey1 = gf::Keycode::P;
        static constexpr auto pauseKey2 = gf::Keycode::Escape;
        gf::SceneManager& manager;
        gf::Scene& pauseScene;

    protected:
        virtual void doUpdate(gf::Time& time);

    public :
        const gf::Action pauseAction {"Pause"};

        GameScene (gf::Vector2i initialSize) = delete;
        GameScene (gf::Vector2i initialSize, gf::SceneManager& manager, gf::Scene& pauseScene);

        // Initialize the scene and it's main entity.
        // The main purpose of this function is to allow the Scene to be destroyed and recreated at will, and reinitialising it
        // If this ends up creating lasting lags, we'll do it another way
        void init();

    };

}
