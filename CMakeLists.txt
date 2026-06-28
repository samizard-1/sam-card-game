cmake_minimum_required(VERSION 3.16)
project(sam_card_game LANGUAGES CXX)

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

add_executable(sam_card_game
    src/main.cpp
    src/app/Game.cpp
)
target_include_directories(sam_card_game PRIVATE src)
target_link_libraries(sam_card_game PRIVATE raylib)
