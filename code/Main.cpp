#include <gf/Monitor.h>
#include <scenes/PlatformerManager.h>

int main() {
    // Getting screen size
    static gf::Vector2i ScreenSize = gf::Monitor::getPrimaryMonitor().getPhysicalSize();

    // Creation of the manager
    platformer::PlatformerManager manager("Platform Protocol", ScreenSize);

    manager.run();

    return 0;
}
