#pragma once

#include <string>

#include "players/Player.hpp"

namespace players {

// A human player. Their moves ultimately come from mouse/keyboard input.
class HumanPlayer : public Player {
  public:
    explicit HumanPlayer(std::string name);

    const std::string &name() const override;
    void takeTurn(rules::GameState &state) override;

  private:
    std::string name_;
};

} // namespace players
