cmake_minimum_required(VERSION 3.10)

project(Platformer
  VERSION 0.1.0
  LANGUAGES CXX C
)

include(GNUInstallDirs)

find_package(gf REQUIRED COMPONENTS graphics)
find_package(Threads)

add_executable(Platformer
        #Code
        code/Main.cpp
        code/player/Character.cpp
        code/world/World.cpp
        code/blocks/BlockManager.cpp
        code/blocks/BlockTypes.cpp
        code/blocks/BlockType.cpp
        code/physics/Physics.cpp
        code/world/generators/WorldGenerator.cpp
        code/world/generators/TestGenerator.cpp
        code/world/generators/basic_generator/BasicWorldGenerator.cpp
        code/camera/Camera.cpp
        code/text/TextEntity.cpp
        code/scenes/PlatformerManager.cpp
        code/scenes/GameScene.cpp
        code/scenes/Menu.cpp
        code/scenes/PauseScene.cpp
        code/scenes/EndScene.cpp
        #Third party librairies
        librairies/pugixml-1.14/src/pugixml.cpp
)

target_include_directories(Platformer
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/code"
    ${CMAKE_CURRENT_BINARY_DIR}
    #"${CMAKE_CURRENT_SOURCE_DIR}/tests/googletest/googletest/include"
    #"${CMAKE_CURRENT_SOURCE_DIR}/tests/googletest/googletest"
)

target_link_libraries(Platformer
PUBLIC
    gf::graphics
    Threads::Threads
)

target_compile_options(Platformer
  PRIVATE
    "-Wall" "-Wextra" "-pedantic" "-g" "-fPIC"
)

set_target_properties(Platformer
  PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
)
