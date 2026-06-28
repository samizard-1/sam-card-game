cmake_minimum_required(VERSION 3.16)
project(sam_card_game LANGUAGES CXX)

# Allow FetchContent deps that declare older cmake_minimum_required versions
# (e.g. doctest v2.4.11 uses 3.0) to configure under CMake 4.x without error.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generated for clang-tidy and IDEs.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# raylib is downloaded and built automatically on first configure.
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

# doctest: a tiny header-only unit-test framework.
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
)
FetchContent_MakeAvailable(doctest)

# All game logic lives here so the app AND the tests can use it.
# TODO(you): add each new .cpp file to this list as you create it.
add_library(game_core
    src/app/Game.cpp
    src/cards/Card.cpp
    src/cards/Deck.cpp
    src/render/Animation.cpp
    src/render/CardRenderer.cpp
    src/rules/GameState.cpp
    src/rules/Rules.cpp
    src/players/HumanPlayer.cpp
    src/players/AIPlayer.cpp
    src/input/InputHandler.cpp
)
target_include_directories(game_core PUBLIC src)
target_link_libraries(game_core PUBLIC raylib)

# The playable game.
add_executable(sam_card_game src/main.cpp)
target_link_libraries(sam_card_game PRIVATE game_core)

# The unit tests.
enable_testing()
add_executable(unit_tests
    tests/deck_test.cpp
    tests/animation_test.cpp
)
target_link_libraries(unit_tests PRIVATE game_core doctest::doctest)
add_test(NAME unit_tests COMMAND unit_tests)
