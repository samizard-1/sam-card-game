#include "rules/GameState.hpp"

namespace rules {

void GameState::reset() {
    drawPile = cards::Deck{};
    table.clear();
    currentPlayer = 0;
}

} // namespace rules
