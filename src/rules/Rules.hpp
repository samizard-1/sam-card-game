#pragma once

#include "rules/GameState.hpp"

namespace rules {

// The rulebook for the game: setup and turn flow. Everything here is a stub.
//
// TODO(you): this is where your game's RULES live. Implement these functions in
// Rules.cpp, and add new ones (e.g. isLegalMove, applyMove, scoreRound).

// Deals the opening table and chooses the first player.
void setup(GameState &state);

// Advances to the next player's turn.
void advanceTurn(GameState &state);

// Returns true when the match is over.
bool isGameOver(const GameState &state);

} // namespace rules
