// Rules implementation. This is your rulebook: setup(), advanceTurn(), and
// isGameOver() define how the game starts, progresses, and ends.
#include "rules/Rules.hpp"

namespace rules {

void setup(GameState &state) {
    // TODO(you): deal the opening hand/table and choose the first player.
    state.reset();
    state.drawPile = cards::Deck::buildStandardDeck();
}

void advanceTurn(GameState &state) {
    // TODO(you): implement real turn order (number of players, phases, skips).
    // Placeholder: toggle between two players.
    state.currentPlayer = 1 - state.currentPlayer;
}

bool isGameOver(const GameState &state) {
    // TODO(you): replace with your real end/win condition.
    return state.drawPile.empty();
}

} // namespace rules
