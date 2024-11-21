#include <iostream>
#include <gf/Window.h>
#include <gf/RenderWindow.h>
#include <gf/Event.h>
#include <gf/Color.h>

int main() {
    // Creating window and renderer

    gf::Window window("Platformer", {1920, 1080});
    gf::RenderWindow renderer(window);

    renderer.clear(gf::Color::White);

    // Game loop

    while (window.isOpen()) {

        gf::Event event{};

        while (window.pollEvent(event)) {
            switch (event.type) {
                case gf::EventType::Closed:
                    window.close();
                    break;
                default:
                    break;
            }
        }

        renderer.clear();
        renderer.display();
    }

    return 0;
}
