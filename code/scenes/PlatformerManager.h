#pragma once
#include <gf/Font.h>
#include <scenes/GameScene.h>
#include <gf/SceneManager.h>
#include <gf/Vector.h>
#include <scenes/Menu.h>
#include <scenes/PauseScene.h>
#include <scenes/EndScene.h>

namespace platformer {

    class PlatformerManager final : public gf::SceneManager {
    public:
        gf::Font font;

        // Constants used within the other classes
        static constexpr gf::Vector2f gameViewSize{100.0f, 100.0f};
        const std::string gameTitle;
        float charSize;
        gf::Vector2i ScreenSize;

    private:
        std::optional<uint64_t> forcedSeed;

        GameScene gameScene;
        Menu menu;
        PauseScene pause;
        EndScene endScreen;

    public:
        virtual ~PlatformerManager() = default;

        void loadGame();
        void loadMenu(bool replace=true);
        void loadPause();
        void unpause();
        void loadEndScreen(int score, bool isVictory);

        void setForcedSeed(uint64_t forcedSeed);
        std::optional<uint64_t> getForcedSeed() const;

        PlatformerManager(const std::string &title, gf::Vector2i size, std::optional<uint64_t> forcedSeed = std::nullopt, gf::Flags<gf::WindowHints> hints=gf::All);
    };
}
