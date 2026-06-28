#pragma once

#include <string>

#include "players/Player.hpp"

namespace players {

// The NPC / computer opponent.
class AIPlayer : public Player {
  public:
    explicit AIPlayer(std::string name);

    const std::string &name() const override;
    void takeTurn(rules::GameState &state) override;

  private:
    std::string name_;
};

} // namespace players
