// Deck implementation. buildStandardDeck() is the one place you define which
// cards exist in the game; all other Deck methods are generic helpers.
#include "cards/Deck.hpp"

#include <algorithm>
#include <cassert>
#include <random>

namespace cards {

Deck Deck::buildStandardDeck() {
    Deck deck;
    // TODO(you): replace these placeholder cards with your game's real deck.
    // If you change the count here, update the test in tests/deck_test.cpp.
    for (int i = 1; i <= 5; ++i) {
        Card card;
        card.id = i;
        card.name = "Card " + std::to_string(i);
        deck.add(card);
    }
    return deck;
}

std::size_t Deck::size() const { return cards_.size(); }

bool Deck::empty() const { return cards_.empty(); }

void Deck::add(const Card &card) { cards_.push_back(card); }

Card Deck::draw() {
    assert(
        !cards_
             .empty()); // Debug-only guard (compiled out when NDEBUG is set, e.g. Release builds).
    Card top = cards_.back();
    cards_.pop_back();
    return top;
}

void Deck::shuffle(unsigned int seed) {
    std::mt19937 rng(seed);
    std::shuffle(cards_.begin(), cards_.end(), rng);
}

const std::vector<Card> &Deck::cards() const { return cards_; }

} // namespace cards
