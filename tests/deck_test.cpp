#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "cards/Deck.hpp"

TEST_CASE("a freshly built deck has the expected number of cards") {
    cards::Deck deck = cards::Deck::buildStandardDeck();
    CHECK(deck.size() == 5);
}

TEST_CASE("drawing a card reduces the deck size by one") {
    cards::Deck deck = cards::Deck::buildStandardDeck();
    const std::size_t before = deck.size();
    deck.draw();
    CHECK(deck.size() == before - 1);
}
