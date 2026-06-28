#include "app/Game.hpp"

#include "raylib.h"

namespace app {

Game::Game(int width, int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
}

Game::~Game() { CloseWindow(); }

void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        EndDrawing();
    }
}

void Game::update(float /*dt*/) {
    // TODO(you): per-frame game updates (input, animations, turn flow) go here.
}

void Game::draw() const {
    DrawText("Sam's Card Game", 20, 20, 20, DARKGRAY);
    // TODO(you): draw the table, cards, and UI here.
}

}  // namespace app
