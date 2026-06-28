#pragma once

#include <cstddef>
#include <vector>

#include "cards/Card.hpp"

namespace cards {

// A collection of cards you can build, shuffle, and draw from.
class Deck {
  public:
    // Builds the full deck for this game.
    // TODO(you): replace the placeholder cards with your real deck definition.
    static Deck buildStandardDeck();

    std::size_t size() const;
    bool empty() const;

    void add(const Card &card);

    // Removes and returns the top card. Precondition: !empty().
    Card draw();

    // Shuffles in place. The seed makes shuffles repeatable (handy for tests).
    void shuffle(unsigned int seed);

    const std::vector<Card> &cards() const;

  private:
    std::vector<Card> cards_;
};

} // namespace cards
