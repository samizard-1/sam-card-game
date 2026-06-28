#pragma once

#include <string>

namespace cards {

// Which side of a card is showing. Used by rendering and the flip animation.
enum class Facing { Down, Up };

// One card in the game.
//
// TODO(you): add the custom attributes your game's cards need. This is NOT a
// standard 52-card deck, so there is intentionally no suit/rank — design your
// own fields here (e.g. cost, power, card type, effect id).
struct Card {
    int id = 0;       // unique id within a deck
    std::string name; // display name shown on the face
    Facing facing = Facing::Down;

    // Text drawn on the card face. Falls back to "Card #<id>" if unnamed.
    std::string label() const;
};

} // namespace cards
