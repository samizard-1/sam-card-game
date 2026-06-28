#pragma once

namespace app {

// Owns the application window and the main game loop. This class is the spine
// of the program: every frame it updates state, then draws the scene.
//
// TODO(you): as you build the game, add members here for the deck, players,
// the current GameState, and whatever the screen needs to show.
class Game {
public:
    Game(int width, int height, const char* title);
    ~Game();

    // Runs the main loop until the user closes the window.
    void run();

private:
    void update(float dt);  // advance state by dt seconds
    void draw() const;      // render the current frame
};

}  // namespace app
