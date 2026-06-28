#pragma once

#include "raylib.h"

#include "cards/Card.hpp"
#include "input/InputHandler.hpp"
#include "render/Animation.hpp"
#include "render/CardRenderer.hpp"

namespace app {

// Owns the application window and the main game loop. This class is the spine of
// the program: every frame it updates state, then draws the scene.
//
// TODO(you): grow this into the real game — replace the single demo card with a
// deck, a GameState, and players, and call into rules/ each turn.
class Game {
  public:
    Game(int width, int height, const char *title);
    ~Game();

    // Runs the main loop until the user closes the window.
    void run();

  private:
    void update(float dt);        // advance state by dt seconds
    void draw() const;            // render the current frame
    Rectangle cardBounds() const; // where the demo card sits on screen

    cards::Card demoCard_;         // the one card in the reference slice
    bool showFace_ = false;        // is the face (vs back) currently up?
    bool swappedThisFlip_ = false; // have we swapped sides during this flip?

    render::FlipAnimation flip_;
    render::CardRenderer renderer_;
    input::InputHandler input_;
};

} // namespace app
