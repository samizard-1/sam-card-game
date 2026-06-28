// AIPlayer implementation. takeTurn() is where the computer opponent lives;
// add all NPC decision-making logic here.
#include "players/AIPlayer.hpp"

#include <utility>

#include "rules/GameState.hpp"

namespace players {

AIPlayer::AIPlayer(std::string name) : name_(std::move(name)) {}

const std::string &AIPlayer::name() const { return name_; }

void AIPlayer::takeTurn(rules::GameState & /*state*/) {
    // TODO(you): THIS IS THE NPC. Implement the computer opponent's decision-
    // making here: read `state`, pick a legal move per Rules, and apply it.
    // Start simple (e.g. play the first legal move) before getting clever.
}

} // namespace players
