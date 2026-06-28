#include "players/HumanPlayer.hpp"

#include <utility>

#include "rules/GameState.hpp"

namespace players {

HumanPlayer::HumanPlayer(std::string name) : name_(std::move(name)) {}

const std::string &HumanPlayer::name() const { return name_; }

void HumanPlayer::takeTurn(rules::GameState & /*state*/) {
    // TODO(you): a human's move comes from input. When you build real turns,
    // read the chosen card/action from InputHandler and apply it to `state`.
}

} // namespace players
