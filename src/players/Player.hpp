#pragma once

#include <string>

namespace rules {
struct GameState; // forward declaration — full definition in rules/GameState.hpp
}

namespace players {

// Common interface for every participant — human or NPC. The turn loop calls
// takeTurn() without caring which kind of player it is, so adding the computer
// opponent later needs no changes elsewhere.
class Player {
  public:
    virtual ~Player() = default;

    // Display name of this player.
    virtual const std::string &name() const = 0;

    // Decide and apply this player's move for the current turn.
    // TODO(you): define what a "move" is and how it changes the GameState.
    virtual void takeTurn(rules::GameState &state) = 0;
};

} // namespace players
