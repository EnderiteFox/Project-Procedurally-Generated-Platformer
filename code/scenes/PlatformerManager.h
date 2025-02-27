#pragma once
#include <gf/Scene.h>
#include <scenes/GameScene.h>
#include <gf/SceneManager.h>
#include <gf/Vector.h>
#include <scenes/Menu.h>
#include <scenes/PauseScene.h>
#include <scenes/EndScene.h>

namespace platformer {

    class PlatformerManager final : public gf::SceneManager {
    public:
        virtual ~PlatformerManager() = default;

        // Constants used within the other classes
        static constexpr int baseCharacterSize = 30;
        static constexpr gf::Vector2f gameViewSize{100.0f, 100.0f};
        gf::Font font;
        const std::string gameTitle;
        int charSize;
        gf::Vector2i ScreenSize;

        void loadGame();
        void loadMenu(bool replace=true);
        void loadPause();
        void unPause();
        void loadEndScreen(int score, bool isVictory);

        PlatformerManager(const std::string &title, gf::Vector2i size, gf::Flags<gf::WindowHints> hints=gf::All);

    private:
        GameScene gameScene;
        Menu menu;
        PauseScene pause;
        EndScene endScreen;
    };
}
