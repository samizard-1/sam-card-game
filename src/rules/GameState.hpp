#pragma once

#include <vector>

#include "cards/Card.hpp"
#include "cards/Deck.hpp"

namespace rules {

// All the state of a single match. Keep this as plain data — the rules logic in
// Rules.hpp reads and changes it.
//
// TODO(you): add the fields your game tracks (player hands, discard pile,
// scores, current phase, ...).
struct GameState {
    cards::Deck drawPile;           // cards left to draw
    std::vector<cards::Card> table; // cards currently shown on the table
    int currentPlayer = 0;          // index of the player whose turn it is

    // Clears everything back to an empty match.
    void reset();
};

} // namespace rules
