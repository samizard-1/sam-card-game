#pragma once

#include <cstddef>
#include <vector>

#include "cards/Card.hpp"

namespace cards {

// A collection of cards you can build, shuffle, and draw from.
class Inventory {
  public:
    std::size_t size() const;
    bool empty() const;

    void add(const Card &card);

    const std::vector<Card> &cards() const;

  private:
    std::vector<Card> cards_;
};

} // namespace cards
