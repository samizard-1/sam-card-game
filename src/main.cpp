// Entry point for the game. Constructs the Game (window + main loop) and runs
// it until the player closes the window. See app/Game.* for everything else.
#include "app/Game.hpp"

int main() {
    app::Game game(960, 600, "Sam's Card Game");
    game.run();
    return 0;
}
