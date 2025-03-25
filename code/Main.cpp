#include <gf/Monitor.h>
#include <scenes/PlatformerManager.h>

int main(const int argc, char* argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: ./Platformer (forced seed)");
        return 1;
    }

    const std::optional<uint64_t> forcedSeed = argc == 1
        ? std::nullopt
        : std::make_optional(strtoll(argv[1], nullptr, 10));

    // Getting screen size
    static gf::Vector2i ScreenSize = gf::Monitor::getPrimaryMonitor().getPhysicalSize();

    // Creation of the manager
    platformer::PlatformerManager manager("Platform Protocol", ScreenSize, forcedSeed);

    manager.run();

    return 0;
}
