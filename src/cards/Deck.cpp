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
    // Card demoCard;
    // demoCard.id = 1;
    // demoCard.name = "Hello Card";
    // deck.add(demoCard);

    // inventoryDemoCard.id = 2;
    // inventoryDemoCard.name = "Hello Card";
    // inventoryDemoCard.facing = cards::Facing::Up;
    // cardTwo.id = 3;
    // cardTwo.name = "Card Two";
    // inventoryCardTwo.id = 4;
    // inventoryCardTwo.name = "Card Two";

    int i = 1;
    for (char c : {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                   'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'}) {
        Card card;
        card.id = i++;
        // card.name = std::to_string(c) + " Card";
        card.name = std::string(1, c) + " Card";
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
