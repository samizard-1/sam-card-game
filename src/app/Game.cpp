// Game implementation. This is the spine of the application: it owns the
// window, drives the main loop, and wires together cards, rules, and rendering.
#include "app/Game.hpp"

#include "raylib.h"

namespace app {

namespace {
constexpr float kFlipDuration = 0.4f; // seconds for one flip
constexpr float kCardWidth = 180.0f;
constexpr float kCardHeight = 260.0f;
} // namespace

Game::Game(int width, int height, const char *title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    // TODO(you): build the real deck and deal a hand instead of one demo card.
    demoCard_.id = 1;
    demoCard_.name = "Hello Card";
}

Game::~Game() { CloseWindow(); }

Rectangle Game::cardBounds() const {
    return Rectangle{(static_cast<float>(GetScreenWidth()) - kCardWidth) / 2.0f,
                     (static_cast<float>(GetScreenHeight()) - kCardHeight) / 2.0f, kCardWidth,
                     kCardHeight};
}

void Game::run() {
    while (!WindowShouldClose()) {
        update(GetFrameTime());

        BeginDrawing();
        ClearBackground(RAYWHITE);
        draw();
        EndDrawing();
    }
}

void Game::update(float dt) {
    // Click the card to start a flip (ignored while one is already running).
    if (!flip_.active() && input_.clickedIn(cardBounds())) {
        flip_.start(kFlipDuration);
        swappedThisFlip_ = false;
    }

    flip_.update(dt);

    // Swap which side is visible exactly once, at the midpoint of the flip.
    if (flip_.active() && flip_.pastMidpoint() && !swappedThisFlip_) {
        showFace_ = !showFace_;
        swappedThisFlip_ = true;
    }
}

void Game::draw() const {
    DrawText("Click the card to flip it.", 20, 20, 20, DARKGRAY);

    const float scale = flip_.active() ? flip_.horizontalScale() : 1.0f;
    renderer_.draw(demoCard_, cardBounds(), scale, showFace_);
}

} // namespace app
