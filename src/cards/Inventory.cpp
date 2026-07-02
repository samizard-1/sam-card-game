// Inventory implementation. InitializeEmptyInventory() is the one place you define which
// cards exist in the game; all other Inventory methods are generic helpers.
#include "cards/Inventory.hpp"

#include <cassert>

namespace cards {

std::size_t Inventory::size() const { return cards_.size(); }

bool Inventory::empty() const { return cards_.empty(); }

void Inventory::add(const Card &card) { cards_.push_back(card); }

const std::vector<Card> &Inventory::cards() const { return cards_; }

} // namespace cards
